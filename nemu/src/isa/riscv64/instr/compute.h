#define def_EHelper_compute_reg_imm(name) \
  def_EHelper_compute_reg(name)\
  def_EHelper_compute_imm(concat(name,i))\

#define def_EHelper_compute_imm(name) def_EHelper(name) {\
  concat(rtl_, name) (s, ddest, dsrc1, id_src2->imm);\
}\

#define def_EHelper_compute_reg(name) def_EHelper(name) {\
  concat(rtl_, name) (s, ddest, dsrc1, dsrc2);\
}\

def_EHelper(auipc) {
  rtl_li(s, ddest, id_src1->imm + s->pc);
}

def_EHelper(slt) {
  *ddest = interpret_relop(RELOP_LT, *dsrc1, *dsrc2);
}

def_EHelper(slti) {
  *ddest = interpret_relop(RELOP_LT, *dsrc1, id_src2->imm);
}

def_EHelper(sltui) {
  *ddest = interpret_relop(RELOP_LTU, *dsrc1, id_src2->imm);
}

def_EHelper(sltu) {
  *ddest = interpret_relop(RELOP_LTU, *dsrc1, *dsrc2);
}

def_EHelper(srai) {
  rtl_srai(s, ddest, dsrc1, id_src2->imm & 0x01f);
}

def_EHelper(sra) {
  rtl_sra(s, ddest, dsrc1, dsrc2);
}


def_EHelper_compute_reg_imm(add)
def_EHelper_compute_reg_imm(sub)
def_EHelper_compute_reg_imm(and)
def_EHelper_compute_reg_imm(or)
def_EHelper_compute_reg_imm(xor)
def_EHelper_compute_reg_imm(sll)
def_EHelper_compute_reg_imm(srl)


def_EHelper_compute_reg_imm(addw)
def_EHelper_compute_reg_imm(subw)
def_EHelper_compute_reg_imm(sllw)
def_EHelper_compute_reg_imm(srlw)
def_EHelper_compute_reg_imm(sraw)

