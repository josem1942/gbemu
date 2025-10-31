namespace gbemu.GameBoy;

public delegate void WriteDelegate(ushort address, byte value);
public delegate byte ReadDelegate(ushort address);
public delegate void InterruptDelegate(InterruptType interrupt);

public enum InterruptType
{
    VBlank,
    LCD,
    Timer,
    Serial,
    Joypad
}

public static class Extensions
{
    public static byte ClearBit(this byte value, int bit)
    {
        return (byte)(value & ~(0x01 << bit));
    }

    public static byte SetBit(this byte value, int bit)
    {
        return (byte)(value | (0x01 << bit));
    }

    public static bool TestBit(this byte value, int bit)
    {
        return ((value >> bit) & 0x01) != 0;
    }
}

/*
namespace gbemu.GameBoy.IO;

public class IOMapper
{
    public const int SB = 0x01; // Serial transfer data
    public const int SC = 0x02; // Serial transfer control

    public Serial Serial;
    public Timer Timer;

    public byte[] IO { get; set; }

    public IOMapper()
    {
        IO = new byte[0x80];
        Serial = new Serial(IO);
        Timer = new Timer(IO);

        ZeroIO();
    }

    public void ZeroIO()
    {
        for (int i = 0; i < IO.Length; i++)
            IO[i] = 0;
    }

    public void SetDefaultIO()
    {
        this.IO[0x00] = 0xCF;
        this.IO[0x02] = 0x7E;
        this.IO[0x04] = 0xAB;
        this.IO[0x07] = 0xF8;
        this.IO[0x0F] = 0xE1;
        this.IO[0x10] = 0x80;
        this.IO[0x11] = 0xBF;
        this.IO[0x12] = 0xF3;
        this.IO[0x13] = 0xFF;
        this.IO[0x14] = 0xBF;
        this.IO[0x16] = 0x3F;
        this.IO[0x18] = 0xFF;
        this.IO[0x19] = 0xBF;
        this.IO[0x1A] = 0x7F;
        this.IO[0x1B] = 0xFF;
        this.IO[0x1C] = 0x9F;
        this.IO[0x1D] = 0xFF;
        this.IO[0x1E] = 0xBF;
        this.IO[0x20] = 0xFF;
        this.IO[0x23] = 0xBF;
        this.IO[0x24] = 0x77;
        this.IO[0x25] = 0xF3;
        this.IO[0x26] = 0xF1;
        this.IO[0x41] = 0x85;
        this.IO[0x46] = 0xFF;
        this.IO[0x47] = 0xFC;
        this.IO[0x50] = 0x01;
    }

    public byte ReadRegister(ushort address)
    {
        return IO[address ^ 0xFF00];
    }

    public void WriteRegister(ushort address, byte value)
    {
        IO[address ^ 0xFF00] = value;
    }

    // Update registers that might need updating every machine cycle
    public void UpdateCycle(int cycles)
    {
        Serial.SerialUpdate();
        Timer.UpdateTimers(cycles);
    }

    // Update registers that might need updating every VBlank (1/60 seconds)
    public void UpdateVblank()
    {

    }
}

*/

