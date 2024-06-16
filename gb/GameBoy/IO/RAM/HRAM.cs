namespace gbemu.GameBoy.IO.RAM;

public class HRAM : IIODevice
{
    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    private byte[] ram;

    public HRAM()
    {
        ram = new byte[0x80];
        Reset(false);
    }

    public bool IsValidAddress(ushort address)
    {
        return address >= 0xFF80 && address < 0xFFFF;
    }

    public byte Read(ushort address)
    {
        if (IsValidAddress(address))
            return ram[address ^ 0xFF80];
        return 0xFF;
    }

    public void Reset(bool skipBoot)
    {
        for (int i = 0; i < ram.Length; i++)
            ram[i] = 0x00;
    }

    public void Write(ushort address, byte data)
    {
        if (IsValidAddress(address))
            ram[address ^ 0xFF80] = data;
    }
}
