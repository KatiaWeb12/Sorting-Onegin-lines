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
stringInfo = {
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

// // Функция: выделение блока динамической памяти под одну строку и возврат адреса новой строки
// char* myStrdup(const char* str){
//
//         void* extraMemory = calloc(strlen(str) + 1, sizeof(char));
//         assert(extraMemory);
//
//         strcpy((char*) extraMemory, str);
//
//         return (char*)extraMemory;
// }

// // Функция: считывание строк файла через несколько блоков памяти
// void readTextBySeveralPartsOfMemory(const char* fileName, char* text[], const size_t stringCount, const size_t stringLength){
//
//     FILE* file = fopen(fileName, "r");
//     assert(file);
//
//     char buffer[10000] = "";
//     size_t i = 0;
//
//     while (i < stringCount && fgets(buffer, sizeof(buffer), file) != NULL)
//     {
//         text[i] = myStrdup(buffer);
//         i++;
//     }
//
//     fclose(file);
// }


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
        Function: Allocation of a dynamic memory block
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

void swapPtrLines(char** value1, char** value2){
    /*
        Function: Exchange of string pointers
        Returns: void
    */
    char* temp = *value1;
    *value1 = *value2;
    *value2 = temp;

    return;
}

// Функция: Сравнение строк без знаков препинания и пробелов
int myStrcmp(const char* str1, const char* str2){

    assert(str1);
    assert(str2);

    size_t strInd1 = 0;
    size_t strInd2 = 0;

    while(!isalpha(str1[strInd1]) && str1[strInd1] != '\0') strInd1++;
    while(!isalpha(str2[strInd2]) && str2[strInd2] != '\0') strInd2++;

    while(str1[strInd1] != '\0' && str2[strInd2] != '\0')
    {
        if (str1[strInd1] != str2[strInd2]) return str1[strInd1] - str2[strInd2];
        strInd1++;
        strInd2++;
        while(!isalpha(str1[strInd1]) && str1[strInd1] != '\0') strInd1++;
        while(!isalpha(str2[strInd2]) && str2[strInd2] != '\0') strInd2++;
    }

    if (str1[strInd1] == '\0' && str2[strInd2] == '\0') return 0;
    if (str1[strInd1] == '\0') return -1;

    return 1;


}

// Функция: Определение порядка сортировки
int compareLeftToRight(const void* ptrLine1, const void* ptrLine2){

    const char* str1 = *(const char**)ptrLine1;
    const char* str2 = *(const char**)ptrLine2;

    return myStrcmp(str1, str2);

}

// Функция: Сортировка пузырьком
void bubbleSort(char* massive[], const size_t length, int (*compare)(const void*, const void*)){

    assert(massive);

    if(length == 0){
        printf("%s", "An empty array has been passed");
        return;
    }

    for(size_t nPass = 0; nPass < length; nPass++){
            for(size_t i = 0; i < length - nPass - 1; i++){
                if(compare(&massive[i], &massive[i+1]) > 0){
                    swapPtrLines(&massive[i], &massive[i+1]);
                }
            }
    }
}





