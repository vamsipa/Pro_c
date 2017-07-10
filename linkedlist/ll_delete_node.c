/*

 Write a function to delete a node (except the tail) in a singly linked list, given only access to that node.

Supposed the linked list is 1 -> 2 -> 3 -> 4 and you are given the third node with value 3, the linked list should become 1 -> 2 -> 4 after calling your function.

 */



/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
void deleteNode(struct ListNode* node) {
    
    struct ListNode *temp = NULL;
    temp = node;
    
    while(temp->next!=NULL) 
    {
        temp->val = temp->next->val;
               
        if(temp->next->next == NULL) {
            temp->next = NULL;
            break;
        }
        else
            temp = temp->next;
    }
    
    
}
