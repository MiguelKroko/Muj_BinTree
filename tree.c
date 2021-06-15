#include <math.h>
#include "tree.h"

/* Definice funkci NACITANI Z KONZOLE --------------------------------------------*/

bool GetString(char *line, int maxLen) {
    if (line == NULL) return false;
    ClearStdin();
    if (fgets(line,maxLen,stdin) == NULL) {
        printf("Konec souboru.\n");
        return false;
    }
    return true;
}


bool GetChar(char *c) {
    if (c == NULL) return false;
    char line[BUFF_SIZE];
    if (GetString(line, BUFF_SIZE) == true) {
        *c = line[0];
        return true;
    }
    printf("Chyba");
    return false;
}

bool GetDouble(double *val){
    if (val == NULL) return false;
    char line[BUFF_SIZE];
    double hodnota;
    int ok = 0;

    while (ok != 1) {
        if (GetString(line, BUFF_SIZE) == false) return false;
        ok = sscanf(line,"%lf",&hodnota);
        if (ok != 1) printf("Chyba - je treba zadat cislo");
    }

    *val = hodnota;
    return true;
}

void ClearStdin() {
    fflush(stdin);
}

/* Definice funkci PRACE S DATY ---------------------------------------------------*/

int DataCmp(Data_t *d1, Data_t *d2) {
    return strcmp(d1->name, d2->name);
}

bool DataFullCmp(Data_t *d1, Data_t *d2) {
    if (d1 == NULL || d2 == NULL) return false;
    float epsilon = 0.001f;
    if (strcmp(d1->name, d2->name) != 0) return false;
    if (fabs(d1->age - d2->age) < epsilon) return true;
    return false;
}


bool DataGet(Data_t *data) {
    if (data == NULL) return false;

    printf("Zadej jmeno: ");
    if (GetString(data->name, 255) == false) return false;
    int poziceEOL = strcspn(data->name, "\n\r"); // ustrihuti textu za koncem radku na tento znak vlozim NUL
    data->name[poziceEOL] = 0;

    printf("Zadej vek: ");
    if (GetDouble(&data->age) == false) return false;

    return true;
}

bool DataPrint(Data_t *data) {
    if (data == NULL) return false;
    printf("Jmeno: %s, vek: %0.1lf\n", data->name, data->age);
    return true;
}

/* Definice funkci STRUKTURA BINARNIHO STROMU -------------------------------------*/

void Print_Node(TreeNode *node) {
    DataPrint(&node->data);
    printf(" | L -> ");
    if (node->left == NULL) printf("NULL\n");
    else DataPrint(&node->left->data);
    printf(" | R -> ");
    if (node->right == NULL) printf("NULL\n");
    else DataPrint(&node->right->data);
}

void Preorder(TreeNode *node) {
    if (node == NULL) return;
    Print_Node(node);
    Preorder(node->left);
    Preorder(node->right);
}

void Inorder(TreeNode *node) {
    if (node == NULL) return;
    Inorder(node->left);
    Print_Node(node);
    Inorder(node->right);
}

void Postorder(TreeNode *node) {
    if (node == NULL) return;
    Postorder(node->left);
    Postorder(node->right);
    Print_Node(node);
}


void Tree_Rec_Clear(TreeNode * node) {
    if (node == NULL) return;
    Tree_Rec_Clear(node->left);
    Tree_Rec_Clear(node->right);
    free(node);
}

/**
 * @brief Tree_Print Prints tree in console
 * @param node  Pointer at the next node
 * @param previous  Pointer at previous node that we were working with
 * @param spaces    The number of spaces for console
 */
