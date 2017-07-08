
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  int data;
  struct node *next;
  
} node;

//declaring global head of list
node *head=NULL;



//function prints list
int printlist() {

  node *dummy =head;
  if(dummy==NULL)
    printf("\n List is empty");

  else {
    printf("\n");
    while(dummy!=NULL) {

      printf("%d\t",dummy->data);
      dummy=dummy->next;

      }    
  }
  return 0;  
}


//insert elements into list
int appendlist(int data) {

  node *dummy=NULL;
  node *newnode = (node*) malloc(sizeof(node));
  newnode->data=data;
  newnode->next=NULL;
  
  if(head==NULL) {
    head = newnode;   
  }
  else {
    dummy = head;
    while(dummy->next!=NULL) {

      dummy = dummy->next;   
      
    }
    dummy->next=newnode;
  }
  //free(dummy);
  return 0;
}


//length of list
int lenlist() {

  int len=0;
  node *temp_lenlist=head;
  while(temp_lenlist != NULL) {

    len++;
    temp_lenlist=temp_lenlist->next;
  }  

  return len;
}

//function to remove element of linked list

void remlist(int rem) {

  node *dummy=head;; 
  
  if(dummy==NULL) {
    printf("list is empty");

  }
  else {
        
    for(;dummy->data!=rem;dummy=dummy->next)
    {
      printf("\t %u",dummy->next);

    }

  }
  
  

}





//----------------------------------------------
int main() {


  int leng;
  printlist();
  appendlist(12);
  appendlist(24);
  appendlist(05);
  printlist();
  printf("\nlength of list is:%d",lenlist());
  remlist(5);
  return 0;
}


