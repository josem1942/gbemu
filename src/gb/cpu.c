#include "cpu.h"

#include <stdlib.h>

gb_cpu_core* gb_cpu_init() {
    gb_cpu_core* cpu = malloc(sizeof(gb_cpu_core));

    if (cpu == nullptr)
        return nullptr;

    cpu->cycles = 0;
    cpu->instruction_cycles = 0;
    cpu->skip_cycles = 1; // skip beginning fetch stage.

    cpu->opcode = 0;

    cpu->bus_read = nullptr;
    cpu->bus_write = nullptr;

    cpu->registers.af = 0;
    cpu->registers.bc = 0;
    cpu->registers.de = 0;
    cpu->registers.hl = 0;
    cpu->registers.sp = 0;
    cpu->registers.pc = 0;

    cpu->registers.ime = false;
    cpu->is_halted = false;
    cpu->ime = false;
    cpu->interrupting = false;

    return cpu;
}

bool gb_cpu_interrupts_pending(gb_cpu_core *cpu) {
    uint8_t i_f = cpu->bus_read(cpu->bus, 0xff0f);
    uint8_t i_e = cpu->bus_read(cpu->bus, 0xff0f);

    if (i_f & GB_CPU_INTERRUPT_VBLANK && i_e & GB_CPU_INTERRUPT_VBLANK) {
        return true;
    }
    if (i_f & GB_CPU_INTERRUPT_LCD && i_e & GB_CPU_INTERRUPT_LCD) {
        return true;
    }
    if (i_f & GB_CPU_INTERRUPT_TIMER && i_e & GB_CPU_INTERRUPT_TIMER) {
        return true;
    }
    if (i_f & GB_CPU_INTERRUPT_SERIAL && i_e & GB_CPU_INTERRUPT_SERIAL) {
        return true;
    }
    if (i_f & GB_CPU_INTERRUPT_JOYPAD && i_e & GB_CPU_INTERRUPT_JOYPAD) {
        return true;
    }

    return false;
}

void gb_cpu_interrupt(gb_cpu_core *cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->ime = false;
        uint8_t i_f = cpu->bus_read(cpu->bus, 0xff0f);
        uint8_t i_e = cpu->bus_read(cpu->bus, 0xff0f);

        if (i_f & GB_CPU_INTERRUPT_VBLANK && i_e & GB_CPU_INTERRUPT_VBLANK) {
            cpu->addr_bus = 0x40;
        }
        else if (i_f & GB_CPU_INTERRUPT_LCD && i_e & GB_CPU_INTERRUPT_LCD) {
            cpu->addr_bus = 0x48;
        }
        else if (i_f & GB_CPU_INTERRUPT_TIMER && i_e & GB_CPU_INTERRUPT_TIMER) {
            cpu->addr_bus = 0x50;
        }
        else if (i_f & GB_CPU_INTERRUPT_SERIAL && i_e & GB_CPU_INTERRUPT_SERIAL) {
            cpu->addr_bus = 0x58;
        }
        else if (i_f & GB_CPU_INTERRUPT_JOYPAD && i_e & GB_CPU_INTERRUPT_JOYPAD) {
            cpu->addr_bus = 0x60;
        }

        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, cpu->registers.pc >> 8);
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 3) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, cpu->registers.pc & 0xFF);
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.pc = cpu->addr_bus;
        cpu->interrupting = false;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_clock(gb_cpu_core *cpu) {
    if (cpu == nullptr)
        return;

    cpu->cycles++;

    if (cpu->skip_cycles > 0) {
        cpu->skip_cycles--;
        return;
    }

    if (cpu->interrupting) {
        gb_cpu_interrupt(cpu);
        return;
    }

    gb_cpu_execute(cpu);

    if (cpu->instruction_cycles == 0) {
        if (gb_cpu_interrupts_pending(cpu) && cpu->ime) {
            cpu->interrupting = true;
            gb_cpu_interrupt(cpu);
            return;
        }

        cpu->opcode = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;

        if (cpu->enable_ime) {
            cpu->enable_ime = false;
            cpu->ime = true;
        }
    }
}

void gb_cpu_destroy(gb_cpu_core *cpu) {
    if (cpu != nullptr) {
        free(cpu);
    }
}

