/*
    A Linked-List with different types of data: str, int and float 

    WARNING: IN CASE OF ERROR IN DELETE FUNCTIONS I DONT HAVE IMPLEMENT ANYTHING
        error because of the source

*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define add_start(head, data)                            \
                        _Generic( (data),                \
                            int  : iadd_start_ll,        \
                            char*: sadd_start_ll,        \
                            double: fadd_start_ll) ( head, data )

#define add_after(p, data)                                  \
                        _Generic( (data),                   \
                            int   : iadd_after_ll,          \
                            char* : sadd_after_ll,          \
                            double: fadd_after_ll) ( p, data )

#define del_start(head, source)                             \
                        _Generic( (source),                 \
                            int*   : idel_start_ll,         \
                            char*  : sdel_start_ll,         \
                            double*: fdel_start_ll) ( head, source )

#define del_after(p, source)                              \
                        _Generic( (source),               \
                            int*   : idel_after_ll,       \
                            char*  : sdel_after_ll,       \
                            double*: fdel_after_ll) ( p, source )


struct Node
{
    enum{
        INT, STRING, DOUBLE
    }type;
    
    union{
        double real;
        char s[100];
        int i;
    }data;

    struct Node* next;
};

typedef struct Node llnode;
typedef struct Node* llptr;

//initialize 
void init_ll(llptr* p);

/*    TYPE: INT   */
//add in the beggining
bool iadd_start_ll(llptr* head, int in_data);
//add after a specific node
bool iadd_after_ll(llptr p, int in_data);
//delete start
bool idel_start_ll(llptr* head, int* out_data);
//delete after a specific node
bool idel_after_ll(llptr p, int* out_data);


/*    TYPE: STRING   */
//add in the beggining
bool sadd_start_ll(llptr* head, char* in_str);
//add after a specific node
bool sadd_after_ll(llptr p, char* in_str);
//delete start
bool sdel_start_ll(llptr* head, char* in_str);
//delete after a specific node
bool sdel_after_ll(llptr p, char* in_str);

/*    TYPE: FLOAT    */
//add in the beggining
bool fadd_start_ll(llptr* head, double in_data);
//add after a specific node
bool fadd_after_ll(llptr p, double in_data);
//delete start
bool fdel_start_ll(llptr* head, double* out_data);
//delete after a specific node
bool fdel_after_ll(llptr p, double* out_data);


//print Linked-List 
void print_LL(llptr head);


int main(void)
{
    printf("\n\t\tHello World Again !!!\n\n");

    llptr head, cur;
    int itemp;
    double ftemp;
    char s1[] = "Hello";
    char s2[] = "Chocolate and music";
    char s[200];
    init_ll(&head);

    add_start(&head, 10);
    add_start(&head, s1);
    add_start(&head, s2);
    add_start(&head, 3.14);
    add_start(&head, 12393);
    add_start(&head, 2.81);
    print_LL(head);
    // 2.81 -> 12393-> 3.14 -> "Chocolate and music" -> "Hello" -> 10

    cur = head;       // cur = 2.81
    cur = cur->next;  // cur = 12393
    add_after(cur, 333221);
    print_LL(head);

    // 2.81 -> 12393-> 333221 -> 3.14 -> "Chocolate and music" -> "Hello" -> 10
    add_after(cur, "Books and Rain");
    print_LL(head); 
    //  2.81 -> 12393-> "Books and Rain" -> 333221 -> 3.14 -> "Chocolate and music" -> "Hello" -> 10
    
    
    del_after(head, &itemp);
    print_LL(head);
    printf("\nitemp = %d\n", itemp);
    
    del_start(&head, &ftemp);
    print_LL(head);
    printf("\nftemp = %lf\n", ftemp);
    
    del_start(&head, &itemp);
    print_LL(head);


    return 0;
}


void init_ll(llptr* p){
    *p = NULL;
}


/*          TYPE: INT           */
bool iadd_start_ll(llptr* head, int in_data){
    
    llnode* newnode = (llptr)malloc(sizeof(llnode));
    if(!newnode){
        printf("Error at allocating memory!\n");
        return false;}

    newnode->data.i  = in_data;
    newnode->type = INT;
    newnode->next = *head;
    *head = newnode;
    return true;
}

