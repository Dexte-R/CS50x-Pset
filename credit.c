#include <cs50.h>
#include <stdio.h>

long power(int x, int n);

int main(void)
{
    long num = get_long("Number: ");
    int i = 1; // init length of num
    long digit;
    int even = 0; //even numbers that will be multiplied by 2 and whose digits will be summed
    int odd = 0; //will be summed up
    do //obtain length of num
    {
        digit = num % power(10, i);
        i++;
    }
    while (digit != num);
    int length = i - 1;

    int first; //first digit 
    int second; //second digit
    if (length == 13 || length == 15 || length == 16)
    {
        for (int j = 1; j <= length; j++)
        {
            digit = num % 10; //get last digit
            num /= 10; //floor division to get rid of last digit
            if (j == length)
            {
                first = digit;
            }
            if (j == length - 1)
            {
                second = digit;
            }
            if (j % 2 == 1) // select odd placement
            {
                odd += digit;
            }
            else
            {
                int product = digit * 2; //product range: [0*2, 9*2] = [0, 18]
                if (product >= 10)
                {
                    int a = 1 + product % 10;
                    even += a;
                }
                else
                {
                    even += product;
                }
            }
        }

        int total = even + odd;
        if (total % 10 == 0) //check validitiy of num
        {
            int code = first * 10 + second;
            if (code == 34 || code == 37) 
            {
                if (length == 15)
                {   
                    printf("AMEX\n");
                }
            }
            else if (code == 51 || code == 52 || code == 53 || code == 54 || code == 55) 
            {
                if (length == 16)
                {
                    printf("MASTERCARD\n");
                }
            }
            else if (first == 4)
            {
                if (length == 13 || length == 16)
                {   
                    printf("VISA\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
            
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

long power(int x, int n)
{
    long power = 1;
    for (int i = 0; i < n; i++)
    {
        power *= x;
    }
    return power;
}
