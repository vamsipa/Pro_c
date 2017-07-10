/*
  Given a singly linked list, determine if it is a palindrome.

Follow up:
Could you do it in O(n) time and O(1) space?
  


 */


/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
bool isPalindrome(struct ListNode* head) {
    
    struct ListNode *temp = head;
    struct ListNode *frontnode, *backnode;
    int front,back,len =0;
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
        
        if(frontnode->val == backnode->val) {
            frontnode=frontnode->next;
            front++;
            back--;
         }
        else
            return false;
            
    }
    
    return true;
    
}