void gb_cpu_execute(gb_cpu_core* cpu) {
    switch (cpu->opcode) {
        case 0x00: gb_cpu_nop(cpu); break;
        case 0x10: gb_cpu_stop(cpu); break;
        case 0x27: gb_cpu_daa(cpu); break;
        case 0x37: gb_cpu_scf(cpu); break;
        case 0x76: gb_cpu_halt(cpu); break;
        case 0xf3: gb_cpu_di(cpu); break;
        case 0xfb: gb_cpu_ei(cpu); break;
        case 0x2f: gb_cpu_cpl(cpu); break;
        case 0x3f: gb_cpu_ccf(cpu); break;

        case 0xcb: gb_cpu_cb(cpu); break;

        case 0x07: gb_cpu_rlca(cpu); break;
        case 0x17: gb_cpu_rla(cpu); break;

        case 0x40: gb_cpu_ld_r8_r8(cpu, &cpu->registers.b, &cpu->registers.b); break;
        case 0x41: gb_cpu_ld_r8_r8(cpu, &cpu->registers.b, &cpu->registers.c); break;
        case 0x42: gb_cpu_ld_r8_r8(cpu, &cpu->registers.b, &cpu->registers.d); break;
        case 0x43: gb_cpu_ld_r8_r8(cpu, &cpu->registers.b, &cpu->registers.e); break;
        case 0x44: gb_cpu_ld_r8_r8(cpu, &cpu->registers.b, &cpu->registers.h); break;
        case 0x45: gb_cpu_ld_r8_r8(cpu, &cpu->registers.b, &cpu->registers.l); break;
        case 0x46: gb_cpu_ld_r8_hl(cpu, &cpu->registers.b); break;
        case 0x47: gb_cpu_ld_r8_r8(cpu, &cpu->registers.b, &cpu->registers.a); break;
        case 0x06: gb_cpu_ld_r8_n8(cpu, &cpu->registers.b); break;

        case 0x48: gb_cpu_ld_r8_r8(cpu, &cpu->registers.c, &cpu->registers.b); break;
        case 0x49: gb_cpu_ld_r8_r8(cpu, &cpu->registers.c, &cpu->registers.c); break;
        case 0x4a: gb_cpu_ld_r8_r8(cpu, &cpu->registers.c, &cpu->registers.d); break;
        case 0x4b: gb_cpu_ld_r8_r8(cpu, &cpu->registers.c, &cpu->registers.e); break;
        case 0x4c: gb_cpu_ld_r8_r8(cpu, &cpu->registers.c, &cpu->registers.h); break;
        case 0x4d: gb_cpu_ld_r8_r8(cpu, &cpu->registers.c, &cpu->registers.l); break;
        case 0x4e: gb_cpu_ld_r8_hl(cpu, &cpu->registers.c); break;
        case 0x4f: gb_cpu_ld_r8_r8(cpu, &cpu->registers.c, &cpu->registers.a); break;
        case 0x0e: gb_cpu_ld_r8_n8(cpu, &cpu->registers.c); break;

        case 0x50: gb_cpu_ld_r8_r8(cpu, &cpu->registers.d, &cpu->registers.b); break;
        case 0x51: gb_cpu_ld_r8_r8(cpu, &cpu->registers.d, &cpu->registers.c); break;
        case 0x52: gb_cpu_ld_r8_r8(cpu, &cpu->registers.d, &cpu->registers.d); break;
        case 0x53: gb_cpu_ld_r8_r8(cpu, &cpu->registers.d, &cpu->registers.e); break;
        case 0x54: gb_cpu_ld_r8_r8(cpu, &cpu->registers.d, &cpu->registers.h); break;
        case 0x55: gb_cpu_ld_r8_r8(cpu, &cpu->registers.d, &cpu->registers.l); break;
        case 0x56: gb_cpu_ld_r8_hl(cpu, &cpu->registers.d); break;
        case 0x57: gb_cpu_ld_r8_r8(cpu, &cpu->registers.d, &cpu->registers.a); break;
        case 0x16: gb_cpu_ld_r8_n8(cpu, &cpu->registers.d); break;

        case 0x58: gb_cpu_ld_r8_r8(cpu, &cpu->registers.e, &cpu->registers.b); break;
        case 0x59: gb_cpu_ld_r8_r8(cpu, &cpu->registers.e, &cpu->registers.c); break;
        case 0x5a: gb_cpu_ld_r8_r8(cpu, &cpu->registers.e, &cpu->registers.d); break;
        case 0x5b: gb_cpu_ld_r8_r8(cpu, &cpu->registers.e, &cpu->registers.e); break;
        case 0x5c: gb_cpu_ld_r8_r8(cpu, &cpu->registers.e, &cpu->registers.h); break;
        case 0x5d: gb_cpu_ld_r8_r8(cpu, &cpu->registers.e, &cpu->registers.l); break;
        case 0x5e: gb_cpu_ld_r8_hl(cpu, &cpu->registers.e); break;
        case 0x5f: gb_cpu_ld_r8_r8(cpu, &cpu->registers.e, &cpu->registers.a); break;
        case 0x1e: gb_cpu_ld_r8_n8(cpu, &cpu->registers.e); break;

        case 0x60: gb_cpu_ld_r8_r8(cpu, &cpu->registers.h, &cpu->registers.b); break;
        case 0x61: gb_cpu_ld_r8_r8(cpu, &cpu->registers.h, &cpu->registers.c); break;
        case 0x62: gb_cpu_ld_r8_r8(cpu, &cpu->registers.h, &cpu->registers.d); break;
        case 0x63: gb_cpu_ld_r8_r8(cpu, &cpu->registers.h, &cpu->registers.e); break;
        case 0x64: gb_cpu_ld_r8_r8(cpu, &cpu->registers.h, &cpu->registers.h); break;
        case 0x65: gb_cpu_ld_r8_r8(cpu, &cpu->registers.h, &cpu->registers.l); break;
        case 0x66: gb_cpu_ld_r8_hl(cpu, &cpu->registers.h); break;
        case 0x67: gb_cpu_ld_r8_r8(cpu, &cpu->registers.h, &cpu->registers.a); break;
        case 0x26: gb_cpu_ld_r8_n8(cpu, &cpu->registers.h); break;

        case 0x68: gb_cpu_ld_r8_r8(cpu, &cpu->registers.l, &cpu->registers.b); break;
        case 0x69: gb_cpu_ld_r8_r8(cpu, &cpu->registers.l, &cpu->registers.c); break;
        case 0x6a: gb_cpu_ld_r8_r8(cpu, &cpu->registers.l, &cpu->registers.d); break;
        case 0x6b: gb_cpu_ld_r8_r8(cpu, &cpu->registers.l, &cpu->registers.e); break;
        case 0x6c: gb_cpu_ld_r8_r8(cpu, &cpu->registers.l, &cpu->registers.h); break;
        case 0x6d: gb_cpu_ld_r8_r8(cpu, &cpu->registers.l, &cpu->registers.l); break;
        case 0x6e: gb_cpu_ld_r8_hl(cpu, &cpu->registers.l); break;
        case 0x6f: gb_cpu_ld_r8_r8(cpu, &cpu->registers.l, &cpu->registers.a); break;
        case 0x2e: gb_cpu_ld_r8_n8(cpu, &cpu->registers.l); break;

        case 0x70: gb_cpu_ld_hl_r8(cpu, &cpu->registers.b); break;
        case 0x71: gb_cpu_ld_hl_r8(cpu, &cpu->registers.c); break;
        case 0x72: gb_cpu_ld_hl_r8(cpu, &cpu->registers.d); break;
        case 0x73: gb_cpu_ld_hl_r8(cpu, &cpu->registers.e); break;
        case 0x74: gb_cpu_ld_hl_r8(cpu, &cpu->registers.h); break;
        case 0x75: gb_cpu_ld_hl_r8(cpu, &cpu->registers.l); break;
        case 0x77: gb_cpu_ld_hl_r8(cpu, &cpu->registers.a); break;
        case 0x36: gb_cpu_ld_hl_n8(cpu); break;;

        case 0x78: gb_cpu_ld_r8_r8(cpu, &cpu->registers.a, &cpu->registers.b); break;
        case 0x79: gb_cpu_ld_r8_r8(cpu, &cpu->registers.a, &cpu->registers.c); break;
        case 0x7a: gb_cpu_ld_r8_r8(cpu, &cpu->registers.a, &cpu->registers.d); break;
        case 0x7b: gb_cpu_ld_r8_r8(cpu, &cpu->registers.a, &cpu->registers.e); break;
        case 0x7c: gb_cpu_ld_r8_r8(cpu, &cpu->registers.a, &cpu->registers.h); break;
        case 0x7d: gb_cpu_ld_r8_r8(cpu, &cpu->registers.a, &cpu->registers.l); break;
        case 0x7e: gb_cpu_ld_r8_hl(cpu, &cpu->registers.a); break;
        case 0x7f: gb_cpu_ld_r8_r8(cpu, &cpu->registers.a, &cpu->registers.a); break;
        case 0x3e: gb_cpu_ld_r8_n8(cpu, &cpu->registers.a); break;

        case 0xe0: gb_cpu_ldh_n16_a(cpu); break;
        case 0xf0: gb_cpu_ldh_a_n16(cpu); break;
        case 0xe2: gb_cpu_ldh_c_a(cpu); break;
        case 0xf2: gb_cpu_ldh_a_c(cpu); break;

        case 0xea: gb_cpu_ld_n16_a(cpu); break;
        case 0xfa: gb_cpu_ld_a_n16(cpu); break;

        case 0x02: gb_cpu_ld_r16_a(cpu, &cpu->registers.bc); break;
        case 0x12: gb_cpu_ld_r16_a(cpu, &cpu->registers.de); break;
        case 0x22: gb_cpu_ld_hli_a(cpu); break;
        case 0x32: gb_cpu_ld_hld_a(cpu); break;

        case 0x0a: gb_cpu_ld_a_r16(cpu, &cpu->registers.bc); break;
        case 0x1a: gb_cpu_ld_a_r16(cpu, &cpu->registers.de); break;
        case 0x2a: gb_cpu_ld_a_hli(cpu); break;
        case 0x3a: gb_cpu_ld_a_hld(cpu); break;

        case 0x01: gb_cpu_ld_r16_n16(cpu, &cpu->registers.bc); break;
        case 0x11: gb_cpu_ld_r16_n16(cpu, &cpu->registers.de); break;
        case 0x21: gb_cpu_ld_r16_n16(cpu, &cpu->registers.hl); break;
        case 0x31: gb_cpu_ld_sp_n16(cpu); break;
        case 0x08: gb_cpu_ld_n16_sp(cpu); break;

        case 0xc1: gb_cpu_pop(cpu, &cpu->registers.b, &cpu->registers.c); break;
        case 0xd1: gb_cpu_pop(cpu, &cpu->registers.d, &cpu->registers.e); break;
        case 0xe1: gb_cpu_pop(cpu, &cpu->registers.h, &cpu->registers.l); break;
        case 0xf1: gb_cpu_pop(cpu, &cpu->registers.a, &cpu->registers.f); break;
        case 0xc5: gb_cpu_push(cpu, &cpu->registers.b, &cpu->registers.c); break;
        case 0xd5: gb_cpu_push(cpu, &cpu->registers.d, &cpu->registers.e); break;
        case 0xe5: gb_cpu_push(cpu, &cpu->registers.h, &cpu->registers.l); break;
        case 0xf5: gb_cpu_push(cpu, &cpu->registers.a, &cpu->registers.f); break;

        case 0xf8: gb_cpu_ld_hl_sp_e8(cpu); break;
        case 0xf9: gb_cpu_ld_sp_hl(cpu); break;

        case 0x20: gb_cpu_jr_cc_n16(cpu, GB_CPU_FLAGS_Z, true); break;
        case 0x30: gb_cpu_jr_cc_n16(cpu, GB_CPU_FLAGS_C, true); break;
        case 0x28: gb_cpu_jr_cc_n16(cpu, GB_CPU_FLAGS_Z, false); break;
        case 0x38: gb_cpu_jr_cc_n16(cpu, GB_CPU_FLAGS_C, false); break;
        case 0x18: gb_cpu_jr_n16(cpu); break;

        case 0xc0: gb_cpu_ret_cc(cpu, GB_CPU_FLAGS_Z, true); break;
        case 0xd0: gb_cpu_ret_cc(cpu, GB_CPU_FLAGS_C, true); break;
        case 0xc8: gb_cpu_ret_cc(cpu, GB_CPU_FLAGS_Z, false); break;
        case 0xd8: gb_cpu_ret_cc(cpu, GB_CPU_FLAGS_C, false); break;
        case 0xc9: gb_cpu_ret(cpu); break;
        case 0xd9: gb_cpu_reti(cpu); break;

        case 0xc2: gb_cpu_jp_cc_n16(cpu, GB_CPU_FLAGS_Z, true); break;
        case 0xd2: gb_cpu_jp_cc_n16(cpu, GB_CPU_FLAGS_C, true); break;
        case 0xca: gb_cpu_jp_cc_n16(cpu, GB_CPU_FLAGS_Z, false); break;
        case 0xda: gb_cpu_jp_cc_n16(cpu, GB_CPU_FLAGS_C, false); break;
        case 0xc3:gb_cpu_jp_n16(cpu); break;
        case 0xe9:gb_cpu_jp_hl(cpu); break;

        case 0xc4: gb_cpu_call_cc(cpu, GB_CPU_FLAGS_Z, true); break;
        case 0xd4: gb_cpu_call_cc(cpu, GB_CPU_FLAGS_C, true); break;
        case 0xcc: gb_cpu_call_cc(cpu, GB_CPU_FLAGS_Z, false); break;
        case 0xdc: gb_cpu_call_cc(cpu, GB_CPU_FLAGS_C, false); break;
        case 0xcd:gb_cpu_call(cpu); break;

        case 0xc7: gb_cpu_call_vec(cpu, 0x0000); break;
        case 0xcf: gb_cpu_call_vec(cpu, 0x0008); break;
        case 0xd7: gb_cpu_call_vec(cpu, 0x0010); break;
        case 0xdf: gb_cpu_call_vec(cpu, 0x0018); break;
        case 0xe7: gb_cpu_call_vec(cpu, 0x0020); break;
        case 0xef: gb_cpu_call_vec(cpu, 0x0028); break;
        case 0xf7: gb_cpu_call_vec(cpu, 0x0030); break;
        case 0xff: gb_cpu_call_vec(cpu, 0x0038); break;

        case 0x04: gb_cpu_inc_r8(cpu, &cpu->registers.b); break;
        case 0x14: gb_cpu_inc_r8(cpu, &cpu->registers.d); break;
        case 0x24: gb_cpu_inc_r8(cpu, &cpu->registers.h); break;
        case 0x34: gb_cpu_inc_hl(cpu); break;
        case 0x0c: gb_cpu_inc_r8(cpu, &cpu->registers.c); break;
        case 0x1c: gb_cpu_inc_r8(cpu, &cpu->registers.e); break;
        case 0x2c: gb_cpu_inc_r8(cpu, &cpu->registers.l); break;
        case 0x3c: gb_cpu_inc_r8(cpu, &cpu->registers.a); break;

        case 0x05: gb_cpu_dec_r8(cpu, &cpu->registers.b); break;
        case 0x15: gb_cpu_dec_r8(cpu, &cpu->registers.d); break;
        case 0x25: gb_cpu_dec_r8(cpu, &cpu->registers.h); break;
        case 0x35: gb_cpu_dec_hl(cpu); break;
        case 0x0d: gb_cpu_dec_r8(cpu, &cpu->registers.c); break;
        case 0x1d: gb_cpu_dec_r8(cpu, &cpu->registers.e); break;
        case 0x2d: gb_cpu_dec_r8(cpu, &cpu->registers.l); break;
        case 0x3d: gb_cpu_dec_r8(cpu, &cpu->registers.a); break;

        case 0x80: gb_cpu_add_a_r8(cpu, &cpu->registers.b); break;
        case 0x81: gb_cpu_add_a_r8(cpu, &cpu->registers.c); break;
        case 0x82: gb_cpu_add_a_r8(cpu, &cpu->registers.d); break;
        case 0x83: gb_cpu_add_a_r8(cpu, &cpu->registers.e); break;
        case 0x84: gb_cpu_add_a_r8(cpu, &cpu->registers.h); break;
        case 0x85: gb_cpu_add_a_r8(cpu, &cpu->registers.l); break;
        case 0x86: gb_cpu_add_a_hl(cpu); break;
        case 0x87: gb_cpu_add_a_r8(cpu, &cpu->registers.a); break;

        case 0x88: gb_cpu_adc_a_r8(cpu, &cpu->registers.b); break;
        case 0x89: gb_cpu_adc_a_r8(cpu, &cpu->registers.c); break;
        case 0x8a: gb_cpu_adc_a_r8(cpu, &cpu->registers.d); break;
        case 0x8b: gb_cpu_adc_a_r8(cpu, &cpu->registers.e); break;
        case 0x8c: gb_cpu_adc_a_r8(cpu, &cpu->registers.h); break;
        case 0x8d: gb_cpu_adc_a_r8(cpu, &cpu->registers.l); break;
        case 0x8e: gb_cpu_adc_a_hl(cpu); break;
        case 0x8f: gb_cpu_adc_a_r8(cpu, &cpu->registers.a); break;

        case 0x90: gb_cpu_sub_a_r8(cpu, &cpu->registers.b); break;
        case 0x91: gb_cpu_sub_a_r8(cpu, &cpu->registers.c); break;
        case 0x92: gb_cpu_sub_a_r8(cpu, &cpu->registers.d); break;
        case 0x93: gb_cpu_sub_a_r8(cpu, &cpu->registers.e); break;
        case 0x94: gb_cpu_sub_a_r8(cpu, &cpu->registers.h); break;
        case 0x95: gb_cpu_sub_a_r8(cpu, &cpu->registers.l); break;
        case 0x96: gb_cpu_sub_a_hl(cpu); break;
        case 0x97: gb_cpu_sub_a_r8(cpu, &cpu->registers.a); break;

        case 0x98: gb_cpu_sbc_a_r8(cpu, &cpu->registers.b); break;
        case 0x99: gb_cpu_sbc_a_r8(cpu, &cpu->registers.c); break;
        case 0x9a: gb_cpu_sbc_a_r8(cpu, &cpu->registers.d); break;
        case 0x9b: gb_cpu_sbc_a_r8(cpu, &cpu->registers.e); break;
        case 0x9c: gb_cpu_sbc_a_r8(cpu, &cpu->registers.h); break;
        case 0x9d: gb_cpu_sbc_a_r8(cpu, &cpu->registers.l); break;
        case 0x9e: gb_cpu_sbc_a_hl(cpu); break;
        case 0x9f: gb_cpu_sbc_a_r8(cpu, &cpu->registers.a); break;

        case 0xa0: gb_cpu_and_a_r8(cpu, &cpu->registers.b); break;
        case 0xa1: gb_cpu_and_a_r8(cpu, &cpu->registers.c); break;
        case 0xa2: gb_cpu_and_a_r8(cpu, &cpu->registers.d); break;
        case 0xa3: gb_cpu_and_a_r8(cpu, &cpu->registers.e); break;
        case 0xa4: gb_cpu_and_a_r8(cpu, &cpu->registers.h); break;
        case 0xa5: gb_cpu_and_a_r8(cpu, &cpu->registers.l); break;
        case 0xa6: gb_cpu_and_a_hl(cpu); break;
        case 0xa7: gb_cpu_and_a_r8(cpu, &cpu->registers.a); break;

        case 0xa8: gb_cpu_xor_a_r8(cpu, &cpu->registers.b); break;
        case 0xa9: gb_cpu_xor_a_r8(cpu, &cpu->registers.c); break;
        case 0xaa: gb_cpu_xor_a_r8(cpu, &cpu->registers.d); break;
        case 0xab: gb_cpu_xor_a_r8(cpu, &cpu->registers.e); break;
        case 0xac: gb_cpu_xor_a_r8(cpu, &cpu->registers.h); break;
        case 0xad: gb_cpu_xor_a_r8(cpu, &cpu->registers.l); break;
        case 0xae: gb_cpu_xor_a_hl(cpu); break;
        case 0xaf: gb_cpu_xor_a_r8(cpu, &cpu->registers.a); break;

        case 0xb0: gb_cpu_or_a_r8(cpu, &cpu->registers.b); break;
        case 0xb1: gb_cpu_or_a_r8(cpu, &cpu->registers.c); break;
        case 0xb2: gb_cpu_or_a_r8(cpu, &cpu->registers.d); break;
        case 0xb3: gb_cpu_or_a_r8(cpu, &cpu->registers.e); break;
        case 0xb4: gb_cpu_or_a_r8(cpu, &cpu->registers.h); break;
        case 0xb5: gb_cpu_or_a_r8(cpu, &cpu->registers.l); break;
        case 0xb6: gb_cpu_or_a_hl(cpu); break;
        case 0xb7: gb_cpu_or_a_r8(cpu, &cpu->registers.a); break;

        case 0xb8: gb_cpu_cp_a_r8(cpu, &cpu->registers.b); break;
        case 0xb9: gb_cpu_cp_a_r8(cpu, &cpu->registers.c); break;
        case 0xba: gb_cpu_cp_a_r8(cpu, &cpu->registers.d); break;
        case 0xbb: gb_cpu_cp_a_r8(cpu, &cpu->registers.e); break;
        case 0xbc: gb_cpu_cp_a_r8(cpu, &cpu->registers.h); break;
        case 0xbd: gb_cpu_cp_a_r8(cpu, &cpu->registers.l); break;
        case 0xbe: gb_cpu_cp_a_hl(cpu); break;
        case 0xbf: gb_cpu_cp_a_r8(cpu, &cpu->registers.a); break;

        case 0xc6: gb_cpu_add_a_n8(cpu); break;
        case 0xd6: gb_cpu_sub_a_n8(cpu); break;
        case 0xe6: gb_cpu_and_a_n8(cpu); break;
        case 0xf6: gb_cpu_or_a_n8(cpu); break;
        case 0xce: gb_cpu_adc_a_n8(cpu); break;
        case 0xde: gb_cpu_sbc_a_n8(cpu); break;
        case 0xee: gb_cpu_xor_a_n8(cpu); break;
        case 0xfe: gb_cpu_cp_a_n8(cpu); break;

        case 0x03: gb_cpu_inc_r16(cpu, &cpu->registers.bc); break;
        case 0x13: gb_cpu_inc_r16(cpu, &cpu->registers.de); break;
        case 0x23: gb_cpu_inc_r16(cpu, &cpu->registers.hl); break;
        case 0x33: gb_cpu_inc_r16(cpu, &cpu->registers.sp); break;

        case 0x0b: gb_cpu_dec_r16(cpu, &cpu->registers.bc); break;
        case 0x1b: gb_cpu_dec_r16(cpu, &cpu->registers.de); break;
        case 0x2b: gb_cpu_dec_r16(cpu, &cpu->registers.hl); break;
        case 0x3b: gb_cpu_dec_r16(cpu, &cpu->registers.sp); break;

        case 0x09: gb_cpu_add_hl_r16(cpu, &cpu->registers.bc); break;
        case 0x19: gb_cpu_add_hl_r16(cpu, &cpu->registers.de); break;
        case 0x29: gb_cpu_add_hl_r16(cpu, &cpu->registers.hl); break;
        case 0x39: gb_cpu_add_hl_sp(cpu); break;
        case 0xe8: gb_cpu_add_sp_e8(cpu); break;

        default: gb_cpu_nop(cpu); break;
    }
}

