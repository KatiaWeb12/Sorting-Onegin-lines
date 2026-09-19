#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
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

// Prototypes


int main(){

    const size_t stringCount = 5323;
    const size_t stringLength = 10000;
    char* text[stringCount] = {};

    // Set the file names
    struct files usedFiles;
    usedFiles.logFile = "log-files/log.txt";
    usedFiles.testFile = "used-files/test.txt";
    usedFiles.oneginFile = "used-files/onegin.txt";
    usedFiles.resultFile = "used-files/result.txt";

    // Read the text from the file
    readText(usedFiles.oneginFile, text, stringCount, stringLength);

    //Копия массива с указателями на строки
    char* startText[stringCount];
    for (size_t i = 0; i < stringCount; i++)
    {
        startText[i] = text[i];
    }

    bubbleSort(text, stringCount, &compareLeftToRight);

    writeText("result.txt", text, stringCount, "--------Part1. Bubble sorting.");

    for (size_t i = 0; i < stringCount; i++)
    {
        free(text[i]);
    }

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

// Функция: выделение блока динамической памяти под одну строку и возврат адреса новой строки
char* myStrdup(const char* str){

        void* extraMemory = calloc(strlen(str) + 1, sizeof(char));
        assert(extraMemory);

        strcpy((char*) extraMemory, str);

        return (char*)extraMemory;
}

// Функция: считывание строк файла через несколько блоков памяти
void readTextBySeveralPartsOfMemory(const char* fileName, char* text[], const size_t stringCount, const size_t stringLength){

    FILE* file = fopen(fileName, "r");
    assert(file);

    char buffer[10000] = "";
    size_t i = 0;

    while (i < stringCount && fgets(buffer, sizeof(buffer), file) != NULL)
    {
        text[i] = myStrdup(buffer);
        i++;
    }

    fclose(file);
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

// Функция: Обмен указателей
void swapPtrLines(char** value1, char** value2){
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





