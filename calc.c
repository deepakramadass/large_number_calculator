#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define NEW(x) (x*)malloc(sizeof(x))

int k = 0;
int lengths[2];
int addition = 0;
int carry_count = 0;
int carry = 0;
int subtraction = 0;
int borrow = 0;
int borrow_count = 0;
int product_count = 0;
FILE *file;
int create_mod(k){
	int i = 0;
	int temp = 1;
	for(i = 0; i < k; i++)
		 temp = temp * 10;
	return temp;
}


typedef struct node{
	int value;			///> the part of the number corresponding to block size
	struct node* next;	///> pointer to the next node
	struct node* prev;	///>pointer to prev node
} node;					///> name of the stuct

typedef struct {
	int num;			///> the number of node is the list
	node* head;		///> points to the start of the list
	node* tail;		///> points to the end of the list
} root;					///> defined as the root

/**
  * This functions creates a new node
  * @param value to be stored in the node
  * @return pointer to the node created
*/			
node * make_node(int value){
	node *temp;
	temp = NEW(node);
	if(temp != NULL){
		temp->value = value;
		temp->next = NULL;
		temp->prev = NULL;
	}
	return temp;
}

/**
  * This functions creates a new root
  * @return pointer to the root created
*/			
root * make_root(void){
	root *r;
	r = NEW(root);
	if(r != NULL){
		r->num = 0;
		r->head = NULL;
		r->tail = NULL;
	}
	return r;
}


int insert_at_head(root *r, int value)
{ node * temp;
  temp = make_node(value);
 
  if (temp == NULL) return -1;  // fail, cannot create new NODE

  if (r == NULL) 
  { r = make_root();
    if (r == NULL) return -1;   // fail, cannot create ROOT
  }
     
  (r->num)++;

  if (r->num == 1)              // if previously the list is empty
    	r->tail = temp;
  else
	r->head->prev = temp;

  temp->next = r->head;
  r->head = temp;
  	
  return 0;
}


/**
  * This function inserts nodes at the end of the linked list
  * @param r points to the root of the list 
  * @param value is the data that is to be stored in the node
  * @return 0/1 success/failure
  */
int insert_at_tail(root *r,int value){
	node *temp;
	temp = make_node(value);
	
	if(temp == NULL) return 1;
	
	if(r == NULL){
		r = make_root();
		if(r == NULL) return 1;
	}
	
	(r->num)++;
		
	if(r->num == 1)
		r->head = r->tail = temp;
	else{
		r->tail->next = temp;
		temp->prev = r->tail;
		r->tail = temp;
	}
	return 0;
}

/**
  * This function displays the linked list
  */
void display_list(root *r)
{ node * temp;

  if (r != NULL)        // this is safer than (r.num != 0), why?
                        // if r==NULL, there will be a segmentation fault
                        //      when you try to use r.num
  { temp = r->head;
    do 
    { printf("%3d ", (temp->value));
      temp = temp->next;
    } while (temp != NULL);
  }
  printf("\n");
}

/**
  * This function frees the memory from the list
  * @param r1 points to the root of the list to be freed
  */
void clear_memory(root *r){
	node *temp, *temp1;
	temp = r->head;
	temp1 = r->head->next;
	do{
		free(temp);
		temp = temp1;
		temp1 = temp1->next;
	}while(temp1 != NULL);
	free(temp1);
}

/** 
  * This function is used to store a linked list
  * in a file
  * @param r points to the root of the list
  */
void store_list(root *r){
	int i = 0;
	node *temp;
   	temp = r->head;
	char hold[k];
	hold[k] = '\0';
	char *store = NULL;
	
	if(temp->value != 0){	
		sprintf(hold,"%d",temp->value);
		fprintf(file,"%s",hold);
	}
	temp = temp->next;	
	do{
		sprintf(hold,"%d",temp->value);
		if(strlen(hold) != k){
			store = calloc(k+1,sizeof(char));
			while(i < (k - strlen(hold))){
				store[i] = '0';
				i++;
			}
			i = 0;
			strncat(store,hold,strlen(hold));
			strncpy(hold,store,k);
			free(store);
		} 
		temp = temp->next; 
		fprintf(file,"%s",hold);
	}while(temp != NULL);
	fprintf(file,"%c",'\n');	
}

/**
  * This function removes \n' and pre-appends a 0 if odd
  * @param store points to the string to be converted
  * @param digits is the number of characters in the string
  */
