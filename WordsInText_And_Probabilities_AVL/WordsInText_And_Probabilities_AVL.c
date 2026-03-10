/*
το αρχειο που θα επεξεργαστω θα περασει πρωτα απο το file_to_file_sentence_split για να εχει την συγκεκριμενη μορφοποιηση.
στοχευει στο να αναλυσει κειμενο μορφοποιημενο με συγκεκριμενο τροπο.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

struct node{
    int cnt;  //πληθος συνολικων εμφανισεων σε ολες τις προτασεις
    char word[60];  // συμβολοσειρα με μαξ 59 χαρακτηρες
    
    struct node *left;
    struct node *right;
    int height;
    
    int *array; // περιεχει τους αριθμους των προτασεων που βρισκεται.
    int elem_array; // περιεχει το συνολικο πληθος των προτασεων
};


typedef struct node TREE_NODE;
typedef struct node* TREE_PTR;

// initialiaze
void TR_init(TREE_PTR* root);
int TR_empty(TREE_PTR root);
//insert
int TR_insert_root(TREE_PTR *root, char *x, int sentence);

int max(int x, int y);
int height(TREE_PTR R);

TREE_PTR AVL_rotate_R(TREE_PTR C);
TREE_PTR AVL_rotate_L(TREE_PTR C);
TREE_PTR AVL_rotate_LR(TREE_PTR C);
TREE_PTR AVL_rotate_RL(TREE_PTR C);

TREE_PTR TR_insert_AVL(TREE_PTR *root, char *x, int sentence);
void TR_print_AVL(TREE_PTR root);
int TR_search_AVL(TREE_PTR root, char *x, TREE_PTR *word);
int TR_UPDATE(TREE_PTR word, int sentence);

// My functions
void fileCopy(char *old_file, char *new_file, int *number_of_sentences);
void sum_words_total(TREE_PTR root, int *sum);
void AVL_words(TREE_PTR *root, FILE *fp_new, int total_sentenc);
float probability_of_a_in_sentence(TREE_PTR root,char *string, int total_sentences);
float depended_prob_of_a_given_b(TREE_PTR root,char *str1, char *str2, int total_sentences, char **p);    // str1 = a, str2 = b
void array_of_sentences(char *str, char ***ptrs, int total_sent);


int main()
{
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    char old_file[] = "test.txt";
    char new_file[] = "new_lines.txt";  
    char lots_lines[] = "random_lines.txt";
   
    FILE *fp_new;
    TREE_PTR root;
    int total_sentences = 0, sum_words = 0;
    char **sentences;

    // οταν θελω να πειραματιστω με τα αλλα κειμενα αλλαζω εδω το old_file
    fileCopy(lots_lines, new_file, &total_sentences); // μορφοποιω το αρχειο οπως θελω
    TR_init(&root); // αρχικοποιω το δενδρο
    
    fp_new = fopen(new_file, "r"); // ανοιγω το καινουργιο μορφοποιημενο αρχειο
    AVL_words(&root, fp_new, total_sentences); // περναω τις λεξεις στο δεντρο
    fclose(fp_new);

    // TR_print_AVL(root);  // εκτυπωνω τις λεξεις
    sum_words_total(root, &sum_words); // επιστρεφει το αθροισμα του συνολικου πληθους λεξεων στη μεταβλητη sum 
    printf("\n\n Total words = %d", sum_words);

    array_of_sentences(new_file, &sentences, total_sentences);

    char str1[] = "in";
    char str2[] = "it";

    float prob = probability_of_a_in_sentence(root, str1, total_sentences);
    printf("\n prob of the word %s to be found in a sentence = %f\n", str1,prob);
    prob = depended_prob_of_a_given_b(root, str1, str2, total_sentences, sentences);
    printf("\n prob of the word '%s' to be found in a sentence | given that the word '%s' exitst  = %f\n\n", str1, str2, prob);

    end = clock();
    cpu_time_used = (double)(end-start)/CLOCKS_PER_SEC;
    printf("\nTime: %f", cpu_time_used);
    return 0;
}


void TR_init(TREE_PTR* root){
    *root = NULL;
}


int TR_empty(TREE_PTR root){
    return root == NULL;
}


int TR_insert_root(TREE_PTR *root, char *x, int sentence){
    TREE_PTR newnode;

    if( *root != NULL)
        return FALSE;

    newnode = (TREE_PTR)malloc(sizeof(TREE_NODE));
    if(!newnode)
    {
        printf("adynamia desmeusis mnimis");
        return FALSE;
    }

    strcpy(newnode->word,  x);
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->cnt = 1;
    
    newnode->array = (int*)malloc(sizeof(int));
    if(!newnode->array)
    {
        printf("\nAdynamia desmeusis mnimis / TR_insert_AVL\n");
        return FALSE;
    }
    newnode->array[0] = sentence;
    newnode->elem_array = 1;

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


TREE_PTR TR_insert_AVL(TREE_PTR *root, char *x, int sentence)
{
	if (*root==NULL)
	{
		TR_insert_root(root, x, sentence);
		(*root)->height = 1; 
		return *root; 
	}
	else if (strcmp(x, (*root)->word) < 0)
	{
		(*root)->left = TR_insert_AVL(&((*root)->left), x, sentence);
		if (height((*root)->left)-height((*root)->right) == 2)
		{
			if(strcmp(x, (*root)->left->word) < 0)
				(*root) = AVL_rotate_R(*root);
			else
				(*root) = AVL_rotate_LR(*root);
		}
	}
	else if (strcmp(x, (*root)->word) > 0)
	{
		(*root)->right = TR_insert_AVL(&((*root)->right), x, sentence);
		if (height((*root)->left)-height((*root)->right) == -2)
		{
			if(strcmp(x, (*root)->right->word) > 0)
				(*root) = AVL_rotate_L(*root);
			else
				(*root) = AVL_rotate_RL(*root);
		}
	}
	// else x == root->x : den kanoyme tipota
	
	(*root)->height = max(height((*root)->left), height((*root)->right)) + 1;
	return (*root); 
}

int TR_search_AVL(TREE_PTR root, char *x, TREE_PTR *word)
{
	TREE_PTR current = root;
	
	while (current != NULL)
	{
		if (strcmp(x, current->word) == 0)
		{	
            *word = current;
            return TRUE;
        }    
		else if (strcmp(x, current->word) < 0 )
			current=current->left;
		else // x > current->data 
			current=current->right;
	}

    *word = NULL;
	return FALSE;
}

int TR_UPDATE(TREE_PTR word, int sentence){
    
    int flag = 0;
    word->cnt++;
    
    for(int i=0; i < word->elem_array; i++)
    {
        if(sentence == word->array[i])
        {   
            flag = 1;
            break;
        }
    }
    
    if(flag == 0)
    {
        word->array = (int*)realloc(word->array, sizeof(int) * (word->elem_array + 1));
        if(!word->array)
        {
            printf("\nAdynamia desmeusis mnimis! / TR_UPSATE\n");
            return FALSE;
        }   
        word->array[word->elem_array] = sentence;
        word->elem_array++;
    }
    
    return TRUE;

}

void TR_print_AVL(TREE_PTR root){
	
    if (root->left != NULL)
		TR_print_AVL(root->left);

	printf("%11s\tcnt = %d\t elem = %d array = ", root->word, root->cnt, root->elem_array);
    for(int i = 0; i < root->elem_array; i++)
        printf("%d ", root->array[i]);
    printf("\n");

    if (root->right != NULL)
		TR_print_AVL(root->right);
}


void sum_words_total(TREE_PTR root, int *sum)
{  
    if(root->left != NULL)
        sum_words_total(root->left, sum);

    *sum = *sum + root->cnt;

    if(root->right != NULL)
        sum_words_total(root->right, sum);
}

void fileCopy(char *old_file, char *new_file, int *number_of_sentences)
{
    FILE *fp_new, *fp_old;

    fp_old = fopen(old_file, "r");
    fp_new = fopen(new_file, "w");
    char buffer[1000];
    char c;
    int counter = 0, flag = 0;

    // παιρνω την μορφοποιηση που θελω απο το κειμενο.
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
        else if(c == '\n' || (flag == 1 && c == ' '))
        {   
            flag = 0;
            continue;
        }
        else
        {   
            flag = 0;
            buffer[counter] =  c + 32 * ( c>=65 && c<=90);
            counter++;
        }
    }
    fclose(fp_new);
    fclose(fp_old);

}

void AVL_words(TREE_PTR *root, FILE *fp_new, int total_sentenc){

    int k =1; // enas metritis
    int length, search;
    int no_sentence = 0;
    char garbage[3], buff[100];
    TREE_PTR word;
    
    while(k <= total_sentenc)
    {

        fscanf(fp_new, "%d\n", &length); // παιρνω το length της προτασης, και απορροφω το χαρακτηρα αλλαγης γραμμης \n
        fgets(buff, length, fp_new); // γραφω την προταση στο buff, δεν πιανει την . στο τελος της προτασης, αρα μετα πρεπει να απορροφησω την τελεια\n
        fgets(garbage, 3, fp_new); // απορροφαει το .\n
        no_sentence++;

        // χωριζω τις λεξεις της προτασης.
        char *temp_word = strtok(buff, " \",!:?");
        
        /*  κανεις search:
            1) αν ειναι FALSE, τοτε κανεις TR_insert_AVL
            2) αν ειναι TRUE, τοτε κανεις update γιατι ηδη υπαρχει η λεξη στο δεντρο.
        */
        while(temp_word != NULL){
            
            // τις τοποθετω στο δεντρο
            search = TR_search_AVL(*root, temp_word, &word);

            if(search == TRUE)
                TR_UPDATE(word, no_sentence);
            else
                TR_insert_AVL(root, temp_word, no_sentence);

            temp_word = strtok(NULL, " \",!:?");
        }

        k++;
    }
}

