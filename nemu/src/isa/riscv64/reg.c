#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
	//printf("isa_reg_display\n");
        for(int idx=0; idx<32; idx++){
                const char *reg = regs[check_reg_idx(idx)];
                printf("%-16s%-16lx%-16ld\n", reg, gpr(idx), gpr(idx));
        }

}

word_t isa_reg_str2val(const char *s, bool *success) {
  	for(int idx=0; idx<32; idx++){
                const char *reg = regs[check_reg_idx(idx)];
                if(!strcmp(reg, s)){
			*success = true;
			return gpr(idx);
		}
        }
	*success = false;
	printf("invalid register name:%s !\n", s);
	return 0;

}
