#include <stdio.h>
#include <stdlib.h>

#include "tree.h"



typedef enum _Menu {
    init = 1,
    clear = 2,
    insert = 3,
    delete = 4,
    findNode = 5,
    nodeCount = 6,
    treePreorder = 7,
    treeInorder = 8,
    treePostorder = 9,
    menu = 10,
    treePrint = 11,
    end = 12,
} Menu;

void printMenu() {
    printf("1 - Inicializace stromu\n"
           "2 - Smazani stromu\n"
           "3 - Vloz prvek\n"
           "4 - Smazani prvku stromu\n"
           "5 - Najdi prvek\n"
           "6 - Spocti prvky\n"
           "7 - Projdi strom preorder\n"
           "8 - Projdi strom inorder\n"
           "9 - Projdi strom postorder\n"
           "0 - Najdi prvky s duplicitnimi daty\n"
           "M - Menu\n"
           "P - Vytiskni strom\n"
           "E - End\n"
           );




}
/* MAIN ------------------------------------------------- */
int main(int argc, char** argv) {
(void) argc;
(void) argv;


    bool run = true;
    bool init = false;
    char zvol = 0x00;
    Tree tree;
    TreeNode *ptrSearch = NULL;
    //TreeNode *ptrSearch = tree.root;
    printMenu();


    while (run) {
        if (init == true) printf("\nStrom obsahuje %u nodu\n", Tree_Count_Nods(tree));
        printf("Zadej svou volbu\n");
        if (GetChar(&zvol) == false) continue;

        switch (zvol) {
        case '0':
            if (init) {
                printf("Zadani hledanych dat.\n");
                Data_t data;
                if (DataGet(&data) == false) {
                    printf("Chyba zadani dat.\n");
                    continue;
                }
                unsigned countDupl = Tree_Dupl_Count(&tree, data);
                if (countDupl == 0) printf("Data NEBYLA nalezena.\n");
                else printf("Data BYLA nalezena %u krat.\n", countDupl);
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case '1':
            if (init) printf("Strom byl uz inicializovan.\n");
            else {
                init = Tree_Init(&tree);
                if (init) printf("Inicializace se zdarila.\n");
                else printf("Inicializace selhala.\n");
            }
            break;
        case '2':
            if (init) {
                Tree_Clear(&tree);
                printf("Prvky stromu byly smazany.\n");
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case '3':
            if (init) {
                printf("Zadani dat jednoho prvku.\n");
                Data_t data;
                if (DataGet(&data) == false) {
                    printf("Chyba zadani dat.\n");
                    continue;
                }
                if (Tree_Insert(&tree, data) == false) {
                    // printf("Vlozeni dat do stromu selhalo. Mozna redunance dat.\n");
                    printf("Vlozeni dat do stromu selhalo.\n");
                    continue;
                }
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case '4':
            if (init) {
                printf("Zadani dat ke smazani.\n");
                Data_t data;
                if (DataGet(&data) == false) {
                    printf("Chyba zadani dat.\n");
                    continue;
                }
                Tree_Del(&tree, data);
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case '5':
            if (init) {
                printf("Zadani hledanych dat.\n");
                Data_t data;
                if (DataGet(&data) == false) {
                    printf("Chyba zadani dat.\n");
                    continue;
                }
                TreeNode * ptrNode;
                if (ptrSearch != NULL) {
                    printf("Chces hledat od posledniho nalezu? a = ANO. Jinak se hleda od zacatku\n");
                    if (GetChar(&zvol) == false) continue;
                    if (zvol != 'a' && zvol != 'A')
                    {
                        ptrSearch = tree.root;
                        printf("Hledam od zacatku stromu.\n");
                        ptrNode = Tree_Find_Node(ptrSearch, data);
                    }
                    else {
                        printf("Hledam od posledniho nalezu dale.\n");
                        ptrNode = Tree_Find_Node(ptrSearch->left, data);
                    }
                }
                else {
                    ptrSearch = tree.root;
                    printf("Hledam od zacatku stromu.\n");
                    ptrNode = Tree_Find_Node(ptrSearch, data);
                }
                if (ptrNode == NULL) {
                    printf("Data NEBYLA nalezena.\n");
                    ptrSearch = NULL;
                }
                else {
                    printf("Data BYLA nalezena.\n");
                    ptrSearch = ptrNode;
                }
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case '6':
            if (init) {
                printf("Pocet prvku ve strome je: %u\n", Tree_Count_Nods(tree));
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case '7':
            if (init) {
                printf("Prochazeni stromu PREorder.\n");
                Preorder(tree.root);
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case '8':
            if (init) {
                printf("Prochazeni stromu INorder.\n");
                Inorder(tree.root);
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case '9':
            if (init) {
                printf("Prochazeni stromu POSTorder.\n");
                Postorder(tree.root);
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case 'p':
        case 'P':
            if (init) {
                printf("\n");
                Tree_Print(tree.root, tree.root, 0);
            }
            else printf("Je trebna nejdrive inicializovat strom.\n");
            break;
        case 'm':
        case 'M':
            printMenu();
            break;
        case 'e':
        case 'E':
            run = false;
            printf("K O N E C.\n");
            break;
        default:
            printf("Neznama volba. Vyber moznost:\n\n");
            printMenu();
            break;
        }
    }

    if (init) Tree_Clear(&tree);
    return 0;
}