bool iadd_after_ll(llptr p, int in_data){
    
    llnode* newnode = (llptr)malloc(sizeof(llnode));
    if(!newnode){
        printf("Error at allocating memory!\n");
        return false;}

    newnode->type = INT;
    newnode->data.i = in_data;
    newnode->next = p->next;
    p->next = newnode;
    return true;
}

bool idel_start_ll(llptr* head, int* out_data){
    
    llptr cur = *head;
    if((*head) == NULL)
        return false;


    // reject deletion if the node doesn't contain the correct datatype
    if(cur->type != INT) 
        return false;

    *out_data = cur->data.i;
    *head = cur->next;
    free(cur);
    return true;
}

bool idel_after_ll(llptr p, int* out_data){
    
    llptr current = p->next;
    if (current == NULL)
        return false;

    // reject deletion if the node doesn't contain the correct datatype
    if(current->type != INT) 
        return false;

    *out_data = current->data.i;   
    p->next = current->next;
    free(current);   
    return true;
}


/*          TYPE: STRING           */
bool sadd_start_ll(llptr* head, char* in_str){

    llptr newnode = (llptr)malloc(sizeof(llnode));
    if( !newnode ){
        printf("Error at allocating memory!\n");
        return false;}

    strcpy(newnode->data.s, in_str);
    newnode->type = STRING;
    newnode->next = *head;
    *head = newnode;
    
    return true;
}

//add after a specific node
bool sadd_after_ll(llptr p, char* in_str){
    
    llnode* newnode = (llptr)malloc(sizeof(llnode));
    if(!newnode){
        printf("Error at allocating memory!\n");
        return false;}

    strcpy(newnode->data.s, in_str);
    newnode->type = STRING;
    newnode->next = p->next;
    p->next = newnode;
    return true;    
}

//delete start
bool sdel_start_ll(llptr* head, char* in_str){

    llptr cur = *head;
    if(*head == NULL)
        return false;

    // reject deletion if the node doesn't contain the correct datatype
    if(cur->type != STRING) 
        return false;

    strcpy(in_str, cur->data.s);
    *head = cur->next;
    free(cur);
    return true;
}

//delete after
bool sdel_after_ll(llptr p, char* in_str){

    llptr cur = p->next;
    if (cur == NULL)
        return false;

    // reject deletion if the node doesn't contain the correct datatype
    if(cur->type != STRING) 
        return false;

    strcpy(in_str, cur->data.s);
    p->next = cur->next;
    free(cur);   
    return true;
}


/*      FLOAT       */
//add in the beggining
bool fadd_start_ll(llptr* head, double in_data){
    
    llnode* newnode = (llptr)malloc(sizeof(llnode));
    if(!newnode){
        printf("Error at allocating memory!\n");
        return false;}

    newnode->data.real  = in_data;
    newnode->type = DOUBLE;
    newnode->next = *head;
    *head = newnode;
    return true;
}

//add after a specific node
bool fadd_after_ll(llptr p, double in_data){

    llnode* newnode = (llptr)malloc(sizeof(llnode));
    if(!newnode){
        printf("Error at allocating memory!\n");
        return false;}

    newnode->type = DOUBLE;
    newnode->data.real = in_data;
    newnode->next = p->next;
    p->next = newnode;
    return true;
}

//delete start
bool fdel_start_ll(llptr* head, double* out_data){

    llptr cur = *head;
    if(*head == NULL)
        return false;

    // reject deletion if the node doesn't contain the correct datatype
    if(cur->type != DOUBLE) 
        return false;

    *out_data = cur->data.real;
    *head = cur->next;
    free(cur);
    return true;
}

//delete after
bool fdel_after_ll(llptr p, double* out_data){
    
    llptr current = p->next;
    if (current == NULL)
        return false;

    // reject deletion if the node doesn't contain the correct datatype
    if(current->type != DOUBLE) 
        return false;

    *out_data = current->data.real;   
    p->next = current->next;
    free(current);   
    return true;
}


/*     OTHERS    */
void print_LL(llptr head){
    
    int cnt = 1;
    printf("\n\tLINKED LIST\n");   
    while(head != NULL)
    {
        if( head->type == INT )
            printf("%d: %d ->\n", cnt, head->data.i);
        else if (head->type == STRING)
            printf("%d: %s ->\n", cnt, head->data.s);
        else
            printf("%d: %.2lf ->\n", cnt, head->data.real);
        cnt++;
        head = head->next;
    }
}