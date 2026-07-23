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


void random_numbers(int *array, int pass_len, int num, int ch, int CH, int symb);

void password(char *str, int pass_len, int num, int ch, int CH, int symb);

int main(){

    // initiliaze the seed of random numbers
    srand(time(0));

    clock_t start, end;
    double cpu_time;
    start = clock();

    char secret[25]; 
    int symb ;   // number of symbols in the password 
    int num;     // number of numbers in the password
    int ch;      // number of  small characters in the password
    int CH;      // number of big characters in the password
    int length;       // total length of password

    // NECESSARY: n = num + ch + CH
    length = 18;
    num = 5;
    ch = 6;
    CH = 4;
    symb = 3;

    if( length != num + ch + CH + symb ){
        printf("Check the constants, length must be equal to the sum of all variables");
        return 0;
    }

    // printing 1000 random passwords and counting time  
    for(int i=0; i<1000; i++){   
        password(secret, length, num, ch, CH, symb);
        printf("\n%s", secret);
    }

    end  = clock();
    cpu_time = (double)(end - start)/CLOCKS_PER_SEC;
    printf("\n\tTime: %f", cpu_time);

    return 0;
}
/* 
0-9 have ascii values of 48-57
A-Z have ascii values of 65-90
a-z have ascii values of 97-122
*/

void random_numbers(int *array, int pass_len, int num, int ch, int CH, int symb)
{
    int i;
    for(i=0; i<pass_len; i++){
        if(i < num)
            array[i] = rand() % 10 + 48;    // This part is for the random ascii numbers of 0-9
        else if(i >= num && i < ch + num)
            array[i] = rand() % 26 + 65;    // This part is for the random ascii numbers of A-Z
        else if(i >= ch + num && i < ch + num + CH)
            array[i] = rand() % 26 + 97;    // This part is for the random ascii numbers of a-z
        else
            array[i] = symbols[rand() % 8]; // This part is for the random ascii numbers of the symbols 
    }

}

void password(char *str, int pass_len, int num, int ch, int CH, int symb)
{
    int *array;                 // the array with the random ascii values
    int *randomNumbers;         
    int i, j;
    int check, temp;
    int zero = rand()%pass_len; // the position of 0(zero)

    array = (int*)malloc(sizeof(int) * pass_len);
    if(!array){
        printf("Error at allocating memory!");
        exit(0);
    }
    
    randomNumbers = (int*)malloc(sizeof(int) * pass_len);
    if(!randomNumbers){
        printf("Error at allocating memory!");
        exit(0);
    }

    // fill the randomNumbers array with 0's
    for(i=0; i<pass_len; i++)
        randomNumbers[i] = 0;
    
    random_numbers(array, pass_len, num, ch, CH, symb);

    // in this step we randomly insert the n-1 numbers (0 is already inserted)
    // for example if pass_len = 7 this means we nead to assign 6 numbers 
    // one outcome is this randomNumbers[] = {1, 3, 4, 0, 2, 6, 5}
    for(i=0; i<pass_len; i++)
    {
        if(i == zero)
            continue;
            // the position of zero is already occupied from the beginning of the function 
        do{
            check = 0;
            while((temp = rand()%pass_len) == 0); // we do not care when temp = 0

            for(j=0; j<pass_len; j++)
            {
                if(temp == randomNumbers[j]) // if temp already exists in the array then we randomly assign number to temp again 
                {
                    check = 1;
                    break; 
                }
            }
        }while(check == 1);

        randomNumbers[i] = temp;
    }

    for(i=0; i<pass_len; i++)
        str[i] = array[randomNumbers[i]] ; 
    str[pass_len] = '\0';
}

/*

function random_numbers:
we have an array and we fill it with random ascii values with this order
--------------+-------------+--------------+---------------+
|             |             |             |                |
|             |             |             |                |
--------------+-------------+--------------+---------------+
    num             ch          CH                  symb

randomNumbers[n]:
we have have 0, 1, 2, 3, ..., n numbers randomly assigned to the array 
e.g.  n = 10

------+------+------+------+------+------+------+------+------+------+
|   3     9      8      1     5       6     7      2      4       0  |
------+------+------+------+------+------+------+------+------+------+
    0      1     2      3       4      5    6       7       8     9    <- positions of array

*/