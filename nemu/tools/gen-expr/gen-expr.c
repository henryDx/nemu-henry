#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static int buf_cnt = 0;
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
static void gen_rand_expr();
static int depth = 0;
static int choose(int n){
	return rand()%n;
}

static void gen(char c){
	if(buf_cnt>=65535){
		printf("gen error\n");
		//buf_cnt = 0;
		//depth = 0;
		//gen_rand_expr();
		return;
	}
	buf[buf_cnt]=c;
	buf_cnt++;
	buf[buf_cnt]='\0';
}

static void gen_num(){
	for(int i=0;i<10;i++){
		int n = rand()%10;
		if(n==0){
			break;
		}
		gen(n+'0');
	}
	gen(rand()%10 +'0');
	

}

static void gen_rand_op(){
	switch(choose(4)){
		case 0:gen('+');break;
		case 1:gen('-');break;
		case 2:gen('*');break;
		default:gen('/');break;
	}
}

static void gen_rand_expr() {
	depth++;
  switch (choose(3)){
  	case 0: gen_num(); break;
	case 1: gen('(');gen_rand_expr();gen(')');break;
	default: gen_rand_expr();gen_rand_op(); gen_rand_expr();break;
  }
  	depth--;
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf_cnt=0;
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    if(fscanf(fp, "%d", &result) == -1){
    	printf("fscanf failed \n");
    }
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
