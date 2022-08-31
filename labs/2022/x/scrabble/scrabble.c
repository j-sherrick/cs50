#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int     compute_score(string word);
void    print_winner(int score1, int score2);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    print_winner(score1, score2);

    return 0;
}

int compute_score(string word)
{
    int score = 0;

    // We can just dereference the string pointer to work with individual chars in the array
    while (*word != '\0')
    {
        if (islower(*word))
        {
            // Gets the point value for the correct index in the array
            score += POINTS[*word - 'a'];
        }
        if (isupper(*word))
        {
            // Gets the point value for the correct index in the array
            score += POINTS[*word - 'A'];
        }
        // Increments string pointer to the next character
        ++word;
    }
    // Be polite and put the string pointer back where we found it.
    word -= (sizeof(word - 1));

    return score;
}

void print_winner(int score1, int score2)
{
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }

}