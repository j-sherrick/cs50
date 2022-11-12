#include "helpers.h"
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
    RGBTRIPLE swap_pixel;
    for (int row = 0; row < height; ++row)
    {
        for (int left = 0, right = width - 1; left < right; ++left, --right)
        {
            swap_pixel = image[row][left];
            image[row][left] = image[row][right];
            image[row][right] = swap_pixel;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // New pixel array for storing blurred values
    RGBTRIPLE imgblur[height][width];
    // Pointer to use for operations on current pixel
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
            // Loop over neighbors of current pixel
            for (int i = 0; i < BOXSIZE; i++)
            {
                // Add offset to current coordinates
                int nX = x + xoff[i];
                int nY = y + yoff[i];
                // Check offset coordinate is not out of bounds
                if (nX < 0 || nX >= width || nY < 0 || nY >= height)
                {
                    continue;
                }
                // Get pointer to neighbor
                pixel = &image[nY][nX];
                // Add current RGB values to sum
                r_sum += pixel->rgbtRed;
                g_sum += pixel->rgbtGreen;
                b_sum += pixel->rgbtBlue;
            }
            // Move pixel pointer to blurred image array
            pixel = &imgblur[y][x];
            // Calculate averages
            pixel->rgbtRed = r_sum / BOXSIZE;
            pixel->rgbtGreen = g_sum / BOXSIZE;
            pixel->rgbtBlue = b_sum / BOXSIZE;
        }
    }
    
    // Copy blurred image over to original
    for (int i = 0; i < height; i++)
    {
        memcpy(image[i], imgblur[i], width * sizeof(RGBTRIPLE));
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
