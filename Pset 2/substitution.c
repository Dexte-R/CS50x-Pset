#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("1 argument expected. \n");
        return 1;
    }
    // checks for cipher:
    // 1. contains 26 characters.
    // 2. All characters are alphabetical.
    // 3. Each alphabet appears once ONLY.
    int i = 0;
    int letters = 0;
    while (argv[1][i] != '\0') //loop till end of string.
    {
        if (isalpha(argv[1][i]))
        {
            letters++;
        }
        i++;
    }
    if (letters != 26) //check cond 1 and 2.
    {
        printf("Cipher must contain 26 letters. \n");
        return 1;
    }
    else //check cond 3
    {
        /*
        int unique = 0;
        for (int j = 'a'; j <= 'z'; j++)
        {
            unique += j; //sum of ascii code values from 'a' to 'z'
            //flawed logic: 'a' + 'z' = 219 = 'b' + 'c' => now I have 2 copies of 'b' & 'c'.
        }

        int sum = 0;
        for (int k = 0; k < letters; k++)
        {
            sum += tolower(argv[1][k]); //sum of ascii code values in cipher
        }

        if (sum != unique)
        {
            printf("Each alphabet must appear once. \n");
            return 1;
        }
        */
        int unique[26] = {0};
        for (int j = 0; j < strlen(argv[1]); j++)
        {
            int letter = tolower(argv[1][j]) - 97;
            unique[letter] = 1;
        }
        for (int k = 0; k < 26; k++)
        {
            if (unique[k] == 0)
            {
                printf("Each alphabet must appear once. \n");
                return 1;
            }
        }
    }
    // passed all 3 conditions, now we can convert plain text to cipher text.
    string text_in = get_string("plaintext:  ");
    char text_out[(int) strlen(text_in)]; //create array with same size to store ciphertext
    int p = 0;
    while(text_in[p] != '\0') //loop till end of input string
    {
        if (isupper(text_in[p]))
        {
            //get position of cipher corresponding to alphabet of text_out[p]
            int pos = text_in[p] - 65; 
            text_out[p] = toupper(argv[1][pos]);
        }
        else if (islower(text_in[p]))
        {
            int pos = text_in[p] - 'a'; 
            text_out[p] = tolower(argv[1][pos]);
        }
        else
        {
            text_out[p] = text_in[p];
        }
        p++;
    }
    printf("ciphertext: ");
    for (int m = 0; m < strlen(text_in); m++)
    {
        printf("%c", text_out[m]);
    }    
    printf("\n");
}