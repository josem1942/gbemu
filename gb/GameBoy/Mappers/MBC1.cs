namespace gbemu.GameBoy.Mappers;

public class MBC1 : IMapper
{
    List<byte[]> romBanks;
    int bank;

    byte[] ram;
    bool ramEnabled;

    public MBC1()
    {
        romBanks = new List<byte[]>();
        bank = 0;
        ramEnabled = false;
        ram = new byte[0x2000];
    }

    public byte[] GetERAM()
    {
        return ram;
    }

    public void LoadROM(byte[] data)
    {
        int totalBanks = data.Length / 0x4000;
        int offset = 0;

        romBanks.Clear();

        for (int i = 0; i < totalBanks; i++)
        {
            romBanks.Add(new byte[0x4000]);
            Array.Copy(data, offset, romBanks[i], 0, 0x4000);
            offset += 0x4000;
        }
    }

    public byte ReadROM(ushort address)
    {
        if (address < 0x4000)
            return romBanks[0][address];
        else if (address < 0x8000)
            return romBanks[bank + 1][address ^ 0x4000];
        else
            return 0;
    }

    public byte ReadRAM(ushort address)
    {
        if (address < 0x2000 && ramEnabled)
            return ram[address];
        else
            return 0xFF;
    }

    public void WriteROM(ushort address, byte data)
    {
        if (address < 0x2000)
        {
            if ((data & 0x0F) == 0x0A)
            {
                ramEnabled = true;
            }
            else
            {
                ramEnabled = false;
            }
        }
        else if (address < 0x4000)
        {
            bank = (data & 0x1F) - 1;
            if (bank < 0)
                bank = 0;
        }
    }

    public void WriteRAM(ushort address, byte data)
    {
        if (address < 0x2000 && ramEnabled)
            ram[address] = data;
    }
}
