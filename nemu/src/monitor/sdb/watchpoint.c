#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char *expr;
  int cur;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

WP * new_wp();
void free_wp(WP* wp);

void add_wp(char* e){
	bool ret = false;
	int cur = expr(e, &ret);
	if(ret == false){
		printf("invalid express\n");
		return;
	}
        WP * wp = new_wp();
        int len = strlen(e);
        wp->expr = (char *)malloc(sizeof(char)*(len+1));
        strcpy(wp->expr, e);
        wp->cur = cur;
	printf("Hardware watchpoint %d is set!\n",wp->NO);	
}

void delete_wp(int no){
	free_wp(&wp_pool[no]);
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
	assert(free != NULL);
	WP* wp_new = free_;
	free_ = free_->next;
	wp_new->next = head;
	head =  wp_new;
	printf("head:%d", head->NO);
	if(head->next==NULL){
		printf("next: NULL\n");
	}
	else{
		printf("next: %d",head->next->NO);
	}
	return wp_new;
}

void free_wp(WP* wp){
	WP* prev = head;
	if(prev ==NULL){
		printf("empty list!\n");
		return;
	}
	if(wp == prev){
		prev = NULL;
	}
	else{
		while(prev->next!=wp){
			prev = prev->next;
			assert(prev!=NULL);
		}
	}
	if(prev!=NULL){
		prev->next=wp->next;
	}
	wp->next = free_;
	free_ = wp;
	free(wp->expr);
	wp->expr = NULL;
}

bool scan_wp(){
	WP* tmp = head;
	while(tmp!=NULL){
		bool success = false;
		int new_val = expr(tmp->expr, &success);
		if(new_val != tmp->cur){
			tmp->cur = new_val;
			printf("break at the watchpoint %d!\n",tmp->NO);
			//nemu_state.state = NEMU_STOP;
			return false;
		}
		tmp=tmp->next;
	}
	return true;
}

void display_wp(){
	WP* tmp = head;
	while(tmp != NULL){
		printf("Watchpoint %d : %s\n", tmp->NO, tmp->expr);
		tmp=tmp->next;
	}
}
