using gbemu.GameBoy.IO;
using gbemu.GameBoy.IO.RAM;
using System.Net;
using Timer = gbemu.GameBoy.IO.Timer;

namespace gbemu.GameBoy;

public class GBInstance
{
    public CPU CPU { get; set; }
    public HRAM HRAM { get; set; }
    public APU APU { get; set; }
    public PPU PPU { get; set; }
    public WRAM WRAM { get; set; }
    public BIOS BIOS { get; set; }
    public Cartridge Cartridge { get; set; }
    public Serial Serial { get; set; }
    public Timer Timer { get; set; }
    public Joypad Joypad { get; set; }
    public Interrupt Interrupt { get; set; }

    public byte[] RAM { get; set; }

    private List<IIODevice> devices;

    public GBInstance()
    {
        this.RAM = new byte[0xFFFF];

        this.CPU = new CPU();
        this.HRAM = new HRAM();
        this.PPU = new PPU();
        this.WRAM = new WRAM();
        this.BIOS = new BIOS();
        this.Cartridge = new Cartridge();
        this.Serial = new Serial();
        this.Timer = new Timer();
        this.APU = new APU();
        this.Joypad = new Joypad();
        this.Interrupt = new Interrupt();

        this.devices = new List<IIODevice>();
        devices.Add(this.HRAM);
        devices.Add(this.PPU);
        devices.Add(this.WRAM);
        devices.Add(this.BIOS);
        devices.Add(this.Cartridge);
        devices.Add(this.Timer);
        devices.Add(this.APU);
        devices.Add(this.Serial);
        devices.Add(this.Interrupt);
        devices.Add(this.Joypad);

        this.CPU.Read = (address) =>
        {
            foreach (var i in devices)
                if (i.IsValidAddress(address))
                    return i.Read(address);
            return 0xFF;
        };

        this.CPU.Write = (address, data) =>
        {
            foreach (var i in devices)
                if (i.IsValidAddress(address))
                {
                    i.Write(address, data);
                    return;
                }    
            RAM[address] = data;
        };

        this.Timer.RequestInterrupt = RequestInterrupt;
        this.Joypad.RequestInterrupt = RequestInterrupt;
        this.PPU.RequestInterrupt = RequestInterrupt;
        this.APU.RequestInterrupt = RequestInterrupt;
        this.PPU.ReadIO = (address) =>
        {
            foreach (var i in devices)
                if (i.IsValidAddress(address))
                    return i.Read(address);
            return 0xFF;
        };

        Reset();
    }

    public void Reset()
    {
        if (BIOS.BootROMLoaded)
            CPU.Reset(false);
        else
            CPU.Reset(true);

        foreach (var i in devices)
            if (BIOS.BootROMLoaded)
                i.Reset(false);
            else
                i.Reset(true);
    }

    public void LoadROM(byte[] rom)
    {
        Cartridge.LoadROM(rom);
        Reset();
    }

    public void LoadBootROM(byte[] rom)
    {
        BIOS.LoadROM(rom);
        Reset();
    }

    public void RequestInterrupt(InterruptType interrupt)
    {
        Interrupt.IF = (byte)(Interrupt.IF | (0x01 << (int)interrupt));
    }

    public void ExecuteFrame()
    {
        const int frameCycleCount = 69905;
        int tCycles = 0;

        while (tCycles < frameCycleCount)
        {
            CPU.ProcessInterrupts();
            CPU.Fetch();

            int mCycles = CPU.ExecuteUnprefixed();

            tCycles += mCycles * 4; // convert m to t

            Timer.Update(mCycles * 4);
            PPU.Update(mCycles * 4);
            APU.Update();
            Serial.SerialUpdate();
        }

    }
}
