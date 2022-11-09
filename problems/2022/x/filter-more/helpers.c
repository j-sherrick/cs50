#include "helpers.h"
//#include "bmp.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BOXSIZE 9

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE *pixel = NULL;
    WORD gray_avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel = &image[i][j];
            gray_avg = (pixel->rgbtBlue + pixel->rgbtGreen + pixel->rgbtRed) / 3;
            pixel->rgbtBlue = gray_avg;
            pixel->rgbtGreen = gray_avg;
            pixel->rgbtRed = gray_avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    printf("Image has height=%d width=%d\n", height, width);
    RGBTRIPLE imgblur[height][width];
    RGBTRIPLE *pixel = NULL;

    // Offsets to use in obtaining neighbors of each pixel
    int xoff[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    int yoff[] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
 
    WORD r_sum, g_sum, b_sum;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //r_sum = 0;
            g_sum = 0;
            b_sum = 0;
            for (int i = 0; i < BOXSIZE; i++)
            {
                int nX = x + xoff[i];
                int nY = y + yoff[i];
                if (nX < 0 || nX >= width || nY < 0 || nY >= height)
                {
                    //printf("Neighbor offset out of bounds:\nnX=%d nY=%d, h=%d w=%d\n", nX, nY, height, width);
                    continue;
                }
                pixel = &image[nY][nX];
                //r_sum += pixel->rgbtRed;
                g_sum += pixel->rgbtGreen;
                b_sum += pixel->rgbtBlue;
            }
            pixel = &imgblur[y][x];
            pixel->rgbtRed = 250;
            pixel->rgbtGreen = g_sum / BOXSIZE;
            pixel->rgbtBlue = b_sum / BOXSIZE;
            if (x == width - 1) printf("wrote the last pixel at x=%d y=%d\n", x, y);
        }
    }
    FILE *out_ptr = fopen("./images/blur_dbg.bmp", "w");
    if(!out_ptr)
    {
        printf("Oops! Couldn't open output file.\n");
        return;
    }
    
    // Copy blurred image over to original
    for (int i = 0; i < height; i++)
    {
        memcpy(image[i], imgblur[i], width);
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
