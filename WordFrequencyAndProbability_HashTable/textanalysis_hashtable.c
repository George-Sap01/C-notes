/*
    Using a Hash Table to count the number of appearances of words and probalities of appearing in them. 
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define table_size 7 // 11491

struct NODE{
    int num_appearances;  // number of total appearances in the text  
    char word[60];    // string with 59 characters max            
    
    int *array;       // contains the numbers of the sentences in the order of appearances in the text
    int array_length; // total memory of the array
    int elem_count;   // number of elements in the array

    struct NODE* next;
};

typedef struct NODE word;

struct POINTERS{
    word *start;
    word *end;
};
typedef struct POINTERS ptrs;

ptrs hash_table[table_size];


int str_len(char *s);
int hash_func(char *s);

/* --- HASH TABLE --- */
// initialize  table
void init_hash_table(ptrs *hash_table);
// add person
int insert_hash_table(ptrs *hash_table, char *s, int sentence, int total_sentences);
// print table
void print_table(ptrs *hash_table);
// search person in hash_table
int search_hash_table(ptrs *hash_table, char *s, word** ptr);


// My functions
void  fileCopy(char *old_file, char *new_file, int *number_of_sentences);
int   word_update(word* ptr, int sentence);
void  HashTable_words(ptrs *hash_table, FILE *fp_new, int total_sentenc);
int   sum_words_total(ptrs *hash_table);
float probability_of_a_in_sentence(ptrs *hash_table, char *string, int total_sentences);
float depended_prob_of_a_given_b(ptrs *hash_table, char *str1, char *str2, int total_sentences);    // str1 = a, str2 = b

int main()
{
    clock_t start, end;
    double cpu_time;

    start = clock();

    char filename[] = "test.txt";
    char filename2[] = "random_lines.txt";
    char newfilename[] = "newLines.txt";
    FILE *fp_new;
    int total_sentences = 0, sum_words = 0;
    
    
    // STEP-1
    fileCopy(filename2, newfilename, &total_sentences);
    init_hash_table(hash_table);
    

    // STEP-2
    fp_new = fopen(newfilename, "r"); // open new file to extract words
    HashTable_words(hash_table, fp_new, total_sentences);
    fclose(fp_new);
    
    print_table(hash_table);
    
    sum_words = sum_words_total(hash_table);
    printf("\nNum of sentences = %d", total_sentences);
    printf("\nsum = %d\n", sum_words);
    
    
    char str1[] = "string3";
    char str2[] = "string1";
    printf("\nprob of %s in a sentence = %f", str1, probability_of_a_in_sentence(hash_table, str1, total_sentences));
    printf("\nprob of %s in a sentence given %s = %f", str1, str2, depended_prob_of_a_given_b(hash_table, str1, str2, total_sentences));
    

    char str3[] = "string5";
    char str4[] = "string9";
    printf("\nprob of %s in a sentence = %f", str3, probability_of_a_in_sentence(hash_table, str3, total_sentences));
    printf("\nprob of %s in a sentence given %s = %f", str3, str4, depended_prob_of_a_given_b(hash_table, str3, str4, total_sentences));    

    // comments are for the other file: filename2
    // char str3[] = "the";
    // char str4[] = "to";
    // printf("\nprob of '%s' in a sentence = %f", str3, probability_of_a_in_sentence(hash_table, str3, total_sentences));
    // printf("\nprob of '%s' in a sentence given '%s' = %f", str3, str4, depended_prob_of_a_given_b(hash_table, str3, str4, total_sentences));
    
    
    end = clock();
    cpu_time = (double)(end - start)/CLOCKS_PER_SEC;
    printf("\nTime:  %f", cpu_time);

    return 0;
}

/* ----------- Functions ----------- */
int str_len(char *s){
    // returns the length of the word
    int i = 0;
    while( s[i] != '\0' ) 
        i++;
    return i;
}

