
namespace gbemu.GameBoy.IO;

public class BIOS : IIODevice
{
    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    public byte[] ROM { get; set; }
    public bool BootROMLoaded { get; set; }

    byte reg = 0;

    public BIOS()
    {
        ROM = new byte[0x0100];
        BootROMLoaded = false;
    }

    public bool IsValidAddress(ushort address)
    {
        if ((address < 0x0100 && reg == 0) || address == 0xFF50)
            return true;

        return false;
    }

    public void LoadROM(byte[] rom)
    {
        if (rom.Length > 0x0100)
            Array.Copy(rom, ROM, 0x0100);
        else
            Array.Copy(rom, ROM, rom.Length);

        BootROMLoaded = true;
    }

    public byte Read(ushort address)
    {
        if (address < 0x0100)
            return ROM[address];

        return reg;
    }

    public void Reset(bool skipBoot)
    {
        if (BootROMLoaded)
            reg = 0;
        else
            reg = 0x01;
    }

    public void Write(ushort address, byte data)
    {
        reg = data;
    }
}
