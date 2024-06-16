namespace gbemu.GameBoy.IO;

public class APU : IIODevice
{
    private byte[] soundRegisters;
    private byte[] audioBuffer;
    private int bufferIndex;
    private const int BufferSize = 2048;

    private double sampleRate = 44100.0;
    private double waveSample;
    private int dutyStep;
    private int frequencyTimer;
    private int soundLength;
    private int envelopeVolume;
    private bool soundEnabled;

    public byte[] AudioBuffer => audioBuffer;

    public WriteDelegate? WriteIO { get; set; }
    public ReadDelegate? ReadIO { get; set; }
    public InterruptDelegate? RequestInterrupt { get; set; }

    public APU()
    {
        soundRegisters = new byte[0x30];
        audioBuffer = new byte[BufferSize];
        Reset(false);
    }

    public bool IsValidAddress(ushort address)
    {
        return address >= 0xFF10 && address <= 0xFF3F;
    }

    public void Reset(bool skipBoot)
    {
        for (int i = 0; i < soundRegisters.Length; i++)
        {
            soundRegisters[i] = 0x00;
        }
        bufferIndex = 0;
        waveSample = 0.0;
        dutyStep = 0;
        frequencyTimer = 0;
        soundLength = 0;
        envelopeVolume = 0;
        soundEnabled = false;
    }

    public byte Read(ushort address)
    {
        if (!IsValidAddress(address))
            return 0xFF;

        return soundRegisters[address - 0xFF10];
    }

    public void Write(ushort address, byte data)
    {
        if (IsValidAddress(address))
        {
            soundRegisters[address - 0xFF10] = data;
            HandleSoundRegisterWrite(address, data);
        }
    }

    private void HandleSoundRegisterWrite(ushort address, byte data)
    {
        switch (address)
        {
            case 0xFF16: // NR21
                soundLength = 64 - (data & 0x3F);
                break;
            case 0xFF17: // NR22
                envelopeVolume = data >> 4;
                break;
            case 0xFF18: // NR23
            case 0xFF19: // NR24
                UpdateFrequency();
                if ((address == 0xFF19) && (data & 0x80) != 0)
                {
                    soundEnabled = true;
                    dutyStep = 0;
                }
                break;
        }
    }

    private void UpdateFrequency()
    {
        int frequency = ((soundRegisters[0x19 - 0x10] & 0x07) << 8) | soundRegisters[0x18 - 0x10];
        frequencyTimer = (2048 - frequency) * 4;
    }

    public void Update()
    {
        if (soundEnabled && bufferIndex < BufferSize)
        {
            frequencyTimer--;
            if (frequencyTimer <= 0)
            {
                UpdateFrequency();
                dutyStep = (dutyStep + 1) % 8;
            }

            byte dutyPattern = (byte)((soundRegisters[0x16 - 0x10] & 0xC0) >> 6);
            bool output = false;

            switch (dutyPattern)
            {
                case 0:
                    output = (dutyStep == 0);
                    break;
                case 1:
                    output = (dutyStep == 0 || dutyStep == 1);
                    break;
                case 2:
                    output = (dutyStep >= 0 && dutyStep <= 2);
                    break;
                case 3:
                    output = (dutyStep >= 0 && dutyStep <= 4);
                    break;
            }

            waveSample = output ? envelopeVolume : 0;
            audioBuffer[bufferIndex++] = (byte)waveSample;
        }
    }
}