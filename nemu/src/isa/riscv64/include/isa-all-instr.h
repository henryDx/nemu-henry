#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(auipc) f(ld) f(sd) f(inv) f(nemu_trap) f(addi) f(jump) f(ebreak) f(jump_reg) f(lb) f(lh) f(lw) f(lbu) f(lhu) f(lwu)

def_all_EXEC_ID();
