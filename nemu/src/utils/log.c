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
  Elf64_Shdr shdr;
  for(int i=0;i<ehdr.e_shnum;i++){
    assert(fread(&shdr, sizeof(Elf64_Shdr), 1, fp));
    if(shdr.sh_type == SHT_SYMTAB){
      //printf("index:%d\n",i);
      break;
    }
  }
  assert(shdr.sh_type == SHT_SYMTAB);
  int item_cnt = shdr.sh_size/sizeof(Elf64_Sym);
  printf("table size:%ld, item size:%ld\n",shdr.sh_size, sizeof(Elf64_Sym));
  fseek(fp, shdr.sh_offset, SEEK_SET);
  for(int i=0;i<item_cnt;i++){
    Elf64_Sym sym;
    assert(fread(&sym, sizeof(Elf64_Sym), 1, fp));
    printf("value:%ld size:%ld \n",sym.st_value,sym.st_size);
  }
}

bool log_enable() {
  return MUXDEF(CONFIG_TRACE, (g_nr_guest_instr >= CONFIG_TRACE_START) &&
         (g_nr_guest_instr <= CONFIG_TRACE_END), false);
}