void fix_string(char* store, size_t digits){
	//digits--;
	int append = (digits) % k;
	char *temp = malloc(digits + k - append + 1);
	temp = store;
	if((digits) % k != 0){
		char *dst = NULL;
		dst = (char*)malloc(digits + k - append);
		int i = 0;
		for(i = 0; i < (k - append); i++)
			dst[i] = '0';
		memcpy(dst+ k - append,store,digits);
		strncpy(temp,dst,digits+k-append);
		free(dst);
	}
}	

/**
  * This function stores the number in the linked list
  * @param store points to the string to be converted
  */
void store_num(char* store, root *r,int number){
	int i = 0;
	char *dst = NULL; 
	dst = malloc(k+1);
	dst[k] ='\0';
	for(i = 0; i <= (number-k);){
		strncpy(dst,&store[i],k);
		//printf("dst %s\n",dst);
		i+= k;
		insert_at_tail(r,atoi(dst));
	}
	free(dst);	 
}

/**
  * This functions adds zeros to the head of the
  * smaller list 
  * @param r points to the list to be padded
  * @param lenght the number nodes to be added
  */
void zero_pad(root *r, int length){
	while(length > 0){
		insert_at_head(r,0);
		length--;
	}
}

/** 
  * This functions duplicates a list
  * @param r1 is the list to be duplicated
  * @param r2 is the new list
  */
void copy_lists(root *r1, root*r2){
	node *temp;
	temp = r1->head;

	do{
		insert_at_tail(r2,temp->value);
		temp = temp->next;
	}while(temp != NULL);
}
	
	
/**
  * This function adds the two linked lists
  * @param r1 points to the root of 1st linked list - larger list
  * @param r2 points to the root of 2nd linked list - smaller list
  */
void add_lists(root *r1, root *r2, root *res, int mul){
	root *sum = NULL;
	if(!mul){
		if(res == NULL)
			sum = make_root();
		else 
			sum = res;
	}
	else
		sum = make_root();
	node *temp1,*temp2;
	int mod = create_mod(k);
	
	temp1 = r1->tail;
	temp2 = r2->tail;
	int result = 0;

	do{
		if(temp1->value != 0 && temp2->value != 0)
			addition++;
		result = (((temp1->value) + (temp2->value)) % mod);
		if(carry != 0){
			//carry_count++;
			addition ++;
			result = (result + carry) % mod;
		}
		carry = (((temp1->value) + (temp2->value) + carry) / mod);
		
		insert_at_head(sum,result);
		temp1 = temp1->prev;
		temp2 = temp2->prev;
		
	}while(temp2 != NULL || temp1 != NULL);	
	
	
/*	if(temp1 != NULL){ 	
		do{
			if(carry != 0){
				carry_count++;
				addition ++;
			}
			result = (temp1->value + carry) % mod;
			carry = (((temp1->value) + carry) / mod);
			insert_at_head(sum,(result));
			temp1 = temp1->prev;
		}while(temp1 != NULL);
	} */
	if(carry != 0)
		insert_at_head(sum,carry);
	if(mul){
		res->head = sum->head;	
		res->tail = sum->tail;
		free(sum);
	}
	if(res == NULL){
		fprintf(file,"sum %d\n",addition);
		store_list(sum);
		printf("addition\n");
		display_list(sum);
		addition = 0;
		carry_count = 0;
		clear_memory(sum);
		free(sum);
	}
}

/**
  * This function subtracts the two linked lists
  * @param r1 points to the root of 1st linked list - larger list
  * @param r2 points to the root of 2nd linked list - smaller list
  */
void subtract_lists(root *r1, root *r2){
	root *difference = NULL;
	difference = make_root();
	node *temp1, *temp2;
	
	temp1 = r1->tail;
	temp2 = r2->tail;
	int result = 0;
	int mod = create_mod(k);
	int mark = 0;

	do{
		if((temp2->value) != 0)
			subtraction++;
		
		if((temp1->value) < (temp2->value)){
			result = mod - (temp2->value) + (temp1->value);	
			mark++;
		}
		else
			result = (temp1->value) - (temp2->value);
		
		if(borrow){
			subtraction++;
			if(borrow > result){
				result = mod - borrow + result;
				mark++;
			}
			else
				result = result - borrow;
		borrow = 0;
		}
		if(mark){
			borrow = mark;
			borrow_count+= mark;
			mark = 0;
		}
		temp1 = temp1->prev;
		temp2 = temp2->prev;
		insert_at_head(difference,result);
	
	}while(temp2 != NULL || temp1 !=  NULL);
	fprintf(file,"difference %d\n",subtraction + borrow_count);
	store_list(difference);
	printf("difference\n");
	display_list(difference);
	clear_memory(difference);
	free(difference);
}

