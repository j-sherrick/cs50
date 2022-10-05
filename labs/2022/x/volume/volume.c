// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to buffer
    uint8_t head_buffer[HEADER_SIZE];
    size_t header_code = fread(head_buffer, sizeof(uint8_t), HEADER_SIZE, input);
    if (header_code != HEADER_SIZE)
    {
        printf("Could not read header.\n");
        return 1;
    } 
    // Write header to output file
    fwrite(head_buffer, sizeof(uint8_t), HEADER_SIZE, output);

    // Get number of 16-bit audio samples in audio data portion
    size_t ad_size = ((head_buffer[43] << 24) | (head_buffer[42] << 16) | (head_buffer[41] << 8) | (head_buffer[40]));
    ad_size /= 2;

    // Copy audio data to buffer
    int16_t audio_buffer[ad_size];
    size_t buffer_read = fread(audio_buffer, sizeof(int16_t), ad_size, input);
    if (buffer_read != ad_size)
    {
        printf("Could not read audio portion.\n");
        return 1;
    }

    // Multiply audio samples by volume factor
    for (int i = 0; i < ad_size; i++)
    {
        audio_buffer[i] *= factor;
    }
    // Write volume adjusted samples to output file
    size_t w_02 = fwrite(audio_buffer, sizeof(int16_t), ad_size, output);

    // Close files
    fclose(input);
    fclose(output);
}
