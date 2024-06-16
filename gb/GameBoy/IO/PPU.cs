namespace gbemu.GameBoy.IO;

public struct PPURGB
{
    public byte R { get; set; }
    public byte G { get; set; }
    public byte B { get; set; }

    public PPURGB(byte r, byte g, byte b)
    {
        R = r;
        G = g;
        B = b;
    }
}

public class PPU : IIODevice
{
    private PPURGB[] rgbPalette;
    private byte[] OAM;
    private byte[] VRAM;

    public byte[] OutputBuffer;

    public byte LCDC { get; set; }
    public byte STAT { get; set; }
    public byte SCY  { get; set; }
    public byte SCX  { get; set; }
    public byte LY   { get; set; }
    public byte LYC  { get; set; }
    public byte DMA  { get; set; }
    public byte BGP  { get; set; }
    public byte OBP0 { get; set; }
    public byte OBP1 { get; set; }
    public byte WY   { get; set; }
    public byte WX   { get; set; }

    private bool lcdStatus;
    private int scanlineCycles;

    private bool mode0;
    private bool mode1;
    private bool mode2;
    private bool mode3;

    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    public PPU()
    {
        OAM = new byte[0xA0];
        VRAM = new byte[0x2000];

        rgbPalette =
        [
            new(0x9b, 0xbc, 0x0f),
            new(0x8b, 0xac, 0x0f),
            new(0x30, 0x62, 0x30),
            new(0x0f, 0x38, 0x0f),
        ];
        rgbPalette =
        [
            new(0xff, 0xff, 0xff),
            new(0xa9, 0xa9, 0xa9),
            new(0x00, 0x00, 0x00),
            new(0x00, 0x00, 0x00),
        ];
        

        OutputBuffer = new byte[160 * 144 * 3];
        scanlineCycles = 0;

        Reset(false);
    }

    public bool IsValidAddress(ushort address)
    {
        return (address >= 0x8000 && address < 0xA000) || (address >= 0xFE00 && address < 0xFEA0) || (address >= 0xFF40 && address < 0xFF4F);
    }

    public byte Read(ushort address)
    {
        if (address >= 0x8000 && address < 0xA000)
            return VRAM[address ^ 0x8000];
        else if (address >= 0xFE00 && address < 0xFEA0)
            return OAM[address ^ 0xFE00];
        else
        {
            switch (address)
            {
                case 0xFF40:
                    return LCDC;
                case 0xFF41:
                    return STAT;
                case 0xFF42:
                    return SCY;
                case 0xFF43:
                    return SCX;
                case 0xFF44:
                    return LY;
                case 0xFF45:
                    return LYC;
                case 0xFF46:
                    return DMA;
                case 0xFF47:
                    return BGP;
                case 0xFF48:
                    return OBP0;
                case 0xFF49:
                    return OBP1;
                case 0xFF4A:
                    return WY;
                case 0xFF4B:
                    return WX;
                default:
                    return 0xFF;
            }
        }    
    }

    public void Reset(bool skipBoot)
    {
        for (int i = 0; i < OAM.Length; i++)
            OAM[i] = 0x00;
        for (int i = 0; i < VRAM.Length; i++)
            VRAM[i] = 0x00;

        for (int i = 0; i < OutputBuffer.Length; i += 3)
        {
            OutputBuffer[i] = 0;
            OutputBuffer[i + 1] = 0;
            OutputBuffer[i + 2] = 0;
        }

        lcdStatus = false;
        scanlineCycles = 0;

        mode0 = false;
        mode1 = false;
        mode2 = false;
        mode3 = false;

        LCDC = 0;
        STAT = 0;
        SCY  = 0;
        SCX  = 0;
        LY   = 0;
        LYC  = 0;
        DMA  = 0;
        BGP  = 0;
        OBP0 = 0;
        OBP1 = 0;
        WY   = 0;
        WX   = 0;
    }

    public void Write(ushort address, byte data)
    {
        if (address >= 0x8000 && address < 0xA000)
            VRAM[address ^ 0x8000] = data;
        else if (address >= 0xFE00 && address < 0xFEA0)
            OAM[address ^ 0xFE00] = data;
        else
        {
            switch (address)
            {
                case 0xFF40:
                    LCDC = data; break;
                case 0xFF41:
                    STAT = data; break;
                case 0xFF42:
                    SCY = data; break;
                case 0xFF43:
                    SCX = data; break;
                case 0xFF44:
                    LY = 0; break;
                case 0xFF45:
                    LYC = data; break;
                case 0xFF46:
                    DMA = data; OAMTransfer(DMA); break;
                case 0xFF47:
                    BGP = data; break;
                case 0xFF48:
                    OBP0 = data; break;
                case 0xFF49:
                    OBP1 = data; break;
                case 0xFF4A:
                    WY = data; break;
                case 0xFF4B:
                    WX = data; break;
            }
        }
    }

