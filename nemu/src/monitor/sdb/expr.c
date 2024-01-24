#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_HEX, TK_D, TK_REG, TK_VAR, TK_REF,TK_NE,TK_LE,TK_GE,TK_AND,

  /* TODO: Add more token types */

};

enum {
  OP_PLUS=0, OP_MINUS, OP_MULTI, OP_DIV, OP_AND, OP_EQ, OP_NE, OP_LEFT, OP_RIGHT, OP_SIZE,
};

int op_priv[OP_SIZE] = {1,1,0,0,2,2,2,-1,-1};

bool is_op(int type);
word_t eval(word_t p, word_t q, bool *success);
int get_ref(int addr);

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", OP_PLUS},         // plus
  {"==", OP_EQ},        // equal
  {"&&", OP_AND},
  //{"<=", TK_LE},
  //{">=", TK_GE},
  {"!=", OP_NE},
  {"0[xX][0-9a-fA-F]+", TK_HEX},
  {"[0-9]+", TK_D},
  {"\\$((\\$0)|(ra)|((s|g|t)p)|(t[0-2])|(s[0-1])|(a[0-7])|(s([2-9]|10|11))|(t[3-6]))", TK_REG},
  {"\\*", OP_MULTI},
  {"\\/", OP_DIV},
  {"\\-", OP_MINUS },
  {"\\(", OP_LEFT},
  {"\\)", OP_RIGHT},
  {"[A-Za-z]\\w*", TK_VAR},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
	printf("e[position]:%s\n", e+position);
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
	memset(tokens[nr_token].str, 0, 32);
        switch (rules[i].token_type) {
		case TK_NOTYPE:break;
		case TK_HEX:;
			       int val = 0;
			       strncpy(tokens[nr_token].str, substr_start, substr_len);
			       sscanf(tokens[nr_token].str, "%x", &val);
			       sprintf(tokens[nr_token].str, "%d", val);
			       tokens[nr_token].type = TK_D;
			       nr_token++;
			       break;
			       
		case TK_REG:
			       strncpy(tokens[nr_token].str, substr_start+1, substr_len-1);
			       //printf("index:%d token type:%d, token str:%s \n",nr_token ,tokens[nr_token].type, tokens[nr_token].str);
			       bool success = false;
			       int reg_val = isa_reg_str2val(tokens[nr_token].str, &success);
			       if(!success){
			       	break;
			       }
                               sprintf(tokens[nr_token].str, "%d", reg_val);
			       tokens[nr_token].type = TK_D;
			       nr_token++;
			       break;
		case OP_MULTI: if(nr_token-1<0 || (is_op(tokens[nr_token-1].type) || tokens[nr_token-1].type == OP_LEFT)){
			  		tokens[nr_token].type = TK_REF;
			  }
			  else{
			  	tokens[nr_token].type = OP_MULTI;     
			  }
			  nr_token++;
			  break;
		default: tokens[nr_token].type = rules[i].token_type;
			 strncpy(tokens[nr_token].str, substr_start, substr_len);
			 nr_token++;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
/*  for(int i = 0; i < nr_token;i++){
  	printf("index:%d token type:%d, token str:%s \n",i ,tokens[i].type, tokens[i].str);
  }*/
  /* TODO: Insert codes to evaluate the expression. */
  //TODO();

  *success = true;
  printf("nr_token:%d\n", nr_token);
  int res = eval(0, nr_token-1, success);
  printf("res:%d\n",res);
  return res;

}

bool is_op(int type){
	return type>=0 && type <= OP_NE;
}

int get_ref(int addr){
	return 0;
}

bool check_parentheses(int p, int q){
	if(tokens[p].type!='(' || tokens[q].type!=')'){
		return false;
	}
	int left_cnt=0;
	for(int i=p+1;i<=q-1;i++){
		if(tokens[i].type == '('){
			left_cnt++;
		}
		else if(tokens[i].type == ')'){
			left_cnt--;
			if(left_cnt<0){
				return false;
			}
		}
	}
	return !left_cnt;
}

word_t eval(word_t p, word_t q, bool* success){
	//printf("eval p:%lu, q:%lu\n",p,q);
	if(!success){
		printf("error happend before, abort!");
		return 0;
	}

	if(p>q){
		assert(0);
		return 0;
	}
	else if(p == q){
		assert(tokens[p].type == TK_D);
		printf("val:%d\n",atoi(tokens[p].str));
		return atoi(tokens[p].str);
	}
	else if(check_parentheses(p, q) == true){
		return eval(p+1, q-1, success);
	}
	else{
		int right_cnt = 0;
		int t = q;
		int mid = -1;
		while(t>p){
			//print_type(t,tokens[t].type);

			if(tokens[t].type == OP_RIGHT){
				right_cnt++;
			}
       			else if(tokens[t].type == OP_LEFT){
				right_cnt--;
				if(right_cnt <0){
					for(int i=p; i<=q;i++){
						printf("%d ",tokens[i].type );
					}
					printf("/n");
					assert(right_cnt>=0);
				}
			}
			else if(is_op(tokens[t].type)){
				if(mid == -1 || op_priv[tokens[t].type] > op_priv[tokens[mid].type]){
					mid = t;
				}
			}			
			t--;
		}
		if(mid == -1 && tokens[p].type == TK_REF){
			return get_ref(eval(p+1,q,success));
		}
		else if(mid != -1){
			switch(tokens[mid].type){
				case OP_PLUS:return eval(p, mid-1, success)+eval(mid+1,q,success);
				case OP_MINUS:return eval(p, mid-1, success)-eval(mid+1,q,success);
				case OP_MULTI:return eval(p, mid-1, success)*eval(mid+1,q,success);
				case OP_DIV:;
					 word_t div = eval(mid+1,q,success);
					 if(div!=0){
					 	return eval(p, mid-1,success)/div;
					 }
					 printf("divide 0 error!");
				case OP_EQ: return eval(p, mid-1, success)==eval(mid+1,q,success);
				case OP_NE: return eval(p, mid-1, success)!=eval(mid+1,q,success);
				case OP_AND: return eval(p, mid-1, success) && eval(mid+1,q,success);
				default:;
			}
		}
		*success = false;
		printf("gramma error!");
		return 0;

	}

}
