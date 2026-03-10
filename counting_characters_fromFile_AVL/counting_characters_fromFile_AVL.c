/*
    A -> 65 , Z -> 90  
    a -> 97 , z -> 122

    space -> 32
    !     -> 33
    "     -> 34
    .     -> 46
    ?     -> 63
    ,     -> 44
    -     -> 45
    :     -> 58 
    ;     -> 59
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0


struct node{
    char character;
    int counter;
    struct node* right;
    struct node* left;
    int height;
};


typedef struct node TREE_NODE;
typedef struct node* TREE_PTR;

// initialiaze
void TR_init(TREE_PTR* root);
int TR_empty(TREE_PTR root);
//insert
int TR_insert_root(TREE_PTR *root, char x);

int max(int x, int y);
int height(TREE_PTR R);

TREE_PTR AVL_rotate_R(TREE_PTR C);
TREE_PTR AVL_rotate_L(TREE_PTR C);
TREE_PTR AVL_rotate_LR(TREE_PTR C);
TREE_PTR AVL_rotate_RL(TREE_PTR C);

TREE_PTR TR_insert_AVL(TREE_PTR *root, char x);
void TR_print_AVL(TREE_PTR root);

int TR_UPDATE(TREE_PTR *root, char x);
void sum(TREE_PTR root, int *a);


int main()
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    FILE *fp = NULL;
    char name_file[] = "try2.txt";
    char c;
    int array_lower[26], array_upper[26];
    int array_symbols[9] = {32, 33, 34, 44, 45, 46, 63, 58, 59};
    int i;

    TREE_PTR lower, upper, symbols;

    // αρχικοποιηση των ριζων 
    TR_init(&lower);
    TR_init(&upper);
    TR_init(&symbols);

    // αρχικοποιηση των πινακων 
    for(i=0; i<26; i++){    
        array_lower[i] = i + 97;
        array_upper[i] = i + 65;}

    // βαζω τους πινακες στα AVL TREES
    for(i=0; i<26; i++){    
        TR_insert_AVL(&lower, array_lower[i]);
        TR_insert_AVL(&upper, array_upper[i]);}

    for(i=0; i<9; i++)
        TR_insert_AVL(&symbols, array_symbols[i]);

    // ανοιγω το αρχειο 
    fp = fopen(name_file, "r");
    
    while(1)
    {
        c = fgetc(fp);
        if(c == EOF)    
            break;

        if(c >= 65 && c<= 90) //κεφαλαια
            TR_UPDATE(&upper, c);
        else if (c>=97 && c<=122) //mikra
            TR_UPDATE(&lower, c);
        else if(c==32 || c==33 || c==44 || c==46 || c==63 || c==34 || c == 45 || c==58 || c==59)
            TR_UPDATE(&symbols, c);
        else
            continue;
    }

    printf("\n\n");
    TR_print_AVL(upper);
    printf("\n\n");
    TR_print_AVL(lower);
    printf("\n\n");
    TR_print_AVL(symbols);
    printf("\n\n");
    
    int a = 0;
    sum(symbols, &a);
    sum(upper, &a);
    sum(lower, &a);
    printf("\nTotal characters -> %d", a);

    end  = clock();
    cpu_time_used = ((double)(end-start)) / CLOCKS_PER_SEC;
    printf("\ntime: %lf", cpu_time_used);
    fclose(fp);
    return 0;
}


void TR_init(TREE_PTR* root){
    *root = NULL;
}


int TR_empty(TREE_PTR root){
    return root == NULL;
}


int TR_insert_root(TREE_PTR *root, char x){
    TREE_PTR newnode;

    if( *root != NULL)
        return FALSE;

    newnode = (TREE_PTR)malloc(sizeof(TREE_NODE));
    if(!newnode)
    {
        printf("adynamia desmeusis mnimis");
        return FALSE;
    }

    newnode->character = x;
    newnode->counter = 0;
    newnode->left = NULL;
    newnode->right = NULL;
    
    *root = newnode;
    return TRUE;
}


int max(int x, int y){
	return (x>y) ? x : y;
}


int height(TREE_PTR R){
    return (R==NULL) ? 0 : R->height;
}


TREE_PTR AVL_rotate_R(TREE_PTR C){
	TREE_PTR L = C->left; 
	C->left = L->right;  
	L->right = C; 
	C->height = max(height(C->left), height(C->right)) + 1;
	L->height = max(height(L->left), height(L->right)) + 1;
	return L; 
}


TREE_PTR AVL_rotate_L(TREE_PTR C){
   TREE_PTR R = C->right; 
   C->right = R->left; 
   R->left = C; 
   C->height = max(height(C->left), height(C->right)) + 1;
   R->height = max(height(R->left), height(R->right)) + 1;
   return R; 
}


TREE_PTR AVL_rotate_LR(TREE_PTR C){
   C->left = AVL_rotate_L(C->left);

   return AVL_rotate_R(C); 
}


TREE_PTR AVL_rotate_RL(TREE_PTR C){
   C->right = AVL_rotate_R(C->right);
 
   return AVL_rotate_L(C); 
}


TREE_PTR TR_insert_AVL(TREE_PTR *root, char x)
{
	if (*root==NULL)
	{
		TR_insert_root(root, x);
		(*root)->height = 1; 
		return *root; 
	}
	else if (x < (*root)->character)
	{
		(*root)->left = TR_insert_AVL(&((*root)->left), x);
		if (height((*root)->left)-height((*root)->right) == 2)
		{
			if(x < (*root)->left->character)
				(*root) = AVL_rotate_R(*root);
			else
				(*root) = AVL_rotate_LR(*root);
		}
	}
	else if (x > (*root)->character)
	{
		(*root)->right = TR_insert_AVL(&((*root)->right), x);
		if (height((*root)->left)-height((*root)->right) == -2)
		{
			if(x > (*root)->right->character)
				(*root) = AVL_rotate_L(*root);
			else
				(*root) = AVL_rotate_RL(*root);
		}
	}
	// else x == root->x : den kanoyme tipota
	
	(*root)->height = max(height((*root)->left), height((*root)->right)) + 1;
	return (*root); 
}


void TR_print_AVL(TREE_PTR root){
	
    if (root->left!=NULL)
		TR_print_AVL(root->left);

	printf("%c => %d\n", root->character, root->counter);
	
    if (root->right!=NULL)
		TR_print_AVL(root->right);
}

int TR_UPDATE(TREE_PTR *root, char x){
    
    TREE_PTR current = *root;

    while(current != NULL)
    {
        if(current->character == x){
            current->counter += 1;
            return TRUE;
        }
        else if( current->character > x )
            current = current->left;
        else
        current = current->right;
    }

    if(current == NULL)
        return FALSE;
}

void sum(TREE_PTR root, int *a)
{
    
    TREE_PTR current = root;

    if(current != NULL)
    {
        *a = *a + current->counter;
        sum(current->left, a);
        sum(current->right, a);
    }
}