    public void Update(int tcycles)
    {
        UpdateLCD();

        if (!lcdStatus)
            return;

        scanlineCycles += tcycles;

        if (!mode2)
        {
            if (STAT.TestBit(5))
            {
                RequestInterrupt?.Invoke(InterruptType.LCD);
            }
            mode2 = true;
            STAT = STAT.ClearBit(0);
            STAT = STAT.SetBit(1);
        }

        if (scanlineCycles > 80)
        {
            mode3 = true;
            STAT = STAT.SetBit(1);
            STAT = STAT.SetBit(0);
        }

        if (scanlineCycles > 252)
        {
            if (STAT.TestBit(3))
            {
                RequestInterrupt?.Invoke(InterruptType.LCD);
            }
            mode0 = true;
            STAT = STAT.ClearBit(1);
            STAT = STAT.ClearBit(0);
        }

        if (scanlineCycles >= 456)
        {
            scanlineCycles = 0;
            mode0 = false;
            mode1 = false;
            mode2 = false;
            mode3 = false;

            LY++;

            if (LY == LYC && STAT.TestBit(6))
            {
                RequestInterrupt?.Invoke(InterruptType.LCD);
            }

            if (LY == 144)
            {
                if (STAT.TestBit(4))
                {
                    RequestInterrupt?.Invoke(InterruptType.LCD);
                }
                RequestInterrupt?.Invoke(InterruptType.VBlank);
            }
            else if (LY > 153)
                LY = 0;
            else if (LY < 144)
                DrawLine();
            else
            {
                mode1 = true;
                STAT = STAT.ClearBit(1);
                STAT = STAT.SetBit(0);
            }
        }
    }

    private void DrawLine()
    {
        if (LCDC.TestBit(0))
            DrawBg();
        if (LCDC.TestBit(0) && LCDC.TestBit(5))
            DrawWindow();
        if (LCDC.TestBit(1))
            DrawObj();
    }

    private void DrawWindow()
    {
        if (LY < WY || WX < 0 || WX >= 167)
            return;

        ushort windowTileMapAddress = (LCDC.TestBit(6)) ? (ushort)0x9C00 : (ushort)0x9800;
        ushort tileDataAddress = (LCDC.TestBit(4)) ? (ushort)0x8000 : (ushort)0x8800;

        int y = LY - WY;

        int tileRow = y / 8;

        for (int pixel = Math.Max(WX - 7, 0); pixel < 160; pixel++)
        {
            int x = pixel - (WX - 7);

            int tileCol = x / 8;

            ushort tileAddress = (ushort)(windowTileMapAddress + tileRow * 32 + tileCol);
            byte tileNumber = VRAM[tileAddress - 0x8000]; // VRAM is assumed to start at 0x8000

            ushort tileLocation;
            if ((LCDC & 0x10) != 0)
                tileLocation = (ushort)(tileDataAddress + tileNumber * 16);
            else
                tileLocation = (ushort)(tileDataAddress + ((sbyte)tileNumber + 128) * 16);

            int line = y % 8;
            byte data1 = VRAM[tileLocation + line * 2 - 0x8000];
            byte data2 = VRAM[tileLocation + line * 2 + 1 - 0x8000];

            int colorBit = 7 - (x % 8);
            int colorId = ((data2 >> colorBit) & 1) << 1 | ((data1 >> colorBit) & 1);
            byte color = (byte)((BGP >> (colorId * 2)) & 0x03);

            PPURGB rgbColor = rgbPalette[color];
            int index = (LY * 160 + pixel) * 3;

            OutputBuffer[index] = rgbColor.R;
            OutputBuffer[index + 1] = rgbColor.G;
            OutputBuffer[index + 2] = rgbColor.B;
        }
    }

