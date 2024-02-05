def_EHelper(auipc) {
  rtl_li(s, ddest, id_src1->imm + s->pc);
}

def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(addw) {
  rtl_addw(s, ddest, dsrc1, dsrc2);
}

def_EHelper(subw) {
  rtl_subw(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sllw) {
  rtl_sllw(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srlw) {
  rtl_srlw(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sraw) {
  rtl_sraw(s, ddest, dsrc1, dsrc2);
}

def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slt) {
  //rtl_slt(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltu) {
  //rtl_sltu(s, ddest, dsrc1, dsrc2);
  
}

def_EHelper(xor) {
  rtl_xor(s, ddest, dsrc1, dsrc2);
}

def_EHelper(or) {
  rtl_or(s, ddest, dsrc1, dsrc2);
}

def_EHelper(and) {
  rtl_and(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sll) {
  rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srl) {
  rtl_srl(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sra) {
  rtl_sra(s, ddest, dsrc1, dsrc2);
}