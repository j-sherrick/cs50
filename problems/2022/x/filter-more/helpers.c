#include "helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


const int BOXSIZE = 9;

// Offsets for getting each neighbor in a 3x3 box around each pixel
const int XOFF[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
const int YOFF[] = {1, 1, 1, 0, 0, 0, -1, -1, -1};

// Flattened sobel operator
const int GX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
const int GY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

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
            gray_avg = lround((pixel->rgbtBlue + pixel->rgbtGreen + pixel->rgbtRed) / 3.0);
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
 
    int r_sum, g_sum, b_sum;

    int out_sum;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            r_sum = 0;
            g_sum = 0;
            b_sum = 0;
            out_sum = 0;
            // Loop over neighbors of current pixel
            for (int i = 0; i < BOXSIZE; i++)
            {
                // Add offset to current coordinates
                int nX = x + XOFF[i];
                int nY = y + YOFF[i];
                // Check offset coordinate is not out of bounds
                if (nX < 0 || nX >= width || nY < 0 || nY >= height)
                {
                    ++out_sum;
                    continue;
                }
                pixel = &image[nY][nX];
                r_sum += pixel->rgbtRed;
                g_sum += pixel->rgbtGreen;
                b_sum += pixel->rgbtBlue;
            }
            pixel = &imgblur[y][x];

            // Calculate averages
            int box_count = BOXSIZE - out_sum;
            int r_avg = lround((float)r_sum / box_count);
            int g_avg = lround((float)g_sum / box_count);
            int b_avg = lround((float)b_sum / box_count);
            // Write averages
            pixel->rgbtRed = r_avg;
            pixel->rgbtGreen = g_avg;
            pixel->rgbtBlue = b_avg;
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
    RGBTRIPLE img_edge[height][width];
    RGBTRIPLE *pixel = NULL;
    // Gx kernel sums
    int gx_rsum, gx_gsum, gx_bsum;
    // Gy kernel sums
    int gy_rsum, gy_gsum, gy_bsum;
    // Neighbor coordinates
    int nY, nX;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            gx_rsum = 0;
            gx_gsum = 0;
            gx_bsum = 0;

            gy_rsum = 0;
            gy_gsum = 0;
            gy_bsum = 0;
            // Get neighbors of current pixel
            for (int i = 0; i < BOXSIZE; ++i)
            {
                nY = y + YOFF[i];
                nX = x + XOFF[i];
                if (nX < 0 || nX >= width || nY < 0 || nY >= height)
                {
                    continue;
                }
                // Get a pointer to the unaltered image, and calculate Sobel sums
                pixel = &image[nY][nX];

                gx_rsum += pixel->rgbtRed * GX[i];
                gx_gsum += pixel->rgbtGreen * GX[i];
                gx_bsum += pixel->rgbtBlue * GX[i];
                
                gy_rsum += pixel->rgbtRed * GY[i];
                gy_gsum += pixel->rgbtGreen * GY[i];
                gy_bsum += pixel->rgbtBlue * GY[i];

            }

            // Write values to edge enhanced image
            pixel = &img_edge[y][x];
            int r_val = lround(sqrt(gx_rsum * gx_rsum + gy_rsum * gy_rsum));
            int g_val = lround(sqrt(gx_gsum * gx_gsum + gy_gsum * gy_gsum));
            int b_val = lround(sqrt(gx_bsum * gx_bsum + gy_bsum * gy_bsum));

            // Cap values at 255
            pixel->rgbtRed = r_val > 255 ? 255 : r_val;
            pixel->rgbtGreen = g_val > 255 ? 255: g_val;
            pixel->rgbtBlue = b_val > 255 ? 255: b_val;
        }
    }

    // Write filtered image over original
    for (int i = 0; i < height; i++)
    {
        memcpy(image[i], img_edge[i], width * sizeof(RGBTRIPLE));
    }
}