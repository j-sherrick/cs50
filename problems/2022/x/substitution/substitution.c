#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_ERROR "Key must contain 26 characters"
#define ARGS_ERROR "Usage: ./substitution key"
#define ALPH_ERROR "Key must only contain alphabetic characters"
#define DUPL_ERROR "Key must not contain any duplicate letters"

// Checks if the user has supplied a command line argument
void check_args(int args);

// Checks if a proposed key contains any non-alphabetic characters
int contains_nonalpha(string str);

// Checks if two letters are the same, ignoring case
int same_letter(char a, char b);

// Checks if a proposed key has any duplicate letters
int contains_duplicates(string str);

// Validates the proposed cipher key. Terminates program if key is invalid
void validate_key(string str);

// Gets the key index of a letter
int letter_index(char a);

// Encrypts plaintext using the provided cipher key
void encrypt(string txt, string key, char secret[]);




int main(int argc, char *argv[])
{
    // Check command line arguments
    check_args(argc);

    // Check key is valid
    validate_key(argv[1]);

    // Get text to encrypt from user
    string plaintext = get_string("plaintext: ");

    // Encrypt the text provided by the user
    char ciphertext[strlen(plaintext) + 1];
    encrypt(plaintext, ciphertext, argv[1]);

    // Print back the encrypted text
    printf("ciphertext: %s\n", ciphertext);
}



/**
 * Checks that the user has supplied exactly one command line argument. Prints the
 * corresponding error message and provides an example of appropriate usage, then
 * terminates the program.
 * 
 * @param args The number of command line arguments.
 */
void check_args(int args)
{
    if(args < 2)
    {
        printf("No argument provided.\n%s\n", ARGS_ERROR);
        exit(1);
    }
    else if(args > 2)
    {
        printf("Too many arguments provided.\n%s\n", ARGS_ERROR);
        exit(1);
    }
}

/**
 * Checks if a user supplied string contains any characters that are not letters.
 * 
 * @param str A user supplied string that may or may not contain only letters.
 * @return int true (1) if a non-alphabetic character was found.
 */
int contains_nonalpha(string str)
{
    while(*str != '\0')
    {
        if(!isalpha(*str))
        {
            return 1;
        }
        ++str;
    }
    return 0;
}

/**
 * Checks if a string contains any letters more than once. Assumes the string
 * has already been validated to only contain letters.
 * 
 * @param str A user supplied string known to only contain letters.
 * @return int true (1) if any letter occurs more than once.
 */
int contains_duplicates(string str)
{
    int n = strlen(str);

    for(int i = 0; i < n - 1; ++i)
    {
        for(int j = i + 1; j < n; ++j)
        {
            if(same_letter(str[i], str[j]))
            {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * Checks if two letters are the same, regardless of case.
 * 
 * @param a The first character to check.
 * @param b The second character to check.
 * @return 1 (true) if the letters are the same, 0 (false) if not.
 */
int same_letter(char a, char b)
{
    if(islower(a) && isupper(b))
    {
        return a == tolower(b);
    }
    else if(isupper(a) && islower(b))
    {
        return tolower(a) == b;
    }
    else
    {
        return a == b;
    }
}

/**
 * Performs a sequence of checks to validate that a user supplied string can be
 * used as a cipher key. Prints an error message and terminates the program if the
 * key is found to be invalid for any reason.
 * 
 * Key must conform to the following:
 *  - Must contain exactly 26 characters
 *  - Must contain only letters
 *  - Letters must occur only once
 * 
 * @param str a user supplied string to validate.
 */
void validate_key(string str)
{
    if(strlen(str) != 26)
    {
        printf("%s\n", SIZE_ERROR);
        exit(1);
    }
    if(contains_nonalpha(str))
    {
        printf("%s\n", ALPH_ERROR);
        exit(1);
    }
    if(contains_duplicates(str))
    {
        printf("%s\n", DUPL_ERROR);
        exit(1);
    }
}

/**
 * Gets the 0 based index of a letter, where Aa = 0, Bb = 1, and so on. Assumes the
 * char is actually a letter.
 * 
 * @param a An alphabetic character.
 * @return int The 0 based index of the character.
 */
int letter_index(char a)
{
    return 
        // If the letter is upper, subtract 'A', if not subtract 'a'
        isupper(a) ? (a - 'A') : (a - 'a');
}

/**
 * This is where the magic happens. Encrypts plaintext and produces ciphertext, respecting the 
 * original case of the plaintext.
 * 
 * @param txt The plaintext supplied by the user.
 * @param secret An empty char array same size as txt to hold the ciphertext.
 * @param key The encryption key
 */
void encrypt(string txt, string key, char secret[])
{
    int i = 0;
    while(*txt != '\0')
    {
        // If this char is a letter, perform substitution
        if(isalpha(*txt))
        {
            // Get the next letter from the cipher key
           char next_char = *(key + letter_index(*txt));
           if(isupper(*txt))
            {
                secret[i++] = toupper(next_char);
            }
            else
            {
                secret[i++] = tolower(next_char);
            }
        }
        else
        {
            // This is whitespace or punctuation, no need to substitute
            secret[i++] = *txt;
        }
        ++txt;
    }
    // Add the terminating null byte and POOF it's a string!!
    secret[i] = '\0';
}