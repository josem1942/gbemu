namespace gbemu.GameBoy.Mappers;

public class MBC3 : IMapper
{
    List<byte[]> romBanks;
    List<byte[]> ramBanks;
    int bank;

    int ramBank;

    bool ramEnabled;

    public MBC3()
    {
        romBanks = new List<byte[]>();
        bank = 0;
        ramBank = 0;
        ramEnabled = false;
        ramBanks = new List<byte[]>();

        for (int i = 0; i < 4; i++)
            ramBanks.Add(new byte[0x2000]);
    }

    public byte[] GetERAM()
    {
        return ramBanks[ramBank];
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
            return ramBanks[ramBank][address];
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
            bank = data - 1;
            if (bank < 0)
                bank = 0;
        }
        else if (address < 0x6000)
        {
            if (data < 4)
                ramBank = data;
        }
    }

    public void WriteRAM(ushort address, byte data)
    {
        if (address < 0x2000 && ramEnabled)
            ramBanks[ramBank][address] = data;
    }
}