#pragma region registers

void gb_cpu_add(gb_cpu_core* cpu, uint8_t* dst, const uint8_t* src, bool carry) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N);

    uint8_t c = cpu->registers.f & GB_CPU_FLAGS_C && carry ? 1 : 0;

    if ((*dst & 0xF) + (*src & 0xF) + c > 0xF) {
        cpu->registers.f |= GB_CPU_FLAGS_H;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_H);
    }

    if (*dst + *src + c > 0xFF) {
        cpu->registers.f |= GB_CPU_FLAGS_C;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_C);
    }

    *dst += *src + c;

    if (*dst == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_and(gb_cpu_core* cpu, const uint8_t* src) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_C);
    cpu->registers.f |=  GB_CPU_FLAGS_H;

    cpu->registers.a &= *src;

    if (cpu->registers.a == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_bit(gb_cpu_core* cpu, uint8_t* src, uint8_t bit) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N);
    cpu->registers.f |=  GB_CPU_FLAGS_H;

    if ((1 << bit & *src) == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_res(gb_cpu_core* cpu, uint8_t* dst, uint8_t bit) {
    *dst &= ~(1 << bit);
}

void gb_cpu_set(gb_cpu_core* cpu, uint8_t* dst, uint8_t bit) {
    *dst |= 1 << bit;
}

void gb_cpu_swap(gb_cpu_core* cpu, uint8_t* dst) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H | GB_CPU_FLAGS_C);

    uint8_t old = (*dst) & 0xF;
    (*dst) >>= 4;
    (*dst) |= (old << 4);

    if (*dst == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_rl(gb_cpu_core* cpu, uint8_t* dst, bool set_zero_flag) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H);

    bool b7_set = *dst & 0x80;

    (*dst) <<= 1;
    if (cpu->registers.f & GB_CPU_FLAGS_C) {
        (*dst) |= 0x1;
    }

    if (b7_set) {
        cpu->registers.f |= GB_CPU_FLAGS_C;
    }
    else {
        cpu->registers.f &= ~GB_CPU_FLAGS_C;
    }

    if (*dst == 0 && set_zero_flag) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    } else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_rlc(gb_cpu_core* cpu, uint8_t* dst, bool set_zero_flag) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H);

    bool b7_set = *dst & 0x80;

    (*dst) <<= 1;

    if (b7_set) {
        cpu->registers.f |= GB_CPU_FLAGS_C;
        *dst |= 0x1;
    }
    else {
        cpu->registers.f &= ~GB_CPU_FLAGS_C;
    }

    if (*dst == 0 && set_zero_flag) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    } else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_rr(gb_cpu_core* cpu, uint8_t* dst, bool set_zero_flag) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H);

    bool b0_set = *dst & 0x1;

    (*dst) >>= 1;
    if (cpu->registers.f & GB_CPU_FLAGS_C) {
        (*dst) |= (1 << 7);
    }

    if (b0_set) {
        cpu->registers.f |= GB_CPU_FLAGS_C;
    }
    else {
        cpu->registers.f &= ~GB_CPU_FLAGS_C;
    }

    if (*dst == 0 && set_zero_flag) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    } else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_rrc(gb_cpu_core* cpu, uint8_t* dst, bool set_zero_flag) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H);

    bool b0_set = *dst & 0x1;

    (*dst) >>= 1;

    if (b0_set) {
        cpu->registers.f |= GB_CPU_FLAGS_C;
        *dst |= (1 << 7);
    }
    else {
        cpu->registers.f &= ~GB_CPU_FLAGS_C;
    }

    if (*dst == 0 && set_zero_flag) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    } else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

