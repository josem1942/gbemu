namespace gbemu.GameBoy;

public partial class CPU
{
    public WriteDelegate Write { get; set; }
    public ReadDelegate Read { get; set; }

    public bool IsHalted { get; set; }

    public void Reset(bool skipBoot)
    {
        if (skipBoot)
        {
            #region default values
            A = 0x01;
            ZF = true;
            CF = true;
            HF = true;
            NF = false;
            B = 0x00;
            C = 0x13;
            D = 0x00;
            E = 0xD8;
            H = 0x01;
            L = 0x4D;
            SP = 0xFFFE;
            PC = 0x0100;
            Opcode = 0;
            Instruction = "power off (no bootrom)";
            IME = false;

            IsHalted = false;

            #endregion
        }
        else
        {
            AF = 0;
            BC = 0;
            DE = 0;
            HL = 0;
            SP = 0;
            PC = 0;
            Opcode = 0;
            Instruction = "power off";
            IME = false;

            IsHalted = false;
        }
    }
}
