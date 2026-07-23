/*
a double linked list with two pointers: start and end, where start points to the first element of the list and end points to the last

            struct pointers
        ____________________              ----------------------             ---------------------                
        |                   |             |                    |             |                    |
root =  | start      end    |   start ->  | data, next, prev   |    -> ...   | data, next, prev   |  <- end
        |                   |             |                    |    <-       |                    |
        ---------------------             ---------------------              ---------------------
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
typedef struct node *box_ptr;

// root of the double linked list 
struct pointers{
    box_ptr start;
    box_ptr end;    
};

typedef struct pointers ptrs;

//initialize 
void list_initial(ptrs *root);
// return data
elem list_data(box_ptr p);
// insert element 
int list_insert_start(ptrs *root, elem x);
int list_insert_after(ptrs *root, box_ptr p, elem x);
// delete element 
int list_delete_start(ptrs *root, elem *x);
int list_delete_after(ptrs *root, box_ptr p, elem *x);
// print the list 
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
    // root->end = NULL; different way, same meaning
}

elem list_data(box_ptr p){
    return p->data;
}

int list_insert_start(ptrs *root, elem x)
{
    box_ptr newnode;

    newnode  = (box_ptr)malloc(sizeof(Node));
    if(!newnode){
        printf("Error at allocating memory");
        return FALSE;
    }

    newnode->data = x;
    newnode->prev = NULL; //its inserted in the beggining, so the prev pointer is not gonna point to anything 
    
    if( (*root).start == NULL ) // list is empty
        (*root).end = newnode;
    else
        ((*root).start)->prev = newnode;

    // if the list is not empty, then the root.end pointer doesn't need to change its value because we don't insert something in the end 
    newnode->next = (*root).start;
    (*root).start = newnode;
    return TRUE; 
}

int list_insert_after(ptrs *root, box_ptr p, elem x)
{
    // we add a node after the node that is pointed by p
    box_ptr newnode;

    newnode  = (box_ptr)malloc(sizeof(Node));
    if(!newnode){
        printf("Error at allocating memory");
        return FALSE;
    }
    newnode->data = x;
    newnode->prev = p;
    newnode->next = p->next;
    // in case we are adding in the end, we change the value of root.end
    (p->next == NULL) ? ((*root).end = newnode) : ( (p->next)->prev = newnode );

    // if (p->next == NULL)
    //     root->end = newnode;
    // else
    //     (p->next)->prev = newnode;

    p->next = newnode;
    return TRUE;
}

int list_delete_start(ptrs *root, elem *x)
{
    if((*root).start == NULL) // list is empty
        return FALSE;

    box_ptr temp = (*root).start; // temp points to the first element
    *x = temp->data;

    // in the case the list has only one element
    (temp->next == NULL) ? ((*root).end = NULL) : ((temp->next)->prev = NULL);
    (*root).start = temp->next;
    /*
    if(root.end == root.start){
        root->start = NULL;
        root->end = NULL;
    }
    else{
        root.start = temp->next;
        temp->next->prev = NULL;
    }
    */
    free(temp);
    return TRUE;

}

int list_delete_after(ptrs *root, box_ptr p, elem *x){

    if(p->next == NULL) // there is not a node to delete 
        return FALSE;

    box_ptr temp = p->next;
    *x = temp->data;
    
    // if the condition is true, means i am deleting the last node
    (temp->next == NULL) ? ((*root).end = p) : (temp->next->prev = p);
    p->next = temp->next;
    /*
    if(temp->next == NULL){
        root->end = p;
        p->next = NULL;
    }
    else{
        p->next = temp->next;
        temp->next->prev = p;

    }*/
    free(temp);
    return TRUE;
}

void list_print(ptrs *root)
{
    box_ptr current = (*root).start;

    while(current != NULL){
        printf("%d ", current->data);
        current = current->next;
    }
}