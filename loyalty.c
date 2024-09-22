// Daniel A. Gomez
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 19

typedef struct customer {
char name[MAXLEN+1];
int points;
} customer;
typedef struct treenode {
customer* cPtr;
int size;
struct treenode* left;
struct treenode* right;
} treenode;

int add(char name[MAXLEN + 1], treenode *loyTree, int checker);
void sub(char name[MAXLEN + 1], treenode *loyTree);
int del(char name[MAXLEN + 1], treenode *loyTree, int checker);
void replaceLeft(treenode *firTree, treenode *secTree);
void replaceRight(treenode *firTree, treenode *secTree);
void search(char name[MAXLEN + 1], treenode *loyTree, int depth);
void count_smaller(char name[MAXLEN + 1], treenode *loyTree, int counter);
int array_store(customer **cusArr, treenode *loyTree, int tracker);
int partition(customer **values, int start, int end);
void swap(customer **a, customer **b);
void quickSort(customer **values, int start, int end);
int letPartition(customer **values, int start, int end);
void letterSort(customer **values, int start, int end);
void samePoints(customer **cusArr, int end);

int main()
{
    // Initializing the loyalty tree used for the rest of program.
    treenode *loyTree = malloc(sizeof(treenode));
    loyTree->cPtr = NULL;
    loyTree->size = 0;

    // Scan the users input for how many actions will be taken by the program.
    int numInputs;
    scanf("%d", &numInputs);

    // The program will execute however many commands the user decided in the beginning.
    for(int i = 0; i < numInputs; i++)
    {

        // Scan both which command the user wants executed and with which name.
        char command[100];
        char name[MAXLEN + 1];
        int checker = 0;
        scanf("%s %s", command, name);

        // Given what command the user chooses, the program takes them to the associated
        // function to execute said name.
        if(strcmp(command, "add") == 0)
            add(name, loyTree, checker);
        else if(strcmp(command, "sub") == 0)
            sub(name, loyTree);
        else if(strcmp(command, "del") == 0)
            del(name, loyTree, checker);
        else if(strcmp(command, "search") == 0)
        {
            int depth = -1;
            search(name, loyTree, depth);
        }
        else if(strcmp(command, "count_smaller") == 0)
        {
            int counter = 0;
            count_smaller(name, loyTree, counter);
        }
    }

    // After the user chosen commands have finished being executed, a customer array is created
    // where each customer node is placed into.
    int tracker = 0;
    int arrLen = loyTree->size;
    customer **cusArr = malloc(arrLen * sizeof(customer*));

    // First, each customer is placed into the customer array in alphabetical order, directly from
    // the tree. Afterwards, it is arranged in point order, before finally being organized into alphabetical
    // order where the number of points between customers is tied.
    array_store(cusArr, loyTree, tracker);
    quickSort(cusArr, 0, arrLen - 1);
    samePoints(cusArr, arrLen);

    // Print out the customer names and points in the order of the customer array.
    for(int i = arrLen - 1; i >= 0; i--)
        printf("%s %d\n", cusArr[i]->name, cusArr[i]->points);

    // Data is freed.
    for(int i = 0; i < arrLen; i++)
        free(cusArr[i]);
    free(cusArr);
    free(loyTree);
}

