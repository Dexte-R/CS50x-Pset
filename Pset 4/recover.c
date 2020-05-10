#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // check for exactly 1 command line argument
    if (argc != 2)
    {
        printf("Please exactly 1 argument, i.e. the name of the input file. \n");
        return 1;
    }
    // check if input file can be opened
    FILE* inptr = fopen(argv[1], "rb");
    if (inptr == NULL)
    {
        printf("File could not be opened \n");
        return 1;
    }
    // create buffer to store BYTES read from file
    BYTE buffer[512];
    // counter for jpeg files found in file
    int counter = 0;
    // create char array to store filename of jpeg file "###.jpg" (7 char + 1 char to terminate array of char)
    char fname[8];
    // create pointer to jpeg file
    FILE* outptr = NULL;
    // read entire file
    while (fread(buffer, sizeof(BYTE), 512, inptr) != 0)
    {
        // check for jpeg type file header
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (counter != 0)
            {
                // close current jpeg file
                fclose(outptr);
            }
            // create correct file name
            sprintf(fname, "%03i.jpg", counter);
            // create new jpeg file
            outptr = fopen(fname, "wb");
            fwrite(buffer, sizeof(BYTE), 512, outptr);
            counter++;
        }
        // write to existing jpeg file
        else
        {
            if (outptr != NULL)
                fwrite(buffer, sizeof(BYTE), 512, outptr);
        }
    }
    fclose(inptr);
}