#include <stdio.h>
#include "cs50.h"

/**
 * Reports whether a number is a syntactically valid credit card number.
 *  - Checks that it has a valid Luhn checksum
 *  - Checks that it has an appropriate number of digits
 */
int is_valid_cc(long cc);

/**
 * Reports what type of credit card a number belongs to.
 * INVALID if it is not a valid cc number.
 */
void print_cc_type(long cc);

int main(int argc, char *argv[])
{
    long usr_input = get_long("Number: ");

    if (is_valid_cc(usr_input))
        print_cc_type(usr_input);
    else
        print_cc_type(0);

    return 0;
}

int is_valid_cc(long cc)
{
    int even_sum = 0, odd_sum = 0, num_digits = 0;

    while (cc > 0)
    {
        // tmp stores each digit starting from right side of cc number
        int tmp = cc % 10;
        // If num_digits is even, current digit does not need to be doubled.
        if (num_digits % 2 == 0)
        {
            odd_sum += tmp;
        }
        else
        {
            // If num_digits is odd, double tmp
            tmp *= 2;
            // If tmp is two digits, add them both together
            if (tmp > 9)
                even_sum += (tmp % 10) + (tmp / 10);
            else
                even_sum += tmp;
        }

        cc /= 10;
        ++num_digits;
    }
    // Final result of the Luhn algorithm
    int checksum = (odd_sum + even_sum) % 10;
    // Check that the card number has a valid number of digits
    int is_valid_length = (num_digits == 13) || (num_digits == 15) || (num_digits == 16);
    // Return true if card number has valid checksum and has correct number of digits
    return (checksum == 0) && is_valid_length;
}

void print_cc_type(long cc)
{
    // Reduces the cc number to just the first two digits
    while (cc >= 100) cc /= 10;

    if (cc >= 40 && cc < 50)
        printf("VISA\n");
    else if (cc >= 51 && cc < 56)
        printf("MASTERCARD\n");
    else if (cc == 34 || cc == 37)
        printf("AMEX\n");
    else
        printf("INVALID\n");
}