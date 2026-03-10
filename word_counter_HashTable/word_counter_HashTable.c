#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define table_size 7 // 11491

struct NODE{
    int cnt;  //πληθος συνολικων εμφανισεων σε ολο το κειμενο.
    char word[60];  // συμβολοσειρα με μαξ 59 χαρακτηρες
    
    struct NODE* next;
};

typedef struct NODE word;

struct POINTERS{
    word *start;
    word *end;
};
typedef struct POINTERS ptrs;

ptrs hash_table[table_size];


int len(char *s);
int hash_func(char *s);

/* --- HASH TABLE --- */
// initialize  table
void init_hash_table(ptrs *hash_table);
// add person
int insert_hash_table(ptrs *hash_table, char *s);
// print table
void print_table(ptrs *hash_table);
// search person in hash_table
int search_hash_table(ptrs *hash_table, char *s, word** ptr);


// My functions
void HashTable_words(ptrs *hash_table, char *filename);
int sum_words_total(ptrs *hash_table);

int main()
{
    clock_t start, end;
    double cpu_time;

    start = clock();

    char filename1[] = "test.txt";
    char filename2[] = "random_lines.txt";
    char filename3[] = "book.txt";
    
    int sum_words;
    
    
    // STEP-1
    init_hash_table(hash_table);

    // STEP-2
    HashTable_words(hash_table, filename1);
    print_table(hash_table);

    sum_words = sum_words_total(hash_table);
    printf("\n\tTotal words are: %d\n", sum_words);
    end = clock();
    cpu_time = (double)(end - start)/CLOCKS_PER_SEC;
    printf("\nTime:  %f", cpu_time);

    return 0;
}


/* --------- START: HASH TABLE --------- */
void init_hash_table(ptrs *hash_table){
    for(int i=0; i<table_size; i++) 
    {    
        (*(hash_table + i)).start = NULL; //hash_table[i].start = NULL
        (*(hash_table + i)).end = NULL;   //hash_table[i].end = NULL
    }
}


int len(char *s){
    int i = 0;
    while( s[i] != '\0' ) 
        i++;
    return i;
}

int hash_func(char *s){
    
    int length = len(s);
    if(length > 1)
        return (s[0] + 2*s[1] + 5*s[length - 2] + s[length - 1]) % table_size;
    else
        return ((3 * s[0] + 5) % table_size); 
}

// insert θα κανεις μια λεξη οταν δεν υπαρχει στο table, αρα θα μπαινει πρωτη φορα
int insert_hash_table(ptrs *hash_table, char *s){
    
    word *newnode;
    newnode = (word*)malloc(sizeof(word));
    if(!newnode){
        printf("Error at allocating memory");
        exit(0);
    }

    /*  start: initializing newnode  */
    strcpy(newnode->word, s);
    newnode->cnt = 1;
    newnode->next = NULL; //mpainei sto telos
    /*  end: initializing newnode  */
   
    int index = hash_func(s);
    
    if(hash_table[index].start != NULL) 
    {   // το ptr start της δομης εκεινης της θεσης του πινακα δειχνει
        // ηδη καπου το προσθετω στην λιστα
        hash_table[index].end->next = newnode;
        hash_table[index].end = newnode;
        return TRUE;
    }
    else //δεν δειχνει καπου αρα το βαζω στην αρχη
    {    
        hash_table[index].start = newnode;
        hash_table[index].end = newnode;
        return TRUE;
    }
}

void print_table(ptrs *hash_table){
    
    printf("START\n");
    for(int i=0; i<table_size; i++) 
    {    
        if(hash_table[i].start == NULL)
            printf("\t%i  ----", i);
        else
        {   
            printf("\t%i ", i);
            word *current = hash_table[i].start;
            while(current != NULL)
            {
                printf("|WORD: %s, appearances: %d| -> ", current->word, current->cnt);
                current = current->next;
                printf("\n          ");
            }
            printf("NULL"); 
        }
        printf("\n\n");
    }
    printf("END\n");
}


int search_hash_table(ptrs *hash_table, char *s, word** ptr)
{
    int index = hash_func(s);
    if(hash_table[index].start == NULL)
        return FALSE;

    word *cur = hash_table[index].start;

    while(cur != NULL)
    {
        if(s[0] == cur->word[0] && strcmp(s, cur->word) == 0)
        {   
            *ptr = cur;
            return TRUE;
        }
        cur = cur->next;
    }
    *ptr = NULL;
    return FALSE;
}
/* --------- END: HASH TABLE --------- */


/* --------- START: FILE --------- */
void HashTable_words(ptrs *hash_table, char *filename){

    FILE *fp = fopen(filename, "r");
    char c; 
    char buffer[1000];
    int counter, search, flag;
    word* w; // pointer to help with the update
    
    while((c = fgetc(fp)) != EOF)
    {   
        //i play football.
        // i Play football
        flag = 0;
        counter = 0;
        while( (c>=65 && c<= 90) || (c>=97 && c<=122) )
        {
            flag = 1;
            buffer[counter] = c + 32 * (c>=65 && c<= 90);
            counter++;
            c = fgetc(fp);

            // αν δεν ειναι γραμμα ή αν αν τελειωσε το αρχειο 
            if( !((c>=65 && c<= 90) || (c>=97 && c<=122)) || c==EOF)
            {    
                buffer[counter] = '\0';
                break;
            }
        }

        if(flag == 0)
            continue;
        else
        {
            search = search_hash_table(hash_table, buffer, &w);
            if(search == TRUE)
                w->cnt++;
            else // τοποθετω την λεξη στο Hash Table
                insert_hash_table(hash_table, buffer);
        }
    }
}
/* --------- END: FILE --------- */

int sum_words_total(ptrs *hash_table)
{
    word *cur;
    int i, result = 0;

    for(i=0; i<table_size; i++)
    {
        cur = hash_table[i].start;
        while(cur != NULL){
            result += cur->cnt;
            cur =  cur->next;
        }
    }
    return result;
}
