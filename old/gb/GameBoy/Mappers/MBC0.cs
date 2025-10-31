namespace gbemu.GameBoy.Mappers;

public class MBC0 : IMapper
{
    byte[] rom;
    byte[] ram;

    public MBC0()
    {
        rom = new byte[0x8000];
        ram = new byte[0x2000];
    }

    public byte[] GetERAM()
    {
        return ram;
    }

    public void LoadROM(byte[] data)
    {
        if (data.Length > 0x8000)
            Array.Copy(data, rom, 0x8000);
        else
            Array.Copy(data, rom, data.Length);
    }

    public byte ReadROM(ushort address)
    {
        if (address < 0x8000)
            return rom[address];
        else
            return 0;
    }

    public byte ReadRAM(ushort address)
    {
        if (address < 0x2000)
            return ram[address];
        else
            return 0;
    }

    public void WriteROM(ushort address, byte data)
    {
        return;
    }

    public void WriteRAM(ushort address, byte data)
    {
        if (address < 0x2000)
            ram[address] = data;
    }
}
