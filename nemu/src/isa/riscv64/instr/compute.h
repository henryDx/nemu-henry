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
  
}

def_EHelper(slti) {
  
}

def_EHelper(sltui) {
  
}

def_EHelper(sltu) {
  
}



def_EHelper_compute_reg_imm(add)
def_EHelper_compute_reg_imm(sub)
def_EHelper_compute_reg_imm(and)
def_EHelper_compute_reg_imm(or)
def_EHelper_compute_reg_imm(xor)
def_EHelper_compute_reg_imm(sll)
def_EHelper_compute_reg_imm(srl)
def_EHelper_compute_reg_imm(sra)

def_EHelper_compute_reg_imm(addw)
def_EHelper_compute_reg_imm(subw)
def_EHelper_compute_reg_imm(sllw)
def_EHelper_compute_reg_imm(srlw)
def_EHelper_compute_reg_imm(sraw)

