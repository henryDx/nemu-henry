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