namespace gbemu.GameBoy;

public partial class CPU
{
    /// <summary>
    /// Fetches an opcode, increases PC, and decodes it to show instruction neumonic or however its spelled.
    /// Look at <seealso cref="ExecuteUnprefixed"/> for reason as to why its so big :p
    /// </summary>
    public void Fetch()
    {
        if (IsHalted)
            return;

        Opcode = Read(PC);
        PC++;

        // quick shortcuts
        ushort u16 = (ushort)((Read((ushort)(PC + 1)) << 8) | Read(PC));
        byte u8 = Read(PC);
        sbyte i8 = (sbyte)Read(PC);

        // ... sorry
        switch (Opcode)
        {
            #region x8/lsm
            #region 0x40
            case 0x40:
                Instruction = "LD B,B";
                return;
            case 0x41:
                Instruction = "LD B,C";
                return;
            case 0x42:
                Instruction = "LD B,D";
                return;
            case 0x43:
                Instruction = "LD B,E";
                return;
            case 0x44:
                Instruction = "LD B,H";
                return;
            case 0x45:
                Instruction = "LD B,L";
                return;
            case 0x46:
                Instruction = "LD B,(HL)";
                return;
            case 0x47:
                Instruction = "LD B,A";
                return;

            case 0x48:
                Instruction = "LD C,B";
                return;
            case 0x49:
                Instruction = "LD C,C";
                return;
            case 0x4A:
                Instruction = "LD C,D";
                return;
            case 0x4B:
                Instruction = "LD C,E";
                return;
            case 0x4C:
                Instruction = "LD C,H";
                return;
            case 0x4D:
                Instruction = "LD C,L";
                return;
            case 0x4E:
                Instruction = "LD C,(HL)";
                return;
            case 0x4F:
                Instruction = "LD C,A";
                return;
            #endregion

            #region 0x50
            case 0x50:
                Instruction = "LD D,B";
                return;
            case 0x51:
                Instruction = "LD D,C";
                return;
            case 0x52:
                Instruction = "LD D,D";
                return;
            case 0x53:
                Instruction = "LD D,E";
                return;
            case 0x54:
                Instruction = "LD D,H";
                return;
            case 0x55:
                Instruction = "LD D,L";
                return;
            case 0x56:
                Instruction = "LD D,(HL)";
                return;
            case 0x57:
                Instruction = "LD D,A";
                return;

            case 0x58:
                Instruction = "LD E,B";
                return;
            case 0x59:
                Instruction = "LD E,C";
                return;
            case 0x5A:
                Instruction = "LD E,D";
                return;
            case 0x5B:
                Instruction = "LD E,E";
                return;
            case 0x5C:
                Instruction = "LD E,H";
                return;
            case 0x5D:
                Instruction = "LD E,L";
                return;
            case 0x5E:
                Instruction = "LD E,(HL)";
                return;
            case 0x5F:
                Instruction = "LD E,A";
                return;
            #endregion

            #region 0x60
            case 0x60:
                Instruction = "LD H,B";
                return;
            case 0x61:
                Instruction = "LD H,C";
                return;
            case 0x62:
                Instruction = "LD H,D";
                return;
            case 0x63:
                Instruction = "LD H,E";
                return;
            case 0x64:
                Instruction = "LD H,H";
                return;
            case 0x65:
                Instruction = "LD H,L";
                return;
            case 0x66:
                Instruction = "LD H,(HL)";
                return;
            case 0x67:
                Instruction = "LD H,A";
                return;

            case 0x68:
                Instruction = "LD L,B";
                return;
            case 0x69:
                Instruction = "LD L,C";
                return;
            case 0x6A:
                Instruction = "LD L,D";
                return;
            case 0x6B:
                Instruction = "LD L,E";
                return;
            case 0x6C:
                Instruction = "LD L,H";
                return;
            case 0x6D:
                Instruction = "LD L,L";
                return;
            case 0x6E:
                Instruction = "LD L,(HL)";
                return;
            case 0x6F:
                Instruction = "LD L,A";
                return;
            #endregion

            #region 0x70
            case 0x70:
                Instruction = "LD (HL),B";
                return;
            case 0x71:
                Instruction = "LD (HL),C";
                return;
            case 0x72:
                Instruction = "LD (HL),D";
                return;
            case 0x73:
                Instruction = "LD (HL),E";
                return;
            case 0x74:
                Instruction = "LD (HL),H";
                return;
            case 0x75:
                Instruction = "LD (HL),L";
                return;
            case 0x77:
                Instruction = "LD (HL),A";
                return;

            case 0x78:
                Instruction = "LD A,B";
                return;
            case 0x79:
                Instruction = "LD A,C";
                return;
            case 0x7A:
                Instruction = "LD A,D";
                return;
            case 0x7B:
                Instruction = "LD A,E";
                return;
            case 0x7C:
                Instruction = "LD A,H";
                return;
            case 0x7D:
                Instruction = "LD A,L";
                return;
            case 0x7E:
                Instruction = "LD A,(HL)";
                return;
            case 0x7F:
                Instruction = "LD A,A";
                return;
            #endregion

            #region 0x02
            case 0x02:
                Instruction = "LD (BC),A";
                return;
            case 0x12:
                Instruction = "LD (DE),A";
                return;
            case 0x22:
                Instruction = "LD (HL+),A";
                return;
            case 0x32:
                Instruction = "LD (HL-),A";
                return;
            #endregion

            #region 0x06
            case 0x06:
                Instruction = $"LD B,{u8:X2}";
                return;
            case 0x16:
                Instruction = $"LD D,{u8:X2}";
                return;
            case 0x26:
                Instruction = $"LD H,{u8:X2}";
                return;
            case 0x36:
                Instruction = $"LD (HL),{u8:X2}";
                return;
            #endregion

            #region 0x0A
            case 0x0A:
                Instruction = "LD A,(BC)";
                return;
            case 0x1A:
                Instruction = "LD A,(DE)";
                return;
            case 0x2A:
                Instruction = "LD A,(HL+)";
                return;
            case 0x3A:
                Instruction = "LD A,(HL-)";
                return;
            #endregion

            #region 0x0E
            case 0x0E:
                Instruction = $"LD C,{u8:X2}";
                return;
            case 0x1E:
                Instruction = $"LD E,{u8:X2}";
                return;
            case 0x2E:
                Instruction = $"LD L,{u8:X2}";
                return;
            case 0x3E:
                Instruction = $"LD A,{u8:X2}";
                return;
            #endregion

            #region 0xE0 0xF0
            case 0xE0:
                Instruction = $"LD (0xFF00+{u8:X2}),A";
                return;
            case 0xF0:
                Instruction = $"LD A,(0xFF00+{u8:X2})";
                return;
            case 0xE2:
                Instruction = "LD (0xFF00+C),A";
                return;
            case 0xF2:
                Instruction = "LD A,(0xFF00+C)";
                return;
            case 0xEA:
                Instruction = $"LD ({u16}:X4),A";
                return;
            case 0xFA:
                Instruction = $"LD A,{u16:X4}";
                return;
            #endregion
            #endregion

            #region x16/lsm
            case 0x01:
                Instruction = $"LD BC,{u16:X4}";
                return;
            case 0x11:
                Instruction = $"LD DE,{u16:X4}";
                return;
            case 0x21:
                Instruction = $"LD HL,{u16:X4}";
                return;
            case 0x31:
                Instruction = $"LD SP,{u16:X4}";
                return;

            case 0x08:
                Instruction = $"LD {u16:X4},SP";
                return;

            case 0xC1:
                Instruction = "POP BC";
                return;
            case 0xD1:
                Instruction = "POP DE";
                return;
            case 0xE1:
                Instruction = "POP HL";
                return;
            case 0xF1:
                Instruction = "POP AF";
                return;

            case 0xC5:
                Instruction = "PUSH BC";
                return;
            case 0xD5:
                Instruction = "PUSH DE";
                return;
            case 0xE5:
                Instruction = "PUSH HL";
                return;
            case 0xF5:
                Instruction = "PUSH AF";
                return;

            case 0xF9:
                Instruction = "LD SP,HL";
                return;
            #endregion

            #region x8/alu
            #region ADD
            case 0x80:
                Instruction = "ADD A,B";
                return;
            case 0x81:
                Instruction = "ADD A,C";
                return;
            case 0x82:
                Instruction = "ADD A,D";
                return;
            case 0x83:
                Instruction = "ADD A,E";
                return;
            case 0x84:
                Instruction = "ADD A,H";
                return;
            case 0x85:
                Instruction = "ADD A,L";
                return;
            case 0x86:
                Instruction = "ADD A,(HL)";
                return;
            case 0x87:
                Instruction = "ADD A,A";
                return;
            case 0xC6:
                Instruction = $"ADD A,{u8:X2}";
                return;
            #endregion

            #region SUB
            case 0x90:
                Instruction = "SUB A,B";
                return;
            case 0x91:
                Instruction = "SUB A,C";
                return;
            case 0x92:
                Instruction = "SUB A,D";
                return;
            case 0x93:
                Instruction = "SUB A,E";
                return;
            case 0x94:
                Instruction = "SUB A,H";
                return;
            case 0x95:
                Instruction = "SUB A,L";
                return;
            case 0x96:
                Instruction = "SUB A,(HL)";
                return;
            case 0x97:
                Instruction = "SUB A,B";
                return;
            case 0xD6:
                Instruction = $"SUB A,{u8:X2}";
                return;
            #endregion

            #region ADC
            case 0x88:
                Instruction = "ADC A,B";
                return;
            case 0x89:
                Instruction = "ADC A,C";
                return;
            case 0x8A:
                Instruction = "ADC A,D";
                return;
            case 0x8B:
                Instruction = "ADC A,E";
                return;
            case 0x8C:
                Instruction = "ADC A,H";
                return;
            case 0x8D:
                Instruction = "ADC A,L";
                return;
            case 0x8E:
                Instruction = "ADC A,(HL)";
                return;
            case 0x8F:
                Instruction = "ADC A,A";
                return;
            case 0xCE:
                Instruction = $"ADC A,{u8:X2}";
                return;
            #endregion

            #region SBC
            case 0x98:
                Instruction = "SBC A,B";
                return;
            case 0x99:
                Instruction = "SBC A,C";
                return;
            case 0x9A:
                Instruction = "SBC A,D";
                return;
            case 0x9B:
                Instruction = "SBC A,E";
                return;
            case 0x9C:
                Instruction = "SBC A,H";
                return;
            case 0x9D:
                Instruction = "SBC A,L";
                return;
            case 0x9E:
                Instruction = "SBC A,(HL)";
                return;
            case 0x9F:
                Instruction = "SBC A,A";
                return;
            case 0xDE:
                Instruction = $"SBC A,{u8:X2}";
                return;
            #endregion

            #region INC
            case 0x04:
                Instruction = "INC B";
                return;
            case 0x14:
                Instruction = "INC D";
                return;
            case 0x24:
                Instruction = "INC H";
                return;
            case 0x34:
                Instruction = "INC (HL)";
                return;

            case 0x0C:
                Instruction = "INC C";
                return;
            case 0x1C:
                Instruction = "INC E";
                return;
            case 0x2C:
                Instruction = "INC L";
                return;
            case 0x3C:
                Instruction = "INC A";
                return;
            #endregion

            #region DEC
            case 0x05:
                Instruction = "DEC B";
                return;
            case 0x15:
                Instruction = "DEC D";
                return;
            case 0x25:
                Instruction = "DEC H";
                return;
            case 0x35:
                Instruction = "DEC (HL)";
                return;

            case 0x0D:
                Instruction = "DEC C";
                return;
            case 0x1D:
                Instruction = "DEC E";
                return;
            case 0x2D:
                Instruction = "DEC L";
                return;
            case 0x3D:
                Instruction = "DEC A";
                return;
            #endregion

            #region AND
            case 0xA0:
                Instruction = "AND A,B";
                return;
            case 0xA1:
                Instruction = "AND A,C";
                return;
            case 0xA2:
                Instruction = "AND A,D";
                return;
            case 0xA3:
                Instruction = "AND A,E";
                return;
            case 0xA4:
                Instruction = "AND A,H";
                return;
            case 0xA5:
                Instruction = "AND A,L";
                return;
            case 0xA6:
                Instruction = "AND A,(HL)";
                return;
            case 0xA7:
                Instruction = "AND A,A";
                return;
            case 0xE6:
                Instruction = $"AND A,{u8:X2}";
                return;
            #endregion

            #region OR
            case 0xB0:
                Instruction = "OR A,B";
                return;
            case 0xB1:
                Instruction = "OR A,C";
                return;
            case 0xB2:
                Instruction = "OR A,D";
                return;
            case 0xB3:
                Instruction = "OR A,E";
                return;
            case 0xB4:
                Instruction = "OR A,H";
                return;
            case 0xB5:
                Instruction = "OR A,L";
                return;
            case 0xB6:
                Instruction = "OR A,(HL)";
                return;
            case 0xB7:
                Instruction = "OR A,A";
                return;
            case 0xF6:
                Instruction = $"OR A,{u8:X2}";
                return;
            #endregion

            #region XOR
            case 0xA8:
                Instruction = "XOR A,B";
                return;
            case 0xA9:
                Instruction = "XOR A,C";
                return;
            case 0xAA:
                Instruction = "XOR A,D";
                return;
            case 0xAB:
                Instruction = "XOR A,E";
                return;
            case 0xAC:
                Instruction = "XOR A,H";
                return;
            case 0xAD:
                Instruction = "XOR A,L";
                return;
            case 0xAE:
                Instruction = "XOR A,(HL)";
                return;
            case 0xAF:
                Instruction = "XOR A,A";
                return;
            case 0xEE:
                Instruction = $"XOR A,{u8:X2}";
                return;
            #endregion

            #region CP
            case 0xB8:
                Instruction = "CP A,B";
                return;
            case 0xB9:
                Instruction = "CP A,C";
                return;
            case 0xBA:
                Instruction = "CP A,D";
                return;
            case 0xBB:
                Instruction = "CP A,E";
                return;
            case 0xBC:
                Instruction = "CP A,H";
                return;
            case 0xBD:
                Instruction = "CP A,L";
                return;
            case 0xBE:
                Instruction = "CP A,(HL)";
                return;
            case 0xBF:
                Instruction = "CP A,A";
                return;
            case 0xFE:
                Instruction = $"CP A,{u8:X2}";
                return;
            #endregion

            #region MISC
            case 0x27:
                Instruction = "DAA";
                return;
            case 0x37:
                Instruction = "SCF";
                return;

            case 0x2F:
                Instruction = "CPL";
                return;
            case 0x3F:
                Instruction = "CCF";
                return;
            #endregion

            #endregion

            #region x16/alu
            #region INC
            case 0x03:
                Instruction = "INC BC";
                return;
            case 0x13:
                Instruction = "INC DE";
                return;
            case 0x23:
                Instruction = "INC HL";
                return;
            case 0x33:
                Instruction = "INC SP";
                return;
            #endregion

            #region DEC
            case 0x0B:
                Instruction = "DEC BC";
                return;
            case 0x1B:
                Instruction = "DEC DE";
                return;
            case 0x2B:
                Instruction = "DEC HL";
                return;
            case 0x3B:
                Instruction = "DEC SP";
                return;
            #endregion

            #region ADD
            case 0x09:
                Instruction = "ADD HL,BC";
                return;
            case 0x19:
                Instruction = "ADD HL,DE";
                return;
            case 0x29:
                Instruction = "ADD HL,HL";
                return;
            case 0x39:
                Instruction = "ADD HL,SP";
                return;

            case 0xE8:
                Instruction = $"ADD SP,{i8:X2}";
                return;
            #endregion

            case 0xF8:
                Instruction = $"LD HL,SP+{i8:X2}";
                return;

            #endregion

            #region x8/rsb
            case 0x07:
                Instruction = "RLCA";
                return;
            case 0x17:
                Instruction = "RLA";
                return;
            case 0x0F:
                Instruction = "RRCA";
                return;
            case 0x1F:
                Instruction = "RRA";
                return;
            #endregion

            #region control/br
            #region JR
            case 0x20:
                Instruction = $"JR NZ,{i8:X2}";
                return;
            case 0x30:
                Instruction = $"JR C,{i8:X2}";
                return;
            case 0x28:
                Instruction = $"JR Z,{i8:X2}";
                return;
            case 0x38:
                Instruction = $"JR C,{i8:X2}";
                return;
            case 0x18:
                Instruction = $"JR {i8:X2}";
                return;
            #endregion

            #region JP
            case 0xC2:
                Instruction = $"JP NZ,{u16:X4}";
                return;
            case 0xD2:
                Instruction = $"JP NC,{u16:X4}";
                return;
            case 0xC3:
                Instruction = $"JP {u16:X4}";
                return;
            case 0xE9:
                Instruction = "JP HL";
                return;
            case 0xCA:
                Instruction = $"JP Z,{u16:X4}";
                return;
            case 0xDA:
                Instruction = $"JP C,{u16:X4}";
                return;
            #endregion

            #region CALL
            case 0xCD:
                Instruction = $"CALL {u16:X4}";
                return;
            case 0xC4:
                Instruction = $"CALL NZ,{u16:X4}";
                return;
            case 0xD4:
                Instruction = $"CALL NC,{u16:X4}";
                return;
            case 0xCC:
                Instruction = $"CALL Z,{u16:X4}";
                return;
            case 0xDC:
                Instruction = $"CALL C,{u16:X4}";
                return;
            #endregion

            #region RST
            case 0xC7:
                Instruction = "RST 00h";
                return;
            case 0xD7:
                Instruction = "RST 10h";
                return;
            case 0xE7:
                Instruction = "RST 20h";
                return;
            case 0xF7:
                Instruction = "RST 30h";
                return;

            case 0xCF:
                Instruction = "RST 08h";
                return;
            case 0xDF:
                Instruction = "RST 18h";
                return;
            case 0xEF:
                Instruction = "RST 28h";
                return;
            case 0xFF:
                Instruction = "RST 38h";
                return;
            #endregion

            #region RET
            case 0xC0:
                Instruction = "RET NZ";
                return;
            case 0xD0:
                Instruction = "RET NC";
                return;

            case 0xC8:
                Instruction = "RET Z";
                return;
            case 0xD8:
                Instruction = "RET C";
                return;

            case 0xC9:
                Instruction = "RET";
                return;
            case 0xD9:
                Instruction = "RETI";
                return;
            #endregion
            #endregion

            #region control/misc
            case 0x00:
                Instruction = "NOP";
                return;
            case 0x10:
                Instruction = "STOP";
                return;
            case 0x76:
                Instruction = "HALT";
                return;
            case 0xFB:
                Instruction = "EI";
                return;
            case 0xF3:
                Instruction = "DI";
                return;
            case 0xCB:
                string r8 = "";
                int bit = (Read(PC) & 0x38) >> 3;

                switch (Read(PC) & 0x07)
                {
                    case 0:
                        r8 = "B";
                        break;
                    case 1:
                        r8 = "C";
                        break;
                    case 2:
                        r8 = "D";
                        break;
                    case 3:
                        r8 = "E";
                        break;
                    case 4:
                        r8 = "H";
                        break;
                    case 5:
                        r8 = "L";
                        break;
                    case 6:
                        r8 = "(HL)";
                        break;
                    case 7:
                        r8 = "A";
                        break;
                }

                switch (Read(PC) & 0xC0)
                {
                    case 0x00:
                        switch (bit)
                        {
                            case 0:
                                Instruction = $"RLC {r8}";
                                break;
                            case 1:
                                Instruction = $"RRC {r8}";
                                break;
                            case 2:
                                Instruction = $"RL {r8}";
                                break;
                            case 3:
                                Instruction = $"RR {r8}";
                                break;
                            case 4:
                                Instruction = $"SLA {r8}";
                                break;
                            case 5:
                                Instruction = $"SRA {r8}";
                                break;
                            case 6:
                                Instruction = $"SWAP {r8}";
                                break;
                            case 7:
                                Instruction = $"SRL {r8}";
                                break;
                        }
                        break;
                    case 0x40:
                        Instruction = $"BIT {bit},{r8}";
                        break;
                    case 0x80:
                        Instruction = $"RES {bit},{r8}";
                        break;
                    case 0xC0:
                        Instruction = $"SET {bit},{r8}";
                        break;
                }
                return;
            #endregion

            default:
                Instruction = "ILLEGAL";
                return;
        }
    }

    
    /// <summary>
    /// Executes the opcode fetched from <seealso cref="Fetch"/>
    /// </summary>
    /// <returns>The number of cycles it took in machine cycles (1, 2, 3, 4 instead of 4, 8, 12, 16)</returns>
    public int ExecuteUnprefixed()
    {
        // quick shortcuts
        ushort u16 = (ushort)((Read((ushort)(PC + 1)) << 8) | Read(PC));
        byte u8 = Read(PC);
        sbyte i8 = (sbyte)Read(PC);
        
        // in case of use
        int iTemp = 0;
        bool bTemp = false;

        // uhh, i couldve used the method i used for the cb prefix opcodes but
        // i also wanted to make it easier to debug, sorry lol.
        switch (Opcode)
        {
            #region x8/lsm
            #region 0x40
            case 0x40:
                Instruction = "LD B,B";
                B = B;
                return 1;
            case 0x41:
                Instruction = "LD B,C";
                B = C;
                return 1;
            case 0x42:
                Instruction = "LD B,D";
                B = D;
                return 1;
            case 0x43:
                Instruction = "LD B,E";
                B = E;
                return 1;
            case 0x44:
                Instruction = "LD B,H";
                B = H;
                return 1;
            case 0x45:
                Instruction = "LD B,L";
                B = L;
                return 1;
            case 0x46:
                Instruction = "LD B,(HL)";
                B = Read(HL);
                return 2;
            case 0x47:
                Instruction = "LD B,A";
                B = A;
                return 1;

            case 0x48:
                Instruction = "LD C,B";
                C = B;
                return 1;
            case 0x49:
                Instruction = "LD C,C";
                C = C;
                return 1;
            case 0x4A:
                Instruction = "LD C,D";
                C = D;
                return 1;
            case 0x4B:
                Instruction = "LD C,E";
                C = E;
                return 1;
            case 0x4C:
                Instruction = "LD C,H";
                C = H;
                return 1;
            case 0x4D:
                Instruction = "LD C,L";
                C = L;
                return 1;
            case 0x4E:
                Instruction = "LD C,(HL)";
                C = Read(HL);
                return 2;
            case 0x4F:
                Instruction = "LD C,A";
                C = A;
                return 1;
            #endregion

            #region 0x50
            case 0x50:
                Instruction = "LD D,B";
                D = B;
                return 1;
            case 0x51:
                Instruction = "LD D,C";
                D = C;
                return 1;
            case 0x52:
                Instruction = "LD D,D";
                D = D;
                return 1;
            case 0x53:
                Instruction = "LD D,E";
                D = E;
                return 1;
            case 0x54:
                Instruction = "LD D,H";
                D = H;
                return 1;
            case 0x55:
                Instruction = "LD D,L";
                D = L;
                return 1;
            case 0x56:
                Instruction = "LD D,(HL)";
                D = Read(HL);
                return 2;
            case 0x57:
                Instruction = "LD D,A";
                D = A;
                return 1;

            case 0x58:
                Instruction = "LD E,B";
                E = B;
                return 1;
            case 0x59:
                Instruction = "LD E,C";
                E = C;
                return 1;
            case 0x5A:
                Instruction = "LD E,D";
                E = D;
                return 1;
            case 0x5B:
                Instruction = "LD E,E";
                E = E;
                return 1;
            case 0x5C:
                Instruction = "LD E,H";
                E = H;
                return 1;
            case 0x5D:
                Instruction = "LD E,L";
                E = L;
                return 1;
            case 0x5E:
                Instruction = "LD E,(HL)";
                E = Read(HL);
                return 2;
            case 0x5F:
                Instruction = "LD E,A";
                E = A;
                return 1;
            #endregion

            #region 0x60
            case 0x60:
                Instruction = "LD H,B";
                H = B;
                return 1;
            case 0x61:
                Instruction = "LD H,C";
                H = C;
                return 1;
            case 0x62:
                Instruction = "LD H,D";
                H = D;
                return 1;
            case 0x63:
                Instruction = "LD H,E";
                H = E;
                return 1;
            case 0x64:
                Instruction = "LD H,H";
                H = H;
                return 1;
            case 0x65:
                Instruction = "LD H,L";
                H = L;
                return 1;
            case 0x66:
                Instruction = "LD H,(HL)";
                H = Read(HL);
                return 2;
            case 0x67:
                Instruction = "LD H,A";
                H = A;
                return 1;

            case 0x68:
                Instruction = "LD L,B";
                L = B;
                return 1;
            case 0x69:
                Instruction = "LD L,C";
                L = C;
                return 1;
            case 0x6A:
                Instruction = "LD L,D";
                L = D;
                return 1;
            case 0x6B:
                Instruction = "LD L,E";
                L = E;
                return 1;
            case 0x6C:
                Instruction = "LD L,H";
                L = H;
                return 1;
            case 0x6D:
                Instruction = "LD L,L";
                L = L;
                return 1;
            case 0x6E:
                Instruction = "LD L,(HL)";
                L = Read(HL);
                return 2;
            case 0x6F:
                Instruction = "LD L,A";
                L = A;
                return 1;
            #endregion

            #region 0x70
            case 0x70:
                Instruction = "LD (HL),B";
                Write(HL, B);
                return 2;
            case 0x71:
                Instruction = "LD (HL),C";
                Write(HL, C);
                return 2;
            case 0x72:
                Instruction = "LD (HL),D";
                Write(HL, D);
                return 2;
            case 0x73:
                Instruction = "LD (HL),E";
                Write(HL, E);
                return 2;
            case 0x74:
                Instruction = "LD (HL),H";
                Write(HL, H);
                return 2;
            case 0x75:
                Instruction = "LD (HL),L";
                Write(HL, L);
                return 2;
            case 0x77:
                Instruction = "LD (HL),A";
                Write(HL, A);
                return 2;

            case 0x78:
                Instruction = "LD A,B";
                A = B;
                return 1;
            case 0x79:
                Instruction = "LD A,C";
                A = C;
                return 1;
            case 0x7A:
                Instruction = "LD A,D";
                A = D;
                return 1;
            case 0x7B:
                Instruction = "LD A,E";
                A = E;
                return 1;
            case 0x7C:
                Instruction = "LD A,H";
                A = H;
                return 1;
            case 0x7D:
                Instruction = "LD A,L";
                A = L;
                return 1;
            case 0x7E:
                Instruction = "LD A,(HL)";
                A = Read(HL);
                return 2;
            case 0x7F:
                Instruction = "LD A,A";
                A = A;
                return 1;
            #endregion

            #region 0x02
            case 0x02:
                Instruction = "LD (BC),A";
                Write(BC, A);
                return 2;
            case 0x12:
                Instruction = "LD (DE),A";
                Write(DE, A);
                return 2;
            case 0x22:
                Instruction = "LD (HL+),A";
                Write(HL, A);
                HL++;
                return 2;
            case 0x32:
                Instruction = "LD (HL-),A";
                Write(HL, A);
                HL--;
                return 2;
            #endregion

            #region 0x06
            case 0x06:
                Instruction = $"LD B,{u8:X2}";
                B = u8;
                PC++;
                return 2;
            case 0x16:
                Instruction = $"LD D,{u8:X2}";
                D = u8;
                PC++;
                return 2;
            case 0x26:
                Instruction = $"LD H,{u8:X2}";
                H = u8;
                PC++;
                return 2;
            case 0x36:
                Instruction = $"LD (HL),{u8:X2}";
                Write(HL, u8);
                PC++;
                return 3;
            #endregion

            #region 0x0A
            case 0x0A:
                Instruction = "LD A,(BC)";
                A = Read(BC);
                return 2;
            case 0x1A:
                Instruction = "LD A,(DE)";
                A = Read(DE);
                return 2;
            case 0x2A:
                Instruction = "LD A,(HL+)";
                A = Read(HL);
                HL++;
                return 2;
            case 0x3A:
                Instruction = "LD A,(HL-)";
                A = Read(HL);
                HL--;
                return 2;
            #endregion

            #region 0x0E
            case 0x0E:
                Instruction = $"LD C,{u8:X2}";
                C = u8;
                PC++;
                return 2;
            case 0x1E:
                Instruction = $"LD E,{u8:X2}";
                E = u8;
                PC++;
                return 2;
            case 0x2E:
                Instruction = $"LD L,{u8:X2}";
                L = u8;
                PC++;
                return 2;
            case 0x3E:
                Instruction = $"LD A,{u8:X2}";
                A = u8;
                PC++;
                return 2;
            #endregion

            #region 0xE0 0xF0
            case 0xE0:
                Instruction = $"LD (0xFF00+{u8:X2}),A";
                Write((ushort)(0xFF00 + u8), A);
                PC++;
                return 3;
            case 0xF0:
                Instruction = $"LD A,(0xFF00+{u8:X2})";
                A = Read((ushort)(0xFF00 + u8));
                PC++;
                return 3;
            case 0xE2:
                Instruction = "LD (0xFF00+C),A";
                Write((ushort)(0xFF00 + C), A);
                return 2;
            case 0xF2:
                Instruction = "LD A,(0xFF00+C)";
                A = Read((ushort)(0xFF00 + C));
                return 2;
            case 0xEA:
                Instruction = $"LD ({u16}:X4),A";
                Write(u16, A);
                PC += 2;
                return 4;
            case 0xFA:
                Instruction = $"LD A,({u16:X4})";
                A = Read(u16);
                PC += 2;
                return 4;
            #endregion
            #endregion

            #region x16/lsm
            case 0x01:
                Instruction = $"LD BC,{u16:X4}";
                BC = u16;
                PC += 2;
                return 3;
            case 0x11:
                Instruction = $"LD DE,{u16:X4}";
                DE = u16;
                PC += 2;
                return 3;
            case 0x21:
                Instruction = $"LD HL,{u16:X4}";
                HL = u16;
                PC += 2;
                return 3;
            case 0x31:
                Instruction = $"LD SP,{u16:X4}";
                SP = u16;
                PC += 2;
                return 3;

            case 0x08:
                Instruction = $"LD {u16:X4},SP";
                Write((ushort)(u16 + 1), (byte)((SP & 0xFF00) >> 8));
                Write(u16, (byte)(SP & 0x00FF));
                PC += 2;
                return 5;

            case 0xC1:
                Instruction = "POP BC";
                C = Read(SP);
                SP++;
                B = Read(SP);
                SP++;
                return 3;
            case 0xD1:
                Instruction = "POP DE";
                E = Read(SP);
                SP++;
                D = Read(SP);
                SP++;
                return 3;
            case 0xE1:
                Instruction = "POP HL";
                L = Read(SP);
                SP++;
                H = Read(SP);
                SP++;
                return 3;
            case 0xF1:
                Instruction = "POP AF";
                F = Read(SP);
                SP++;
                A = Read(SP);
                SP++;
                F &= 0xF0;
                return 3;

            case 0xC5:
                Instruction = "PUSH BC";
                SP--;
                Write(SP, B);
                SP--;
                Write(SP, C);
                return 4;
            case 0xD5:
                Instruction = "PUSH DE";
                SP--;
                Write(SP, D);
                SP--;
                Write(SP, E);
                return 4;
            case 0xE5:
                Instruction = "PUSH HL";
                SP--;
                Write(SP, H);
                SP--;
                Write(SP, L);
                return 4;
            case 0xF5:
                Instruction = "PUSH AF";
                SP--;
                Write(SP, A);
                SP--;
                Write(SP, F);
                return 4;

            case 0xF9:
                Instruction = "LD SP,HL";
                SP = HL;
                return 2;
            #endregion

            #region x8/alu
            #region ADD
            case 0x80:
                Instruction = "ADD A,B";
                NF = false;
                CF = A + B > 0xFF;
                HF = ((A & 0x0F) + (B & 0x0F)) > 0x0F;
                A += B;
                ZF = A == 0;
                return 1;
            case 0x81:
                Instruction = "ADD A,C";
                NF = false;
                CF = A + C > 0xFF;
                HF = ((A & 0x0F) + (C & 0x0F)) > 0x0F;
                A += C;
                ZF = A == 0;
                return 1;
            case 0x82:
                Instruction = "ADD A,D";
                NF = false;
                CF = A + D > 0xFF;
                HF = ((A & 0x0F) + (D & 0x0F)) > 0x0F;
                A += D;
                ZF = A == 0;
                return 1;
            case 0x83:
                Instruction = "ADD A,E";
                NF = false;
                CF = A + E > 0xFF;
                HF = ((A & 0x0F) + (E & 0x0F)) > 0x0F;
                A += E;
                ZF = A == 0;
                return 1;
            case 0x84:
                Instruction = "ADD A,H";
                NF = false;
                CF = A + H > 0xFF;
                HF = ((A & 0x0F) + (H & 0x0F)) > 0x0F;
                A += H;
                ZF = A == 0;
                return 1;
            case 0x85:
                Instruction = "ADD A,L";
                NF = false;
                CF = A + L > 0xFF;
                HF = ((A & 0x0F) + (L & 0x0F)) > 0x0F;
                A += L;
                ZF = A == 0;
                return 1;
            case 0x86:
                Instruction = "ADD A,(HL)";
                NF = false;
                CF = A + Read(HL) > 0xFF;
                HF = ((A & 0x0F) + (Read(HL) & 0x0F)) > 0x0F;
                A += Read(HL);
                ZF = A == 0;
                return 2;
            case 0x87:
                Instruction = "ADD A,A";
                NF = false;
                CF = A + A > 0xFF;
                HF = ((A & 0x0F) + (A & 0x0F)) > 0x0F;
                A += A;
                ZF = A == 0;
                return 1;
            case 0xC6:
                Instruction = $"ADD A,{u8:X2}";
                NF = false;
                CF = A + u8 > 0xFF;
                HF = ((A & 0x0F) + (u8 & 0x0F)) > 0x0F;
                A += u8;
                ZF = A == 0;
                PC++;
                return 2;
            #endregion

            #region SUB
            case 0x90:
                Instruction = "SUB A,B";
                NF = true;
                HF = (A & 0x0F) < (B & 0x0F);
                CF = A < B;
                A -= B;
                ZF = A == 0;
                return 1;
            case 0x91:
                Instruction = "SUB A,C";
                NF = true;
                HF = (A & 0x0F) < (C & 0x0F);
                CF = A < C;
                A -= C;
                ZF = A == 0;
                return 1;
            case 0x92:
                Instruction = "SUB A,D";
                NF = true;
                HF = (A & 0x0F) < (D & 0x0F);
                CF = A < D;
                A -= D;
                ZF = A == 0;
                return 1;
            case 0x93:
                Instruction = "SUB A,E";
                NF = true;
                HF = (A & 0x0F) < (E & 0x0F);
                CF = A < E;
                A -= E;
                ZF = A == 0;
                return 1;
            case 0x94:
                Instruction = "SUB A,H";
                NF = true;
                HF = (A & 0x0F) < (H & 0x0F);
                CF = A < H;
                A -= H;
                ZF = A == 0;
                return 1;
            case 0x95:
                Instruction = "SUB A,L";
                NF = true;
                HF = (A & 0x0F) < (L & 0x0F);
                CF = A < L;
                A -= L;
                ZF = A == 0;
                return 1;
            case 0x96:
                Instruction = "SUB A,(HL)";
                NF = true;
                HF = (A & 0x0F) < (Read(HL) & 0x0F);
                CF = A < Read(HL);
                A -= Read(HL);
                ZF = A == 0;
                return 2;
            case 0x97:
                Instruction = "SUB A,B";
                NF = true;
                HF = (A & 0x0F) < (A & 0x0F);
                CF = A < A;
                A -= A;
                ZF = A == 0;
                return 1;
            case 0xD6:
                Instruction = $"SUB A,{u8:X2}";
                NF = true;
                HF = (A & 0x0F) < (u8 & 0x0F);
                CF = A < u8;
                A -= u8;
                ZF = A == 0;
                PC++;
                return 2;
            #endregion

            #region ADC
            case 0x88:
                Instruction = "ADC A,B";
                bTemp = CF;
                iTemp = A + B + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (B & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x89:
                Instruction = "ADC A,C";
                bTemp = CF;
                iTemp = A + C + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (C & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x8A:
                Instruction = "ADC A,D";
                bTemp = CF;
                iTemp = A + D + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (D & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x8B:
                Instruction = "ADC A,E";
                bTemp = CF;
                iTemp = A + E + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (E & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x8C:
                Instruction = "ADC A,H";
                bTemp = CF;
                iTemp = A + H + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (H & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x8D:
                Instruction = "ADC A,L";
                bTemp = CF;
                iTemp = A + L + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (L & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x8E:
                Instruction = "ADC A,(HL)";
                bTemp = CF;
                iTemp = A + Read(HL) + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (Read(HL) & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                return 2;
            case 0x8F:
                Instruction = "ADC A,A";
                bTemp = CF;
                iTemp = A + A + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (A & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0xCE:
                Instruction = $"ADC A,{u8:X2}";
                bTemp = CF;
                iTemp = A + u8 + (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = false;
                HF = ((A & 0x0F) + (u8 & 0x0F) + (bTemp ? 1 : 0)) > 0x0F;
                CF = iTemp > 0xFF;
                A = (byte)(iTemp & 0xFF);
                PC++;
                return 2;
            #endregion

            #region SBC
            case 0x98:
                Instruction = "SBC A,B";
                iTemp = A - B - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (B & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (B + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x99:
                Instruction = "SBC A,C";
                iTemp = A - C - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (C & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (C + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x9A:
                Instruction = "SBC A,D";
                iTemp = A - D - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (D & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (D + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x9B:
                Instruction = "SBC A,E";
                iTemp = A - E - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (E & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (E + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x9C:
                Instruction = "SBC A,H";
                iTemp = A - H - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (H & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (H + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x9D:
                Instruction = "SBC A,L";
                iTemp = A - L - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (L & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (L + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0x9E:
                Instruction = "SBC A,(HL)";
                iTemp = A - Read(HL) - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (Read(HL) & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (Read(HL) + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                return 2;
            case 0x9F:
                Instruction = "SBC A,A";
                iTemp = A - A - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (A & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (A + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                return 1;
            case 0xDE:
                Instruction = $"SBC A,{u8:X2}";
                iTemp = A - u8 - (CF ? 1 : 0);
                ZF = (iTemp & 0xFF) == 0;
                NF = true;
                HF = ((A & 0x0F) - (u8 & 0x0F) - (CF ? 1 : 0)) < 0;
                CF = A < (u8 + (CF ? 1 : 0));
                A = (byte)(iTemp & 0xFF);
                PC++;
                return 2;
            #endregion

            #region INC
            case 0x04:
                Instruction = "INC B";
                B++;
                NF = false;
                ZF = B == 0;
                HF = (B & 0x0F) == 0;
                return 1;
            case 0x14:
                Instruction = "INC D";
                D++;
                NF = false;
                ZF = D == 0;
                HF = (D & 0x0F) == 0;
                return 1;
            case 0x24:
                Instruction = "INC H";
                H++;
                NF = false;
                ZF = H == 0;
                HF = (H & 0x0F) == 0;
                return 1;
            case 0x34:
                Instruction = "INC (HL)";
                u8 = Read(HL); // temp hijack u8
                u8++;
                Write(HL, u8);
                NF = false;
                ZF = u8 == 0;
                HF = (u8 & 0x0F) == 0;
                return 3;

            case 0x0C:
                Instruction = "INC C";
                C++;
                NF = false;
                ZF = C == 0;
                HF = (C & 0x0F) == 0;
                return 1;
            case 0x1C:
                Instruction = "INC E";
                E++;
                NF = false;
                ZF = E == 0;
                HF = (E & 0x0F) == 0;
                return 1;
            case 0x2C:
                Instruction = "INC L";
                L++;
                NF = false;
                ZF = L == 0;
                HF = (L & 0x0F) == 0;
                return 1;
            case 0x3C:
                Instruction = "INC A";
                A++;
                NF = false;
                ZF = A == 0;
                HF = (A & 0x0F) == 0;
                return 1;
            #endregion

            #region DEC
            case 0x05:
                Instruction = "DEC B";
                B--;
                ZF = B == 0;
                NF = true;
                HF = (B & 0x0F) == 0x0F;
                return 1;
            case 0x15:
                Instruction = "DEC D";
                D--;
                ZF = D == 0;
                NF = true;
                HF = (D & 0x0F) == 0x0F;
                return 1;
            case 0x25:
                Instruction = "DEC H";
                H--;
                ZF = H == 0;
                NF = true;
                HF = (H & 0x0F) == 0x0F;
                return 1;
            case 0x35:
                Instruction = "DEC (HL)";
                u8 = Read(HL);
                u8--;
                Write(HL, u8);
                ZF = u8 == 0;
                NF = true;
                HF = (u8 & 0x0F) == 0x0F;
                return 3;

            case 0x0D:
                Instruction = "DEC C";
                C--;
                ZF = C == 0;
                NF = true;
                HF = (C & 0x0F) == 0x0F;
                return 1;
            case 0x1D:
                Instruction = "DEC E";
                E--;
                ZF = E == 0;
                NF = true;
                HF = (E & 0x0F) == 0x0F;
                return 1;
            case 0x2D:
                Instruction = "DEC L";
                L--;
                ZF = L == 0;
                NF = true;
                HF = (L & 0x0F) == 0x0F;
                return 1;
            case 0x3D:
                Instruction = "DEC A";
                A--;
                ZF = A == 0;
                NF = true;
                HF = (A & 0x0F) == 0x0F;
                return 1;
            #endregion

            #region AND
            case 0xA0:
                Instruction = "AND A,B";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & B) == 0;
                A &= B;
                return 1;
            case 0xA1:
                Instruction = "AND A,C";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & C) == 0;
                A &= C;
                return 1;
            case 0xA2:
                Instruction = "AND A,D";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & D) == 0;
                A &= D;
                return 1;
            case 0xA3:
                Instruction = "AND A,E";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & E) == 0;
                A &= E;
                return 1;
            case 0xA4:
                Instruction = "AND A,H";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & H) == 0;
                A &= H;
                return 1;
            case 0xA5:
                Instruction = "AND A,L";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & L) == 0;
                A &= L;
                return 1;
            case 0xA6:
                Instruction = "AND A,(HL)";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & Read(HL)) == 0;
                A &= Read(HL);
                return 2;
            case 0xA7:
                Instruction = "AND A,A";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & A) == 0;
                A &= A;
                return 1;
            case 0xE6:
                Instruction = $"AND A,{u8:X2}";
                NF = false;
                HF = true;
                CF = false;
                ZF = (A & u8) == 0;
                A &= u8;
                PC++;
                return 2;
            #endregion

            #region OR
            case 0xB0:
                Instruction = "OR A,B";
                NF = false;
                HF = false;
                CF = false;
                A |= B;
                ZF = A == 0;
                return 1;
            case 0xB1:
                Instruction = "OR A,C";
                NF = false;
                HF = false;
                CF = false;
                A |= C;
                ZF = A == 0;
                return 1;
            case 0xB2:
                Instruction = "OR A,D";
                NF = false;
                HF = false;
                CF = false;
                A |= D;
                ZF = A == 0;
                return 1;
            case 0xB3:
                Instruction = "OR A,E";
                NF = false;
                HF = false;
                CF = false;
                A |= E;
                ZF = A == 0;
                return 1;
            case 0xB4:
                Instruction = "OR A,H";
                NF = false;
                HF = false;
                CF = false;
                A |= H;
                ZF = A == 0;
                return 1;
            case 0xB5:
                Instruction = "OR A,L";
                NF = false;
                HF = false;
                CF = false;
                A |= L;
                ZF = A == 0;
                return 1;
            case 0xB6:
                Instruction = "OR A,(HL)";
                NF = false;
                HF = false;
                CF = false;
                A |= Read(HL);
                ZF = A == 0;
                return 2;
            case 0xB7:
                Instruction = "OR A,A";
                NF = false;
                HF = false;
                CF = false;
                A |= A;
                ZF = A == 0;
                return 1;
            case 0xF6:
                Instruction = $"OR A,{u8:X2}";
                NF = false;
                HF = false;
                CF = false;
                A |= u8;
                ZF = A == 0;
                PC++;
                return 2;
            #endregion

            #region XOR
            case 0xA8:
                Instruction = "XOR A,B";
                NF = false;
                HF = false;
                CF = false;
                A ^= B;
                ZF = A == 0;
                return 1;
            case 0xA9:
                Instruction = "XOR A,C";
                NF = false;
                HF = false;
                CF = false;
                A ^= C;
                ZF = A == 0;
                return 1;
            case 0xAA:
                Instruction = "XOR A,D";
                NF = false;
                HF = false;
                CF = false;
                A ^= D;
                ZF = A == 0;
                return 1;
            case 0xAB:
                Instruction = "XOR A,E";
                NF = false;
                HF = false;
                CF = false;
                A ^= E;
                ZF = A == 0;
                return 1;
            case 0xAC:
                Instruction = "XOR A,H";
                NF = false;
                HF = false;
                CF = false;
                A ^= H;
                ZF = A == 0;
                return 1;
            case 0xAD:
                Instruction = "XOR A,L";
                NF = false;
                HF = false;
                CF = false;
                A ^= L;
                ZF = A == 0;
                return 1;
            case 0xAE:
                Instruction = "XOR A,(HL)";
                NF = false;
                HF = false;
                CF = false;
                A ^= Read(HL);
                ZF = A == 0;
                return 2;
            case 0xAF:
                Instruction = "XOR A,AA";
                NF = false;
                HF = false;
                CF = false;
                A ^= A;
                ZF = A == 0;
                return 1;
            case 0xEE:
                Instruction = $"XOR A,{u8:X2}";
                NF = false;
                HF = false;
                CF = false;
                A ^= u8;
                ZF = A == 0;
                PC++;
                return 2;
            #endregion

            #region CP
            case 0xB8:
                Instruction = "CP A,B";
                NF = true;
                HF = (A & 0x0F) < (B & 0x0F);
                CF = A < B;
                ZF = (A - B) == 0;
                return 1;
            case 0xB9:
                Instruction = "CP A,C";
                NF = true;
                HF = (A & 0x0F) < (C & 0x0F);
                CF = A < C;
                ZF = (A - C) == 0;
                return 1;
            case 0xBA:
                Instruction = "CP A,D";
                NF = true;
                HF = (A & 0x0F) < (D & 0x0F);
                CF = A < D;
                ZF = (A - D) == 0;
                return 1;
            case 0xBB:
                Instruction = "CP A,E";
                NF = true;
                HF = (A & 0x0F) < (E & 0x0F);
                CF = A < E;
                ZF = (A - E) == 0;
                return 1;
            case 0xBC:
                Instruction = "CP A,H";
                NF = true;
                HF = (A & 0x0F) < (H & 0x0F);
                CF = A < H;
                ZF = (A - H) == 0;
                return 1;
            case 0xBD:
                Instruction = "CP A,L";
                NF = true;
                HF = (A & 0x0F) < (L & 0x0F);
                CF = A < L;
                ZF = (A - L) == 0;
                return 1;
            case 0xBE:
                Instruction = "CP A,(HL)";
                NF = true;
                HF = (A & 0x0F) < (Read(HL) & 0x0F);
                CF = A < Read(HL);
                ZF = (A - Read(HL)) == 0;
                return 2;
            case 0xBF:
                Instruction = "CP A,A";
                NF = true;
                HF = (A & 0x0F) < (A & 0x0F);
                CF = A < A;
                ZF = (A - A) == 0;
                return 1;
            case 0xFE:
                Instruction = $"CP A,{u8:X2}";
                NF = true;
                HF = (A & 0x0F) < (u8 & 0x0F);
                CF = A < u8;
                ZF = (A - u8) == 0;
                PC++;
                return 2;
            #endregion

            #region MISC
            case 0x27:
                Instruction = "DAA";
                if (!NF)
                {
                    if (CF || A > 0x99)
                    {
                        A += 0x60;
                        CF = true;
                    }
                    if (HF || (A & 0x0F) > 0x09)
                    {
                        A += 0x06;
                    }
                }
                else
                {
                    if (CF)
                        A -= 0x60;
                    if (HF)
                        A -= 0x06;
                }

                ZF = A == 0;
                HF = false;

                return 1;
            case 0x37:
                Instruction = "SCF";
                NF = false;
                HF = false;
                CF = true;
                return 1;

            case 0x2F:
                Instruction = "CPL";
                NF = true;
                HF = true;
                A = (byte)~A;
                return 1;
            case 0x3F:
                Instruction = "CCF";
                NF = false;
                HF = false;
                CF = !CF;
                return 1;
            #endregion

            #endregion

            #region x16/alu
            #region INC
            case 0x03:
                Instruction = "INC BC";
                BC++;
                return 2;
            case 0x13:
                Instruction = "INC DE";
                DE++;
                return 2;
            case 0x23:
                Instruction = "INC HL";
                HL++;
                return 2;
            case 0x33:
                Instruction = "INC SP";
                SP++;
                return 2;
            #endregion

            #region DEC
            case 0x0B:
                Instruction = "DEC BC";
                BC--;
                return 2;
            case 0x1B:
                Instruction = "DEC DE";
                DE--;
                return 2;
            case 0x2B:
                Instruction = "DEC HL";
                HL--;
                return 2;
            case 0x3B:
                Instruction = "DEC SP";
                SP--;
                return 2;
            #endregion

            #region ADD
            case 0x09:
                Instruction = "ADD HL,BC";
                NF = false;
                HF = ((HL & 0x0FFF) + (BC & 0x0FFF)) > 0x0FFF;
                CF = HL + BC > 0xFFFF;

                HL += BC;
                return 2;
            case 0x19:
                Instruction = "ADD HL,DE";
                NF = false;
                HF = ((HL & 0x0FFF) + (DE & 0x0FFF)) > 0x0FFF;
                CF = HL + DE > 0xFFFF;

                HL += DE;
                return 2;
            case 0x29:
                Instruction = "ADD HL,HL";
                NF = false;
                HF = ((HL & 0x0FFF) + (HL & 0x0FFF)) > 0x0FFF;
                CF = HL + HL > 0xFFFF;

                HL += HL;
                return 2;
            case 0x39:
                Instruction = "ADD HL,SP";
                NF = false;
                HF = ((HL & 0x0FFF) + (SP & 0x0FFF)) > 0x0FFF;
                CF = HL + SP > 0xFFFF;

                HL += SP;
                return 2;

            case 0xE8:
                Instruction = $"ADD SP,{i8:X2}";
                int oldsp = SP;
                int newsp = SP + i8;

                SP = (ushort)newsp;

                ZF = false;
                NF = false;
                HF = ((oldsp ^ i8 ^ newsp) & 0x10) != 0;
                CF = ((oldsp ^ i8 ^ newsp) & 0x100) != 0;

                PC++;
                return 4;
            #endregion

            case 0xF8:
                Instruction = $"LD HL,SP+{i8:X2}";
                HL = (ushort)(SP + i8);

                ZF = false;
                NF = false;
                HF = ((SP & 0x0F) + (i8 & 0x0F)) > 0x0F;
                CF = ((SP & 0xFF) + (i8 & 0xFF)) > 0xFF;

                PC++;
                return 3;

            #endregion

            #region x8/rsb
            case 0x07:
                Instruction = "RLCA";
                NF = false;
                ZF = false;
                HF = false;
                CF = (A & 0x80) != 0;
                A = (byte)((A << 1) | (CF ? 1 : 0));
                return 1;
            case 0x17:
                Instruction = "RLA";
                NF = false;
                ZF = false;
                HF = false;
                u8 = (byte)(A & 0x80);
                A = (byte)((A << 1) | (CF ? 1 : 0));
                CF = u8 != 0;
                return 1;
            case 0x0F:
                Instruction = "RRCA";
                NF = false;
                ZF = false;
                HF = false;
                CF = (A & 0x01) != 0;
                A = (byte)((A >> 1) | (CF ? 0x80 : 0));
                return 1;
            case 0x1F:
                Instruction = "RRA";
                NF = false;
                ZF = false;
                HF = false;
                u8 = (byte)(A & 0x01);
                A = (byte)((A >> 1) | (CF ? 0x80 : 0));
                CF = u8 != 0;
                return 1;
            #endregion

            #region control/br
            #region JR
            // TODO: maybe add safeguards against out of bounds?
            case 0x20:
                Instruction = $"JR NZ,{i8:X2}";
                if (!ZF)
                {
                    PC = (ushort)(PC + i8);
                    PC++;
                    return 3;
                }
                else
                {
                    PC++;
                    return 2;
                }
            case 0x30:
                Instruction = $"JR C,{i8:X2}";
                if (!CF)
                {
                    PC = (ushort)(PC + i8);
                    PC++;
                    return 3;
                }
                else
                {
                    PC++;
                    return 2;
                }
            case 0x28:
                Instruction = $"JR Z,{i8:X2}";
                if (ZF)
                {
                    PC = (ushort)(PC + i8);
                    PC++;
                    return 3;
                }
                else
                {
                    PC++;
                    return 2;
                }
            case 0x38:
                Instruction = $"JR C,{i8:X2}";
                if (CF)
                {
                    PC = (ushort)(PC + i8);
                    PC++;
                    return 3;
                }
                else
                {
                    PC++;
                    return 2;
                }
            case 0x18:
                Instruction = $"JR {i8:X2}";
                PC = (ushort)(PC + i8);
                PC++;
                return 3;
            #endregion

            #region JP
            case 0xC2:
                Instruction = $"JP NZ,{u16:X4}";
                if (!ZF)
                {
                    PC = u16;
                    return 4;
                }
                else
                {
                    PC += 2;
                    return 3;
                }
            case 0xD2:
                Instruction = $"JP NC,{u16:X4}";
                if (!CF)
                {
                    PC = u16;
                    return 4;
                }
                else
                {
                    PC += 2;
                    return 3;
                }
            case 0xC3:
                Instruction = $"JP {u16:X4}";
                PC = u16;
                return 4;
            case 0xE9:
                Instruction = "JP HL";
                PC = HL;
                return 1;
            case 0xCA:
                Instruction = $"JP Z,{u16:X4}";
                if (ZF)
                {
                    PC = u16;
                    return 4;
                }
                else
                {
                    PC += 2;
                    return 3;
                }
            case 0xDA:
                Instruction = $"JP C,{u16:X4}";
                if (CF)
                {
                    PC = u16;
                    return 4;
                }
                else
                {
                    PC += 2;
                    return 3;
                }
            #endregion

            #region CALL
            case 0xCD:
                Instruction = $"CALL {u16:X4}";
                PC += 2;
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = u16;
                return 6;
            case 0xC4:
                Instruction = $"CALL NZ,{u16:X4}";
                PC += 2;
                if (!ZF)
                {
                    SP--;
                    Write(SP, (byte)(PC >> 8));
                    SP--;
                    Write(SP, (byte)(PC & 0x00FF));

                    PC = u16;
                    return 6;
                }
                else
                {
                    return 3;
                }
            case 0xD4:
                Instruction = $"CALL NC,{u16:X4}";
                PC += 2;
                if (!CF)
                {
                    SP--;
                    Write(SP, (byte)(PC >> 8));
                    SP--;
                    Write(SP, (byte)(PC & 0x00FF));

                    PC = u16;
                    return 6;
                }
                else
                {
                    return 3;
                }
            case 0xCC:
                Instruction = $"CALL Z,{u16:X4}";
                PC += 2;
                if (ZF)
                {
                    SP--;
                    Write(SP, (byte)(PC >> 8));
                    SP--;
                    Write(SP, (byte)(PC & 0x00FF));

                    PC = u16;
                    return 6;
                }
                else
                {
                    return 3;
                }
            case 0xDC:
                Instruction = $"CALL C,{u16:X4}";
                PC += 2;
                if (CF)
                {
                    SP--;
                    Write(SP, (byte)(PC >> 8));
                    SP--;
                    Write(SP, (byte)(PC & 0x00FF));

                    PC = u16;
                    return 6;
                }
                else
                {
                    return 3;
                }
            #endregion

            #region RST
            case 0xC7:
                Instruction = "RST 00h";
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = 0x00;
                return 4;
            case 0xD7:
                Instruction = "RST 10h";
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = 0x10;
                return 4;
            case 0xE7:
                Instruction = "RST 20h";
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = 0x20;
                return 4;
            case 0xF7:
                Instruction = "RST 30h";
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = 0x30;
                return 4;

            case 0xCF:
                Instruction = "RST 08h";
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = 0x08;
                return 4;
            case 0xDF:
                Instruction = "RST 18h";
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = 0x18;
                return 4;
            case 0xEF:
                Instruction = "RST 28h";
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = 0x28;
                return 4;
            case 0xFF:
                Instruction = "RST 38h";
                SP--;
                Write(SP, (byte)(PC >> 8));
                SP--;
                Write(SP, (byte)(PC & 0x00FF));

                PC = 0x38;
                return 4;
            #endregion

            #region RET
            case 0xC0:
                Instruction = "RET NZ";
                if (!ZF)
                {
                    PC = Read(SP);
                    SP++;
                    PC |= (ushort)(Read(SP) << 8);
                    SP++;
                    return 5;
                }
                else
                {
                    return 2;
                }
            case 0xD0:
                Instruction = "RET NC";
                if (!CF)
                {
                    PC = Read(SP);
                    SP++;
                    PC |= (ushort)(Read(SP) << 8);
                    SP++;
                    return 5;
                }
                else
                {
                    return 2;
                }

            case 0xC8:
                Instruction = "RET Z";
                if (ZF)
                {
                    PC = Read(SP);
                    SP++;
                    PC |= (ushort)(Read(SP) << 8);
                    SP++;
                    return 5;
                }
                else
                {
                    return 2;
                }
            case 0xD8:
                Instruction = "RET C";
                if (CF)
                {
                    PC = Read(SP);
                    SP++;
                    PC |= (ushort)(Read(SP) << 8);
                    SP++;
                    return 5;
                }
                else
                {
                    return 2;
                }

            case 0xC9:
                Instruction = "RET";
                PC = Read(SP);
                SP++;
                PC |= (ushort)(Read(SP) << 8);
                SP++;
                return 4;
            case 0xD9:
                Instruction = "RETI";
                PC = Read(SP);
                SP++;
                PC |= (ushort)(Read(SP) << 8);
                SP++;
                IME = true;
                return 4;
            #endregion
            #endregion

            #region control/misc
            case 0x00:
                Instruction = "NOP";
                return 1;
            case 0x10: // TODO: properly implement STOP when interrupts and timings are implemented
                Instruction = "STOP";
                PC--;
                return 1;
            case 0x76: // TODO: properly implement HALT when interrupts and timings are implemented
                Instruction = "HALT";
                IsHalted = true;
                return 1;
            case 0xFB:
                Instruction = "EI";
                IME = true;
                return 1;
            case 0xF3:
                Instruction = "DI";
                IME = false;
                return 1;
            case 0xCB:
                return ExecutePrefixed();
            #endregion

            default:
                return 1;
        }
    }

    /// <summary>
    /// Executes the opcode fetched from <seealso cref="Fetch"/>
    /// </summary>
    /// <returns></returns>
    public int ExecutePrefixed()
    {
        Opcode = Read(PC);
        PC++;

        int r8 = Opcode & 0x07;
        int bit = (Opcode & 0x38) >> 3;
        byte register = 0;
        byte temp = 0;
        bool took3 = false; // flag if opcode only needed 3 cycles instead of 4 for (HL) opcodes

        // read the value of the register we are modifying/testing/whatever
        switch (r8)
        {
            case 0:
                register = B; break;
            case 1:
                register = C; break;
            case 2:
                register = D; break;
            case 3:
                register = E; break;
            case 4:
                register = H; break;
            case 5:
                register = L; break;
            case 6:
                register = Read(HL); break;
            case 7:
                register = A; break;
        }

        switch (Opcode & 0xC0)
        {
            case 0x00:
                switch (bit)
                {
                    case 0:
                        Instruction = $"RLC {r8}";
                        NF = false;
                        HF = false;
                        CF = (register & 0x80) != 0;
                        register = (byte)((register << 1) | (CF ? 1 : 0));
                        ZF = register == 0;
                        break;
                    case 1:
                        Instruction = $"RRC {r8}";
                        NF = false;
                        HF = false;
                        CF = (register & 0x01) != 0;
                        register = (byte)((register >> 1) | (CF ? 0x80 : 0x00));
                        ZF = register == 0;
                        break;
                    case 2:
                        Instruction = $"RL {r8}";
                        NF = false;
                        HF = false;
                        temp = (byte)(register & 0x80);
                        register = (byte)((register << 1) | (CF ? 1 : 0));
                        CF = temp != 0;
                        ZF = register == 0;
                        break;
                    case 3:
                        Instruction = $"RR {r8}";
                        NF = false;
                        HF = false;
                        temp = (byte)(register & 0x01);
                        register = (byte)((register >> 1) | (CF ? 0x80 : 0x00));
                        CF = temp != 0;
                        ZF = register == 0;
                        break;
                    case 4:
                        Instruction = $"SLA {r8}";
                        NF = false;
                        HF = false;
                        CF = (register & 0x80) != 0;
                        register <<= 1;
                        ZF = register == 0;
                        break;
                    case 5:
                        Instruction = $"SRA {r8}";
                        NF = false;
                        HF = false;
                        CF = (register & 0x01) != 0;
                        temp = (byte)(register & 0x80);
                        register >>= 1;
                        register |= temp;
                        ZF = register == 0;
                        break;
                    case 6:
                        Instruction = $"SWAP {r8}";
                        temp = (byte)(register & 0xF0);
                        temp >>= 4;
                        register <<= 4;
                        register |= temp;
                        NF = false;
                        HF = false;
                        CF = false;
                        ZF = register == 0;
                        break;
                    case 7:
                        Instruction = $"SRL {r8}";
                        NF = false;
                        HF = false;
                        CF = (register & 0x01) != 0;
                        register >>= 1;
                        ZF = register == 0;
                        break;
                }
                break;
            case 0x40:
                took3 = true;
                NF = false;
                HF = true;
                ZF = ((register >> bit) & 0x01) == 0;
                break;
            case 0x80:
                register = (byte)(((0x01 << bit) ^ 0xFF) & register);
                break;
            case 0xC0:
                register = (byte)((0x01 << bit) | register);
                break;
        }

        // save the modified register value to the appropriate register
        switch (r8)
        {
            case 0:
                B = register; return 2;
            case 1:
                C = register; return 2;
            case 2:
                D = register; return 2;
            case 3:
                E = register; return 2;
            case 4:
                H = register; return 2;
            case 5:
                L = register; return 2;
            case 6:
                Write(HL, register); return took3 ? 3 : 4;
            case 7:
                A = register; return 2;
            default:
                return 2;
        }
    }

    
}
