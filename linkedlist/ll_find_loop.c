/*

  A linked list is said to contain a cycle if any node is visited more than once while traversing the list.

Complete the function provided for you in your editor. It has one parameter: a pointer to a Node object named that points to the head of a linked list. Your function must return a boolean denoting whether or not there is a cycle in the list. If there is a cycle, return true; otherwise, return false.

Note: If the list is empty, will be null.


 */


/*
Detect a cycle in a linked list. Note that the head pointer may be 'NULL' if the list is empty.

A Node is defined as: 
    struct Node {
        int data;
        struct Node* next;
    }
*/

bool has_cycle(Node* head) {
    // Complete this function
    // Do not write the main method
    
    struct Node *p1 = head;
    struct Node *p2 = head;
    
    while(p1!=NULL && p2!=NULL && p2->next!=NULL) {
        
        p1=p1->next;
        p2=p2->next->next;
        if(p1==p2) {
            return 1;
        }
        else
            continue;
    }
    return 0;   
}
