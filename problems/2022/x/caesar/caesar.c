#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

char rotate(char c, int n);

int main(void)
{
    int key = 27;
    char plntxt = 'A';
    char rotated = rotate('A', key);
    printf("%c rotated by %d is %c\n", plntxt, key, rotated);

    return 0;
}

char rotate(char c, int n)
{
    if(isupper(c))
    {
        c = ((c - 'A' + n) % 26) + 'A';
        return c;
    }
    else
    {
        c = ((c - 'a' + n) % 26) + 'a';
        return c;
    }
}