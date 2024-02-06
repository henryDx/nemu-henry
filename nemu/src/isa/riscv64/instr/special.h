def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(jump) {
  vaddr_t next_pc = s->pc+id_src1->simm;
  rtl_j(s, next_pc);
  *ddest = s->snpc;
}

def_EHelper(jump_reg) {
  vaddr_t next_pc = (*dsrc1 + id_src2->imm)&(~1);
  *ddest = s->snpc;
  rtl_j(s, next_pc);
}

def_EHelper(ebreak) {
  
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}

#define def_EHelper_branch(name, OP) def_EHelper(name) {\
  vaddr_t next_pc = (s->pc + id_dest->simm)&(~1);\
  rtl_jrelop(s, concat(RELOP_, OP), dsrc1, dsrc2, next_pc);\
}\

def_EHelper_branch(beq, EQ)
def_EHelper_branch(bne, NE)
def_EHelper_branch(blt, LT)
def_EHelper_branch(bge, GE)
def_EHelper_branch(bltu, LTU)
def_EHelper_branch(bgeu, GEU)