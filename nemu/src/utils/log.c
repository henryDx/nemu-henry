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
  if(fread(&ehdr, sizeof(Elf64_Ehdr), 1, fp)<1){
    printf("ehdr_e_shoff:%lx\n",ehdr.e_shoff);
  }
  fseek(fp, ehdr.e_shoff, SEEK_SET);
  Elf64_Shdr shdr;
  for(int i=0;i<ehdr.e_shnum;i++){
    assert(fread(&shdr, sizeof(Elf64_Shdr), 1, fp));
    if(shdr.sh_type == SHT_SYMTAB){
      break;
    }
  }
  assert(shdr.sh_type == SHT_SYMTAB);
}

bool log_enable() {
  return MUXDEF(CONFIG_TRACE, (g_nr_guest_instr >= CONFIG_TRACE_START) &&
         (g_nr_guest_instr <= CONFIG_TRACE_END), false);
}
