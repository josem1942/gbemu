namespace gbemu.GameBoy;

public partial class CPU
{
    public void ProcessInterrupts()
    {
        byte IE = Read(0xFFFF);
        byte IF = Read(0xFF0F);
        ushort intAddress = 0x0000;

        if ((IE & IF) != 0 && IsHalted)
            IsHalted = false;

        if (IME)
        {
            if (IF.TestBit(0))
            {
                if (IE.TestBit(0))
                {
                    intAddress = 0x0040; // VBlank
                    Write(0xFF0F, IF.ClearBit(0));
                }
            }
            else if (IF.TestBit(1))
            {
                if (IE.TestBit(1))
                {
                    intAddress = 0x0048; // STAT
                    Write(0xFF0F, IF.ClearBit(1));
                }
            }
            else if (IF.TestBit(2))
            {
                if (IE.TestBit(2))
                {
                    intAddress = 0x0050; // Timer
                    Write(0xFF0F, IF.ClearBit(2));
                }
            }
            else if (IF.TestBit(3))
            {
                if (IE.TestBit(3))
                {
                    intAddress = 0x0058; // Serial
                    Write(0xFF0F, IF.ClearBit(3));
                }
            }
            else if (IF.TestBit(4))
            {
                if (IE.TestBit(4))
                {
                    intAddress = 0x0060; // Joypad
                    Write(0xFF0F, IF.ClearBit(4));
                }
            }
        }

        // interrupt requested
        if (intAddress != 0x0000)
        {
            IME = false;

            SP--;
            Write(SP, (byte)(PC >> 8));
            SP--;
            Write(SP, (byte)(PC & 0x00FF));

            PC = intAddress;
        }
    }
}