uint8_t gb_cpu_sub(gb_cpu_core* cpu, uint8_t number, bool borrow) {
    cpu->registers.f |=  GB_CPU_FLAGS_N;
    uint8_t c = cpu->registers.f & GB_CPU_FLAGS_C && borrow ? 1 : 0;

    if ((cpu->registers.a & 0xF) < (number & 0xF) + c) {
        cpu->registers.f |=  GB_CPU_FLAGS_H;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_H);
    }

    if ((uint16_t)cpu->registers.a < (uint16_t)number + c) {
        cpu->registers.f |=  GB_CPU_FLAGS_C;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_C);
    }

    uint8_t result = cpu->registers.a - number - c;

    if (result == 0) {
        cpu->registers.f |=  GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }

    return result;
}

void gb_cpu_sla(gb_cpu_core* cpu, uint8_t* dst) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H);

    if (*dst & 0x80) {
        cpu->registers.f |=  GB_CPU_FLAGS_C;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_C);
    }

    *dst <<= 1;

    if (*dst == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    } else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_sra(gb_cpu_core* cpu, uint8_t* dst) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H);

    if (*dst & 0x1) {
        cpu->registers.f |=  GB_CPU_FLAGS_C;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_C);
    }

    bool bit7_set = *dst & 0x80;

    *dst >>= 1;

    if (bit7_set) {
        *dst |= 0x80;
    }

    if (*dst == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    } else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_srl(gb_cpu_core* cpu, uint8_t* dst) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H);

    if (*dst & 0x1) {
        cpu->registers.f |=  GB_CPU_FLAGS_C;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_C);
    }

    *dst >>= 1;

    if (*dst == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    } else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }
}

