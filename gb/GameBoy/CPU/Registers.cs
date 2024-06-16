namespace gbemu.GameBoy;

public partial class CPU
{
    public byte Opcode { get; set; }
    public string Instruction { get; set; } = string.Empty;

    public bool IME { get; set; }

    public ushort SP { get; set; }
    public ushort PC { get; set; }

    public byte A { get; set; }
    public byte B { get; set; }
    public byte C { get; set; }
    public byte D { get; set; }
    public byte E { get; set; }
    public byte F { get; set; }

    public byte H { get; set; }
    public byte L { get; set; }

    public bool ZF
    {
        get
        {
            if ((F >> 7 & 0x01) > 0)
                return true;
            else
                return false;
        }
        set
        {
            if (value)
                F |= 0x80;
            else
                F &= 0x7F;
        }
    }
    public bool NF
    {
        get
        {
            if ((F >> 6 & 0x01) > 0)
                return true;
            else
                return false;
        }
        set
        {
            if (value)
                F |= 0x40;
            else
                F &= 0xBF;
        }
    }
    public bool HF
    {
        get
        {
            if ((F >> 5 & 0x01) > 0)
                return true;
            else
                return false;
        }
        set
        {
            if (value)
                F |= 0x20;
            else
                F &= 0xDF;
        }
    }
    public bool CF
    {
        get
        {
            if ((F >> 4 & 0x01) > 0)
                return true;
            else
                return false;
        }
        set
        {
            if (value)
                F |= 0x10;
            else
                F &= 0xEF;
        }
    }

    public ushort AF
    {
        get
        {
            return (ushort)(A << 8 | F);
        }
        set
        {
            A = (byte)((value & 0xFF00) >> 8);
            F = (byte)(value & 0xFF);
        }
    }
    public ushort BC
    {
        get
        {
            return (ushort)(B << 8 | C);
        }
        set
        {
            B = (byte)((value & 0xFF00) >> 8);
            C = (byte)(value & 0xFF);
        }
    }
    public ushort DE
    {
        get
        {
            return (ushort)(D << 8 | E);
        }
        set
        {
            D = (byte)((value & 0xFF00) >> 8);
            E = (byte)(value & 0xFF);
        }
    }
    public ushort HL
    {
        get
        {
            return (ushort)(H << 8 | L);
        }
        set
        {
            H = (byte)((value & 0xFF00) >> 8);
            L = (byte)(value & 0xFF);
        }
    }
}
