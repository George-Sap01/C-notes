#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    char file_name[] = "random_lines.txt";
    char file_new[] = "new_random_lines.txt";  
    
    FILE *fp_old = fopen(file_name, "r");
    FILE *fp_new = fopen(file_new, "w");
    char c;
    int flag = 0; // int counter = 0;
    
    flag = 0;
    while((c = fgetc(fp_old)) != EOF)
    {
        if(c == '.')
        {
            fprintf(fp_new, "%c\n", c); //στο τελος του αρχειου, μετα την τελευταια . θα υπαρχει ενας χαρακτηρας αλλαγης γραμμης. 
            flag = 1;
        }
        else
        {
            if(flag == 0 && c == ' ') // εδω το κενο ειναι μεταξυ λεξεων.
            {
                fprintf(fp_new, "%c", c);
            }
            else if(flag == 1 && c == ' ') 
            {   
                flag = 0;     
                continue;
            }
            else if(c == '\n')
            {
                flag = 0;
                continue;
            }
            else 
            {    
                flag = 0;
                fprintf(fp_new, "%c", c);
            }
        }
    }
}
/* το flag υπαρχει γιατι οταν εχω: kaakm akmakm. oiewji θελω το κενο μετα την τελια να παραλειφθει και να μην ενσωματωθει στην επομενη προταση.*/



//  με το κατω γραφω και το length της καθε προτασης.
/*    while((c = fgetc(fp_old)) != EOF)
    {
        if(c == '.')
        {
            counter++;
            fprintf(fp_new, "%c\n%d\n", c, counter);
            counter = 0;
            flag = 1;
        }
        else
        {
            if(flag == 0 && c == ' ')
            {
                fprintf(fp_new, "%c", c);
                counter++;
            }
            else if( flag == 1 && c == ' ') 
            {   
                flag = 0;     
                continue;
            }
            else 
            {    
                fprintf(fp_new, "%c", c);
                counter++;
            }
        }
    }
*/