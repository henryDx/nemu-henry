#include <common.h>
#include <elf.h>
extern uint64_t g_nr_guest_instr;
FILE *log_fp = NULL;
FILE *elf_fp = NULL;

void init_log(const char *log_file) {
  log_fp = stdout;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  Log("Log is written to %s", log_file ? log_file : "stdout");
}

void init_elf(const char *elf_file) {
  if (elf_file == NULL) {
    return;
  }
  printf("%s\n",elf_file);
  FILE *fp = fopen(elf_file, "r");
  Assert(fp, "Can not open '%s'", elf_file);
  elf_fp = fp;
  Elf64_Ehdr ehdr;
  assert(fread(&ehdr, sizeof(Elf64_Ehdr), 1, fp));
  fseek(fp, ehdr.e_shoff, SEEK_SET);
  Elf64_Shdr sym_shdr;
  Elf64_Shdr str_shdr;
  for(int i=0;i<ehdr.e_shnum;i++){
    Elf64_Shdr shdr;
    assert(fread(&shdr, sizeof(Elf64_Shdr), 1, fp));
    if(shdr.sh_type == SHT_SYMTAB){
      sym_shdr = shdr;
    }
    if(shdr.sh_type == SHT_STRTAB){
      str_shdr = shdr;
    }
  }

  assert(sym_shdr.sh_type == SHT_SYMTAB);
  int item_cnt = sym_shdr.sh_size/sizeof(Elf64_Sym);
  printf("table size:%ld, item size:%ld\n",sym_shdr.sh_size, sizeof(Elf64_Sym));
  fseek(fp, sym_shdr.sh_offset, SEEK_SET);

  assert(str_shdr.sh_type == SHT_STRTAB);
  int str_cnt = sym_shdr.sh_size/sizeof(Elf64_Sym);
  printf("table size:%ld, item size:%ld\n",sym_shdr.sh_size, sizeof(Elf64_Sym));
  fseek(fp, sym_shdr.sh_offset, SEEK_SET);


}

bool log_enable() {
  return MUXDEF(CONFIG_TRACE, (g_nr_guest_instr >= CONFIG_TRACE_START) &&
         (g_nr_guest_instr <= CONFIG_TRACE_END), false);
}
