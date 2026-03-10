#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct PERSON{
    char name[80]; //string with 79 characters max
    int id; //index in array is id-1
    int *restr; //restrictions
    int num; // number of restrictions
    int secret_santa; //the id of the person who's gonna recieve the gift
};

typedef struct PERSON person;


//secondarie functions 
int len(char *buf);
int string_to_int(char *buffer);
int ipow(int x, int y);

void read_names(char *filename, person **array, int *num); //read names from the file into an array
void print_people(person *array, int num);
void restrictions(person *array, int num); // restrictions for pairs not individuals.
void add_restrictions(person *array, int index, int restriction); // add the restriction in the struct 
int draw(person *array, int num);

int main()
{
    srand(time(NULL));
    
    char filename[] = "names.txt";
    person *array;
    int total_people;
    int check;

    read_names(filename, &array, &total_people);
    print_people(array, total_people);

    restrictions(array, total_people);
    printf("\n\n");
    print_people(array, total_people);
    printf("\n\n");
    while((check = draw(array, total_people)) == 0);
    print_people(array, total_people);

    return 0;
}


void read_names(char *filename, person **array, int *num)
{
    int cnt = 0; // for the array
    int counter = 0; // for the buffer
    char buffer[100], c;
    FILE *fp;
    
    fp = fopen(filename, "r");

    // num of people 
    while( (c=fgetc(fp)) != '\n')
    {
        buffer[counter] = c;
        counter++;    
    }
    buffer[counter] = '\0';
    
    *num = string_to_int(buffer);
    
    *array = (person*)malloc(sizeof(person) * (*num));
    if( !(*array) ){
        printf("Error at allocating memory\n");
        exit(0);
    }

    //get the names in the array
    while( (c = fgetc(fp)) != EOF ){

        counter = 0;
        while( (c>=65 && c<= 90) || (c>=97 && c<=122) || c == ' ')
        {
            buffer[counter] = c;
            counter++;
            c = fgetc(fp);
 
            if( c=='\n' || c==EOF){    
                buffer[counter] = '\0';
                break;
            }
        }

        // strcpy( (*array)[cnt].name, buffer);
        strcpy( ((*array) + cnt)->name, buffer);
        (*array)[cnt].id = cnt + 1;
        (*array)[cnt].num = 0;
        (*array)[cnt].secret_santa = -1;
        cnt++;
    }

    fclose(fp);
}

void print_people(person *array, int num)
{
    for(int i=0; i<num; i++)
    {
        printf("\t%d) %s\n", array[i].id, array[i].name);
        if(array[i].num == 0)
            printf("\t\tNo restrictions!\n");
        else
        {
            printf("\t\tRestrictions: ");
            for(int j=0; j<array[i].num; j++)
                printf("%d ", array[i].restr[j]);
            printf("\n");
        }
        if(array[i].secret_santa != -1)
            printf("\t\t>>(%d) %s\n", array[array[i].secret_santa - 1].id, array[array[i].secret_santa - 1].name);
        else
            printf("\t\tNo secret santa yet!\n");
    }
}

void restrictions(person *array, int num)
{
    int i, j;
    int res1, res2;
    int check;

    printf("\n\n");
    for(i=0; i<num; i++)
        for(j=i+1; j<num; j++)
            printf(" \t\t%s(%d)  -  %s(%d)\n", array[i].name, array[i].id, array[j].name, array[j].id);

    printf("\n");
    while(1)
    {
        printf("\tDo you want to add a restriction? (1-yes, 0-no)\n\t>");
        scanf("%d", &check);

        if(check == 0)
            break;
        
        do{
            printf("\n\tGive the restriction using the numbers in the parentheses.\n\tType it in the following format: number-number (for example 1-3)");
            printf("\n\tThis means that the person with id=1 cannot have for secret santa the person with id=3\n\t>");
            scanf("%d-%d", &res1, &res2);

            if( res1 > num || res1<1 || res2 > num || res2<1 )
                printf("\t\tRestrictions are out of bounds!\n");

        }while( res1 > num || res1<1 || res2 > num || res2<1 );

        add_restrictions(array, res1-1, res2);
    }

    
    
}

void add_restrictions(person *array, int index, int restriction)
{
    int check = 0;

    if( array[index].num == 0 ) // einai adeio
    {
        array[index].restr = (int*)malloc(sizeof(int) * 1);
        if(!array[index].restr)
        {
            printf("Error at memory allocation");
            exit(0);
        }
        
        array[index].restr[0] = restriction;
        array[index].num++;

    }
    else //idi yparxei kapoios periorismos
    {
        for(int i=0; i<array[index].num; i++){
            if(array[index].restr[i] == restriction){
                check = 1;
                break;
            }
        }

        if(check == 1)
            printf("\n\tThis restriction already exists.\n");
        else
        {
            array[index].restr = realloc(array[index].restr, sizeof(int) * (array[index].num + 1));
            if(!array[index].restr)
            {
                printf("Error at memory allocation");
                exit(0);
            }

            array[index].restr[array[index].num] = restriction;
            array[index].num++;
        }
    }
}


int draw(person *array, int num)
{

    int givers[num], receivers[num];
    int cntg = 0, cntr = 0;
    int flag_g, flag_r1, flag_r2;
    int giver, receiver;
    int i, cnt;
    
    while(cntg != num)
    {    
        do{
            flag_g= 0;
            giver = rand() % num + 1; //  1 2 3 4 ... num
            //check if giver is already in the array
            if(cntg != 0)
            {
                for(i=0; i<cntg; i++)
                {
                    if(givers[i] == giver)
                    {
                        flag_g = 1;
                        break;
                    }
                }
            }
        }while(flag_g == 1);
        
        cnt = 0;
        do{ 
            flag_r1 = 0;
            flag_r2 = 0;
            receiver = rand() % num + 1;
            //check if receiver is already in the array
            if(cntr != 0)
            {
                for(i=0; i<cntr; i++)
                {
                    if(receivers[i] == receiver)
                    {
                        flag_r1 = 1;
                        break;
                    }
                }
            }
            //check if receiver is in the restrictions of the giver
            if(array[giver-1].num != 0)
            {
                for(i=0; i<array[giver].num; i++)
                {
                    if(array[giver].restr[i] == receiver)
                    {
                        flag_r2 = 1;
                        break;
                    }
                }
            }

            if(cntg == num-1 && receiver == giver && cnt > 4)
                return 0;

            cnt++;

        }while(flag_r1 == 1 || flag_r2 == 1 || receiver == giver);

        // add the people in the arrays
        receivers[cntr] = receiver;
        givers[cntg] = giver;
        cntg++;
        cntr++;

        // add the secret santa
        array[giver - 1].secret_santa = receiver;
    }

    return 1;
}

/* Secondaries */ 
int len(char *buf){
    int i = 0;
    while(buf[++i] != '\0')
        ;
    return i;
}

int ipow(int x, int y)
{
    int i, res = 1;

    for(i=1; i<=y; i++)
        res *= x;
    return res;
}

int string_to_int(char *buffer)
{
    int length = len(buffer);
    int result = 0;
    int cnt = 0;
    int i;
    /*
    buf = '123' , lenght = 3

    buf[3-1] = 3 
    result += 3 * pow(10, 0)
    buf[3-2] = 2
    result += 2 * pow(10, 1)
    buf[3-3] = 1
    result += 1 * pow(10, 2)
    */

    for(i = length-1; i>-1; i--){
        result += (buffer[i] - '0') * ipow(10, cnt);
        cnt++;
    }
    return result;
}
