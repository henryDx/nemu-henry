#include <cpu/decode.h>
#include "../local-include/rtl.h"


#define INSTR_LIST(f) f(auipc) f(ld) f(sd) f(inv) f(nemu_trap) f(jump) f(ebreak) f(jump_reg) \
        f(lb) f(lh) f(lw) f(lbu) f(lhu) f(lwu) \
        f(addw) f(subw) f(sllw) f(srlw) f(sraw) \
        f(add) f(sub) f(sll) f(srl) f(sra) f(slt) f(sltu) f(xor) f(or) f(and)\
        f(addi) f(subi) f(slli) f(srli) f(srai) f(slti) f(sltui) f(xori) f(ori) f(andi)\
        f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu)

def_all_EXEC_ID();
