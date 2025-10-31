namespace gbemu.GameBoy.IO;

public class Timer : IIODevice
{
    public byte DIV { get; set; } = 0;
    public byte TIMA { get; set; } = 0;
    public byte TMA { get; set; } = 0;
    public byte TAC { get; set; } = 0;

    public int DivCycles = 0;
    public int TimaCycles = 0;

    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    public bool IsValidAddress(ushort address)
    {
        return address >= 0xFF04 && address <= 0xFF07;
    }

    public byte Read(ushort address)
    {
        switch (address)
        {
            case 0xFF04:
                return DIV;
            case 0xFF05:
                return TIMA;
            case 0xFF06:
                return TMA;
            case 0xFF07:
                return TAC;
            default:
                return 0x00;
        }
    }

    public void Reset(bool skipBoot)
    {
        DIV = 0; // ab
        TIMA = 0;
        TMA = 0;
        TAC = 0; // f8
        DivCycles = 0;
        TimaCycles = 0;
    }

    public void Write(ushort address, byte data)
    {
        switch (address)
        {
            case 0xFF04:
                DIV = 0;
                break;
            case 0xFF05:
                TIMA = data;
                break;
            case 0xFF06:
                TMA = data;
                break;
            case 0xFF07:
                TAC = data;
                break;
        }
    }

    public void Update(int tcycles)
    {
        DivCycles += tcycles;
        TimaCycles += tcycles;

        if (DivCycles >= 255)
        {
            DIV++;
            DivCycles = 0;
        }

        bool inc = false;

        switch (TAC)
        {
            case 0x04:
                if (TimaCycles >= 1024)
                {
                    TimaCycles = 0;
                    inc = true;
                }
                break;
            case 0x05:
                if (TimaCycles >= 16)
                {
                    TimaCycles = 0;
                    inc = true;
                }
                break;
            case 0x06:
                if (TimaCycles >= 64)
                {
                    TimaCycles = 0;
                    inc = true;
                }
                break;
            case 0x07:
                if (TimaCycles >= 256)
                {
                    TimaCycles = 0;
                    inc = true;
                }
                break;
        }

        if (inc)
        {
            if (TIMA == 0xFF)
            {
                TIMA = TMA;
                RequestInterrupt?.Invoke(InterruptType.Timer);
            }
            else
            {
                TIMA++;
            }
        }
    }
}
