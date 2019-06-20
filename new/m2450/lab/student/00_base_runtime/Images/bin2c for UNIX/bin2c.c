/*
* The MIT License (MIT)
*
* Copyright (c) 2014-2015 Anthony Birkett
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
* Turns binaries into C char arrays.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32
    #include <ctype.h>
    
    char *_strupr(char *x)
    {
        char *y = x;
        while(*y)
        {
            *y = toupper(*y);
            y++;
        }
        return x;
    }
#endif

int main(int argc, char *argv[])
{
    FILE          *inputFile = NULL;
    FILE          *outputFile = NULL;
    void          *buffer;
    unsigned int  fileLen, i, j, major, minor;
    unsigned char *bufptr;

    if(argc != 4) {
        printf("%s %s %s\n", "Usage:", argv[0], "<binary> <destination file> <array name>");
        return 1;
    }

    inputFile = fopen(argv[1], "rb");
    if(inputFile == NULL)
    {
        printf("%s %s\n", "Unable to open input file:", argv[1]);
        return 1;
    }

    fseek(inputFile, 0, SEEK_END);
    fileLen = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    buffer = calloc(fileLen, 1);

    if (fread(buffer, 1, fileLen, inputFile) != fileLen)
    {
        fclose(inputFile);
        printf("%s %s\n", "Unable to read input file:", argv[1]);
        return 1;
    }

    fclose(inputFile);

    outputFile = fopen(argv[2], "wb");
    if(outputFile == NULL)
    {
        printf("%s %s\n", "Unable to open output file:", argv[2]);
        free(buffer);
        return 1;
    }

    _strupr(argv[3]);
    fprintf(outputFile, "#define %s%s %u\n\n\n", argv[3], "_SIZE", fileLen);
    fprintf(outputFile, "__attribute__((aligned(4)))\n");
    fprintf(outputFile, "unsigned char %s[] = {\n", argv[3]);

    major = fileLen / 16;
    minor = fileLen % 16;

    bufptr = buffer;

    for (i = 0; i < major; i++)
    {
        fprintf(outputFile, "%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
            bufptr[0], bufptr[1], bufptr[2], bufptr[3], bufptr[4], bufptr[5], bufptr[6], bufptr[7],
            bufptr[8], bufptr[9], bufptr[10], bufptr[11], bufptr[12], bufptr[13], bufptr[14], bufptr[15]);

        if ((i == major - 1) && !minor)
        {
            fprintf(outputFile, "\n");
        }
        else {
            fprintf(outputFile, ",\n");
        }

        bufptr += 16;
    }

    if (minor)
    {
        for (j = 0; j < minor - 1; j++)
        {
            fprintf(outputFile, "%u,", *bufptr++);
        }
        fprintf(outputFile, "%u\n", *bufptr);
    }

    fprintf(outputFile, "};\n");
    fclose(outputFile);
    free(buffer);

    return 0;
}
