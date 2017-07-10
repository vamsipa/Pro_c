/*
  Complete the postOrder function in your editor below, which has 1 parameter: a pointer to the root of a binary tree. It must print the values in the tree's postorder traversal as a single line of space-separated values.

 */

/* you only have to complete the function given below.  
Node is defined as  

struct node
{
    int data;
    node* left;
    node* right;
};

*/


void postOrder(node *root) {

    struct node *temp = root;
    
    if(temp!= NULL) {
        postOrder(temp->left);
        postOrder(temp->right);
        printf("%d ", temp->data);
    }
       
    return;
    
}