// In this function, a customer is either inserted into a proper place in the loyalty tree, or
// if the customer already exists, the requested number of points to be added to the customers account is added.
// Then, it prints out the name and current number of points of the customer.
int add(char name[MAXLEN + 1], treenode *loyTree, int checker)
{
    // If the customers name does not exist, their account is created here. Any and all root nodes
    // of this customer get +1 added to their size.
    if(loyTree->cPtr == NULL)
    {

        loyTree->cPtr = malloc(sizeof(customer));
        loyTree->left = malloc(sizeof(treenode));
        loyTree->right = malloc(sizeof(treenode));
        loyTree->left->cPtr = NULL;
        loyTree->right->cPtr = NULL;

        loyTree->size = 1;
        strcpy(loyTree->cPtr->name, name);
        scanf("%d", &loyTree->cPtr->points);
        printf("%s %d\n", loyTree->cPtr->name, loyTree->cPtr->points);
        checker = 0;
        return checker;
    }

    // If the customers name is recognized in the tree, the number of points requested to
    // be added to the account is added.
    else if(strcmp(loyTree->cPtr->name, name) == 0)
    {
        int points;
        scanf("%d", &points);
        loyTree->cPtr->points += points;
        printf("%s %d\n", loyTree->cPtr->name, loyTree->cPtr->points);
        checker = 1;
        return checker;
    }

    // If the customer's name does not match the current node's name, go either to the left
    // or right branch of the node, dependant upon whether the name comes before or after it
    // in alphabetical order. In the case that a new node is created, the root nodes get +1 to
    // it's size.
    else if(strcmp(loyTree->cPtr->name, name) < 0)
    {
        checker = add(name, loyTree->right, checker);
        if(checker == 0)
            loyTree->size++;
        return checker;
    }
    else if(strcmp(loyTree->cPtr->name, name) > 0)
    {
        checker = add(name, loyTree->left, checker);
        if(checker == 0)
            loyTree->size++;
        return checker;
    }
}

// In this function, the customer name is searched for in the loyalty tree, then if it's found
// it subtracts the amount of points specified by the user, down to a minimum of 0, then prints
// out the name and number of points of the customer. Otherwise, it prints that the customer was
// not found.
void sub(char name[MAXLEN + 1], treenode *loyTree)
{
    // If the name was not found, print "name not found", using a waster integer
    // for the points that was desired to be subtracted.
    if(loyTree->cPtr == NULL)
    {
        int waster;
        scanf("%d", &waster);
        printf("%s not found\n", name);
        return;
    }

    // If the name is found, subtract the total number of points specified by the user,
    // down to a minimum of 0, then print the name and number of remaining points.
    if(strcmp(loyTree->cPtr->name, name) == 0)
    {
        int subTotal = 0;
        scanf("%d", &subTotal);
        loyTree->cPtr->points -= subTotal;
        if(loyTree->cPtr->points < 0)
            loyTree->cPtr->points = 0;

        printf("%s %d\n", loyTree->cPtr->name, loyTree->cPtr->points);
        return;
    }

    // If the customer name does not match the current node, check either the left or
    // right branch of the node, dependent upon whether or not it is before or after it
    // in alphabetical order.
    else if(strcmp(loyTree->cPtr->name, name) < 0)
    {
        sub(name, loyTree->right);
        return;
    }
    else if(strcmp(loyTree->cPtr->name, name) > 0)
    {
        sub(name, loyTree->left);
        return;
    }
}

// In this function, the customer's name is searched for in the loyalty tree, and if
// it's there, the node is deleted, and all it's root nodes have their size subtracted by 1.
// If this node is itself a root node, it is replaced by one of it's children nodes.
int del(char name[MAXLEN + 1], treenode *loyTree, int checker)
{

    // If the name is not found in the loyalty tree, print that name was not found.
    if(loyTree->cPtr == NULL)
    {
        printf("%s not found\n", name);
        checker = 1;
        return checker;
    }

    // If the name is found in the loyalty tree, check first whether or not it has any
    // branches. If it does, replace this node with the necessary child node. Otherwise,
    // delete the node and subtract all root nodes size by 1. Then, print that the customer
    // node was deleted.
    if(strcmp(loyTree->cPtr->name, name) == 0)
    {
        if(loyTree->left->cPtr != NULL && loyTree->size > 1)
            replaceLeft(loyTree, loyTree->left);
        else if(loyTree->right->cPtr != NULL && loyTree->size > 1)
            replaceRight(loyTree, loyTree->right);
        else
        {
            free(loyTree->cPtr);
            free(loyTree->left);
            free(loyTree->right);
            loyTree->left = NULL;
            loyTree->right = NULL;
            loyTree->cPtr = NULL;
            loyTree->size = 0;
        }
        printf("%s deleted\n", name);
        checker = 0;
        return checker;
    }

    // If the current customer node does not match the user inputed node, check either
    // the left or right branch of said node, dependent upon whether or not the name comes
    // before or after it in alphabetical order. If a node is deleted, decrease the size of this
    // node by 1.
    else if(strcmp(loyTree->cPtr->name, name) < 0)
    {
        checker = del(name, loyTree->right, checker);
        if(checker == 0)
            loyTree->size--;
        return checker;
    }
    else if(strcmp(loyTree->cPtr->name, name) > 0)
    {
        checker = del(name, loyTree->left, checker);
        if(checker == 0)
            loyTree->size--;
        return checker;
    }
}