void gb_cpu_ld_r8_r8(gb_cpu_core* cpu, uint8_t* dst, uint8_t* src) {
    *dst = *src;
    cpu->instruction_cycles = 0;
}

void gb_cpu_ld_r8_n8(gb_cpu_core* cpu, uint8_t* dst) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        *dst = cpu->data_bus;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_r16_n16(gb_cpu_core* cpu, uint16_t* dst) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        *dst = cpu->data_bus;
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        *dst |= cpu->data_bus << 8;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_hl_r8(gb_cpu_core* cpu, uint8_t* src) {
    if (cpu->instruction_cycles == 0) {
        cpu->bus_write(cpu->bus, cpu->registers.hl, *src);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_hl_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->bus_write(cpu->bus, cpu->registers.hl, cpu->data_bus);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_r8_hl(gb_cpu_core* cpu, uint8_t* dst) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        *dst = cpu->data_bus;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_r16_a(gb_cpu_core* cpu, uint16_t* dst) {
    if (cpu->instruction_cycles == 0) {
        cpu->bus_write(cpu->bus, *dst, cpu->registers.a);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_n16_a(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->addr_bus = cpu->data_bus;
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->addr_bus |= cpu->data_bus << 8;
        cpu->bus_write(cpu->bus, cpu->addr_bus, cpu->registers.a);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_a_n16(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->addr_bus = cpu->data_bus;
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->addr_bus |= cpu->data_bus << 8;
        cpu->registers.a = cpu->bus_read(cpu->bus, cpu->addr_bus);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ldh_n16_a(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->bus_write(cpu->bus, cpu->data_bus + 0xFF00, cpu->registers.a);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ldh_c_a(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->bus_write(cpu->bus, cpu->registers.c + 0xFF00, cpu->registers.a);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_a_r16(gb_cpu_core* cpu, uint16_t* src) {
    if (cpu->instruction_cycles == 0) {
        cpu->registers.a = cpu->bus_read(cpu->bus, *src);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ldh_a_n16(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.a = cpu->bus_read(cpu->bus, cpu->data_bus + 0xFF00);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ldh_a_c(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->registers.a = cpu->bus_read(cpu->bus, cpu->registers.c + 0xFF00);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_hli_a(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->bus_write(cpu->bus, cpu->registers.hl, cpu->registers.a);
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.hl++;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_hld_a(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->bus_write(cpu->bus, cpu->registers.hl, cpu->registers.a);
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.hl--;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_a_hli(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.a = cpu->data_bus;
        cpu->registers.hl++;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_a_hld(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.a = cpu->data_bus;
        cpu->registers.hl--;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_sp_n16(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.sp = cpu->data_bus;
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.sp |= cpu->data_bus << 8;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_n16_sp(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->addr_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->addr_bus |= cpu->bus_read(cpu->bus, cpu->registers.pc) << 8;
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->bus_write(cpu->bus, cpu->addr_bus, cpu->registers.sp & 0xFF);
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 3) {
        cpu->bus_write(cpu->bus, cpu->addr_bus + 1, cpu->registers.sp >> 8);
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_hl_sp_e8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
        cpu->registers.f &= ~(GB_CPU_FLAGS_N);

        if ((cpu->registers.sp & 0xF) + (cpu->data_bus & 0xF) > 0xF) {
            cpu->registers.f |= GB_CPU_FLAGS_H;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_H);
        }

        if ((cpu->registers.sp & 0xFF) + cpu->data_bus > 0xFF) {
            cpu->registers.f |= GB_CPU_FLAGS_C;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_C);
        }

        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.hl = cpu->registers.sp + (int8_t)cpu->data_bus;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ld_sp_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->registers.sp_lo = cpu->registers.l;
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.sp = cpu->registers.hl;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_nop(gb_cpu_core* cpu) {
    cpu->instruction_cycles = 0;
}

void gb_cpu_adc_a_r8(gb_cpu_core* cpu, uint8_t* src) {
    gb_cpu_add(cpu, &cpu->registers.a, src, true);

    cpu->instruction_cycles = 0;
}

void gb_cpu_adc_a_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_add(cpu, &cpu->registers.a, &cpu->data_bus, true);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_adc_a_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_add(cpu, &cpu->registers.a, &cpu->data_bus, true);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_add_a_r8(gb_cpu_core* cpu, uint8_t* src) {
    gb_cpu_add(cpu, &cpu->registers.a, src, false);

    cpu->instruction_cycles = 0;
}

void gb_cpu_add_a_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_add(cpu, &cpu->registers.a, &cpu->data_bus, false);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_add_a_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_add(cpu, &cpu->registers.a, &cpu->data_bus, false);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_add_hl_r16(gb_cpu_core* cpu, uint16_t* src) {
    if (cpu->instruction_cycles == 0) {
        cpu->registers.f &= ~(GB_CPU_FLAGS_N);

        if ((cpu->registers.hl & 0xFFF) + (*src & 0xFFF) > 0xFFF) {
            cpu->registers.f |= GB_CPU_FLAGS_H;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_H);
        }

        if ((uint32_t)cpu->registers.hl + (uint32_t)*src > 0xFFFF) {
            cpu->registers.f |= GB_CPU_FLAGS_C;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_C);
        }

        cpu->registers.hl += *src;

        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_add_hl_sp(gb_cpu_core* cpu) {
    gb_cpu_add_hl_r16(cpu, &cpu->registers.sp);
}

void gb_cpu_add_sp_e8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
        cpu->registers.f &= ~(GB_CPU_FLAGS_N);

        if ((cpu->registers.sp & 0xF) + (cpu->data_bus & 0xF) > 0xF) {
            cpu->registers.f |= GB_CPU_FLAGS_H;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_H);
        }

        if ((cpu->registers.sp & 0xFF) + cpu->data_bus > 0xFF) {
            cpu->registers.f |= GB_CPU_FLAGS_C;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_C);
        }

        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->registers.sp += (int8_t)cpu->data_bus;
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_and_a_r8(gb_cpu_core* cpu, uint8_t* src) {
    gb_cpu_and(cpu, src);

    cpu->instruction_cycles = 0;
}

void gb_cpu_and_a_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_and(cpu, &cpu->data_bus);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_and_a_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_and(cpu, &cpu->data_bus);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_call(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->addr_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->addr_bus |= cpu->bus_read(cpu->bus, cpu->registers.pc) << 8;
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 3) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, cpu->registers.pc >> 8);
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 4) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, cpu->registers.pc & 0xFF);
        cpu->registers.pc = cpu->addr_bus;
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_call_cc(gb_cpu_core* cpu, uint8_t flag, bool not) {
    if (cpu->instruction_cycles == 0) { // M1
        cpu->addr_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->addr_bus |= cpu->bus_read(cpu->bus, cpu->registers.pc) << 8;
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) { // M3
        if ((not && !(cpu->registers.f & flag)) || (!not && cpu->registers.f & flag)) {
            cpu->registers.sp--;
            cpu->instruction_cycles++;
        }
        else { // exec next instr
            cpu->instruction_cycles = 0;
        }
    }
    else if (cpu->instruction_cycles == 3) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, cpu->registers.pc >> 8);
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 4) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, cpu->registers.pc & 0xFF);
        cpu->registers.pc = cpu->addr_bus;
        cpu->instruction_cycles++;
    }
    else { // M6
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ccf(gb_cpu_core* cpu) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_H | GB_CPU_FLAGS_N);
    cpu->registers.f ^= GB_CPU_FLAGS_C;
    cpu->instruction_cycles = 0;
}

void gb_cpu_cp_a_r8(gb_cpu_core* cpu, uint8_t* src) {
    gb_cpu_sub(cpu, *src, false);
    cpu->instruction_cycles = 0;
}

void gb_cpu_cp_a_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_sub(cpu, cpu->data_bus, false);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_cp_a_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_sub(cpu, cpu->data_bus, false);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_cpl(gb_cpu_core* cpu) {
    cpu->registers.f |=  GB_CPU_FLAGS_H | GB_CPU_FLAGS_N;
    cpu->registers.a = ~cpu->registers.a;

    cpu->instruction_cycles = 0;
}

void gb_cpu_daa(gb_cpu_core* cpu) {
    uint8_t adjustment = 0;

    if (cpu->registers.f & GB_CPU_FLAGS_N) {
        if (cpu->registers.f & GB_CPU_FLAGS_H) {
            adjustment += 0x6;
        }
        if (cpu->registers.f & GB_CPU_FLAGS_C) {
            adjustment += 0x60;
        }
        cpu->registers.a -= adjustment;
    }
    else {
        if ((cpu->registers.f & GB_CPU_FLAGS_H) || (cpu->registers.a & 0xF) > 0x9) {
            adjustment += 0x6;
        }
        if ((cpu->registers.f & GB_CPU_FLAGS_C) || (cpu->registers.a > 0x99)) {
            adjustment += 0x60;
            cpu->registers.f |= GB_CPU_FLAGS_C;
        }
        cpu->registers.a += adjustment;
    }

    cpu->registers.f &= ~(GB_CPU_FLAGS_H);

    if (cpu->registers.a == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }

    cpu->instruction_cycles = 0;
}

void gb_cpu_dec_r8(gb_cpu_core* cpu, uint8_t* dst) {
    cpu->registers.f |= GB_CPU_FLAGS_N;

    if ((*dst & 0xF) < 0x1) {
        cpu->registers.f |= GB_CPU_FLAGS_H;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_H);
    }

    (*dst)--;

    if (*dst == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }

    cpu->instruction_cycles = 0;
}

void gb_cpu_dec_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.f |= GB_CPU_FLAGS_N;

        if ((cpu->data_bus & 0xF) < 0x1) {
            cpu->registers.f |= GB_CPU_FLAGS_H;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_H);
        }

        (cpu->data_bus)--;

        if (cpu->data_bus == 0) {
            cpu->registers.f |= GB_CPU_FLAGS_Z;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
        }

        cpu->instruction_cycles++;
    }
    else {
        cpu->bus_write(cpu->bus, cpu->registers.hl, cpu->data_bus);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_dec_r16(gb_cpu_core* cpu, uint16_t* dst) {
    if (cpu->instruction_cycles == 0) {
        (*dst)--;
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_di(gb_cpu_core* cpu) {
    cpu->ime = false;
    cpu->instruction_cycles = 0;
}

void gb_cpu_ei(gb_cpu_core* cpu) {
    cpu->enable_ime = true;
    cpu->instruction_cycles = 0;
}

void gb_cpu_halt(gb_cpu_core* cpu) {
    // TODO: implement halt

    cpu->instruction_cycles = 0;
}

void gb_cpu_inc_r8(gb_cpu_core* cpu, uint8_t* dst) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N);

    if ((*dst & 0xF) + 1 > 0xF) {
        cpu->registers.f |= GB_CPU_FLAGS_H;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_H);
    }

    (*dst)++;

    if (*dst == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }

    cpu->instruction_cycles = 0;
}

void gb_cpu_inc_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.f &= ~(GB_CPU_FLAGS_N);

        if ((cpu->data_bus & 0xF) + 1 > 0xF) {
            cpu->registers.f |= GB_CPU_FLAGS_H;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_H);
        }

        cpu->data_bus++;

        if (cpu->data_bus == 0) {
            cpu->registers.f |= GB_CPU_FLAGS_Z;
        }
        else {
            cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
        }

        cpu->instruction_cycles++;
    }
    else {
        cpu->bus_write(cpu->bus, cpu->registers.hl, cpu->data_bus);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_inc_r16(gb_cpu_core* cpu, uint16_t* dst) {
    if (cpu->instruction_cycles == 0) {
        (*dst)++;
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_jp_n16(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->addr_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->addr_bus |= cpu->bus_read(cpu->bus, cpu->registers.pc) << 8;
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->registers.pc = cpu->addr_bus;
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_jp_cc_n16(gb_cpu_core* cpu, uint8_t flag, bool not) {
    if (cpu->instruction_cycles == 0) {
        cpu->addr_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->addr_bus |= cpu->bus_read(cpu->bus, cpu->registers.pc) << 8;
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        if ((not && !(cpu->registers.f & flag)) || (!not && cpu->registers.f & flag)) {
            cpu->registers.pc = cpu->addr_bus;
            cpu->instruction_cycles++;
        }
        else {
            cpu->instruction_cycles = 0;
        }
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_jp_hl(gb_cpu_core* cpu) {
    cpu->registers.pc = cpu->registers.hl;
    cpu->instruction_cycles = 0;
}

void gb_cpu_jr_n16(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.pc += (int8_t)cpu->data_bus;
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_jr_cc_n16(gb_cpu_core* cpu, uint8_t flag, bool not) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        if ((not && !(cpu->registers.f & flag)) || (!not && cpu->registers.f & flag)) {
            cpu->registers.pc += (int8_t)cpu->data_bus;
            cpu->instruction_cycles++;
        }
        else {
            cpu->instruction_cycles = 0;
        }
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_or_a_r8(gb_cpu_core* cpu, uint8_t* src) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N |  GB_CPU_FLAGS_C | GB_CPU_FLAGS_H);

    cpu->registers.a |= *src;

    if (cpu->registers.a == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~(GB_CPU_FLAGS_Z);
    }

    cpu->instruction_cycles = 0;
}

void gb_cpu_or_a_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_or_a_r8(cpu, &cpu->data_bus);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_or_a_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_or_a_r8(cpu, &cpu->data_bus);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_pop(gb_cpu_core* cpu, uint8_t* hi, uint8_t* lo) {
    if (cpu->instruction_cycles == 0) {
        *lo = cpu->bus_read(cpu->bus, cpu->registers.sp);
        cpu->registers.sp++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        *hi = cpu->bus_read(cpu->bus, cpu->registers.sp);
        cpu->registers.sp++;
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_push(gb_cpu_core* cpu, uint8_t* hi, uint8_t* lo) {
    if (cpu->instruction_cycles == 0) {
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, *hi);
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else {
        cpu->bus_write(cpu->bus, cpu->registers.sp, *lo);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ret(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->registers.pc = cpu->bus_read(cpu->bus, cpu->registers.sp);
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.sp++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->registers.pc |= cpu->bus_read(cpu->bus, cpu->registers.sp) << 8;
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.sp++;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_ret_cc(gb_cpu_core* cpu, uint8_t flag, bool not) {
    if (cpu->instruction_cycles == 0) {
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        if ((not && !(cpu->registers.f & flag)) || (!not && cpu->registers.f & flag)) {
            cpu->registers.pc = cpu->bus_read(cpu->bus, cpu->registers.sp);
            cpu->instruction_cycles++;
        }
        else {
            cpu->instruction_cycles = 0;
        }
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->registers.sp++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 3) {
        cpu->registers.pc |= cpu->bus_read(cpu->bus, cpu->registers.sp) << 8;
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.sp++;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_reti(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->registers.pc = cpu->bus_read(cpu->bus, cpu->registers.sp);
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->registers.sp++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->registers.pc |= cpu->bus_read(cpu->bus, cpu->registers.sp) << 8;
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.sp++;
        cpu->enable_ime = true;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_rla(gb_cpu_core* cpu) {
    gb_cpu_rl(cpu, &cpu->registers.a, false);
    cpu->instruction_cycles = 0;
}

void gb_cpu_rlca(gb_cpu_core* cpu) {
    gb_cpu_rlc(cpu, &cpu->registers.a, false);
    cpu->instruction_cycles = 0;
}

void gb_cpu_rra(gb_cpu_core* cpu) {
    gb_cpu_rr(cpu, &cpu->registers.a, false);
    cpu->instruction_cycles = 0;
}

void gb_cpu_rrca(gb_cpu_core* cpu) {
    gb_cpu_rrc(cpu, &cpu->registers.a, false);
    cpu->instruction_cycles = 0;
}

void gb_cpu_call_vec(gb_cpu_core* cpu, uint16_t vec) {
    if (cpu->instruction_cycles == 0) {
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, cpu->registers.pc >> 8);
        cpu->registers.sp--;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 2) {
        cpu->bus_write(cpu->bus, cpu->registers.sp, cpu->registers.pc & 0xFF);
        cpu->registers.pc = vec;
        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_sbc_a_r8(gb_cpu_core* cpu, uint8_t* dst) {
    cpu->registers.a = gb_cpu_sub(cpu, *dst, true);
    cpu->instruction_cycles = 0;
}

void gb_cpu_sbc_a_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.a = gb_cpu_sub(cpu, cpu->data_bus, true);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_sbc_a_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        cpu->registers.a = gb_cpu_sub(cpu, cpu->data_bus, true);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_scf(gb_cpu_core* cpu) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_N | GB_CPU_FLAGS_H);
    cpu->registers.f |= GB_CPU_FLAGS_C;

    cpu->instruction_cycles = 0;
}

void gb_cpu_stop(gb_cpu_core* cpu) {
    // TODO: implement stop
    cpu->instruction_cycles = 0;
}

void gb_cpu_sub_a_r8(gb_cpu_core* cpu, uint8_t* src) {
    uint8_t result = gb_cpu_sub(cpu, *src, false);
    cpu->registers.a = result;
    cpu->instruction_cycles = 0;
}

void gb_cpu_sub_a_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        uint8_t result = gb_cpu_sub(cpu, cpu->data_bus, false);
        cpu->registers.a = result;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_sub_a_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        uint8_t result = gb_cpu_sub(cpu, cpu->data_bus, false);
        cpu->registers.a = result;
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_xor_a_r8(gb_cpu_core* cpu, uint8_t* src) {
    cpu->registers.f &= ~(GB_CPU_FLAGS_C | GB_CPU_FLAGS_H | GB_CPU_FLAGS_N);

    cpu->registers.a ^= *src;

    if (cpu->registers.a == 0) {
        cpu->registers.f |= GB_CPU_FLAGS_Z;
    }
    else {
        cpu->registers.f &= ~GB_CPU_FLAGS_Z;
    }

    cpu->instruction_cycles = 0;
}

void gb_cpu_xor_a_hl(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.hl);
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_xor_a_r8(cpu, &cpu->data_bus);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_xor_a_n8(gb_cpu_core* cpu) {
    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else {
        gb_cpu_xor_a_r8(cpu, &cpu->data_bus);
        cpu->instruction_cycles = 0;
    }
}

void gb_cpu_decode_cb(gb_cpu_core* cpu, uint8_t* reg, uint8_t bit) {
    switch (cpu->data_bus & 0b11111000) {
        case 0b00000000:
            gb_cpu_rlc(cpu, reg, true);
            break;
        case 0b00001000:
            gb_cpu_rrc(cpu, reg, true);
            break;
        case 0b00010000:
            gb_cpu_rl(cpu, reg, true);
            break;
        case 0b00011000:
            gb_cpu_rr(cpu, reg, true);
            break;
        case 0b00100000:
            gb_cpu_sla(cpu, reg);
            break;
        case 0b00101000:
            gb_cpu_sra(cpu, reg);
            break;
        case 0b00110000:
            gb_cpu_swap(cpu, reg);
            break;
        case 0b00111000:
            gb_cpu_srl(cpu, reg);
            break;
        default: // bit res set
            switch (cpu->data_bus & 0b11000000) {
                case 0b01000000:
                    gb_cpu_bit(cpu, reg, bit);
                    break;
                case 0b10000000:
                    gb_cpu_res(cpu, reg, bit);
                    break;
                case 0b11000000:
                    gb_cpu_set(cpu, reg, bit);
                    break;
                default:
                    break;
            }
            break;
    }
}

void gb_cpu_cb(gb_cpu_core* cpu) {
    uint8_t *reg = nullptr;
    uint8_t bit = (cpu->data_bus >> 3) & 0x7;

    switch (cpu->data_bus & 0x7) {
        case 0: reg = &cpu->registers.b; break;
        case 1: reg = &cpu->registers.c; break;
        case 2: reg = &cpu->registers.d; break;
        case 3: reg = &cpu->registers.e; break;
        case 4: reg = &cpu->registers.h; break;
        case 5: reg = &cpu->registers.l; break;
        case 7: reg = &cpu->registers.a; break;
        default: reg = nullptr; break; // its [hl].
    }

    if (cpu->instruction_cycles == 0) {
        cpu->data_bus = cpu->bus_read(cpu->bus, cpu->registers.pc);
        cpu->registers.pc++;
        cpu->instruction_cycles++;
    }
    else if (cpu->instruction_cycles == 1) {
        if (reg == nullptr) {
            cpu->instruction_cycles++;
            return;
        }

        gb_cpu_decode_cb(cpu, reg, bit);

        cpu->instruction_cycles = 0;
    }
    else if (cpu->instruction_cycles == 2) {
        uint8_t result = cpu->bus_read(cpu->bus, cpu->registers.hl);
        reg = &result;
        gb_cpu_decode_cb(cpu, reg, bit);
        cpu->bus_write(cpu->bus, cpu->registers.hl, result);

        if (cpu->data_bus & 0b01000000) { // bit only takes 3 cycles instead of 4
            cpu->instruction_cycles = 0;
            return;
        }

        cpu->instruction_cycles++;
    }
    else {
        cpu->instruction_cycles = 0;
    }
}
#pragma endregion
