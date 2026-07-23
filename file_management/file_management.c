#include <stdio.h>
#include <stdlib.h>

#define TRUE 1 
#define FALSE 0

struct record{
    char name[80];
    int age;
    // we can add more attributes...
};

/*
    1. OPEN FILE
    2. CLOSE FILE
    3. SHOW NUMBER OF RECORDS
    4. ADD RECORD
    5. PRINT RECORD
    6. PRINT ALL RECORDS
    7. MODIFY RECORD 
    8. DELETE RECORD
    9. CHANGE POSITION BETWEEN TWO RECORDS
    10. EXIT 
*/

int open_file(char *filename, FILE **fp);
int num_deposits(FILE *fp, int *n);
int close_file(FILE *fp);
int add_record(FILE *fp);
int print_record(FILE *fp);
int print_all(FILE *fp);
int modify_record(FILE *fp);
int delete_record(FILE *fp);
int change_pos(FILE *fp);


int main(){

    FILE *fp = NULL;
    int num_of_deposits;
    int choice;
    int check;
    char file_name[] = "DATA.dat";

    while(1)
    {
        printf("\n\n-------------");
        printf("\nMENU");
        printf("\n 1. Open file");
        printf("\n 2. Close file");
        printf("\n 3. Show deposits");
        printf("\n 4. Add record");
        printf("\n 5. Print record");
        printf("\n 6. Print all records");
        printf("\n 7. Modify record");
        printf("\n 8. Delete record");
        printf("\n 9. Change positions between two records");
        printf("\n10. Exit");

        printf("\n\nChoose: ");
        scanf("%d", &choice);

        switch(choice)
        {    
            case 1: 
                check = open_file(file_name, &fp);
                if (check == 0)
                    { printf("ERROR at opening file"); exit(0); }
                break;
            case 2:
                check = close_file(fp);
                if(check == 1)
                    printf("File has closed");
                else 
                    { printf("ERROR at closing file"); exit(0); }
                break;
            case 3:
                check = num_deposits(fp, &num_of_deposits);
                if(check == 1)
                    printf("\nNumber of deposits: %d", num_of_deposits);
                break;
            case 4:
                check = add_record(fp);
                if(check==1)
                    printf("\nThe update is succesfull!");
                else
                    printf("\nERROR at updating the file");
                break;
            case 5:
                check = print_record(fp);
                if(check == 0)
                    printf("\nERROR at printing the record");
                break;
            case 6:
                check = print_all(fp);
                if(check == 0)
                    printf("\nERROR at printing all the records");
                    break;
            case 7:
                check = modify_record(fp);
                if(check == 0)
                    printf("\nERROR at modifying the record");
                else
                    printf("\nUpdate was succesfull");
                break;
            case 8:
                check = delete_record(fp);
                if(check == 0)
                    printf("\nERROR at deleting the record");
                else
                    printf("\nUpdate was succesfull");
                break;
            case 9:
                check = change_pos(fp);
                if(check == 0)
                    printf("\nERROR at changing positsion the records");
                else
                    printf("\nUpdate was succesfull");
                break;
            case 10:
                printf("\nBYE BYE");
                exit(0);
            default:
                printf("\nWRONG CHOICE");
        }
    }
    return 0;
}

/*
    this is the structure of the file, in the beggining there is the number of records and then in order there are the records
    -----------------------------------------------
    |               int n                         |
    |               record 1                      |
    |               record 2                      |
    |               record 3                      |
    |               record 4                      |
                       ...
*/

/*
    returns 1 if the file is successfully open and a pointer to the file
    returns 0 if the does not open successfully
*/

int open_file( char *filename, FILE **fp){
    int number_of_records_by_default_at_start = 0;
    *fp = fopen(filename, "rb");

    // if the file does NOT exist already
    if(*fp == NULL)
    {
        printf("\n%s does not exist", filename);
        *fp = fopen(filename, "wb+");
        if(*fp == NULL){   
            printf("\nERROR AT CREATING THE FILE"); 
            return FALSE; }

        fwrite(&number_of_records_by_default_at_start, sizeof(int), 1, *fp);
        rewind(*fp);
        return TRUE;

    }
    else // if the file does exist already
    {
        printf("\n%s exist", filename);
        fclose(*fp);
        *fp = fopen(filename, "rb+");
        return TRUE;
    }
}


