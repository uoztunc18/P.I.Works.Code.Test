//Done by Umut Öztunç - uoztunc18@ku.edu.tr
//Koç University - Computer Engineering & Economics

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void fillTree();
int checkPrime();
int returnMaxChild();
int findMaxPath();

struct node {
    int data;
    struct node* left;
    struct node* right;
};
 
struct node* newNode(int data)
{
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return (node);
}
    
int main()
{
    char line[256];	
    char *token;
    int levelCount = 0;
    
    FILE *file = fopen("input.txt", "r");
    if (file==NULL){
        printf("No such file ?!");
		return 0;
	}
	while (!feof(file)) {      //getting the number of lines to determine the size of array which will store input numbers in the pyramid
			fgets(line, 256, file);
			levelCount++;
	}
	rewind(file);

    struct node set[(levelCount*(levelCount+1))/2];
    int lvl = 0;
    int count = 0;
    int value;
    
    fgets(line, 256, file);
    token = strtok(line, " ");
    value = atoi(token);
    set[count] = *newNode(value);
    count++;
    lvl++;
    while (!feof(file)) {
        fgets(line, 256, file);
        token = strtok(line, " ");
        value = atoi(token);
        set[count] = *newNode(value);
        set[count-lvl].left = &set[count];  //setting child-parent relation of the LEFTMOST number on the current line and its parent
        count++;

        token = strtok(NULL, " ");
        
        while (token != NULL) {
            value = atoi(token);
            set[count] = *newNode(value);
            
            token = strtok(NULL, " ");
            
            if (token == NULL) {
                set[count-lvl-1].right = &set[count];   //setting child-parent relation of the RİGHMOST number on the current line and its parent
            } else {
                set[count-lvl-1].right = &set[count];   //setting child-parent relation of the middle numbers on the current line and its (their) parent(s)
                set[count-lvl].left = &set[count];
            }
            count++;
        }
        lvl++;
    }

    struct node accum = *newNode(set[0].data);      //start point of the accumulation tree
    fillTree(&set[0], &accum);      //discarding prime numbers and accumulating...

    accum.data += findMaxPath(&accum);      //replacing the data value of accum with the max sum path of the pyramid, which is the expected answer!

    printf("The maximum sum of the numbers from top to bottom is %d.\n", accum.data);

    return 0;
}

//This function creates a new, so-called "ACCUMULATION TREE", discarding the prime numbers
//among the input numbers. The parameters are the start point of the input numbers' tree and a node having 
//a node having created for a start point of the new tree, which will then, grow downwards, recursively. 
//It does not create children nodes for ITSELF(accumulation tree) unless both children
//nodes (in the original input tree) are prime number. 
void fillTree(struct node *input, struct node *tree) {

    if (input->left != NULL && input->right != NULL) {
        if ((checkPrime(input->left->data))&&(checkPrime(input->right->data))) {     //in case of both children with prime numbers, do nothing, stop
            
        } else if (checkPrime(input->left->data)) {                                  //in case of one child with prime number, update the value of this node
            tree->data += input->right->data;                                        //with the value itself plus the value of other non-prime child.
            fillTree(input->right, tree);                                            //Thnaks to that, we accumulate the sum until encountering a seperation.
        } else if (checkPrime(input->right->data)) {                                 //Then call itself recursively on the non-prime child.
            tree->data += input->left->data;
            fillTree(input->left, tree);
        } else {                                                                     //in case of both children with non-prime numbers, it is time to 
            tree->left = newNode(input->left->data);                                 //grow through different paths downwards.
            tree->right = newNode(input->right->data);
            fillTree(input->left, tree->left);
            fillTree(input->right, tree->right);
        }
    }
}

//Prime number controler
int checkPrime(int x) {
    int i;
    for (i = 2; i <= x - 1; i++) {
        if (x % i == 0) return 0;
    }

    if (i == x) return 1;
}

//For a node which has two children, this function returns the maximum among the children
int returnMaxChild(struct node *node) {
    if (node->left->data >= node->right->data) {
        return node->left->data;
    } else {
        return node->right->data;
    }
}

//This function takes the start point of the ACCUMULATION tree as a parameter; and finds the maximum sum, 
//traversing all child-parent relationships recursively from bottom to top. In every call, it determines
//the max among the children and return that number and move one step upward.
int findMaxPath(struct node *node) {
    if (node->left->left == NULL && node->right->right == NULL) {       //for the parent of both-childless nodes
        return returnMaxChild(node);
    } else if (node->left->left == NULL) {                              //for the parent of one child-less node (left one is childless)
        node->right->data += findMaxPath(node->right);
        return returnMaxChild(node);
    } else if (node->right->right == NULL) {                            //for the parent of one child-less node (right one is childless)
        node->left->data += findMaxPath(node->left);
        return returnMaxChild(node);
    } else {
        node->left->data += findMaxPath(node->left);                    //for the parent of two children, both having children too
        node->right->data += findMaxPath(node->right);
        return returnMaxChild(node);
    }
}