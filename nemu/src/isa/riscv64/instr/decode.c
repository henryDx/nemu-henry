#include "../local-include/reg.h"
#include <cpu/ifetch.h>
#include <isa-all-instr.h>

def_all_THelper();

static uint32_t get_instr(Decode *s) {
  return s->isa.instr.val;
}

// decode operand helper
#define def_DopHelper(name) \
  void concat(decode_op_, name) (Decode *s, Operand *op, word_t val, bool flag)

static inline def_DopHelper(i) {
  op->imm = val;
}

static inline def_DopHelper(r) {
  bool is_write = flag;
  static word_t zero_null = 0;
  op->preg = (is_write && val == 0) ? &zero_null : &gpr(val);
}

static inline def_DHelper(I) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  decode_op_i(s, id_src2, (sword_t)s->isa.instr.i.simm11_0, false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}

static inline def_DHelper(J) {
  sword_t simm = (sword_t)(s->isa.instr.j.simm20 << 20) |(s->isa.instr.j.simm19_12 << 12) | (s->isa.instr.j.simm11 << 11)|(s->isa.instr.j.simm10_1 << 1);
  id_src1->simm = simm;
  decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}

static inline def_DHelper(U) {
  decode_op_i(s, id_src1, (sword_t)s->isa.instr.u.simm31_12 << 12, true);
  decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}

static inline def_DHelper(S) {
  decode_op_r(s, id_src1, s->isa.instr.s.rs1, false);
  sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
  decode_op_i(s, id_src2, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.s.rs2, false);
}

static inline def_DHelper(R) {
  decode_op_r(s, id_src1, s->isa.instr.r.rs1, false);
  decode_op_r(s, id_dest, s->isa.instr.r.rd, true);
  decode_op_r(s, id_src2, s->isa.instr.r.rs2, false);
}

static inline def_DHelper(B) {
  decode_op_r(s, id_src1, s->isa.instr.b.rs1, false);
  sword_t simm = (s->isa.instr.b.simm12 << 12) |(s->isa.instr.b.simm11 << 11) | (s->isa.instr.b.simm10_5 << 5)|(s->isa.instr.b.simm4_1 << 1);
  id_dest->simm = simm;
  decode_op_r(s, id_src2, s->isa.instr.b.rs2, false);
}

def_THelper(load) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", lb);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", lh);
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", lw);
  def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??", ld);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", lbu);
  def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", lhu);
  def_INSTR_TAB("??????? ????? ????? 110 ????? ????? ??", lwu);
  return EXEC_ID_inv;
}

def_THelper(store) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", sb);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", sh);
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", sw);
  def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??", sd);
  return EXEC_ID_inv;
}

def_THelper(sri){
  def_INSTR_TAB("0000000 ????? ????? ??? ????? ????? ??", srli);
  def_INSTR_TAB("0100000 ????? ????? ??? ????? ????? ??", srai);
  return EXEC_ID_inv;
}

def_THelper(arithi){
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", addi);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", slli);
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", slti);
  def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??", sltui);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", xori);
  def_INSTR_TAB("??????? ????? ????? 110 ????? ????? ??", ori);
  def_INSTR_TAB("??????? ????? ????? 111 ????? ????? ??", andi);
  def_INSTR_IDTAB("??????? ????? ????? 101 ????? ????? ??", empty, sri);
  return EXEC_ID_inv;
}

def_THelper(add_subw){
  def_INSTR_TAB("0000000 ????? ????? ??? ????? ????? ??", addw);
  def_INSTR_TAB("0100000 ????? ????? ??? ????? ????? ??", subw);
  def_INSTR_TAB("0000001 ????? ????? ??? ????? ????? ??", mulw);
  return EXEC_ID_inv;
}

def_THelper(add_sub){
  def_INSTR_TAB("0000000 ????? ????? ??? ????? ????? ??", add);
  def_INSTR_TAB("0100000 ????? ????? ??? ????? ????? ??", sub);
  return EXEC_ID_inv;
}

def_THelper(srw){
  def_INSTR_TAB("0000000 ????? ????? ??? ????? ????? ??", srlw);
  def_INSTR_TAB("0100000 ????? ????? ??? ????? ????? ??", sraw);
  return EXEC_ID_inv;
}

def_THelper(sr){
  def_INSTR_TAB("0000000 ????? ????? ??? ????? ????? ??", srl);
  def_INSTR_TAB("0100000 ????? ????? ??? ????? ????? ??", sra);
  return EXEC_ID_inv;
}

def_THelper(arithw){
  def_INSTR_IDTAB("??????? ????? ????? 000 ????? ????? ??", empty, add_subw);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", sllw);
  def_INSTR_IDTAB("??????? ????? ????? 101 ????? ????? ??", empty, srw);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", divw);
  return EXEC_ID_inv;
}

def_THelper(srwi){
  def_INSTR_TAB("0000000 ????? ????? ??? ????? ????? ??", srlwi);
  def_INSTR_TAB("0100000 ????? ????? ??? ????? ????? ??", srawi);
  return EXEC_ID_inv;
}

def_THelper(arithwi){
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??",  addwi);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", sllwi);
  def_INSTR_IDTAB("??????? ????? ????? 101 ????? ????? ??", empty, srwi);
  return EXEC_ID_inv;
}

def_THelper(arith){
  def_INSTR_IDTAB("??????? ????? ????? 000 ????? ????? ??", empty, add_sub);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", sll);
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", slt);
  def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??", sltu);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", xor);
  def_INSTR_TAB("??????? ????? ????? 110 ????? ????? ??", or);
  def_INSTR_TAB("??????? ????? ????? 111 ????? ????? ??", and);
  def_INSTR_IDTAB("??????? ????? ????? 101 ????? ????? ??", empty, sr);
  return EXEC_ID_inv;
}

def_THelper(branch){
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", beq);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", bne);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", blt);
  def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", bge);
  def_INSTR_TAB("??????? ????? ????? 110 ????? ????? ??", bltu);
  def_INSTR_TAB("??????? ????? ????? 111 ????? ????? ??", bgeu);
  return EXEC_ID_inv;
}

def_THelper(sys){
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", ebreak);
  return EXEC_ID_inv;
}


def_THelper(main) {
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11100 11", I     , sys);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00000 11", I     , load);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00100 11", I     , arithi);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01100 11", R     , arith);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01110 11", R     , arithw);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00110 11", I     , arithwi);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01000 11", S     , store);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11000 11", B     , branch);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00101 11", U     , auipc);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11011 11", J     , jump);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11001 11", I     , jump_reg);
  def_INSTR_TAB  ("??????? ????? ????? ??? ????? 11010 11",         nemu_trap);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01101 11", I     , lui);
  return table_inv(s);
};

int isa_fetch_decode(Decode *s) {
  s->isa.instr.val = instr_fetch(&s->snpc, 4);
  printf("origin:%x\n",s->isa.instr.val);
  int idx = table_main(s);
  return idx;
}
