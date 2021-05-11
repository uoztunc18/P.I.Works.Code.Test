#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void fillTree();
int checkPrime();
int returnMaxChild();
int findMaxPath();

struct node {
    int data;
    int level;
    int rank;
    struct node* left;
    struct node* right;
};
 
struct node* newNode(int data, int level, int rank)
{
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->data = data;
    node->level = level;
    node->rank = rank;
    node->left = NULL;
    node->right = NULL;
    return (node);
}
    
int main()
{
    char line[256];	
    char *token;
    int levelCount = 0;
    FILE *x = fopen("input.txt", "r");
    if (x==NULL){
        printf("ERROR");
		return 0;
	}
	while (!feof(x)) {
			fgets(line, 256, x);
			levelCount++;
	}
	rewind(x);

    struct node set[(levelCount*(levelCount+1))/2];
    int lvl = 0;
    int count = 0;
    int value;
    
    fgets(line, 256, x);
    token = strtok(line, " ");
    value = atoi(token);
    set[count] = *newNode(value, lvl, count);
    count++;
    lvl++;
    while (!feof(x)) {
        fgets(line, 256, x);
        token = strtok(line, " ");
        value = atoi(token);
        set[count] = *newNode(value, lvl, count);
        set[count-lvl].left = &set[count];
        count++;

        token = strtok(NULL, " ");
        
        while (token != NULL) {
            value = atoi(token);
            set[count] = *newNode(value, lvl, count);
            
            token = strtok(NULL, " ");
            
            if (token == NULL) {
                set[count-lvl-1].right = &set[count];
            } else {
                set[count-lvl-1].right = &set[count];
                set[count-lvl].left = &set[count];
            }
            count++;
        }
        lvl++;
    }

    struct node accum = *newNode(set[0].data, 0, 0);
    fillTree(&set[0], &accum);

    accum.data += findMaxPath(&accum);

    printf("The maximum sum of the numbers from top to bottom is %d.\n", accum.data);

    return 0;
}

void fillTree(struct node *input, struct node *tree) {

    if (input->left != NULL && input->right != NULL) {
        if ((checkPrime(input->left->data))&&(checkPrime(input->right->data))) {
            //finishing
        } else if (checkPrime(input->left->data)) {
            tree->data += input->right->data;
            fillTree(input->right, tree);
        } else if (checkPrime(input->right->data)) {
            tree->data += input->left->data;
            fillTree(input->left, tree);
        } else {
            tree->left = newNode(input->left->data, (tree->level)+1, 0);
            tree->right = newNode(input->right->data, (tree->level)+1, 0);
            fillTree(input->left, tree->left);
            fillTree(input->right, tree->right);
        }
    }
}

int checkPrime(int x) {
    int i;
    for (i = 2; i <= x - 1; i++) {
        if (x % i == 0) return 0;
    }

    if (i == x) return 1;
}

int returnMaxChild(struct node *node) {
    if (node->left->data >= node->right->data) {
        return node->left->data;
    } else {
        return node->right->data;
    }
}

int findMaxPath(struct node *node) {
    if (node->left->left == NULL && node->right->right == NULL) {
        return returnMaxChild(node);
    } else if (node->left->left == NULL) {
        node->right->data += findMaxPath(node->right);
        return returnMaxChild(node);
    } else if (node->right->right == NULL) {
        node->left->data += findMaxPath(node->left);
        return returnMaxChild(node);
    } else {
        node->left->data += findMaxPath(node->left);
        node->right->data += findMaxPath(node->right);
        return returnMaxChild(node);
    }
}