// This function is to replace a deleted root node with one of its children nodes. If this node
// Itself has child nodes, replace that node with one of its children too.
void replaceLeft(treenode *firTree, treenode *secTree)
{
    // If the second node to the left of the parent node does not have another child node to its
    // right, replace the parent node with this second node. If this node has a child node to its
    // left, replace the second node with one of its child nodes just as with the first node.
    if(secTree->right->cPtr == NULL)
    {
        int points = secTree->cPtr->points;
        char name[MAXLEN + 1];
        strcpy(name, secTree->cPtr->name);
        strcpy(firTree->cPtr->name, name);
        firTree->cPtr->points = points;
        firTree->size--;

        if(secTree->size > 1)
        {
            if(secTree->left->cPtr != NULL)
                replaceLeft(secTree, secTree->left);
            else if(secTree->right->cPtr != NULL)
                replaceRight(secTree, secTree->right);
        }
        else
        {
            free(secTree->cPtr);
            free(secTree->left);
            free(secTree->right);
            secTree->left = NULL;
            secTree->right = NULL;
            secTree->cPtr = NULL;
            secTree->size = 0;
        }
        return;
    }

    // If the current node is not the node necessary to to replace the parent node,
    // decrease the size of the second node by one, then use the right branch of the second tree
    // for another replace recursion.
    secTree->size--;
    replaceLeft(firTree, secTree->right);
}

// This function is to replace a deleted root node with one of its children nodes. If this node
// Itself has child nodes, replace that node with one of its children too.
void replaceRight(treenode *firTree, treenode *secTree)
{
    // If the second node to the right of the parent node does not have another child node to its
    // left, replace the parent node with this second node. If this node has a child node to its
    // right, replace the second node with one of its child nodes just as with the first node.
    if(secTree->left->cPtr == NULL)
    {
        int points = secTree->cPtr->points;
        char name[MAXLEN + 1];
        strcpy(name, secTree->cPtr->name);
        strcpy(firTree->cPtr->name, name);
        firTree->cPtr->points = points;
        firTree->size--;

        if(secTree->size > 1)
        {
            if(secTree->left->cPtr != NULL)
                replaceLeft(secTree, secTree->left);
            else if(secTree->right->cPtr != NULL)
                replaceRight(secTree, secTree->right);
        }
        else
        {
            free(secTree->cPtr);
            free(secTree->left);
            free(secTree->right);
            secTree->left = NULL;
            secTree->right = NULL;
            secTree->cPtr = NULL;
            secTree->size = 0;
        }
        return;
    }

    // If the current node is not the node necessary to to replace the parent node,
    // decrease the size of the second node by one, then use the left branch of the second tree
    // for another replace recursion.
    secTree->size--;
    replaceRight(firTree, secTree->left);
}

// In this function, customer is searched for in the loyalty Tree. If it's found, print the name, number
// of points, and depth of the node. Otherwise, print that the customer was not found.
void search(char name[MAXLEN + 1], treenode *loyTree, int depth)
{
    depth++;

    // If the customer name was not found, print that the customer was not found.
    if(loyTree->cPtr == NULL)
    {
        printf("%s not found\n", name);
        return;
    }

    // If the name was found, print out the name, points, and depth of the customer in the loyalty
    // tree.
    if(strcmp(loyTree->cPtr->name, name) == 0)
    {
        printf("%s %d %d\n", name, loyTree->cPtr->points, depth);
        return;
    }

    // If the current node does not match the customer node, check the right or left branch of this node
    // dependent upon whether the name is before or after the current node in alphabetical order.
    else if(strcmp(loyTree->cPtr->name, name) < 0)
        search(name, loyTree->right, depth);
    else if(strcmp(loyTree->cPtr->name, name) > 0)
        search(name, loyTree->left, depth);
}

