/*
  Remove all elements from a linked list of integers that have value val.

Example
Given: 1 --> 2 --> 6 --> 3 --> 4 --> 5 --> 6, val = 6
Return: 1 --> 2 --> 3 --> 4 --> 5 

 */


/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* removeElements(struct ListNode* head, int val) {
    
    struct ListNode *temp = head;
    
    int data = val;
    if (head == NULL)
    {
        return head;
    }
    
    while(head->val==data)
    {
        head = head->next;
        if(head==NULL)
            return head;
    }
        
    while(temp->next!=NULL)
    {
        if(temp->next->val == data)
            {
                temp->next = temp->next->next;
                continue;
            }
        else
            {
                    
            }    
        
        temp=temp->next;
    }
    
    if (temp->next == NULL)
    {
        if(temp->val == data)
            {
                temp = temp->next;
               
            }
    }
    
    return head;
}
