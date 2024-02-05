#define def_EHelper_load_unsinged(name, num) def_EHelper(name) {\
  rtl_lm(s, ddest, dsrc1, id_src2->imm, num);\
  printf("load %lx addr %lx offset %lx\n",*ddest, *dsrc1, id_src2->imm);\
}\

#define def_EHelper_load_singed(name, num) def_EHelper(name) {\
  rtl_lms(s, ddest, dsrc1, id_src2->imm, num);\
  printf("load %lx addr %lx offset %lx\n",*ddest, *dsrc1, id_src2->imm);\
}\

def_EHelper_load_unsinged(ld, 8) 
def_EHelper_load_unsinged(lwu, 4) 
def_EHelper_load_unsinged(lbu, 1) 
def_EHelper_load_unsinged(lhu, 2) 

def_EHelper_load_singed(lw, 4) 
def_EHelper_load_singed(lb, 1) 
def_EHelper_load_singed(lh, 2) 

def_EHelper(sd) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 8);
}