int num_deposits(FILE *fp, int *n)
{   
    int number_read;
    if(fp == NULL){   
        printf("\nFirst open the file");
        return FALSE;
    }
    else{
        number_read = fread(n, sizeof(int), 1, fp);
        rewind(fp);
        if(number_read == 1)
            return TRUE;
        else{
            printf("\nERROR at reading the right amount of data from file");
            return FALSE;
        }
    }
}

int close_file(FILE *fp) {
    if(!fclose(fp))
        return TRUE;
    else
        return FALSE;
    //return TRUE ? (!fclose(fp)) : (return FALSE) 
}

int add_record(FILE *fp){
    if(fp == NULL){   
        printf("First open the file"); 
        return FALSE;}

    struct record temp;
    int c1;
    int number_of_deposits;

    num_deposits(fp, &number_of_deposits);
    printf("Give a name: ");
    scanf("%s", temp.name);
    printf("Give an age: ");
    scanf("%d", &temp.age);

    // moving the cursor appropriately
    c1 = fseek(fp, 4 + (sizeof(struct record)*number_of_deposits), SEEK_SET);
    if(c1==1){
        printf("Error at moving the cursor\n");
        return FALSE;
    }

    // adding record in the file
    c1 = fwrite(&temp, sizeof(struct record), 1, fp);
    if(c1==1){   
        // increasing the number of records
        number_of_deposits++;
        rewind(fp);
        fwrite(&number_of_deposits, sizeof(int), 1, fp);
        rewind(fp);
        return TRUE;
    }
    else    
        return FALSE;
}

int print_record(FILE *fp)
{
    struct record temp;
    int c1;
    int ch;
    int check;
    int number_of_deposits;

    check = num_deposits(fp, &number_of_deposits);
    
    if(fp==NULL){
        printf("\nFirst open the file"); 
        return FALSE;
    }
    else if( number_of_deposits == 0){
        printf("\nFirst add records to file"); 
        return FALSE;    
    }
    else if(check ==0){
        printf("\nError"); 
        return FALSE;        
    }
    
    do{
        printf("\nGive a number between (1 - %d): ", number_of_deposits);
        scanf("%d", &ch);
        
        if (ch<1 || ch>number_of_deposits)
            printf("\nyou gave a wrong number");

    }while(ch<1 || ch>number_of_deposits);

    // moving the cursor
    c1 = fseek(fp, 4 + (sizeof(struct record)*(ch-1)), SEEK_SET);
    if(c1==1)
    {
        printf("Error at moving the cursor\n");
        return FALSE;
    }

    c1 = fread(&temp, sizeof(struct record), 1, fp);
    rewind(fp);   
    if(c1 != 1)    
        return FALSE;
    
    printf("\n");
    printf("\nName: %s", temp.name);
    printf("\nAge: %d", temp.age);

}

int print_all(FILE *fp)
{
    int i;
    struct record temp;
    int m;
    int check;
    int number_of_deposits;

    check=num_deposits(fp, &number_of_deposits);
    
    if(check ==0){
        printf("\nERROR");
        return FALSE;}

    for(i=1; i<=(number_of_deposits+1); i++){
        if(i==1){
            check = fread(&m, sizeof(int), 1, fp);
            if(check != 1) { 
                printf("Error!"); 
                return FALSE;
            }
            printf("\n(number of deposits) n: %d", m);
            printf("\n=========================");
            
        }
        else{       
            check = fread(&temp, sizeof(struct record), 1, fp);   
            if(check != 1)    
                return FALSE;
            printf("\nDEPOSIT no.%d", i-1);
            printf("\nName: %s", temp.name);
            printf("\nAge: %d", temp.age);
            printf("\n****************");
        }
    }
    
    rewind(fp);
    return TRUE;
}


int modify_record(FILE *fp)
{
    struct record temp;
    int ch, check;
    int number_of_deposits;

    check = num_deposits(fp, &number_of_deposits);
    
    if(fp==NULL){
        printf("\nFirst open the file"); 
        return FALSE;
    }
    else if( number_of_deposits == 0){
        printf("\nFirst add records to file"); 
        return FALSE;    
    }
    else if(check ==0){
        printf("\nError"); 
        return FALSE;        
    }
    
    do{
        printf("\nGive a number between (1 - %d): ", number_of_deposits);
        scanf("%d", &ch);
        
        if (ch<1 || ch>number_of_deposits)
            printf("\nyou gave a wrong number");
            
    }while(ch<1 || ch>number_of_deposits);

    printf("\nGive a Name: ");
    scanf("%s", temp.name);
    printf("\nGive an Age: ");
    scanf("%d", &temp.age);
    

    // miving the cursor 
    check = fseek(fp, 4 + (sizeof(struct record)*(ch-1)), SEEK_SET);
    if(check==1){
        printf("\nError at moving the cursor\n");
        return FALSE;
    }

    check = fwrite(&temp, sizeof(struct record), 1, fp);   
    if(check != 1)    
        return FALSE;
    
    rewind(fp);
    return TRUE;
    
}

