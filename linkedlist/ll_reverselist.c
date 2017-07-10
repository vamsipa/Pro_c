/*

  Reverse a singly linked list.

 */


/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* reverseList(struct ListNode* head) {
     
    
    struct ListNode *temp = head;
    struct ListNode *frontnode, *backnode;
    int data,front,back,len =0;
    while(temp!=NULL) {
        len++;
        temp=temp->next;
    }
    
    front=1;
    back=len;
    frontnode=head;
    
    while(front<back) {
        int k=1;
        backnode = head;
        while(k!=back) {
            backnode=backnode->next;
            k++;
        }
        
        data = frontnode->val;
        frontnode->val = backnode->val;
        backnode->val=data;
        
        frontnode=frontnode->next;
        front++;
        back--;
          
    }
   return head;
}
    
    