float probability_of_a_in_sentence(TREE_PTR root, char *string,  int total_sentences){

    TREE_PTR word;
    int search = TR_search_AVL(root, string, &word);

    if(search == TRUE)
        return word->elem_array/(float)total_sentences;
    else 
        return 0.0;
}

float depended_prob_of_a_given_b(TREE_PTR root,char *str1, char *str2, int total_sentences, char **p){

    
    TREE_PTR word1, word2;
    int search1, search2;
    
    search1 = TR_search_AVL(root, str1, &word1); 
    search2 = TR_search_AVL(root, str2, &word2);

    int i;
    int start, finish, middle;
    int total = 0; // ποσες φορες εμφανιζονται στην ιδια προταση

    if(search1 == TRUE && search2 == TRUE) // υπαρχουν και οι δυο λεξεις στο δενδρο
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
                    printf("%d) %s\n", total, p[word1->array[i]-1]);
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

void array_of_sentences(char *str, char ***ptrs, int total_sent)
{
    int n = 0, flag = 0;
    int length;
    char garbage[3];
    FILE *fp = fopen(str, "r");
    
    while(n < total_sent)
    {

        if(flag==0)
        {
            (*ptrs) = (char**)malloc(sizeof(char*));
            if(!(*ptrs)){
                printf("Adynamia desmeusis mnimis!/array_of_sentences");
                exit(0);
            }
            flag = 1;
        }
        else
        {
            (*ptrs) = (char**)realloc(*ptrs, sizeof(char*) * (n+1));
            if(!(*ptrs)){
                printf("Adynamia desmeusis mnimis!/array_of_sentences");
                exit(0);
            }
        }

        fscanf(fp, "%d\n", &length);
        (*ptrs)[n] = (char*)malloc(sizeof(char) * (length+1));
        if((*ptrs)[n] ==  NULL){
            printf("jdjdn");
            exit(0);
        }

        fgets((*ptrs)[n], length+1, fp);
        fgets(garbage, 3, fp);

        n++;
    }

}