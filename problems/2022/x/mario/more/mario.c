#include <stdio.h>

int get_height();                // Gets a height of 1-8 from the user
void clear_input_buffer();       // Clears the input buffer if user enters a non-integer
void print_pyramid(int height);  // Prints the formatted pyramid

const int MAX_HEIGHT = 8;

int main(void)
{

   int height = get_height();
   print_pyramid(height);

   return 0;
}

int get_height()
{
   int height = 0;
   while(height <= 0 || height > MAX_HEIGHT)
   {
      printf("Height: ");
      scanf("%d", &height);
      clear_input_buffer();
   }
   return height;
}

void clear_input_buffer()
{
   char c = 0;
   // Loop over input buffer and consume chars until buffer is empty
   while ((c = getchar()) != '\n' && c != EOF);
}

void print_pyramid(int height)
{
   char bricks[height + 1];

   // Clear junk from array memory or we'll be printing gibberish
   for(int i = 0; i <= height; ++i)
   {
      bricks[i] = '\0';
   }

   for(int i = 0; i < height; ++i)
   {
      bricks[i] = '#';
      printf("%*s  %s\n", height, bricks, bricks);
   }
}