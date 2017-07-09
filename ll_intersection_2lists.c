/*

  Write a program to find the node at which the intersection of two singly linked lists begins.

For example, the following two linked lists:

A:          a1 → a2
                   ↘
                     c1 → c2 → c3
                   ↗            
B:     b1 → b2 → b3

begin to intersect at node c1.

Notes:

    If the two linked lists have no intersection at all, return null.
    The linked lists must retain their original structure after the function returns.
    You may assume there are no cycles anywhere in the entire linked structure.
    Your code should preferably run in O(n) time and use only O(1) memory.

 */


NOTE: Not fully completed


/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    
    
    struct ListNode *tempa = headA;
    struct ListNode *tempb = headB;
    int lena=0, lenb=0,i=0,j=0, diff=0;
    int *p1=NULL,
        *p2=NULL;
    
    while(tempa != NULL) {
                
        lena++;
        tempa=tempa->next;
    }
    
    while(tempb != NULL) {
        
        lenb++;
        tempb=tempb->next;
    }
    
    diff = abs(lena-lenb);
    tempa = headA;
    tempb = headB;
    
    if(lena > lenb) {
        
        int cmn=1;
        while(tempa != NULL) {
            
            if(cmn<diff) {
                tempa=tempa->next;
                cmn++;
            }
            else {
                if(tempa->next == tempb->next) {
                    return tempa; 
                }
                else {
                    tempa=tempa->next;
                    tempb=tempb->next;    
                }
           }
       }
        
    }
    else if(lena < lenb)  {
        
        
        
    }
                   
    return NULL;
}
