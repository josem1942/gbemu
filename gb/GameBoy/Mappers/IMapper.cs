namespace gbemu.GameBoy.Mappers;

public interface IMapper
{
    public void LoadROM(byte[] data);
    public byte[] GetERAM();
    public byte ReadROM(ushort address);
    public byte ReadRAM(ushort address);
    public void WriteROM(ushort address, byte data);
    public void WriteRAM(ushort address, byte data);
}
