namespace gbemu.GameBoy.IO.RAM;

public class WRAM : IIODevice
{
    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    private byte[] ram;

    public WRAM()
    {
        ram = new byte[0x2000];
        Reset(false);
    }

    public bool IsValidAddress(ushort address)
    {
        return (address >= 0xC000 && address < 0xE000);
    }

    public byte Read(ushort address)
    {
        if (IsValidAddress(address))
            return ram[address ^ 0xC000];
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
            ram[address ^ 0xC000] = data;
    }
}
