
namespace gbemu.GameBoy.IO;

public class Serial : IIODevice
{
    public byte SB { get; set; }
    public byte SC { get; set; }

    // These arent really needed
    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    public Serial()
    {
        SB = 0;
        SC = 0;
    }

    public bool IsValidAddress(ushort address)
    {
        if (address == 0xFF01 || address == 0xFF02)
            return true;

        return false;
    }

    public void Reset(bool skipBoot)
    {
        SB = 0;
        SC = 0;
    }

    public byte Read(ushort address)
    {
        switch (address)
        {
            case 0xFF01:
                return SB;
            case 0xFF02:
                return SC;
            default:
                return 0xFF;
        }
    }

    public void Write(ushort address, byte data)
    {
        switch (address)
        {
            case 0xFF01:
                SB = data;
                break;
            case 0xFF02:
                SC = data;
                break;
        }
    }

    public void SerialUpdate()
    {
        if (SC == 0x81)
        {
            Console.Write(Convert.ToChar(SB));
            SC = 0x00;
        }
    }
}
