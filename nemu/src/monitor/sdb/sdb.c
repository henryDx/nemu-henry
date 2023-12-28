#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <memory/vaddr.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
  //printf("%s\n",args);
  char* arg = strtok(args, " ");
  int n = atoi(arg);
  if(strtok(NULL, " ") != NULL){
  	printf("too much arguments\n");
  }
  if(n > 0){
  	cpu_exec(n);
  }
  return 0;
}

static int cmd_info(char *args){
	char *arg = strtok(NULL, "");
	//printf("%s\n", arg);
	if(arg == NULL){
		printf("no arguments\n");
		return 0;
	}
	if(!strcmp(arg, "r")){
		//printf("is r\n");
		isa_reg_display();
	}
	else if(!strcmp(arg, "w")){
	
	}
	else{
		printf("unknown arguments\n");
	}
	return 0;
}

static int cmd_x(char * args){
        int n=0;
	vaddr_t vaddr = 0;
	if(sscanf(args, "%d %lx",&n, &vaddr)<2){
		printf("read argument failed!\n");
		return 0;
	}
        //printf("%d %lx\n",n,vaddr);
	if(vaddr < 0x80000000){
		printf("invalid address!\n");
		return 0;
	}
	vaddr_t vaddr_ori = vaddr;
        while(vaddr < vaddr_ori+n){
		printf("%-16lx:", vaddr);
		for(int j=0;j<8&&vaddr<vaddr_ori+n;j++){
			printf("%1lx\n", vaddr_read(vaddr,2));
			vaddr+=2;
		}
		printf("\n");
	}
	return 0;
	
}

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Step into", cmd_si},
  { "info", "print register or watchpoint", cmd_info},
  { "x", "display tge value of memory", cmd_x},
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
