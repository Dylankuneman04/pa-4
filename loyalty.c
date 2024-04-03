#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19

typedef struct customer {
    char name[MAXLEN + 1];
    int points;
} customer;

typedef struct treenode {
    customer *cPtr;
    int size;
    struct treenode *left;
    struct treenode *right;
} treenode;

// Function prototypes
void insertCustomer(treenode **root, customer *c);
void addPoints(treenode **root, char *name, int points);
void subtractPoints(treenode *root, char *name, int points);
void deleteCustomer(treenode **root, char *name);
void searchCustomer(treenode *root, char *name, int depth);
int countSmaller(treenode *root, char *name);
void storeInArray(treenode *node, customer **arr, int *idx);
void mergeSort(customer **arr, int l, int r);
void merge(customer **arr, int l, int m, int r);
void printCustomers(customer **arr, int n);
treenode *findMaxNode(treenode *node);
treenode *deleteNode(treenode *root, char *name);

int main() {
    int n;
    scanf("%d", &n);
    getchar(); // Consume newline character
    
    treenode *root = NULL;
    treenode *nodeBuffer = NULL;

    for (int i = 0; i < n; i++) {
        printf("%d\n", i);
    
        char command[20];
        scanf("%s", command);

        if (strcmp(command, "add") == 0) {
            char name[MAXLEN + 1];
            int points;
            scanf("%s %d", name, &points);
            addPoints(&root, name, points);
        } else if (strcmp(command, "sub") == 0) {
            char name[MAXLEN + 1];
            int points;
            scanf("%s %d", name, &points);
            subtractPoints(root, name, points);
        } else if (strcmp(command, "del") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);
            root = deleteNode(root, name);
            if (root != NULL) {
                printf("%s deleted\n", name);
            }
            
        } else if (strcmp(command, "search") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);
            searchCustomer(root, name, 0);
        } else if (strcmp(command, "count_smaller") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);
            int count = countSmaller(root, name);
            printf("%d\n", count);
        }
        printf("hello\n");

    }
    printf("Made it here again");
    // Step 5: Sort customers by loyalty points and name
    if (root != NULL) {
        customer **arr = malloc(root->size * sizeof(customer *));
        int idx = 0;
        storeInArray(root, arr, &idx);
        mergeSort(arr, 0, root->size - 1);

        // Step 6: Print sorted customers
        printCustomers(arr, root->size);

        // Free memory
        for (int i = 0; i < root->size; i++) {
            free(arr[i]);
        }
        free(arr);
    }

    return 0;
}

// Function definitions
void insertCustomer(treenode **root, customer *c) {
    if (*root == NULL) {
        *root = malloc(sizeof(treenode));
        (*root)->cPtr = c;
        (*root)->size = 1;
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else {
        int cmp = strcmp(c->name, (*root)->cPtr->name);
        if (cmp < 0) {
            insertCustomer(&(*root)->left, c);
            (*root)->size++; // Increment size when inserting into the left subtree
        } else if (cmp > 0) {
            insertCustomer(&(*root)->right, c);
            (*root)->size++; // Increment size when inserting into the right subtree
        } else {
            // Customer already exists, update points
            (*root)->cPtr->points += c->points;
            free(c);  // Free memory allocated for redundant customer
        }
    }
}

void addPoints(treenode **root, char *name, int points) {;
    if (*root == NULL) {
        // Customer not found, create a new node
        customer *c = malloc(sizeof(customer));
        strcpy(c->name, name);
        c->points = points;
        insertCustomer(root, c); // Pass the address of the root pointer
        printf("%s %d\n", name, c->points); // Print the new customer's information
        return;
    }

    int cmp = strcmp(name, (*root)->cPtr->name);
    if (cmp < 0)
        addPoints(&((*root)->left), name, points); // Pass the address of the left child pointer
    else if (cmp > 0)
        addPoints(&((*root)->right), name, points); // Pass the address of the right child pointer
    else {
        // Customer found, update points
        (*root)->cPtr->points += points;
        printf("%s %d\n", (*root)->cPtr->name, (*root)->cPtr->points); // Print the updated information
    }
}

void subtractPoints(treenode *root, char *name, int points) {
    if (root == NULL) {
        printf("%s not found\n", name);
        return;
    }

    int cmp = strcmp(name, root->cPtr->name);
    if (cmp < 0)
        subtractPoints(root->left, name, points);
    else if (cmp > 0)
        subtractPoints(root->right, name, points);
    else {
        root->cPtr->points -= points;
        if (root->cPtr->points < 0)
            root->cPtr->points = 0;
        printf("%s %d\n", name, root->cPtr->points);
    }
}

treenode *findMaxNode(treenode *node) {
    while (node->right != NULL)
        node = node->right;
    return node;
}

treenode *deleteNode(treenode *root, char *name) {
    if (root == NULL) {
        printf("%s not found\n", name);
        return NULL;
    }

    int cmp = strcmp(name, root->cPtr->name);
    if (cmp < 0)
        root->left = deleteNode(root->left, name);
    else if (cmp > 0)
        root->right = deleteNode(root->right, name);
    else {
        if (root->left == NULL) {
            treenode *temp = root->right;
            free(root->cPtr);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            treenode *temp = root->left;
            free(root->cPtr);
            free(root);
            return temp;
        } else {
            // Node to be deleted has two children
            treenode *temp = findMaxNode(root->left);
            strcpy(root->cPtr->name, temp->cPtr->name);
            root->cPtr->points = temp->cPtr->points;
            root->left = deleteNode(root->left, temp->cPtr->name);
        }
    }
    root->size -= 1;
    return root;
}

void searchCustomer(treenode *root, char *name, int depth) {
    if (root == NULL) {
        printf("%s not found\n", name);
        return;
    }

    int cmp = strcmp(name, root->cPtr->name);
    if (cmp < 0)
        searchCustomer(root->left, name, depth + 1);
    else if (cmp > 0)
        searchCustomer(root->right, name, depth + 1);
    else {
        printf("%s %d %d\n", name, root->cPtr->points, depth);
    }
}

int countSmaller(treenode *root, char *name) {
    if (root == NULL)
        return 0;

    int cmp = strcmp(name, root->cPtr->name);
    if (cmp <= 0)
        return countSmaller(root->left, name);
    else
        return 1 + root->left->size + countSmaller(root->right, name);
}

void storeInArray(treenode *node, customer **arr, int *idx) {
     if (node == NULL)
        return;

    storeInArray(node->left, arr, idx);

    if (*idx >= node->size) {
        printf("Array index out of bounds\n");
        return;
    }

    arr[*idx] = node->cPtr;
    (*idx)++;

    storeInArray(node->right, arr, idx);
}

void mergeSort(customer **arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void merge(customer **arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    customer *L[n1], *R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i]->name, R[j]->name) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void printCustomers(customer **arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", arr[i]->name, arr[i]->points);
    }
}