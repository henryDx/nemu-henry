#define def_EHelper_load_unsinged(name, num) def_EHelper(name) {\
  rtl_lm(s, ddest, dsrc1, id_src2->imm, num);\
}\

#define def_EHelper_load_singed(name, num) def_EHelper(name) {\
  rtl_lms(s, ddest, dsrc1, id_src2->imm, num);\
}\

def_EHelper_load_unsinged(ld, 8) 
def_EHelper_load_unsinged(lwu, 4) 
def_EHelper_load_unsinged(lbu, 1) 
def_EHelper_load_unsinged(lhu, 2) 

def_EHelper_load_singed(lw, 4) 
def_EHelper_load_singed(lb, 1) 
def_EHelper_load_singed(lh, 2) 

#define def_EHelper_store(name, num) def_EHelper(name) {\
   rtl_sm(s, ddest, dsrc1, id_src2->imm, num);\
}\

def_EHelper_store(sd, 8)
def_EHelper_store(sb, 1)
def_EHelper_store(sh, 2)
def_EHelper_store(sw, 4)

def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
