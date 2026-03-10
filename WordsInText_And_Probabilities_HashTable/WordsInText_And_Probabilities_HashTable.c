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
    
    int *array; // περιεχει τους αριθμους των προτασεων που βρισκεται.
    int elem_array; // περιεχει το συνολικο πληθος των προτασεων

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
int insert_hash_table(ptrs *hash_table, char *s, int sentence);
// print table
void print_table(ptrs *hash_table);
// search person in hash_table
int search_hash_table(ptrs *hash_table, char *s, word** ptr);


// My functions
void fileCopy(char *old_file, char *new_file, int *number_of_sentences);
int word_update(word* ptr, int sentence);
void HashTable_words(ptrs *hash_table, FILE *fp_new, int total_sentenc);
int sum_words_total(ptrs *hash_table);
float probability_of_a_in_sentence(ptrs *hash_table, char *string, int total_sentences);
float depended_prob_of_a_given_b(ptrs *hash_table, char *str1, char *str2, int total_sentences);    // str1 = a, str2 = b

int main()
{
    clock_t start, end;
    double cpu_time;

    start = clock();

    char filename1[] = "test.txt";
    char filename2[] = "random_lines.txt";
    char newfile[] = "newLines.txt";
    FILE *fp_new;
    int total_sentences = 0, sum_words = 0;
    
    
    // STEP-1
    fileCopy(filename2, newfile, &total_sentences);
    init_hash_table(hash_table);
    

    // STEP-2
    fp_new = fopen(newfile, "r"); //open new file to extract words
    HashTable_words(hash_table, fp_new, total_sentences);
    fclose(fp_new);
    
    //print_table(hash_table);

    char str1[] = "in";
    char str2[] = "it";
    sum_words = sum_words_total(hash_table);
    printf("\nsum = %d", sum_words);
    printf("\nprob = %f", probability_of_a_in_sentence(hash_table, str1, total_sentences));
    printf("\nprob = %f", depended_prob_of_a_given_b(hash_table, str1, str2, total_sentences));
    

    /* search
    char *s = "of";
    word *w;
    search_hash_table(hash_table, s, &w);
    printf("\n%s %d", w->word, w->cnt);  */
    
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
        return (s[0] + s[1] + s[length - 2] + s[length - 1]) % table_size;
    else
        return ((3 * s[0] + 5) % table_size); 
}