void Tree_Print(TreeNode* node, TreeNode* previous, int spaces) {
    // kontrola, zda byly predany ukazatele
    if (node == NULL || previous == NULL) return;

    // rekurzivni vypis bin. stromu
    spaces += 6;
    // pokud je vpravo vetev, zde se vyresi a pak se vrati do aktualniho nodu
    Tree_Print(node->right, node, spaces);
    // aktualni nod
    printf("\n");
    for (int i = 6; i < spaces; i++) {
        printf(" ");
    }
    // jsem v prvnim nodu vetve
    if (previous == node) {
        printf("---%s", node->data.name);
    }
    // jsem v leve vetvi
    else if (node == previous->left) {
        printf("|\n");
        for (int i = 6; i < spaces; i++) {
            printf(" ");
        }
        printf("---%s", node->data.name);
    }
    // jsme v prave vetvi
    else {
        printf("---%s\n", node->data.name);
        for (int i = 6; i < spaces; i++) {
            printf(" ");
        }
        printf("|");
    }
    // pokud je vlevo vetev, zde se vyresi
    Tree_Print(node->left, node, spaces);
}

/*!
 * @brief Counts number of nodes in a tree
 * @param[in] root pointer at tree
 * @param Funkce, která vrací počet uzlů ve stromu
 * @return Returns the number of items in a tree
 */
unsigned Tree_Count_Nods(Tree tree) {
    return tree.nodCount;
}

/*!
 * @brief Initialize the tree, sets itemsCount of Tree to 0
 * @param[in] root Pointer at tree to initialize
 * @param Inicializuje první uzel stromu na NULL a nastaví počet uzlů na 0.
 * @return Returns true if tree was initialized
 */
bool Tree_Init(Tree *tree) {
    if (tree == NULL) return false;

    tree->root = NULL;
    tree->nodCount = 0;

    return true;
}

/*!
 * @brief Deletes every node from a tree *
 * @param[in] root Pointer at tree to clear
 * @param Vymaže všechny uzly stromu a počet uzlů nastaví na 0.
 */
void Tree_Clear(Tree *tree) {
    if (tree == NULL) return;

    Tree_Rec_Clear(tree->root);
    tree->root = NULL;
    tree->nodCount = 0;
}

/*!
 * @brief Creates a node with data and puts it in the tree at the right place.
 * Higher value goes to the left node, lower value to the right node. Use
 * Data_Cmp to compare the data.
 * @param[in] root Pointer at tree, where to store the node
 * @param[in] data Pointer at data
 * @param Vloží uzel do stromu dle pravidel, na porovnání uzlů použijte funkce Data_Cmp
 * @param Lze vlozit stejne data vicekrat, vlozi se doleva
 * @return Returns true if data were inserted correctly, returns false if
 * inserting node was unsuccessful or the tree already has a node with the same
 * values. Increases the itemsCount by 1.
 */
bool Tree_Insert(Tree *tree, Data_t data) {
    if (tree == NULL) return false;

    int cmp;
    TreeNode *ptrActive = tree->root;
    TreeNode *ptrPrev = NULL;

    while (ptrActive != NULL) {
        ptrPrev = ptrActive;
        cmp = DataCmp(&data, &ptrActive->data);
        if (cmp < 0) ptrActive = ptrActive->left;
        else if (cmp > 0) ptrActive = ptrActive->right;
        else ptrActive = ptrActive->left;
    }
    ptrActive = malloc(sizeof (TreeNode));
    if (ptrActive == NULL) return false;
    ptrActive->data = data;
    ptrActive->left = ptrActive->right = NULL;
    if (ptrPrev == NULL) tree->root = ptrActive;
    else {
        if (cmp <= 0) ptrPrev->left = ptrActive;
        else ptrPrev->right = ptrActive;
    }
    ++(tree->nodCount);
    return true;
}

/*!
 * @brief Deletes a specific node from a tree which has the same data as the
 * data from parameter. Deleting has to be done according to rules. If node has
 * more than 1 node connected to it, find node with the lowest value(right most
 * node from node that is being deleted).
 * @param[in] root Pointer at tree, where to delete the node
 * @param[in] data Pointer at the data which we want to remove from a tree
 * @param Smaže uzel ze stromu dle pravidel
 */
