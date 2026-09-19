#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

// Structure for storing filenames
struct files = {
        char logFile[];
        char testFile[];
        char oneginFile[];
        char resultFile[];
    };

// A structure for storing information about a string
struct stringInfo = {
        size_t stringSize;
        char* ptrString;
    };

// Prototypes


int main(){

    // Set the file names
    struct files usedFiles;
    usedFiles.logFile = "log-files/log.txt";
    usedFiles.testFile = "used-files/test.txt";
    usedFiles.oneginFile = "used-files/onegin.txt";
    usedFiles.resultFile = "used-files/result.txt";

    // Variable to store the text of the entire poem
    char* text[];

    // Read the text from the file
    readText(usedFiles.oneginFile, text);

    bubbleSort(text, stringCount, &compareLeftToRight);

    writeText("result.txt", text, stringCount, "--------Part1. Bubble sorting.");

    return 0;
}

void writeTextToLogFileForDebugging(const char* fileName, char* text[], size_t length){
    /*
        Function: Writing text to a log-file for comfortable debugging
        Returns: void
    */

    FILE* file = fopen(fileName, "a");
    assert(file);

    // Write debug information to file
    fprintf(file, "%s\n", text);

    fclose(file);

    return;
}

void writeTextToFile(const char* fileName, char* text[], size_t length, const char* reason){
    /*
        Function: Writing text to a file
        Returns: void
    */

    FILE* file = fopen(fileName, "w");
    assert(file);

    // Write of the reason/title
    fprintf(file, "%s\n", reason);

    // Write text to a file line by line
    for (size_t i = 0; i < length; i++)
    {
        fprintf(file, "%s", text[i]);
    }

    fclose(file);

    return;
}

char* myStrdup(const char* str){
    /*
        Function: Allocate of a dynamic memory block
        Returns: (char*) the address of the allocated block in memory
    */
        void* memory = calloc(strlen(str) + 1, sizeof(char));
        assert(memory);

        strcpy((char*) memory, str);

        return (char*)memory;
}

void readTextToSingleBuffer(const char* fileName, char* text[]){
    /*
        Function: Read text from a file and write it to a single buffer
        Returns: void
    */

    FILE* file = fopen(fileName, "r");
    assert(file);

    // Get information about file
    struct stat fileInfo;
    stat(fileName, &fileInfo);
    size_t fileSize = fileInfo.st_size;

    // Allocate memory for recording
    myStrdup(text);

    // Read file and write information to text
    fread(text, fileSize, 1, file);
    text[fileSize] = '\0';

    fclose(file);
}

void swapLinesInfo(stringInfo* strInfo1, stringInfo* strInfo2){
    /*
        Function: Exchange of stringInfo-structures
        Returns: void
    */

    // Use temporary variable to swapping
    char* temp = *strInfo1;
    *strInfo1 = *strInfo2;
    *strInfo2 = temp;

    return;
}

int compareLeftToRight(const void* ptrLine1, const void* ptrLine2){
    /*
        Function: Compare strings without punctuation and spaces from left to right
        Returns: (int) =0 if the strings are equal
                       >0 if the first is greater than the second
                       <0 if the second is greater than the first
    */

    assert(ptrLine1);
    assert(ptrLine2);

    // Get string values
    const char* str1 = *(const char**)ptrLine1;
    const char* str2 = *(const char**)ptrLine2;

    assert(str1);
    assert(str2);

    // Indexes of string symbols
    size_t strInd1 = 0;
    size_t strInd2 = 0;

    // Search for not punctuation and spaces in strings
    while(!isalpha(str1[strInd1]) && str1[strInd1] != '\0') strInd1++;
    while(!isalpha(str2[strInd2]) && str2[strInd2] != '\0') strInd2++;

    while(str1[strInd1] != '\0' && str2[strInd2] != '\0')
    {
        if (str1[strInd1] != str2[strInd2]) return str1[strInd1] - str2[strInd2];
        strInd1++;
        strInd2++;
        // Search for next not punctuation and spaces in strings
        while(!isalpha(str1[strInd1]) && str1[strInd1] != '\0') strInd1++;
        while(!isalpha(str2[strInd2]) && str2[strInd2] != '\0') strInd2++;
    }

    if (str1[strInd1] == '\0' && str2[strInd2] == '\0') return 0;
    if (str1[strInd1] == '\0') return -1;
    return 1;

}

int compareRightToLeft(const void* ptrLine1, const void* ptrLine2){
    /*
        Function: Compare strings without punctuation and spaces from right to left
        Returns: (int) =0 if the strings are equal
                       >0 if the first is greater than the second
                       <0 if the second is greater than the first
    */

    assert(ptrLine1);
    assert(ptrLine2);

    // Get string values
    const char* str1 = *(const char**)ptrLine1;
    const char* str2 = *(const char**)ptrLine2;

    assert(str1);
    assert(str2);

    // Indexes of string symbols
    size_t strInd1 = 0;
    size_t strInd2 = 0;

    // Reach the end of the strins
    while(str1[strInd1] != '\0') strInd1++;
    while(str1[strInd2] != '\0') strInd2++;

    // Search for not punctuation and spaces in strings
    while(!isalpha(str1[strInd1]) && strInd1 >= 0) strInd1--;
    while(!isalpha(str2[strInd2]) && strInd2 >= 0) strInd2--;

    while(strInd1 >= 0 && strInd2 >= 0)
    {
        if (str1[strInd1] != str2[strInd2]) return str1[strInd1] - str2[strInd2];
        strInd1--;
        strInd2--;
        // Search for next not punctuation and spaces in strings
        while(!isalpha(str1[strInd1]) && strInd1 >= 0) strInd1--;
        while(!isalpha(str2[strInd2]) && strInd2 >= 0) strInd2--;
    }

    if (strInd1 < 0 && strInd2 < 0) return 0;
    if (strInd1 < 0) return -1;
    return 1;

}






