#include <stdio.h>
#include <stdlib.h>

#define TRUE 1 
#define FALSE 0

struct record{
    char name[80];
    int age;
};

/*
    1. ΑΝΟΙΓΜΑ ΑΡΧΕΙΟΥ
    2. ΚΛΕΙΣΙΜΟ ΑΡΧΕΙΟΥ
    3. ΔΕΙΞΕ ΤΟ ΠΛΗΘΟΣ ΤΩΝ ΕΓΓΡΑΦΩΝ 
    4. ΠΡΟΣΘΗΚΗ RECORD (πρεπει να αυξανεται το n )
    5. ΤΥΠΩΣΗ RECORD
    6. ΤΥΠΩΣΗ ΣΤΟΙΧΕΙΩΝ ΑΡΧΕΙΟΥ
    7. MODIFY RECORD 
    8. ΔΙΑΓΡΑΦΗ RECORD
    9. ΑΛΛΑΓΗ ΘΕΣΕΩΝ 
    10. ΤΕΛΟΣ 
*/

int open_file( char *filename, FILE **fp);
int show_deposits(FILE *fp, int *n);
int close_file(FILE *fp);
int add_record(FILE *fp);
int print_record(FILE *fp);
int print_all(FILE *fp);
int modify_record(FILE *fp);
int delete_record(FILE *fp);
int change_pos(FILE *fp);


