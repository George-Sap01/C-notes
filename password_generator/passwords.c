#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
33 -> !
38 -> &
40 -> (
41 -> )
47 -> /
64 -> @
91 -> [
93 -> ]   ΠΛΗΘΟΣ = 8
*/

int symbols[] = {33,38,40,41,47,64,91,93};

void random_numbers(int *array, int n, int num, int ch, int CH, int symb);
void password(char *str, int n, int num, int ch, int CH, int symb);

int main()
{
    srand(time(0));

    clock_t start, end;
    double cpu_time;
    start = clock();

    char secret[25];
    int symb ; // number of symbols in the password 
    int num; // number of numbers in the password
    int ch; // number of  small characters in the password
    int CH; // number of big characters in the password
    int n; // total length of password

    // ΠΡΟΣΟΧΗ!! n = num + ch + CH
    n = 18;
    num = 5;
    ch = 6;
    CH = 4;
    symb = 3;

    for(int i=0; i<1000; i++)
    {   password(secret, n, num, ch, CH, symb);
        printf("\n%s", secret);
    }
    end  = clock();
    cpu_time = (double)(end - start)/CLOCKS_PER_SEC;
    printf("\n\tTime: %f", cpu_time);

    return 0;
}

void random_numbers(int *array, int n, int num, int ch, int CH, int symb)
{
    int i;
    for( i=0; i<n; i++)
    {
        if(i < num )
            array[i] = rand()%10 + 48;
        else if( i >= num && i < ch + num)
            array[i] = rand()%26 + 65;
        else if( i >= ch + num && i < ch + num + CH)
            array[i] = rand()%26 + 97;
        else
            array[i] = symbols[rand() % 8];
    }

}

void password(char *str, int n, int num, int ch, int CH, int symb)
{
    static int *array;
    int i, j;
    int *randomNumbers;

    int temp;
    int check;
    int zero = rand()%n; // edv einai poy tha brisketai to 0

    array = (int*)malloc(sizeof(int) * n);
    if(!array)
    {
        printf("Error at allocating memory!");
        exit(0);
    }
    
    randomNumbers = (int*)malloc(sizeof(int) * n);
    if(!randomNumbers){
        printf("Error at allocating memory!");
        exit(0);
    }

    for(i=0; i<n; i++)
        randomNumbers[i] = 0;
    
    random_numbers(array, n, num, ch, CH, symb);

    for(i=0; i<n; i++)
    {
        if(i == zero)
            continue;
        
        do{
            check = 0;
            while((temp=rand()%n) == 0); // den theloume to temp na einai 0

            for(j=0; j<n; j++)
            {
                if(temp == randomNumbers[j]) //αν το temp υπαρχει ηδη τοτε θα βγαλει flag 
                {
                    check = 1;
                    break;
                }
            }
        }while(check == 1);

        randomNumbers[i] = temp;
    }

    for(i=0; i<n; i++)
        str[i] = array[randomNumbers[i]] ; 
    str[n] = '\0';
}

/*

random_numbers:
παιρνει εναν πινακα, τον array και του περναει στοιχεια με αυτην την διαταξη 
--------------+-------------+--------------+---------------+
|             |             |             |                |
|             |             |             |                |
--------------+-------------+--------------+---------------+
    num             ch          CH                  symb

randomNumbers[n]:
εχεις ως στοιχεια n στο πληθος αριθμους με τυχαια σειρα
π.χ. αν ειναι n = 10

------+------+------+------+------+------+------+------+------+------+
|   3     9      8      1     5       6     7      2      4       0  |
------+------+------+------+------+------+------+------+------+------+
    0      1     2      3       4      5    6       7       8     9    <- θεσεις στον πινακα


*/