// In this function, print out how many customer's are before the entered name in alphabetical order.
void count_smaller(char name[MAXLEN + 1], treenode *loyTree, int counter)
{
    // If the given name is not found in the loyalty tree, print out the counter.
    if(loyTree->cPtr == NULL)
    {
        printf("%d\n", counter);
        return;
    }

    // If the given name was found in the loyalty tree, print out counter plus all the customers
    // in the left branch of the current node.
    if(strcmp(loyTree->cPtr->name, name) == 0)
    {
        if(loyTree->left->cPtr != NULL)
            counter += loyTree->left->size;

        printf("%d\n", counter);
        return;
    }

    // If the current node doesn't match the given name, and if the current name is to the right
    // of the current node in alphabetical order, add the root node's left branch total into the
    // counter, then check the right branch to find the proper place of the given name.
    else if(strcmp(loyTree->cPtr->name, name) < 0)
    {
        if(loyTree->left->cPtr != NULL)
            counter += loyTree->left->size;

        counter++;
        count_smaller(name, loyTree->right, counter);
    }

    // If the current node doesn't match the given name, and if the current name is to the left of
    // of the current node in alphabetical order, go to the left branch in search of the name's proper
    // place.
    else if(strcmp(loyTree->cPtr->name, name) > 0)
        count_smaller(name, loyTree->left, counter);
}

// Store all of the loyalty tree nodes in the customer array, in alphabetical order.
int array_store(customer **cusArr, treenode *loyTree, int tracker)
{
    // If the left branch of the tree is not empty, store all of its nodes in alphabetical order.
    if(loyTree->left->cPtr != NULL)
        tracker = array_store(cusArr, loyTree->left, tracker);

    // Store the current customer node into the customer array.
    cusArr[tracker] = loyTree->cPtr;
    tracker++;

    // If the right branch of the tree is not empty, store all of its nodes in alphabetical order.
    if(loyTree->right->cPtr != NULL)
        tracker = array_store(cusArr, loyTree->right, tracker);

    return tracker;
}

// Swap two customer nodes.
void swap(customer **a, customer **b)
{
    customer *temp = *a;
    *a = *b;
    *b = temp;
}

// Partition two customer nodes by points.
int partition(customer **values, int start, int end)
{
    int i = start;
    int j = end;

    while (i <= j)
    {
        while(i <= end && values[i]->points <= values[start]->points)
            i++;
        while(values[j]->points > values[start]->points)
            j--;
        if(i < j)
            swap(&values[i], &values[j]);
    }

    swap(&values[start], &values[j]);
    return j;
}

// Quickly sort a customer array by points.
void quickSort(customer **values, int start, int end)
{
    if(start >= end)
        return;
    int mid = partition(values, start, end);
    quickSort(values, start, mid - 1);
    quickSort(values, mid + 1, end);
}

// Partition two customer nodes by alphabetical order.
int letPartition(customer **values, int start, int end)
{
    int i = start;
    int j = end;

    while (i <= j)
    {
        while(i <= end && strcmp(values[i]->name, values[start]->name) >= 0)
            i++;
        while(strcmp(values[j]->name, values[start]->name) < 0)
            j--;
        if(i < j)
            swap(&values[i], &values[j]);
    }

    swap(&values[start], &values[j]);
    return j;
}

// Quickly sort a customer array by alphabetical order.
void letterSort(customer **values, int start, int end)
{
    if(start >= end)
        return;
    int mid = letPartition(values, start, end);
    letterSort(values, start, mid - 1);
    letterSort(values, mid + 1, end);
}

// In the case of a number of customers having the same number of points, sort them in
// alphabetical order.
void samePoints(customer **cusArr, int end)
{
    int tracker = 0;
    int tmpStart = 0;
    int tmpEnd;
    for(int i = 0; i < end; i++)
    {
        if(i + 1 < end && cusArr[i]->points == cusArr[i + 1]->points)
        {
            tmpEnd = i + 1;
            if(tracker != 1)
            {
                tracker = 1;
                tmpStart = i;
            }
        }
        if(tmpEnd != i + 1 && tracker == 1)
        {
            letterSort(cusArr, tmpStart, tmpEnd);
            tmpStart = tmpEnd;
            tracker = 0;
        }
    }
}

