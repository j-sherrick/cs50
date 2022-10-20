#include <stdio.h>
#include <stdlib.h>

#include "../helpers.h"

int main(void)
{

    // Define input files
    char *infile = "../images/yard.bmp";
    char *outfile = "../images/yardgray.bmp";

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

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    RGBTRIPLE (*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("You done fucked up son.\n");
        return 3;
    }

    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    for (int i = 0; i < height; i++)
    {
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);
        fseek(inptr, padding, SEEK_CUR);
    }

    grayscale(height, width, image);

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    for (int i = 0; i < height; i++)
    {
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);
        
        for (int j = 0; j < padding; j++)
            fputc(0x00, outptr);
    }

    free(image);

    fclose(inptr);
    fclose(outptr);

    return 0;
}