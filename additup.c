//Eva Fineberg
//Ruby Boyd
//additup.c

#include<strings.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


//This struct builds a node with an integer value with pointers to the previous and next nodes
struct Node {
    int digit; 
    struct Node *prev, *next;
};

//This struct builds a BigInt which is a linked list of digits
//It has pointers to the head and tail of the list an also a constant pointer (super_last) to the end of the list
struct BigInt {
	int count;
	struct Node *first, *last, *super_last;

};

//This is the accumulator linked list which is where we store the current total
struct BigInt accumulator;
struct Node* curr;
//This function makes nodes by mallocing space. It sets their values based on the number passed in.
struct Node* node_maker(int value){
	struct Node* eva = (struct Node*)malloc(sizeof(struct Node));
	if(eva==NULL){
		printf("%s\n", "Malloc failed :(");
		exit(3);
	}
	eva->digit=value;
	eva->prev=NULL;
	eva->next=NULL;
	return eva;

}

//This function frees all the space
void destroy(struct Node* n){
	struct Node* temp;
	temp=n;
	while(n){
		temp=n;
		n=n->next;
		free(temp);
	
	}


}


//This function handles all the formatting requirements, reads in digits, creates nodes and adds them to our linked list.
struct BigInt getInput(){
	int ch;
	struct BigInt number;
	number.count=0;
	while(isspace(ch=getchar())){
	}
	ungetc(ch, stdin);
	while((ch=getchar())=='0'){
	}
	ungetc(ch,stdin);
	curr = node_maker(0); 
	number.first=curr;
	number.last=curr;
	number.super_last=curr;
	while(isdigit(ch=getchar())){
		ch=ch-'0';
	
		struct Node * next= node_maker(0);
		curr->digit=ch;
		if(number.count==0){
			number.first=curr;
		}
		curr->next = next;
		next->prev = curr;
		curr = next;
		number.count++;
			
	}
	ungetc(ch,stdin);
	while((ch=getchar())!='\n'){}
	number.last=curr;
	number.super_last=curr;
	return number;

	
}

//This function prints a BigInt by iterating through the linked list and printing the value of each node.
void printInt(struct BigInt nextInt){

	while(nextInt.first->next!=NULL){
		printf("%d",nextInt.first->digit);
		
		nextInt.first=nextInt.first->next;
	}
	printf("%s","\n");

}


//This function compares two ints.
int cmp(int x, int y){

	if(x<y){
		return -1;
	}
	else if(x>y){
		return 1;
	}
	else{
		return 0;
	}

}



//This function adds the number represented by the linked list t_int to the accumulator which represents the running total.
//It handles carrying in order to add properly.
void addInt(struct BigInt t_int){
	int k=0;
	int countCheck=0;
	int zeroCheck=0;
	int x_small=0; //if accumulator is smaller, this is set to 1
	int y_small=0; //if the accumulator is larger, this is set to 1
	if(t_int.first->digit==0){
		return;
	}
	if((cmp(accumulator.count,t_int.count))<0){
		countCheck=t_int.count;
		zeroCheck=t_int.count-accumulator.count;
		x_small=1;
		for(int i=0; i<zeroCheck; i++){
			struct Node* zero = node_maker(0);
			zero->next=accumulator.first;
			accumulator.first->prev=zero;
			accumulator.first=zero;
			accumulator.count++;
		}
	}
	else if((cmp(accumulator.count,t_int.count))>0){
		countCheck=accumulator.count;
		zeroCheck=accumulator.count-t_int.count;
		y_small=1;
		for(int i=0; i<zeroCheck; i++){
			struct Node* zero = node_maker(0);
			zero->next=t_int.first;
			t_int.first->prev=zero;
			t_int.first=zero;
			t_int.count++;
		}
	}
	else{
		countCheck=t_int.count;
		zeroCheck=t_int.count;
	}
	while(k<countCheck+1){
		
		int n = accumulator.last->digit + t_int.last->digit;
		if(n>9){
			accumulator.last->digit=n-10;
			
			//if carry to null node
			if(accumulator.last->prev==NULL){
				struct Node* carry = node_maker(0);
				carry->digit=1;
				accumulator.last->prev=carry;
				accumulator.last->prev->next=accumulator.last;
				accumulator.first=accumulator.last->prev;
				accumulator.count++;
			}
			else{
				accumulator.last->prev->digit=accumulator.last->prev->digit+1;
			}
			accumulator.last=accumulator.last->prev;
			t_int.last=t_int.last->prev;
		}else{
			accumulator.last->digit=n;
			accumulator.last=accumulator.last->prev;
			t_int.last=t_int.last->prev;
		}	
		
		k++;
		
	}
	accumulator.last=accumulator.super_last;
	if(x_small==1){	
		while(accumulator.first->digit==0){
			accumulator.first=accumulator.first->next;
			free(accumulator.first->prev);
		}
	}
	if(y_small==1 && t_int.count!=0){
		while(t_int.first->digit==0){
			t_int.first=t_int.first->next;
			free(t_int.first->prev);
		}
		
	}
	x_small = 0;
	y_small = 0;
}


//Main calls getInput() and addInt(). 
//It handles the 0 case and EOF case.
//It prints all output.
int main(){
	int ch;
	accumulator.first=node_maker(0) ;
	accumulator.last=accumulator.first;
	accumulator.super_last=accumulator.first;
	accumulator.first->digit=0;
	while((ch=getchar())!=EOF){
		ungetc(ch,stdin);
		struct BigInt temp = getInput();
		if(accumulator.first==accumulator.last && accumulator.last==accumulator.first){
			free(accumulator.first);
			accumulator=temp;
			printf("%s", "Total: ");
			if(accumulator.count==0){
				printf("%d",0);
			}
			else{
				printInt(accumulator);
			}
			continue;
		}
		if(temp.count==0){
			struct BigInt t2;
			struct Node* t = node_maker(0);
			t2.first=t;
			t2.last=t;
			t2.super_last=t;
			t2.count=0;
			temp=t2;
			free(curr);
		}
		addInt(temp);
		printf("%s","Total: ");
		printInt(accumulator);
		destroy(temp.first);
	}
	//time to freeeee space

	destroy(accumulator.first);







}