int hash_func(char *s){
    // returns the hash value of the word/string
    unsigned long hash = 5381;
    int c;
    while ((c = *s++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % table_size;
}


/* --------- START: HASH TABLE --------- */
void init_hash_table(ptrs *hash_table){
    for(int i=0; i<table_size; i++) 
    {    
        (*(hash_table + i)).start = NULL; //hash_table[i].start = NULL
        (*(hash_table + i)).end = NULL;   //hash_table[i].end = NULL
    }
}

// insert happens when the word is not in the Hash Table, so is being entered for the first time,
// total_sentences number is need to  allocate 1/3 of total_sentences bytes for the array
int insert_hash_table(ptrs *hash_table, char *s, int sentence, int total_sentences){
    
    word *newnode;
    newnode = (word*)malloc(sizeof(word));
    if(!newnode){
        printf("Error at allocating memory/ insert_hash_table\n");
        exit(0);
    }

    /*  start: initializing newnode  */
    strcpy(newnode->word, s);
    newnode->num_appearances = 1;
    newnode->next = NULL;
    
    newnode->elem_count = 1;
    newnode->array_length = (total_sentences / 3 < 4) ? 4 : (total_sentences / 3);
    newnode->array = (int*)malloc(sizeof(int) * newnode->array_length);
    if(newnode->array == NULL)
    {
        printf("\nError at allocating memory at array/ insert_hash_table\n");
        return FALSE;
    }
    newnode->array[0] = sentence;
    /*  end: initializing newnode  */
   
    int index = hash_func(s);
    
    if(hash_table[index].start != NULL) 
    {   // ptr->start already shows somewhere
        hash_table[index].end->next = newnode;
        hash_table[index].end = newnode;
        return TRUE;
    }
    else // is inserted in the beggining
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
                printf("|WORD: %s, appearances: %d, sent: ", current->word, current->num_appearances);
                for (int j=0; j<current->elem_count-1; j++)
                    printf("%d, ", current->array[j]);
                printf("%d ", current->array[current->elem_count-1]);
                current = current->next;
                printf("| -> \n          ");
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
    char c;
    char buffer[100];
    int counter_for_word = 0, flag = 0, continuous_spaces = 0;

    // Getting this text format
    /*
        length_of_sentence1
        sentence1
        length_of_sentence2
        sentence2
        length_of_sentence3
        sentence3
        ...
    */

    while((c = fgetc(fp_old)) != EOF)
    {
        if(c == '.'){
            buffer[counter_for_word] = c;
            counter_for_word++;
            buffer[counter_for_word] = '\0';
            
            fprintf(fp_new, "%d\n%s\n", counter_for_word, buffer);
            (*number_of_sentences)++;

            counter_for_word = 0;
            flag = 1; 
            //in the end of the file there will be a changing line character
        }
        else{
            if (c != ' ' && c != '\n'){
                // we have a letter 
                buffer[counter_for_word] = (c>=65 && c<=90) ? (c + 32) : c; // capital case letters to small ones 
                counter_for_word++; 
                continuous_spaces = 0; 
                flag = 0;
            }
            else{   
                // the space here is between words
                if(flag == 0 && c == ' ' && continuous_spaces == 0){
                    buffer[counter_for_word] = ' ';
                    counter_for_word++;
                    continuous_spaces = 1;   // we added already one space
                }
                else if(flag == 1 && c == ' ' || c == '\n')  // the space here is after a dot   
                    continue;
            }
        }
    }

    fclose(fp_new);
    fclose(fp_old);
}

void HashTable_words(ptrs *hash_table, FILE *fp_new, int total_sentenc){

    int no_sentence = 1; // counter for the sentence i am currently working with 
    int length;          // length of the word
    int search;          // TRUE OR FALSE
    char garbage[3];     // collects the '.' and '\n' 
    char buff[1000];     // collects the word
    word* w;             // pointer to help with the update
    
    while(no_sentence <= total_sentenc)
    {

        fscanf(fp_new, "%d\n", &length); // gets the length of the sentence and the new line character('\n') 
        fgets(buff, length, fp_new);     // get the word into buffer, '.' and '\n' remains 
        fgets(garbage, 3, fp_new);       // get the '.' and '\n'
        
        // splitting the words in the sentence 
        char *temp_word = strtok(buff, " ,.");
        /*  
            Searching:
            1) if FALSE, then insert_hash_table
            2) if TRUE, then you do update because the word already exists in the Hash Table
        */
       while(temp_word != NULL){
        /*
            looking to see if the word already exists, if it exists then the memory address 
            is returned also for quick update
        */
            search = search_hash_table(hash_table, temp_word, &w);\
            if(search == TRUE) // updating the word
                word_update(w, no_sentence);
            else // inserting the word for the first time 
                insert_hash_table(hash_table, temp_word, no_sentence, total_sentenc);
            
            temp_word = strtok(NULL, " ,.");
        }
        no_sentence++;
            
    }
}

int word_update(word* ptr, int sentence){
    
    int flag = 0;
    int i;

    ptr->num_appearances++;
    if(sentence == ptr->array[ptr->elem_count - 1])
        flag = 1;
    // it already exists in the array
    
    if(flag == 0)
    {
        ptr->elem_count++;
        if(ptr->elem_count == ptr->array_length){
            ptr->array = (int*)realloc(ptr->array, sizeof(int) * (1.5 * ptr->array_length));
            if(!ptr->array){
                printf("\nERROR AT ALLOCATING MEMORY! / word_update\n");
                return FALSE;}
        
            ptr->array_length = 1.5 * ptr->array_length; 
            ptr->array[ptr->elem_count - 1] = sentence;
        }
        else{
            ptr->array[ptr->elem_count - 1] = sentence;
        }
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
            result += cur->num_appearances;
            cur =  cur->next;
        }
    }
    return result;
}

float probability_of_a_in_sentence(ptrs *hash_table, char *string,  int total_sentences){

    word* w;
    int search = search_hash_table(hash_table, string, &w);

    if(search == TRUE)
        return w->elem_count/(float)total_sentences;
    else 
        return 0.0;
}

float depended_prob_of_a_given_b(ptrs *hash_table, char *str1, char *str2, int total_sentences){ 
    // str1 = a, str2 = b
    word *word1, *word2;
    int search1, search2;
    
    search1 = search_hash_table(hash_table, str1, &word1); 
    search2 = search_hash_table(hash_table, str2, &word2);

    int i = 0, j = 0;
    int total = 0; // num of times the two words appear together in a sentence 

    if(search1 == TRUE && search2 == TRUE) // both words exist in the text
    {
        while (i < word1->elem_count && j < word2->elem_count) 
        {
            if (word1->array[i] == word2->array[j]) {
                total++;
                i++;
                j++;
            } 
            else if (word1->array[i] < word2->array[j])
                i++;
            else 
                j++;
        }
        return total/(float)word2->elem_count;
    }
    else
        return 0.0;
}

/* --------- END: FILE --------- */