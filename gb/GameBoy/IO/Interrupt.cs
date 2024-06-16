namespace gbemu.GameBoy.IO;

public class Interrupt : IIODevice
{
    public byte IE { get; set; }
    public byte IF { get; set; }

    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    public bool IsValidAddress(ushort address)
    {
        return address == 0xFFFF || address == 0xFF0F;
    }

    public byte Read(ushort address)
    {
        if (address == 0xFFFF)
            return IE;
        else if (address == 0xFF0F)
            return IF;
        else
            return 0xFF;
    }

    public void Reset(bool skipBoot)
    {
        IE = 0;
        IF = 0;
    }

    public void Write(ushort address, byte data)
    {
        if (address == 0xFFFF)
            IE = data;
        else if (address == 0xFF0F)
            IF = data;
    }
}
