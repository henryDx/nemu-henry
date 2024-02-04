def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(jump) {
  vaddr_t next_pc = s->pc+id_src1->imm;
  //printf("next_pc = %lx\n", next_pc);
  rtl_j(s, next_pc);
}

def_EHelper(ebreak) {
  
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}
