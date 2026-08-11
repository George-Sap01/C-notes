/*
    A -> 65 , Z -> 90   26 characters
    a -> 97 , z -> 122  26 characters

    space -> 32         9 characters -> 61 characters
    !     -> 33
    "     -> 34
    .     -> 46
    ?     -> 63
    ,     -> 44
    -     -> 45
    :     -> 58 
    ;     -> 59

    1. I am inserting each character in the hash table
    2. Reads `words.txt` character-by-character.
    3. Outputs individual character counts and execution time.


    i was curious to see if i could implement an Hash Table that uses a AVL Tree instead of a Linked Tree.


    ** This project is intended purely for educational and experimental purposes. **
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define TRUE 1
#define FALSE 0
#define table_size 31

struct CHARACTER{
    char c[2];      // character
    int cnt;        // times of appearances 
    struct CHARACTER *left;
    struct CHARACTER *right;
};
typedef struct CHARACTER character;


struct POINTERS {
    character *root;
};
typedef struct POINTERS ptrs;


ptrs hash_table[table_size]; 

int hash_func(char c); 

/*  ---- HASH TABLES FUNCTIONS ----  */
// initialize  table
void init_hash_table(ptrs *hash_table);
// add person
int add_hash_table(ptrs *hash_table, char c);
// print table
void print_table(ptrs *hash_table);
// update counter
int update_counter_character(ptrs *hash_table, char c);


// add in BINARY SEARCH TREE 
int BST_insert(character *oldnode, character *newnode);

/*  ---- TREE functions ----  */
// int TR_insert_left(character *oldnode, character *newnode);
// int TR_insert_right(character *oldnode, character *newnode); 
void TR_preorder(character *node);
void TR_print_node(character *node);


int main()
{
    clock_t start, end;
    double cpu_run_time;
    start = clock();

    FILE *fp;
    char filename[] = "try2.txt";
    char c;
    int array_lower[26], array_upper[26];
    int array_symbols[9] = {32, 33, 34, 44, 45, 46, 63, 58, 59};
    int i;

    /*   initialize the character arrays    */
    for(i=0; i<26; i++){    
        array_lower[i] = i + 97;
        array_upper[i] = i + 65;}

    /*   initialize hash_table   */
    init_hash_table(hash_table);

    /*   inserting the characters/symbols in the  HASH TABLE   */
    for(i=0; i<26; i++){    
        add_hash_table(hash_table, array_lower[i]);
        add_hash_table(hash_table, array_upper[i]);}

    for(i=0; i<9; i++)
        add_hash_table(hash_table, array_symbols[i]);


    //print_table(hash_table);

    /*  ---- open file ----  */   
    fp = fopen(filename, "r");
    while(1)
    {
        c = fgetc(fp);
        if(c == EOF)    
            break;
        update_counter_character(hash_table, c);
    }
    
    // print_table(hash_table);

    end = clock();
    cpu_run_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time: %f", cpu_run_time);
    return 0;
}


int hash_func(char c){
    int result = (c * 3) % table_size;
    return result;
}

// ptrs hash_table[table_size]
void init_hash_table(ptrs *hash_table){
    for(int i=0; i<table_size; i++)     
        (*(hash_table + i)).root = NULL; //hash_table[i].start = NULL
}

int add_hash_table(ptrs *hash_table, char c){
    
    character *newnode;
    newnode = (character*)malloc(sizeof(character));
    if(!newnode){
        printf("Error at allocating memory");
        exit(0);
    }

    newnode->c[0] = c;
    newnode->c[1] = '\0';
    newnode->cnt = 0;
    newnode->left = NULL; 
    newnode->right = NULL;

    int index = hash_func(c);
    
    if(hash_table[index].root != NULL)
        return BST_insert( hash_table[index].root , newnode);
    else // hash_table[index].root == NULL
    {    
        hash_table[index].root = newnode; // this tree is empty 
        return TRUE;
    }
}


void print_table(ptrs *hash_table){
    
    printf("START\n");
    for(int i=0; i<table_size; i++) 
    {    
        if(hash_table[i].root == NULL)
            printf("\t%i  ----", i);
        else
        {   
            printf("\t%i ", i);
            character *current = hash_table[i].root;
            TR_preorder(current); 
        }
        printf(" NULL\n");
    }
    printf("END\n");
}

int update_counter_character(ptrs *hash_table, char c)
{
    character *cur;
    int index = hash_func(c);
    cur = hash_table[index].root;
    
    if(hash_table[index].root == NULL)
        return FALSE;

    while(cur != NULL)
    {
        if(c == cur->c[0])
        {
            cur->cnt++;
            return TRUE;
        }
        else if(c >  cur->c[0])
            cur = cur->right;
        else
            cur = cur->left;
    }
    return FALSE;
}

/*
int TR_insert_left(character *oldnode, character *newnode)
{   
    if( oldnode->left != NULL)
        return FALSE;

    oldnode->left = newnode;
    return TRUE;
}

int TR_insert_right(character *oldnode, character *newnode)
{
    if( oldnode->right != NULL)
        return FALSE;

    oldnode->right = newnode;
    return TRUE;
}
*/

int BST_insert(character *oldnode, character *newnode)
{
    // root is not empty, oldnode is the struct that root is pointing to
    // now we want to insert accordingly, left or right  
    character *cur = oldnode;
    while(1)
    {
        if( newnode->c[0] > cur->c[0]) // right
        {
            if( cur->right == NULL)
            {
                cur->right = newnode;
                return TRUE;
            }
            else
                cur = cur->right;
        }
        else // left
        {   
            if( cur->left == NULL)
            {
                cur->left = newnode;
                return TRUE;
            }
            else
                cur = cur->left;
        }
    }
}

void TR_preorder(character *node)
{
    if( node!=NULL )
    {
        TR_print_node(node);
        TR_preorder(node->left);
        TR_preorder(node->right);
    }
}

void TR_print_node(character *node)
{
    printf("| c: '%s'  cnt: %d | -> ", node->c, node->cnt);
}
