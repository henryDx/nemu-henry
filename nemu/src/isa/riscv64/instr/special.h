def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(jump) {
  printf("pc = %lx\n snpc = %lx\n", s->pc+id_src1->imm, s->snpc);
  rtl_j(s, (vaddr_t)(s->pc+id_src1->imm));
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}
