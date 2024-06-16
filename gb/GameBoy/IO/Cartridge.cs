using gbemu.GameBoy.Mappers;

namespace gbemu.GameBoy.IO;

public class Cartridge : IIODevice
{
    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    private IMapper? mapper;

    public void LoadROM(byte[] rom)
    {
        switch (rom[0x0147])
        {
            case 0x00:
                mapper = new MBC0();
                break;
            case 0x01:
                mapper = new MBC1();
                break;
            case 0x03:
                mapper = new MBC1();
                break;
            case 0x13:
                mapper = new MBC3();
                break;
            case 0x1B:
                mapper = new MBC5();
                break;
        }

        mapper?.LoadROM(rom);
    }

    public bool IsValidAddress(ushort address)
    {
        return address < 0x8000 || (address >= 0xA000 && address < 0xC000);
    }

    public byte Read(ushort address)
    {
        if (mapper == null)
            return 0x00;

        if (address < 0x8000)
            return mapper.ReadROM(address);
        else if (address >= 0xA000 && address < 0xC000)
            return mapper.ReadRAM((ushort)(address ^ 0xA000));

        return 0x00;
    }

    public void Reset(bool skipBoot)
    {
        
    }

    public void Write(ushort address, byte data)
    {
        if (address < 0x8000)
            mapper?.WriteROM(address, data);
        else if (address >= 0xA000 && address < 0xC000)
            mapper?.WriteRAM((ushort)(address ^ 0xA000), data);
    }
}