void Tree_Del(Tree *tree, Data_t data) {
    if (tree == NULL) return;

    bool cmpEqual = false;
    int cmp;
    TreeNode *ptrActive = tree->root;
    TreeNode **ptrPtr = NULL;

    while (ptrActive != NULL) {
        cmp = DataCmp(&data, &ptrActive->data);
        if (cmp < 0) {
            if (cmpEqual) return;
            ptrPtr = &ptrActive->left;
            ptrActive = ptrActive->left;
        }
        else if (cmp > 0) {
            if (cmpEqual) return;
            ptrPtr = &ptrActive->right;
            ptrActive = ptrActive->right;
        }
        else if (cmp == 0) {
            if (DataFullCmp(&data, &ptrActive->data)) {
                printf("MAZU data.\n");
                DataPrint(&ptrActive->data);
                // data se rovnaji chceme je smazat
                if (ptrActive->left == NULL && ptrActive->right == NULL) {  // zadne deti
                    free(ptrActive);
                    *ptrPtr = NULL;
                    --(tree->nodCount);
                    return;
                }
                else if (ptrActive->left != NULL && ptrActive->right != NULL) { // obe deti
                    TreeNode *rightNode = ptrActive->left;
                    TreeNode *prevNode = NULL;
                    while (rightNode->right != NULL) {
                        prevNode = rightNode;
                        rightNode = rightNode->right;
                    }
                    ptrActive->data = rightNode->data;
                    if (prevNode == NULL) ptrActive->left = rightNode->left;
                    else prevNode->right = rightNode->left;
                    free(rightNode);
                    --(tree->nodCount);
                    return;
                }
                else { // jedno dite
                    if (ptrActive->left != NULL) *ptrPtr = ptrActive->left;
                    else *ptrPtr = ptrActive->right;
                    free(ptrActive);
                    --(tree->nodCount);
                    return;
                }
            }
            else {
                ptrPtr = &ptrActive->left;
                ptrActive = ptrActive->left;
                cmpEqual = true;
            }
        }
    }
    printf("Data k mezani NENALEZENA.\n");
}

/*!
 * @brief Vrátí ukazatel na uzel, který drží daná data. Returns pointer at node,
 * which holds the data. Name, age, weight and height has to match, otherwise
 * return NULL
 * @param[in] root Pointer at tree, from which we want to find a node
 * @param[in] data Pointer at searching data
 * @param Vrátí ukazatel na uzel, který drží daná data.
 * @return Returns pointer at node with searched data, if theres no such a node,
 * return NULL
 */
//TreeNode* Tree_Find_Node(Tree tree, Data_t data) {
TreeNode* Tree_Find_Node(TreeNode * const ptrSearch, Data_t data) {
    if (ptrSearch == NULL) return NULL;
    int cmp;
    TreeNode *ptr = ptrSearch;
    while (ptr != NULL) {
        cmp = DataCmp(&data, &ptr->data);
        if (cmp < 0) ptr = ptr->left;
        else if (cmp > 0) ptr = ptr->right;
        else {
            if (DataFullCmp(&data, &ptr->data)) {
                //printf("Data nalezena\n");
                return ptr;
            }
            else {
                ptr = ptr->left;
            }
        }
    }
    return NULL;
}

unsigned Tree_Dupl_Count(Tree *tree, Data_t data) {
    if (tree == NULL) return 0;
    int cmp;
    unsigned count = 0;
    TreeNode *ptr = tree->root;

    while (ptr != NULL) {
        cmp = DataCmp(&data, &ptr->data);
        if (cmp < 0) ptr = ptr->left;
        else if (cmp > 0) ptr = ptr->right;
        else {
            while (cmp == 0 && ptr != NULL) {

                if (DataFullCmp(&data, &ptr->data)) {
                    ++count;
                    printf("Data nalezena %u krat.\n", count);
                    ptr = ptr->left;
                }
                else {
                    ptr = ptr->left;
                }
                if (ptr == NULL) break;
                else cmp = DataCmp(&data, &ptr->data);
            }
            break;
        }
    }

    return count;
}
