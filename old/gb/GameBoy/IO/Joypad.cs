namespace gbemu.GameBoy.IO;

public class Joypad : IIODevice
{
    private byte JOYP;
    private byte state;

    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    public bool IsValidAddress(ushort address)
    {
        return address == 0xFF00;
    }

    public byte Read(ushort address)
    {
        if (!IsValidAddress(address))
            return 0xFF;

        byte result = (byte)(JOYP | 0xCF);

        if (!JOYP.TestBit(5))
                result &= (byte)(0xF0 | (state >> 4));
        if (!JOYP.TestBit(4))
            result &= (byte)(0xF0 | (state & 0x0F));

        return result;
    }

    public void Reset(bool skipBoot)
    {
        JOYP = 0xCF;
        state = 0xFF;
    }

    public void Write(ushort address, byte data)
    {
        if (IsValidAddress(address))
            JOYP = (byte)(data & 0x30);
    }

    public void ButtonDown(JoypadInput input)
    {
        int bit = (int)input;
        state &= (byte)(~(1 << bit));
        RequestInterrupt?.Invoke(InterruptType.Joypad);
    }

    public void ButtonUp(JoypadInput input)
    {
        int bit = (int)input;
        state |= (byte)(1 << bit);
    }
}

public enum JoypadInput
{
    Right = 0,
    Left = 1,
    Up = 2,
    Down = 3,
    A = 4,
    B = 5,
    Select = 6,
    Start = 7
}