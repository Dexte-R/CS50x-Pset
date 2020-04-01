#include <cs50.h>
#include <stdio.h>

void hash(int n);
void space(int n);

int main(void)
{
    int height; //height of pyramid
    do //ask user for height and then keep asking if height is not within [1,8]
    {
        height = get_int("Height: \n"); //ask user for height
    }
    while (height < 1 || height > 8);

    int i;
    for (i = 0; i < height; i++)
    {
        space(height - (i + 1));
        hash(i + 1);
        printf("  ");
        hash(i + 1);   
        printf("\n") ;
    }
}

void hash(int n) //prints the hash that makes up the pyramid
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("#");
    }
}

void space(int n) //prints the white spaces, it's like an inverse pyramid
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf(" ");
    }
}
