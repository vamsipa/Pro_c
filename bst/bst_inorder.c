/*

  Complete the inOrder function in your editor below, which has 1 parameter: a pointer to the root of a binary tree. It must print the values in the tree's inorder traversal as a single line of space-separated values. 

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

void inOrder(node *root) {

    struct node *temp = root;
    
    
    if(temp!= NULL) {
        inOrder(temp->left);
        printf("%d ", temp->data);
        inOrder(temp->right);
    }
    
    return;
}
