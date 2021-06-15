#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFF_SIZE 255

/* Definice struktur -------------------------------------*/

typedef struct {
    char name[BUFF_SIZE]; // jmeno osoby
    double age;     // vek
} Data_t;

typedef struct _TreeNode {
    Data_t data;
    struct _TreeNode *left;
    struct _TreeNode *right;
} TreeNode;

typedef struct _Tree {
    TreeNode *root;
    unsigned nodCount;
} Tree;

/* Deklarace funkci NACITANI Z KONZOLE ---------------------------------------------*/

bool GetString(char *line, int maxLen);
bool GetChar(char *c);
bool GetDouble(double *val);
void ClearStdin();

/* Deklarace funkci PRACE S DATY ---------------------------------------------------*/

int DataCmp(Data_t *d1, Data_t *d2);
bool DataFullCmp(Data_t *d1, Data_t *d2);
bool DataGet(Data_t *data);
bool DataPrint(Data_t *data);

/* Deklarace funkci STRUKTURA BINARNIHO STROMU -------------------------------------*/

void Tree_Rec_Clear(TreeNode * node); // done
unsigned Tree_Count_Nods(Tree tree); // done
bool Tree_Init(Tree *tree); // done
void Tree_Clear(Tree *tree); // done
bool Tree_Insert(Tree *tree, Data_t data); // done
void Tree_Del(Tree *tree, Data_t data);
// TreeNode* Tree_Find_Node(Tree tree, Data_t data);
TreeNode* Tree_Find_Node(TreeNode * const ptrSearch, Data_t data); // done
void Tree_Print(TreeNode *node, TreeNode *previous, int spaces); //done
unsigned Tree_Dupl_Count(Tree *tree, Data_t data); // done

void Print_Node(TreeNode *node);
void Preorder(TreeNode *node);
void Inorder(TreeNode *node);
void Postorder(TreeNode *node);

#endif // TREE_H