// insert θα κανεις μια λεξη οταν δεν υπαρχει στο table, αρα θα μπαινει πρωτη φορα
int insert_hash_table(ptrs *hash_table, char *s, int sentence){
    
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
    newnode->elem_array = 1;

    newnode->array = (int*)malloc(sizeof(int));
    if(newnode->array == NULL)
    {
        printf("\nError at allocating memory at array/ insert_hash_table\n");
        return FALSE;
    }
    newnode->array[0] = sentence;
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
void fileCopy(char *old_file, char *new_file, int *number_of_sentences)
{
    FILE *fp_new, *fp_old;

    fp_old = fopen(old_file, "r");
    fp_new = fopen(new_file, "w");
    char buffer[1000];
    char c;
    int counter = 0, flag = 0;

    // παιρνω την μορφοποιηση που θελω απο το κειμενο.
    /*
        length προταση1
        προταση1
        length προταση2
        προταση2
        ...
    */
    while((c = fgetc(fp_old)) != EOF)
    {
        if(c == '.')
        {
            buffer[counter] = c;
            counter++;
            buffer[counter] = '\0';
            
            fprintf(fp_new, "%d\n%s\n", counter, buffer);
            (*number_of_sentences)++;

            counter = 0;
            flag = 1;
        }
        else if(flag == 1 && (c == ' ' || c == '\n'))
        {   /*flag == 1 && c == ' ' αν υπαρχει κενο μετα την τελεία
              για να μην περασει στην επομενη προταση*/  
            flag = 0;
            continue;
        }
        else if( flag == 0 && c == '\n')
        {
            buffer[counter] = ' ';
            counter++;
            flag = 0;
        }
        else
        {   
            flag = 0;
            buffer[counter] = c + 32 * ( c>=65 && c<=90);
            counter++;
        }
    }
    fclose(fp_new);
    fclose(fp_old);
}

void HashTable_words(ptrs *hash_table, FILE *fp_new, int total_sentenc){

    int k =1; // μετρητης για να μην ξεπερασω τον συνολικο αριθμο προτασεων του αρχειου  
    int no_sentence = 0; // μετρητης που δειχνει σε ποια προταση ειμαι
    int length, search;
    char garbage[3], buff[1000];
    word* w; // pointer to help with the update
    
    while(k <= total_sentenc)
    {

        fscanf(fp_new, "%d\n", &length); // παιρνω το length της προτασης, και απορροφω το χαρακτηρα αλλαγης γραμμης \n
        fgets(buff, length, fp_new); // γραφω την προταση στο buff, δεν πιανει την . στο τελος της προτασης, αρα μετα πρεπει να απορροφησω την τελεια\n
        fgets(garbage, 3, fp_new); // απορροφαει το .\n ('.' + χαρακτηρας αλλαγης γραμμης)
        no_sentence++;

        // χωριζω τις λεξεις της προτασης.
        char *temp_word = strtok(buff, " ,.");
        /*  
            κανεις search:
            1) αν ειναι FALSE, τοτε κανεις TR_insert_AVL
            2) αν ειναι TRUE, τοτε κανεις update γιατι ηδη υπαρχει η λεξη στο Hash Table.
        */
        while(temp_word != NULL){
            /*  ψαχνω να δω αν ηδη υπαρχει, αν ναι επιστρεφει και την διευθυνση θεσης
                για να κανω κατευθειαν το update  */
            search = search_hash_table(hash_table, temp_word, &w);
          
            // τις τοποθετω στο Hash Table
            if(search == TRUE)
                word_update(w, no_sentence);
            else
                insert_hash_table(hash_table, temp_word, no_sentence);

            temp_word = strtok(NULL, " ,.");
        }
        
        k++;
    }
}


int word_update(word* ptr, int sentence){
    
    int flag = 0;
    ptr->cnt++;
    int i;

    for(i=0; i < ptr->elem_array; i++)
    {
        if(sentence == ptr->array[i]){   
            flag = 1;
            break;
        }
    }
    
    if(flag == 0){

        ptr->array = (int*)realloc(ptr->array, sizeof(int) * (ptr->elem_array + 1));
        if(!ptr->array){
            printf("\nERROR AT ALLOCATING MEMORY! / word_update\n");
            return FALSE;}
    
        ptr->array[ptr->elem_array] = sentence;
        ptr->elem_array++;
    }
    return TRUE;
}

int sum_words_total(ptrs *hash_table)
{
    word *cur;
    int i, result =0;

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

float probability_of_a_in_sentence(ptrs *hash_table, char *string,  int total_sentences){

    word* w;
    int search = search_hash_table(hash_table, string, &w);

    if(search == TRUE)
        return w->elem_array/(float)total_sentences;
    else 
        return 0.0;
}

float depended_prob_of_a_given_b(ptrs *hash_table,char *str1, char *str2, int total_sentences){ // str1 = a, str2 = b
    word *word1, *word2;
    int search1, search2;
    
    search1 = search_hash_table(hash_table, str1, &word1); 
    search2 = search_hash_table(hash_table, str2, &word2);

    int i;
    int start, finish, middle;
    int total = 0; // ποσες φορες εμφανιζονται στην ιδια προταση

    if(search1 == TRUE && search2 == TRUE) // υπαρχουν και οι δυο λεξεις
    {
        for( i=0; i < word1->elem_array; i++ ) // με το i θα διαπερναμε τις προτασεις που υπαρχει η λεξη 1
        {
            start = 0;
            finish = word2->elem_array - 1;
            
            while(start <= finish)
            {
                middle = (finish + start)/2;
                if( word1->array[i] == word2->array[middle] ) 
                {
                    total++;
                    finish = -1;
                }
                else if(word1->array[i] > word2->array[middle])
                    start = middle + 1;
                else 
                    finish = middle -1;
            }
        }
        return total/(float)word2->elem_array;
    }
    else
        return 0.0;
}

/* --------- END: FILE --------- */