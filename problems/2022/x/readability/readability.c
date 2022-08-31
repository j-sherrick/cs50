#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Constants for common output strings
#define IN_TEXT "Text: "
#define OUT_TEXT "Grade "

// Counts the total number of alphabetic characters in a string of text.
int count_letters(string txt);

// Counts the total number of words in a string of text.
int count_words(string txt);

// Counts the total number of sentences in a string of text.
int count_sentences(string txt);

// Calculates the average number of letters per 100 words.
double average_letters(int letters, int words);

// Calculates the average number of sentences per 100 words.
double average_sentences(int sentences, int words);

// Prints the result per the specification.
void print_result(int index);

// The whole shebang. Calculates the Coleman-Liau index of a text and prints the result.
void readability(void);

int main(void)
{

    readability();

    return 0;
}

int count_letters(string txt)
{
    int count = 0;

    // txt is a pointer to a char array, which we can dereference to obtain the first character
    while (*txt != '\0')
    {
        // checks if the current char is a letter, then increments to the next char
        if (isalpha(*txt++))
        {
            ++count;
        }
    }

    return count;
}

int count_words(string txt)
{
    int count = 0;

    while (*txt != '\0')
    {
        // Start looping over whitespace until we hit something else
        // This will also increment the pointer for the outer loop since it always gets evaluated
        while (*txt++ == ' ')
        {
            // If the current character is a letter or punctiation i.e. quotation marks, add a word to the count
            // Not checking for punctuation will cause it to skip any word preceded by an opening quote
            if (isalpha(*txt) || ispunct(*txt))
            {
                ++count;
            }
        }
        // This looks for the end of the string and counts the very last word
        if (ispunct(*txt) && *(txt + 1) == '\0')
        {
            ++count;
        }
    }

    return count;
}

int count_sentences(string txt)
{
    int count = 0;

    while (*txt != '\0')
    {
        if (*txt == '.' || *txt == '!' || *txt == '?')
        {
            ++count;
        }
        ++txt;
    }

    return count;
}

double average_letters(int letters, int words)
{
    return (double)letters / words * 100;
}

double average_sentences(int sentences, int words)
{
    return (double)sentences / words * 100;
}

void readability(void)
{

    string txt = get_string("%s", IN_TEXT);

    int letters = count_letters(txt);
    int words = count_words(txt);
    int sentences = count_sentences(txt);

    double avg_letters = average_letters(letters, words);
    double avg_sentences = average_sentences(sentences, words);

    double index = (0.0588 * avg_letters) - (0.296 * avg_sentences) - 15.8;

    index = round(index);

    print_result((int)index);
}

void print_result(int index)
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("%s16+\n", OUT_TEXT);
    }
    else
    {
        printf("%s%d\n", OUT_TEXT, index);
    }
}