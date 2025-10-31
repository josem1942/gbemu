#ifndef CPU_H
#define CPU_H



#define GB_CPU_FLAGS_Z (1 << 7)
#define GB_CPU_FLAGS_N (1 << 6)
#define GB_CPU_FLAGS_H (1 << 5)
#define GB_CPU_FLAGS_C (1 << 4)

#define GB_CPU_INTERRUPT_VBLANK 1
#define GB_CPU_INTERRUPT_LCD (1 << 1)
#define GB_CPU_INTERRUPT_TIMER (1 << 2)
#define GB_CPU_INTERRUPT_SERIAL (1 << 3)
#define GB_CPU_INTERRUPT_JOYPAD (1 << 4)

#include "bus.h"

#include <stdint.h>

// the gameboy registers
typedef struct gb_cpu_registers {
    union {
        uint16_t af;
        struct {
            uint8_t f;
            uint8_t a;
        };
    }; // af, a, f register

    union {
        uint16_t bc;
        struct {
            uint8_t c;
            uint8_t b;
        };
    }; // bc, b, c register

    union {
        uint16_t de;
        struct {
            uint8_t e;
            uint8_t d;
        };
    }; // de, d, e register

    union {
        uint16_t hl;
        struct {
            uint8_t l;
            uint8_t h;
        };
    }; // hl register

    union {
        uint16_t sp;
        struct {
            uint8_t sp_lo;
            uint8_t sp_hi;
        };
    }; // stack pointer

    union {
        uint16_t pc;
        struct {
            uint8_t pc_lo;
            uint8_t pc_hi;
        };
    }; // program counter

    bool ime;
} gb_cpu_registers;

// core cpu data functionality in a struct lol
typedef struct gb_cpu_core {
    uint64_t cpu_cycles; // keep track of all the gameboy cycles
    uint8_t instruction_cycles; // keeps track of the cycles of the current instruction executing.
    uint8_t skip_cycles; // used to skip any cycles if needed. mainly used to skip 1 cycle at the beginning for fetch.

    uint16_t addr_bus; // used for storing any temp 16 bit data
    uint8_t data_bus; // used for storing any temp 8 bit data

    uint8_t opcode; // 16 bits long because of cb prefix. might change this to 8 bit

    gb_bus *bus; // pointer to the main system bus

    uint8_t (*bus_read)(gb_bus *bus, uint16_t address); // function pointer to a bus read
    void (*bus_write)(gb_bus *bus, uint16_t address, uint8_t data); // function pointer to a bus write

    gb_cpu_registers registers;

    bool is_halted; // for halt mode
    bool ime;
    bool enable_ime; // ime is set after the next instruction
    bool interrupting; // interrupt routine

} gb_cpu_core;

// initializes the cpu core struct to its default values, could also be used as a reset.
// cpu - a pointer to a gb_cpu_core struct.
// returns: 0 on success, anything else is an error.
gb_cpu_core* gb_cpu_init();

// checks if any interrupts are pending.
// checks if both a interrupt is pending in the IF register, and if said interrupt is also enabled in the IE register.
bool gb_cpu_interrupts_pending(gb_cpu_core *cpu, bool service);

// this is an interrupt routine, treated as if it was a instruction
void gb_cpu_interrupt(gb_cpu_core *cpu);

// sets all the registers to the default value skipping the boot rom.
void gb_cpu_skip_bootrom(gb_cpu_core *cpu);

// clocks the cpu.
// cpu - a pointer to a gb_cpu_core struct.
void gb_cpu_clock(gb_cpu_core *cpu);

// executes a cycle
void gb_cpu_execute(gb_cpu_core *cpu);

// destroys and frees up object
void gb_cpu_destroy(gb_cpu_core *cpu);

