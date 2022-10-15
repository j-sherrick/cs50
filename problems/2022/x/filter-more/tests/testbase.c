#include <stdio.h>
#include <stdlib.h>

#include "../helpers.h"

int main(void)
{

    // Define input files
    char *infile = "../images/yard.bmp";
    char *outfile = "../images/yardblur.bmp";

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("File %s could not be opened.\n", infile);
        return 1;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 2;
    }

    printf("Input and output file successfully opened!\n");
    fclose(inptr);
    fclose(outptr);

    return 0;
}