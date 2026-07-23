#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    char file_old[] = "random_lines.txt";
    char file_new[] = "new_lines.txt";  
    char c;
    FILE *fp_old = fopen(file_old, "r");
    FILE *fp_new = fopen(file_new, "w");
    int flag = 0; 
    // int counter = 0;
    int continuous_spaces = 0;

    while((c = fgetc(fp_old)) != EOF)
    {
        if(c == '.'){
            fprintf(fp_new, "%c\n", c);
            flag = 1; 
            //in the end of the file there will be a changing line character
        }
        else
        {
            if (c != ' ' && c != '\n' ){
                // we have a letter 
                continuous_spaces = 0; 
                flag = 0;
                fprintf(fp_new, "%c", c);
            }
            else{   
                if(flag == 0 && c == ' ' && continuous_spaces == 0)       // the space here is between words
                {
                    fprintf(fp_new, "%c", c);
                    continuous_spaces = 1;     // we added already one space
                }
                else if(flag == 1 && c == ' ' | c == '\n')  // the space here is after a dot   
                    continue;
            }
                
        }
    }
}

/* flag works when i have something like:
    kaakm akmakm. oiewji , i want the space after the dot to not be included in the next sentence 


//  using this i can include the length for each sentence 
/*    
    while((c = fgetc(fp_old)) != EOF)
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