// i am not going to comment on all the instructions bc its too much time.
// however a lot of these follow a similar pattern.
// there is probably a ton of ways to not only make this more readable and easier to modify but
// also make it more cycle accurate.
// maybe in the future i could revisit this but for now, its gonna stay like this.
// most of the functions here follow a similar pattern, and the *dst and *src are pointers to data or registers.

void gb_cpu_add(gb_cpu_core *cpu, uint8_t *dst, const uint8_t *src, bool carry);
void gb_cpu_and(gb_cpu_core *cpu, const uint8_t *src);
void gb_cpu_bit(gb_cpu_core *cpu, uint8_t *src, uint8_t bit);
void gb_cpu_res(gb_cpu_core *cpu, uint8_t *dst, uint8_t bit);
void gb_cpu_set(gb_cpu_core *cpu, uint8_t *dst, uint8_t bit);
void gb_cpu_swap(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_rl(gb_cpu_core *cpu, uint8_t *dst, bool set_zero_flag);
void gb_cpu_rlc(gb_cpu_core *cpu, uint8_t *dst, bool set_zero_flag);
void gb_cpu_rr(gb_cpu_core *cpu, uint8_t *dst, bool set_zero_flag);
void gb_cpu_rrc(gb_cpu_core *cpu, uint8_t *dst, bool set_zero_flag);
uint8_t gb_cpu_sub(gb_cpu_core *cpu, uint8_t number, bool borrow);
void gb_cpu_sla(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_sra(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_srl(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_ld_r8_r8(gb_cpu_core *cpu, uint8_t *dst, uint8_t *src);
void gb_cpu_ld_r8_n8(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_ld_r16_n16(gb_cpu_core *cpu, uint16_t *dst);
void gb_cpu_ld_hl_r8(gb_cpu_core *cpu, uint8_t *src);
void gb_cpu_ld_hl_n8(gb_cpu_core *cpu);
void gb_cpu_ld_r8_hl(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_ld_r16_a(gb_cpu_core *cpu, uint16_t *dst);
void gb_cpu_ld_n16_a(gb_cpu_core *cpu);
void gb_cpu_ld_a_n16(gb_cpu_core *cpu);
void gb_cpu_ldh_n16_a(gb_cpu_core *cpu);
void gb_cpu_ldh_c_a(gb_cpu_core *cpu);
void gb_cpu_ld_a_r16(gb_cpu_core *cpu, uint16_t *src);
void gb_cpu_ldh_a_n16(gb_cpu_core *cpu);
void gb_cpu_ldh_a_c(gb_cpu_core *cpu);
void gb_cpu_ld_hli_a(gb_cpu_core *cpu);
void gb_cpu_ld_hld_a(gb_cpu_core *cpu);
void gb_cpu_ld_a_hli(gb_cpu_core *cpu);
void gb_cpu_ld_a_hld(gb_cpu_core *cpu);
void gb_cpu_ld_sp_n16(gb_cpu_core *cpu);
void gb_cpu_ld_n16_sp(gb_cpu_core *cpu);
void gb_cpu_ld_hl_sp_e8(gb_cpu_core *cpu);
void gb_cpu_ld_sp_hl(gb_cpu_core *cpu);
void gb_cpu_nop(gb_cpu_core *cpu);
void gb_cpu_adc_a_r8(gb_cpu_core *cpu, uint8_t *src);
void gb_cpu_adc_a_hl(gb_cpu_core *cpu);
void gb_cpu_adc_a_n8(gb_cpu_core *cpu);
void gb_cpu_add_a_r8(gb_cpu_core *cpu, uint8_t *src);
void gb_cpu_add_a_hl(gb_cpu_core *cpu);
void gb_cpu_add_a_n8(gb_cpu_core *cpu);
void gb_cpu_add_hl_r16(gb_cpu_core *cpu, uint16_t *src);
void gb_cpu_add_hl_sp(gb_cpu_core *cpu);
void gb_cpu_add_sp_e8(gb_cpu_core *cpu);
void gb_cpu_and_a_r8(gb_cpu_core *cpu, uint8_t *src);
void gb_cpu_and_a_hl(gb_cpu_core *cpu);
void gb_cpu_and_a_n8(gb_cpu_core *cpu);
void gb_cpu_call(gb_cpu_core *cpu);
void gb_cpu_call_cc(gb_cpu_core *cpu, uint8_t flag, bool not);
void gb_cpu_ccf(gb_cpu_core *cpu);
void gb_cpu_cp_a_r8(gb_cpu_core *cpu, uint8_t *src);
void gb_cpu_cp_a_hl(gb_cpu_core *cpu);
void gb_cpu_cp_a_n8(gb_cpu_core *cpu);
void gb_cpu_cpl(gb_cpu_core *cpu);
void gb_cpu_daa(gb_cpu_core *cpu);
void gb_cpu_dec_r8(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_dec_hl(gb_cpu_core *cpu);
void gb_cpu_dec_r16(gb_cpu_core *cpu, uint16_t *dst);
void gb_cpu_di(gb_cpu_core *cpu);
void gb_cpu_ei(gb_cpu_core *cpu);
void gb_cpu_halt(gb_cpu_core *cpu);
void gb_cpu_inc_r8(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_inc_hl(gb_cpu_core *cpu);
void gb_cpu_inc_r16(gb_cpu_core *cpu, uint16_t *dst);
void gb_cpu_jp_n16(gb_cpu_core *cpu);
void gb_cpu_jp_cc_n16(gb_cpu_core *cpu, uint8_t flag, bool not);
void gb_cpu_jp_hl(gb_cpu_core *cpu);
void gb_cpu_jr_n16(gb_cpu_core *cpu);
void gb_cpu_jr_cc_n16(gb_cpu_core *cpu, uint8_t flag, bool not);
void gb_cpu_or_a_r8(gb_cpu_core *cpu, uint8_t *src);
void gb_cpu_or_a_hl(gb_cpu_core *cpu);
void gb_cpu_or_a_n8(gb_cpu_core *cpu);
void gb_cpu_pop(gb_cpu_core *cpu, uint8_t *hi, uint8_t *lo);
void gb_cpu_push(gb_cpu_core *cpu, uint8_t *hi, uint8_t *lo);
void gb_cpu_ret(gb_cpu_core *cpu);
void gb_cpu_ret_cc(gb_cpu_core *cpu, uint8_t flag, bool not);
void gb_cpu_reti(gb_cpu_core *cpu);
void gb_cpu_rla(gb_cpu_core *cpu);
void gb_cpu_rlca(gb_cpu_core *cpu);
void gb_cpu_rra(gb_cpu_core *cpu);
void gb_cpu_rrca(gb_cpu_core *cpu);
void gb_cpu_call_vec(gb_cpu_core *cpu, uint16_t vec);
void gb_cpu_sbc_a_r8(gb_cpu_core *cpu, uint8_t *dst);
void gb_cpu_sbc_a_hl(gb_cpu_core *cpu);
void gb_cpu_sbc_a_n8(gb_cpu_core *cpu);
void gb_cpu_scf(gb_cpu_core *cpu);
void gb_cpu_stop(gb_cpu_core *cpu);
void gb_cpu_sub_a_r8(gb_cpu_core *cpu, uint8_t *src);
void gb_cpu_sub_a_hl(gb_cpu_core *cpu);
void gb_cpu_sub_a_n8(gb_cpu_core *cpu);
void gb_cpu_xor_a_r8(gb_cpu_core *cpu, uint8_t *src);
void gb_cpu_xor_a_hl(gb_cpu_core *cpu);
void gb_cpu_xor_a_n8(gb_cpu_core *cpu);
void gb_cpu_decode_cb(gb_cpu_core *cpu, uint8_t *reg, uint8_t bit);
void gb_cpu_cb(gb_cpu_core *cpu);

#endif //CPU_H
