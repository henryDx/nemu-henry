#include <cpu/decode.h>
#include "../local-include/rtl.h"


#define INSTR_LIST(f) f(auipc) f(lui) f(ld) f(inv) f(nemu_trap) f(jump) f(ebreak) f(jump_reg) \
        f(lb) f(lh) f(lw) f(lbu) f(lhu) f(lwu) f(rem) f(remu)\
        f(addw) f(subw) f(sllw) f(srlw) f(sraw) f(divw) f(mulw) f(remw) f(remuw) f(divuw)\
        f(add) f(sub) f(sll) f(srl) f(sra) f(slt) f(sltu) f(xor) f(or) f(and) f(mul) f(div) f(divu) \
        f(addi) f(subi) f(slli) f(srli) f(srai) f(slti) f(sltui) f(xori) f(ori) f(andi)\
        f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu)\
        f(addwi) f(sllwi) f(srlwi) f(srawi) \
        f(sd) f(sb) f(sh) f(sw)\

def_all_EXEC_ID();