int delete_record(FILE *fp)
{
    struct record temp;
    int c1;
    int ch;
    int number_of_deposits;

    num_deposits(fp, &number_of_deposits);
    
    if(fp==NULL){   
        printf("First open the file"); 
        return FALSE;
    }
    else if( number_of_deposits == 0){
        printf("First add records to file"); 
        return FALSE;    
    }
    
    do{
        printf("\nGive a number between (1 - %d): ", number_of_deposits);
        scanf("%d", &ch);
        
        if (ch<1 || ch>number_of_deposits)
            printf("you gave a wrong number");
            
    }while(ch<1 || ch>number_of_deposits);

     // moving the cursor in the beggining of the last node 
    c1 = fseek(fp, 4 + (sizeof(struct record)*(number_of_deposits-1)), SEEK_SET);
    if(c1==1){
        printf("Error at moving the cursor\n");
        return FALSE;
    }

    // passing the last node in temp
    fread(&temp, sizeof(struct record), 1, fp);
    
    // moving back the cursor in the start and placing it in the beggining of the node i want to delete,
    // i overwrite in the place the last node of the file (meaning temp) 

    rewind(fp);
    fseek(fp, 4 + (sizeof(struct record)*(ch-1)), SEEK_SET);
    fwrite(&temp, sizeof(struct record), 1, fp);

    // need to decrease the number of deposits in the start of the file
    rewind(fp);
    number_of_deposits--;
    fwrite(&number_of_deposits, sizeof(int), 1, fp);
    
    rewind(fp);
    return TRUE;
    
}


int change_pos(FILE *fp)
{

    struct record temp1, temp2;
    int check;
    int n1, n2;
    int number_of_deposits;

    check = num_deposits(fp, &number_of_deposits);
    
    if(fp==NULL){   
        printf("\nFirst open the file"); 
        return FALSE;
    }
    else if( number_of_deposits == 0 || number_of_deposits==1 ){
        printf("\nFirst add at least 2 records to file"); 
        return FALSE;    
    }
    else if(check ==0){
        printf("\nError"); 
        return FALSE;        
    }

    do{
        printf("\nGive a number between (1 - %d): ", number_of_deposits);
        scanf("%d", &n1);
        
        if (n1<1 || n1>number_of_deposits)
            printf("\nyou gave a wrong number");
            
    }while(n1<1 || n1>number_of_deposits);


    do{
        printf("\nGive a number between (1 - %d): ", number_of_deposits);
        scanf("%d", &n2);
        
        if (n2<1 || n2>number_of_deposits)
            printf("\nyou gave a wrong number");
            
    }while(n2<1 || n2>number_of_deposits);

    // pass to temp1 deposit number1
    fseek(fp, 4 + (sizeof(struct record)*(n1-1)), SEEK_SET);
    check = fread(&temp1, sizeof(struct record), 1, fp);
    if(check !=1){
        printf("\nError at temp1");
        return FALSE;
    }
    rewind(fp);

    // pass to temp2 deposit number2
    fseek(fp, 4 + (sizeof(struct record)*(n2-1)), SEEK_SET);
    check = fread(&temp2, sizeof(struct record), 1, fp);
    if(check !=1){
        printf("\nError at temp2");
        return FALSE;
    }
    rewind(fp);

    // overwrite on  number1 number2 element 
    fseek(fp, 4 + (sizeof(struct record)*(n1-1)), SEEK_SET);
    check = fwrite(&temp2, sizeof(struct record), 1, fp);
    if(check !=1){
        printf("\nError at overwriting ");
        return FALSE;
    }
    rewind(fp);

    // overwrite on  number2 number1 element 
    fseek(fp, 4 + (sizeof(struct record)*(n2-1)), SEEK_SET);
    check = fwrite(&temp1, sizeof(struct record), 1, fp);
    if(check !=1){
        printf("\nError at overwriting");
        return FALSE;
    }
    rewind(fp);
    return TRUE;
    
}