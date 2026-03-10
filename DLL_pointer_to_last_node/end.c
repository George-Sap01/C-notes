/*

ειναι μια λιστα διπλα συνδεδεμενη οπου εχει δυο pointers: start, end οπου ο start δειχνει στο πρωτο στοιχειο της λιστα και ο end αντιστοιχα στο τελευταιο

            struct pointers
        ____________________                                        
        |                   |                              ----------------------          ---------------------
root =  | start      end    |  και μετα κανονικα start ->  | data, next, prev   | -> ...   | data, next, prev   |  <- end
        |                   |                               --------------------- <-        --------------------
        --------------------- 
*/


#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef int elem;

struct node{
    elem data;
    struct node *next;
    struct node *prev;
};

typedef struct node Node;
typedef struct node* box_ptr;

struct pointers{
    box_ptr start;
    box_ptr end;    
};

typedef struct pointers ptrs;

//initial
void list_initial(ptrs *root);
// return data
elem list_data(box_ptr p);
// insert
int list_insert_start(ptrs *root, elem x);
int list_insert_after(ptrs *root, box_ptr p, elem x);
// delete
int list_delete_start(ptrs *root, elem *x);
int list_delete_after(ptrs *root, box_ptr p, elem *x);
// print
void list_print(ptrs *root);

int main()
{
    box_ptr current;
    ptrs root;
    int x;
    list_initial(&root);
    

    list_insert_start(&root, 2);
    list_insert_start(&root, 3);
    list_insert_start(&root, 1);
    list_insert_start(&root, 10);
    list_print(&root);
    
    current = root.start;
    while(current->data != 3)
        current = current->next;

    list_insert_after(&root, current, 5);
    printf("\n");
    list_print(&root);
    printf("\n%d", root.end->data);
    
    list_insert_after(&root, root.end, 100);

    printf("\n\n");
    list_print(&root);
    printf("\n%d", root.end->data);

    list_delete_start(&root, &x);
    printf("\nx = %d\n", x);
    list_print(&root);

    current = root.start;
    while(current->data != 5)
        current = current->next;
    list_delete_after(&root, current, &x);
    printf("\n\nx = %d\n", x);
    list_print(&root);
    printf("\n%d", root.end->data);



    return 0;
}


void list_initial(ptrs *root){
    (*root).start = NULL;
    (*root).end = NULL;
}

elem list_data(box_ptr p){
    return p->data;
}

int list_insert_start(ptrs *root, elem x)
{
    box_ptr newnode;

    newnode  = (box_ptr)malloc(sizeof(Node));
    if(!newnode){
        printf("Adynamia desmeusis mnimis");
        return FALSE;
    }

    newnode->data = x;
    newnode->prev = NULL; //afou to bazeis stin arxi, den tha deixnei kapou pros ta piso o kainourgios komvos
    
    if( (*root).start == NULL ) // αν η λιστα ειναι αδεια 
        (*root).end = newnode;
    else
        ((*root).start)->prev = newnode;

    // αν δεν ειναι αδεια η λιστα τοτε η root.end δειχνει ηδη καπου και δεν πρεπει να μετακινηθει γιατι δεν προσθετουμε κατι στο τελος
    newnode->next = (*root).start;
    (*root).start = newnode;
    return TRUE; 
}

int list_insert_after(ptrs *root, box_ptr p, elem x)
{
    box_ptr newnode;

    newnode  = (box_ptr)malloc(sizeof(Node));
    if(!newnode){
        printf("Adynamia desmeusis mnimis");
        return FALSE;
    }
    newnode->data = x;
    newnode->prev = p;
    newnode->next = p->next;
    // αν ισχυει η συνθηκη, τοτε προσθετουμε στο τελος αρα αλλαζουμε τον δεικτη root.end
    (p->next == NULL) ? ((*root).end = newnode) : ((p->next)->prev = newnode);
    p->next = newnode;
    return TRUE;
}

int list_delete_start(ptrs *root, elem *x)
{
    if((*root).start == NULL) // einai adeia h lista
        return FALSE;

    box_ptr temp = (*root).start;
    *x = temp->data;

    // an i sinthiki isxiei, tote h lista exei mono ena stoixeio
    (temp->next == NULL) ? ((*root).end = NULL) : (temp->next->prev = NULL);
    (*root).start = temp->next;
    free(temp);
    return TRUE;

    /*
    if(root.end == root.start)
    {
        root.start = NULL;
        root.end = NULL;
    }
    else // afoy den peirazo ton teleutaio komvo den asxoloumai me to end
    {
        root.start = temp->next;
        temp->next->prev = NULL;
    }
    */

}

int list_delete_after(ptrs *root, box_ptr p, elem *x){

    if(p->next == NULL) // den yparxei komvos na diagrapso
        return FALSE;

    box_ptr temp = p->next;
    *x = temp->data;
    
    // an i sinthiki isxiei, diagrafo to teleutaio komvo
    (temp->next == NULL) ? ((*root).end = p) : (temp->next->prev = p);
    p->next = temp->next;
    free(temp);
    return TRUE;

    /*
    if(temp->next == NULL)
    {
        root.end = p;
        p->next = NULL;
    }
    else
    {
        p->next = temp->next;
        temp->next->prev = p;

    }*/

}

void list_print(ptrs *root)
{
    box_ptr current = (*root).start;

    while(current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }
}