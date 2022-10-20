#include "helpers.h"
//#include "bmp.h"
#include <stdlib.h>

#define BOXSIZE 9

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE *pixel = NULL;
    BYTE gray_avg;
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
            r_sum = 0;
            g_sum = 0;
            b_sum = 0;
            for (int i = 0; i < BOXSIZE; i++)
            {
                int nX = x + xoff[i];
                int nY = y + yoff[i];
                if (nX < 0 || nX >= width || nY < 0 || nY >= height)
                {
                    continue;
                }
                pixel = &image[nY][nX];
                r_sum += pixel->rgbtRed;
                g_sum += pixel->rgbtGreen;
                b_sum += pixel->rgbtBlue;
            }
            pixel = &imgblur[y][x];
            pixel->rgbtRed = r_sum / BOXSIZE;
            pixel->rgbtGreen = g_sum / BOXSIZE;
            pixel->rgbtBlue = b_sum / BOXSIZE;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
