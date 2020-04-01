#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

float count_words(string input);

int main(void)
{
    string text = get_string("Text: ");
    int index = round(count_words(text)); //get score and round to nearest integer
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}


// use limiting cases to check my boolean logic => check for "a","z","A","Z"
// use NOT logical operators to select what u do not want, instead of defining new bool
float count_words(string input)
{
    int letters = 0, words = 0, sentences = 0;
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        bool lower = input[i] >= 'a' && input[i] <= 'z', upper = input[i] >= 'A' && input[i] <= 'Z';
        bool not_lower = input[i + 1] < 'a' || input[i + 1] > 'z';
        bool not_upper = input[i + 1] < 'A' || input[i + 1] > 'Z';
        if (lower || upper) //check if "i" char is a letter
        {
            letters += 1; //check letters

            if (not_lower && not_upper) // check if "i+1" char is NOT letter
            {
                if (input[i + 1] != '-' && input[i + 1] != 39) //check for hyphenated and apostrophe words
                {
                    words += 1;
                }
                if (input[i + 1] == '.' || input[i + 1] == '!' || input[i + 1] == '?') //check for sentence
                {
                    sentences += 1;
                }
            }
        }
    }
    float L = letters * 100.0 / words, S = (float) sentences * 100.0 / (float) words;
    return 0.0588 * L - 0.296 * S - 15.8;
}