    private void DrawObj()
    {
        bool use8x16 = LCDC.TestBit(2);

        for (int i = 0; i < 40; i++)
        {
            int index = i * 4;
            int y = OAM[index] - 16;
            int x = OAM[index + 1] - 8;
            int tileIndex = OAM[index + 2];
            byte attributes = OAM[index + 3];

            if (LY >= y && LY < y + (use8x16 ? 16 : 8))
            {
                ushort tileDataAddress = (ushort)(0x8000 + (tileIndex * 16));

                int line = LY - y;
                if (attributes.TestBit(6))
                    line = (use8x16 ? 15 : 7) - line;

                byte data1 = VRAM[tileDataAddress + line * 2 - 0x8000];
                byte data2 = VRAM[tileDataAddress + line * 2 + 1 - 0x8000];

                for (int tilePixel = 0; tilePixel < 8; tilePixel++)
                {
                    int colorBit = 7 - tilePixel;
                    if (attributes.TestBit(5))
                        colorBit = tilePixel;

                    int colorId = ((data2 >> colorBit) & 1) << 1 | ((data1 >> colorBit) & 1);
                    if (colorId == 0)
                        continue;

                    byte palette = (attributes.TestBit(4)) ? OBP1 : OBP0;
                    byte color = (byte)((palette >> (colorId * 2)) & 0x03);
                    PPURGB rgbColor = rgbPalette[color];

                    int pixel = x + tilePixel;
                    if (pixel >= 0 && pixel < 160)
                    {
                        if (attributes.TestBit(7) && OutputBuffer[LY * 160 + pixel].Equals(rgbPalette[0]))
                        {
                            int aindex = (LY * 160 + pixel) * 3;

                            OutputBuffer[aindex] = rgbColor.R;
                            OutputBuffer[aindex + 1] = rgbColor.G;
                            OutputBuffer[aindex + 2] = rgbColor.B;
                        }
                        else if (!attributes.TestBit(7))
                        {
                            int aindex = (LY * 160 + pixel) * 3;

                            OutputBuffer[aindex] = rgbColor.R;
                            OutputBuffer[aindex + 1] = rgbColor.G;
                            OutputBuffer[aindex + 2] = rgbColor.B;
                        }
                    }
                }
            }
        }
    }

    private void DrawBg()
    {
        ushort tileMapAddress = LCDC.TestBit(3) ? (ushort)0x1C00 : (ushort)0x1800;
        ushort tileDataAddress = LCDC.TestBit(4) ? (ushort)0x0000 : (ushort)0x0800;

        int y = (SCY + LY) % 256;

        int tileRow = y / 8;

        for (int pixel = 0; pixel < 160; pixel++)
        {
            int x = (SCX + pixel) % 256;

            int tileCol = x / 8;

            ushort tileAddress = (ushort)(tileMapAddress + tileRow * 32 + tileCol);
            byte tileNumber = VRAM[tileAddress];

            ushort tileLocation;
            if ((LCDC & 0x10) != 0)
                tileLocation = (ushort)(tileDataAddress + tileNumber * 16);
            else
                tileLocation = (ushort)(tileDataAddress + ((sbyte)tileNumber + 128) * 16);

            int line = y % 8;
            byte data1 = VRAM[tileLocation + line * 2];
            byte data2 = VRAM[tileLocation + line * 2 + 1];

            int colorBit = 7 - (x % 8);
            int colorId = ((data2 >> colorBit) & 1) << 1 | ((data1 >> colorBit) & 1);
            byte color = (byte)((BGP >> (colorId * 2)) & 0x03);

            PPURGB rgbColor = rgbPalette[color];

            int index = (LY * 160 + pixel) * 3;
            OutputBuffer[index] = rgbColor.R;
            OutputBuffer[index + 1] = rgbColor.G;
            OutputBuffer[index + 2] = rgbColor.B;
        }
    }

    private void OAMTransfer(int addr)
    {
        for (int i = 0; i < 0xA0; i++)
        {
            int src = (addr << 8) + i;
            OAM[i] = ReadIO!.Invoke((ushort)src);
        }
    }

    private void UpdateLCD()
    {
        if (LCDC.TestBit(7))
        {
            if (lcdStatus != true)
            {
                for (int i = 0; i < OutputBuffer.Length; i += 3)
                {
                    OutputBuffer[i] = rgbPalette[3].R;
                    OutputBuffer[i + 1] = rgbPalette[3].G;
                    OutputBuffer[i + 2] = rgbPalette[3].B;
                }
                lcdStatus = true;
            }
        }
        else
        {
            if (lcdStatus == true)
            {
                for (int i = 0; i < OutputBuffer.Length; i += 3)
                {
                    OutputBuffer[i] = 0;
                    OutputBuffer[i + 1] = 0;
                    OutputBuffer[i + 2] = 0;
                }
                lcdStatus = false;
            }
        }
    }
}