/**
  * This function multiples two linked list
  * @param r1 points to the root of the larger number
  * @param r2 points to the root of the smaller number
  */
void multiplication(root *r1, root *r2){
	int num = 0;
	int i = 0, j = 0;
	int result = 0;
	int mod = create_mod(k);
	node *temp1,*temp2;
	if(lengths[0] >= lengths[1]){
		num = lengths[1];

	}
	else{ 
		num = lengths[0];
		//temp1 = r2->tail;
		//temp2 = r1->tail;
	}
	temp1 = r1->tail;
	temp2 = r2->tail;
	root *list1, *list2;
	//list1 = make_root();
	//list2 = make_root();
	root *stores[num];
	root *product;
	product = make_root();
	while(j < num){
		list1 = make_root();
		list2 = make_root();
		int temp = 0;
		while(temp < j){
			//for(i = 0; i < k-1; i++){
				insert_at_tail(list1,0);
				insert_at_tail(list2,0);
			//}
			temp++;
		}
		insert_at_tail(list1,0);
		do{	
			if((temp1->value != 0 && temp2->value != 0) && (temp1->value != 1 && temp2->value != 1))	
				product_count++;
			result = (temp1->value) * (temp2->value);
			insert_at_head(list1,(result / mod));
			insert_at_head(list2,(result % mod));
		label1:	temp1 = temp1->prev;	
		}while(temp1 != NULL);
		stores[j] = make_root();
		insert_at_head(list2,0);
		add_lists(list1, list2,stores[j],0);
		clear_memory(list1);
		clear_memory(list2);
		free(list1);
		free(list2);
		temp2 = temp2->prev;
		temp1 = r1->tail;
		j++;
	}
	zero_pad(product,(stores[num-1]->num));
	for(i = num-1; i >= 0; i--){
		zero_pad(stores[i],(product->num) - (stores[i]->num));
		add_lists(product,stores[i],product,1);
		clear_memory(stores[i]);
		free(stores[i]);
	}
	fprintf(file,"product %d\n",product_count + addition);
	printf("product %d, sum %d\n",product_count,addition);
	store_list(product);
	printf("product:\n");
	display_list(product);
	clear_memory(product);
	free(product);
}
	

	

int main(int argc, char* argv[]){
	file = fopen(argv[4],"w");
	char k_char[2];
	k_char[0] = argv[1][1];
	k_char[1] = '\0';
	k = atoi(k_char);
	root *r1 = NULL , *r2 = NULL;
	r1 = make_root();
	r2 = make_root();
	char *store = NULL;
	size_t digits = 0;
	size_t len = 0;
	int number = 0;
	char *temp_store = malloc(1);
	int i = 0;
	for(i = 2;i <= 3;i++){
		FILE *file =  fopen(argv[i],"r");
		while(fscanf(file,"%c",temp_store) != EOF)
			number++;
		store = malloc(number+1);
		rewind(file);
		if(fgets(store,number+1,file) != NULL)
		//store[number+1] ='\0';
		//if((digits = getdelim(&store, &len, EOF,file )) != -1)
			 fix_string(store,number);
		lengths[i-2] = ceil((float)number/k);
		if(i == 2)
			store_num(store,r1,strlen(store));
		else
			store_num(store,r2,strlen(store));
		number = 0;
		fclose(file);
	}
	if(lengths[0] >= lengths[1]){
		zero_pad(r2,(lengths[0]-lengths[1]));
		printf("number1:\n");
		display_list(r1);
		printf("number2:\n");
		display_list(r2);
		add_lists(r1,r2,NULL,0);
		subtract_lists(r1,r2);
		multiplication(r1, r2);
	}
	else{
		zero_pad(r1,(lengths[1]-lengths[0]));
		printf("number1:\n");
		display_list(r1);
		printf("number2:\n");
		display_list(r2);
		add_lists(r2,r1,NULL,0);
		subtract_lists(r2,r1);
		multiplication(r2, r1);
	}
	free(temp_store);
	clear_memory(r2);
	clear_memory(r1);
	free(r1);
	free(r2);
	free(store);
	return 0;
}
