#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

const uint32_t signature = (0xff << 24) | (0xd8 << 16) | (0xff << 8) | 0xe0;

int jpg_start(const uint8_t block[BLOCK_SIZE]);
 
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *raw_img = fopen(argv[1], "r");
    if(!raw_img)
    {
        printf("Card image could not be opened.\n");
        return 1;
    }
    
    uint8_t buffer[BLOCK_SIZE];
    char fname[25];
    int counter = 0;
    while (fread(buffer, 1, BLOCK_SIZE, raw_img) == BLOCK_SIZE)
    {
        if (jpg_start(buffer))
        {
            counter++;

            sprintf(fname, "%.3d.jpg\0", counter);
            FILE *out_jpg = fopen(fname, "w");
            if (!out_jpg)
            {
                printf("Unable to open output file.\n");
                return 1;
            }
            
            fwrite(buffer, 1, BLOCK_SIZE, out_jpg);
            // Leaves the file pointer past the start of next jpeg
            while (fread(buffer, 1, BLOCK_SIZE, raw_img) == BLOCK_SIZE && !jpg_start(buffer))
            {
                fwrite(buffer, 1, BLOCK_SIZE, out_jpg);
            }
            fclose(out_jpg);
            // Moves the file pointer back to start of next jpeg
            fseek(raw_img, -BLOCK_SIZE, SEEK_CUR);
        }
    }

    fclose(raw_img);
    return 0;
}

int jpg_start(const uint8_t block[BLOCK_SIZE])
{

    uint32_t block_start = (block[0] << 24) | (block[1] << 16) | (block[2] << 8) | (block[3] & 0xf0);

    return signature == block_start;
}