void main()
{
    char file_name[] = "try1.dat";
    FILE *fp = NULL;
    int num_of_deposits;
    int choice;
    int check;

    while(1)
    {
        printf("\n\n-------------");
        printf("\nMENU");
        printf("\n 1. Open file");
        printf("\n 2. Close file");
        printf("\n 3. Show deposits");
        printf("\n 4. Add record");
        printf("\n 5. Print record");
        printf("\n 6. Print all");
        printf("\n 7. Modify record");
        printf("\n 8. Delete record");
        printf("\n 9. Change positions");
        printf("\n10. Exit");

        printf("\n\nChoose: ");
        scanf("%d", &choice);

        switch(choice)
        {    
            case 1: 
                open_file(file_name, &fp);
                break;
            case 2:
                check = close_file(fp);
                if(check == 1)
                    printf("File has closed");
                else 
                    { printf("ERROR at closing file"); exit(0); }
                break;
            case 3:
                check = show_deposits(fp, &num_of_deposits);
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

}

/*
    αυτη ειναι η δομη του αρχειου, στην αρχη εχει το το πληθος των records που εχει και μετα εχει τα records στην σειρα 
    -----------------------------------------------
    |               int n                         |
    |               record 1                      |
    |               record 2                      |
    |               record 3                      |
    |               record 4                      |
                       ...

*/

/*
    γυρναει 1 αν το αρχειο ανοιξε και εναν δεικτη στο αρχειο fp
    γυρναει 0 αν το αρχειο δεν ανοιξε 
*/

int open_file( char *filename, FILE **fp)
{
    int number_of_records_by_default_at_start = 0 ;
    (*fp) = fopen(filename, "rb");

    //αν δεν υπαρχει ηδη το αρχειο
    if((*fp) == NULL)
    {
        printf("\n%s does not exist", filename);
        (*fp) = fopen(filename, "wb+");
        if((*fp) == NULL){   
            printf("\nERROR AT CREATING THE FILE"); 
            return FALSE; }

        fwrite(&number_of_records_by_default_at_start, sizeof(int), 1, (*fp));
        rewind((*fp));
        return TRUE;

    }
    else // αν υπαρχει ηδη το αρχειο
    {
        printf("\n%s exist", filename);
        fclose((*fp));
        (*fp) = fopen(filename, "rb+");
        return TRUE;
    }
}

/*
επιστρεφει 1 αν ολα πηγαν καλα
επιστρεφει 0 αν υπηρξε προβλημα
*/
int show_deposits(FILE *fp, int *n)
{   
    int number_read;
    if(fp == NULL)
    {   
        printf("\nFirst open the file");
        return FALSE;
    }
    else
    {
        number_read = fread(n, sizeof(int), 1, fp);
        rewind(fp);
        if(number_read == 1)
            return TRUE;
        else
        {
            printf("\nERROR at reading the right amount of data from file");
            return FALSE;
        }
    }
}

int close_file(FILE *fp) //μπορω να το κανω και με συντομογραφια με ?
{
    if(!fclose(fp))
        return TRUE;
    else
        return FALSE;
}

int add_record(FILE *fp)
{
    if(fp==NULL)
        {   printf("First open the file"); 
            return FALSE;}

    struct record temp;
    int c1;
    int number_of_deposits;

    show_deposits(fp, &number_of_deposits);
    printf("Give a name: ");
    scanf("%s", temp.name);
    printf("Give an age: ");
    scanf("%d", &temp.age);

    // Μετακινω τον κερσορα μετα τον int n και τα υπολοιπα records
    c1 = fseek(fp, 4 + (sizeof(struct record)*number_of_deposits), SEEK_SET);
    if(c1==1)
    {
        printf("Malakia stin metakinisi kersora\n");
        return FALSE;
    }

    // προσθετω το record στο αρχειο
    c1 = fwrite(&temp, sizeof(struct record), 1, fp);
    if(c1==1)
    {   
        // αυξανω κατα ενα τον αριθμο των εισαγωγων
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

    check = show_deposits(fp, &number_of_deposits);
    
    if(fp==NULL)
    {   printf("\nFirst open the file"); 
        return FALSE;
    }
    else if( number_of_deposits == 0)
    {
        printf("\nFirst add records to file"); 
        return FALSE;    
    }
    else if(check ==0)
    {
        printf("\nError"); 
        return FALSE;        
    }
    
    do{
    printf("\nGive a number between (1 - %d): ", number_of_deposits);
    scanf("%d", &ch);
    
    if (ch<1 || ch>number_of_deposits)
        printf("\nyou gave a wrong number");

    }while(ch<1 || ch>number_of_deposits);

    // μετακινηση του κερσορα 
    c1 = fseek(fp, 4 + (sizeof(struct record)*(ch-1)), SEEK_SET);
    if(c1==1)
    {
        printf("Malakia stin metakinisi kersora\n");
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

    check=show_deposits(fp, &number_of_deposits);
    
    if(check ==0){
        printf("\nERROR");
        return FALSE;}

    for(i=1; i<=(number_of_deposits+1); i++)
    {
        if(i==1)
        {
            check = fread(&m, sizeof(int), 1, fp);
            if(check != 1) { printf("malakia!"); return FALSE;}
            printf("\n(number of deposits) n: %d", m);
            printf("\n=========================");
            
        }
        else
        {       
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

    check = show_deposits(fp, &number_of_deposits);
    
    if(fp==NULL)
    {   printf("\nFirst open the file"); 
        return FALSE;
    }
    else if( number_of_deposits == 0)
    {
        printf("\nFirst add records to file"); 
        return FALSE;    
    }
    else if(check ==0)
    {
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
    

    // μετακινηση του κερσορα 
    check = fseek(fp, 4 + (sizeof(struct record)*(ch-1)), SEEK_SET);
    if(check==1)
    {
        printf("\nMalakia stin metakinisi kersora\n");
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

    show_deposits(fp, &number_of_deposits);
    
    if(fp==NULL)
    {   printf("First open the file"); 
        return FALSE;
    }
    else if( number_of_deposits == 0)
    {
        printf("First add records to file"); 
        return FALSE;    
    }
    
    do{
        printf("\nGive a number between (1 - %d): ", number_of_deposits);
        scanf("%d", &ch);
        
        if (ch<1 || ch>number_of_deposits)
            printf("you gave a wrong number");
            
    }while(ch<1 || ch>number_of_deposits);

    // μετακινηση του κερσορα για το στην αρχη του τελευταιου στοιχειου 
    c1 = fseek(fp, 4 + (sizeof(struct record)*(number_of_deposits-1)), SEEK_SET);
    if(c1==1)
    {
        printf("Malakia stin metakinisi kersora\n");
        return FALSE;
    }

    // αποθηκευω το τελευταιο στοιχειο στο temp
    fread(&temp, sizeof(struct record), 1, fp);
    
    // επιστρεφω τον κερσορα στην αρχη και τον τοποθετω στην αρχη του στοιχειου που θελω να διαγραψω και γραφω πανω του το τελευταιο(δηλ. το temp) 
    rewind(fp);
    fseek(fp, 4 + (sizeof(struct record)*(ch-1)), SEEK_SET);
    fwrite(&temp, sizeof(struct record), 1, fp);

    // πρεπει να μειωσω το n στην αρχη του αρχειου
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

    check = show_deposits(fp, &number_of_deposits);
    


    if(fp==NULL)
    {   printf("\nFirst open the file"); 
        return FALSE;
    }
    else if( number_of_deposits == 0 || number_of_deposits==1 )
    {
        printf("\nFirst add at least 2 records to file"); 
        return FALSE;    
    }
    else if(check ==0)
    {
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

    // γραφω στo temp1 το στοιχειο νουμερο1

    fseek(fp, 4 + (sizeof(struct record)*(n1-1)), SEEK_SET);
    check = fread(&temp1, sizeof(struct record), 1, fp);
    if(check !=1)
    {
        printf("\nmalakia sto temp1");
        return FALSE;
    }
    rewind(fp);

    // γραφω στο temp2 το στοιχειο νουμερο n2

    fseek(fp, 4 + (sizeof(struct record)*(n2-1)), SEEK_SET);
    check = fread(&temp2, sizeof(struct record), 1, fp);
    if(check !=1)
    {
        printf("\nmalakia sto temp2");
        return FALSE;
    }
    rewind(fp);

    // γραφω στο νουμερο n1 το στοιχειο n2
    fseek(fp, 4 + (sizeof(struct record)*(n1-1)), SEEK_SET);
    check = fwrite(&temp2, sizeof(struct record), 1, fp);
    if(check !=1)
    {
        printf("\nmalakia sto grapsimo tou temp1 ");
        return FALSE;
    }
    rewind(fp);

    // γραφω στο νουμερο n1 το στοιχειο n2
    fseek(fp, 4 + (sizeof(struct record)*(n2-1)), SEEK_SET);
    check = fwrite(&temp1, sizeof(struct record), 1, fp);
    if(check !=1)
    {
        printf("\nmalakia sto grapsimo tou temp2 ");
        return FALSE;
    }
    rewind(fp);
    return TRUE;
    

}