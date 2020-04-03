#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct n{
	char adress[100];
	int count;
	struct n * prev;
	struct n * next;
};
typedef struct n node;

//	Parameter: Head * of linked list.
void printList(node * head);

//	Parameters: Head * of linked list, string to be searched | Returns 1 if finds the string, else retuns 0.
int included(node * head, char arg[3]);

//	Parameters: Head * of linked list, string to be searched | Returns the adress of node which includes the string, if string is not found returns NULL.
node * searchList(node * head, char arg[3]);

//	Parameter: Adress member of first node | Creates first node of linked list and fills the adress member with entered parameter. Returns the head *.
node * createList(char a[]);

//	Parameter: Head * of linked list | Clears cache by deleting the list.
void clearCache(node * head);

int main(){
	FILE * fp;
	node * head, * tmp, * tmp2;
	int i,T,L,nodeCount=0,inputType=0;
	char a[100],clear[2],fileName[100],buff[10];
		
	printf("Please enter the name of the file if you want to request pages by a text file.\nPress enter to request manually.\n");
	gets(fileName);
	printf("\n");
	fp=fopen(fileName,"r");
	
	//	If file name is empty (pressed enter) or invalid, user enters requests manually:
	if(strcmp(fileName,"")==0 || fp==NULL){
		if(strcmp(fileName,"")!=0){
			printf("Invalid file name. Program could not run.\n\n");
		}
		inputType=0;
	
		printf("Please enter the threshold value (T):\n");
		scanf("%d",&T);
		
		printf("Please enter the lenght (capacity) (L):\n");
		scanf("%d",&L);
		
		printf("Please enter the requests (Enter 'stop' to end program):\n");
		scanf("%s",&a);
	}
	//	If file name is correctly entered by user, reads requests by the file:
	else{
		inputType=1;
		
		//	Reading T:
		i=0;
		while(buff[i-3]!='='){
			buff[i]=fgetc(fp);
			i++;
		}
		T=atoi(&buff[i-2]);
		
		//	Reading L:
		i=0;
		while(buff[i-3]!='='){
			buff[i]=fgetc(fp);
			i++;
		}
		L=atoi(&buff[i-2]);
		
		fscanf(fp,"%s",a);
	}
	
	head = createList(a);
	printList(head);
	nodeCount++;
	
	while(!feof(fp) && strcmp(a,"stop")!=0){
		
		//	Read request by user (manually):
		if(inputType==0){
			scanf("%s",&a);
		}
		//	Read request by file:
		else{
			fscanf(fp,"%s",a);
		}
		
		//	If request does not exist in the cache buffer, creates a new node.
		if(included(head,a)==0 && strcmp(a,"stop")!=0){	
			tmp2=(node *)malloc(sizeof(node));
			nodeCount++;
			tmp2->next=head;
			tmp2->prev=NULL;
			head->prev=tmp2;
			head=tmp2;
			tmp=tmp2;
			strcpy(tmp->adress,a);
			tmp->count=1;
		}
		
		//	If request exists in the cache buffer, increases count of related node.
		else if(strcmp(a,"stop")!=0){	
			tmp=searchList(head,a);
			tmp->count++;
			
			//	When count is over T, moves the related node to head.
			if(tmp->count > T){
				//	If it is the head node:
				if(tmp->prev==NULL){	
				
				}
				//	If it is the tail node:
				else if(tmp->next==NULL){	
					tmp2=tmp->prev;
					tmp2->next=NULL;
					tmp->prev=NULL;
					head->prev=tmp;
					tmp->next=head;
					head=tmp;
				}
				//	If it is an interval node:
				else{
					tmp2=tmp->prev;
					tmp2->next=tmp->next;
					tmp->next->prev=tmp2;
					tmp->prev=NULL;
					head->prev=tmp;
					tmp->next=head;
					head=tmp;
				}
			}		
		}
		
		//	If capacity exceeded, deletes the tail node.
		if(nodeCount > L){
			while(tmp->next!=NULL){
				tmp=tmp->next;
			}
			tmp2=tmp->prev;
			tmp2->next=NULL;
			tmp->prev=NULL;
			free(tmp);
			nodeCount--;
		}	
		printList(head);
	}
	
	printf("\nDo you wish to clear cache? (Enter 'Y' for yes, 'N' for no)\n");
	scanf("%s",clear);
	
	if(strcmp(clear,"y")==0 || strcmp(clear,"Y")==0){
		clearCache(head);
		printf("Cache cleared.");
	}
	else if(strcmp(clear,"n")==0 || strcmp(clear,"N")==0){
		printf("Cache not cleared.");
	}
	else{
		printf("Invalid answer. Cache not cleared.");
	}
	
	fclose(fp);
	return 0;
}

void printList(node * head){	
	node * tmp;
	tmp = head;
	printf("%s,%d",tmp->adress,tmp->count);
	tmp=tmp->next;
	
	while(tmp!=NULL){
		printf(" <=> %s,%d",tmp->adress,tmp->count);
		tmp=tmp->next;
	}
	printf("\n\n");
}

int included(node * head, char arg[3]){ 	
	node * tmp;
	tmp = head;
	int counter=0;
	while(tmp!=NULL){
		if(strcmp(tmp->adress,arg)==0)
			counter++;
		tmp=tmp->next;	
	}
	return counter;
}

node * searchList(node * head, char arg[3]){ 	
	node * tmp;
	tmp = head;
	int counter=0;
	while(tmp!=NULL){
		if(strcmp(tmp->adress,arg)==0)
			return tmp;
		tmp=tmp->next;	
	}
	return NULL;
}

node * createList(char a[]){
	node * head=(node *)malloc(sizeof(node));
	node * tmp=head;
	tmp->next=NULL;
	tmp->prev=NULL;
	strcpy(tmp->adress,a);
	tmp->count=1;
	return head;
}

void clearCache(node * head){	
	node * tmp;
	node * tmp2;
	tmp = head;
	tmp2 = tmp->next;
	
	while(tmp2!=NULL){
		tmp->prev=NULL;
		tmp->next=NULL;
		free(tmp);
		tmp=tmp2;
		tmp2=tmp->next;	
	}
	head=NULL;
}
