#define def_EHelper_compute_reg_imm(name) \
  def_EHelper_compute_reg(name)\
  def_EHelper_compute_imm(concat(name,i))\

#define def_EHelper_compute_imm(name) def_EHelper(name) {\
  printf("before "#name" a:%ld b:%ld res: %ld, add_res:%ld\n",*dsrc1, id_src2->imm, *ddest, *dsrc1 + id_src2->imm);\
  concat(rtl_, name) (s, ddest, dsrc1, id_src2->imm);\
}\

#define def_EHelper_compute_reg(name) def_EHelper(name) {\
  printf("before "#name" a:%ld b:%ld res: %ld, add_res:%ld\n",*dsrc1, *dsrc2, *ddest, *dsrc1 + *dsrc2);\
  concat(rtl_, name) (s, ddest, dsrc1, dsrc2);\
}\

#define def_EHelper_slt_reg_imm(name, op) \
  def_EHelper_slt_reg(name, op)\
  def_EHelper_slt_imm(concat(name,i), op)\

#define def_EHelper_slt_reg(name, op) def_EHelper(name) {\
printf("before "#name" a:%ld b:%ld \n",*dsrc1, *dsrc2);\
  *ddest = interpret_relop(concat(RELOP_, op), *dsrc1, *dsrc2);\
}\

#define def_EHelper_slt_imm(name, op) def_EHelper(name) {\
printf("before "#name" a:%ld b:%ld res:%d\n",*dsrc1, id_src2->imm, *dsrc1<id_src2->imm);\
  *ddest = interpret_relop(concat(RELOP_, op), *dsrc1, id_src2->imm);\
}\

def_EHelper(auipc) {
  rtl_li(s, ddest, id_src1->imm + s->pc);
}

def_EHelper_slt_reg_imm(slt, LT)
def_EHelper_slt_reg_imm(sltu, LTU)

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

