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
        WP * wp = new_wp();
        int len = strlen(e);
        wp->expr = (char *)malloc(sizeof(char)*(len+1));
        strcpy(wp->expr, e);
        wp->cur = expr(e, &ret);	
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

