namespace gbemu.GameBoy.IO;

public interface IIODevice
{
    public WriteDelegate? WriteIO { get; set; } // Write any address not defined here
    public ReadDelegate? ReadIO { get; set; } // Read any address not defined here
    public InterruptDelegate? RequestInterrupt { get; set; } // Request an interrupt

    public bool IsValidAddress(ushort address); // IO device address
    public void Reset(bool skipBoot); // Resets IO device to default values
    public void Write(ushort address, byte data); //  A write to this IO address
    public byte Read(ushort address); // A read to this IO address
}
