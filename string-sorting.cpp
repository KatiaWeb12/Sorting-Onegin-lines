#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

const size_t MAX_PATH_LENGTH = 300;

// Structure for storing filenames
struct files {
        char path[MAX_PATH_LENGTH];
        char inputFile[MAX_PATH_LENGTH];
        char resultFile[MAX_PATH_LENGTH];
    };

// A structure for storing information about a string
struct stringInfo {
        size_t stringSize;
        char* stringPtr;
    };

// Status of element in partition
struct elStatus {
    size_t left;
    size_t right;
    };

struct codeInfo {
    size_t stringsCount;
    char* textOfEntirePoem;
    stringInfo* stringsInfoMassive;
};

// Prototypes
void deletingResultFile(const char* fileName);
void cancelBuffering();
void setFileNames(struct files* usedFiles, const size_t maxPathLength, int argc, char* argv[]);
void memoryDeallocation(char* text, stringInfo* stringsInfoMassive);
void cleanFile(const char* fileName);
void readText(files usedFiles, codeInfo* mainInfo);
void writeTextToFile(const char* fileName, char* text, size_t stringCount, const char* reason);
void writeStringsToFile(const char* fileName, stringInfo* stringsInfoMassive, size_t length, const char* reason);
char* createMemoryBlock(size_t fileSize);
void readTextIntoSingleBuffer(const char* fileName, char** text);
void recordPtrStrings(char* text, stringInfo stringsInfoMassive[], const size_t stringCount);
void calculateStringsSizes(stringInfo stringsInfoMassive[], size_t stringsCount);
size_t calculateStringsCount(const char* text);
void setStringsInfo(char* text, stringInfo stringsInfoMassive[], size_t stringsCount);
void swapLinesInfo(stringInfo* strInfo1, stringInfo* strInfo2);
int compareLeftToRight(const void* ptrLine1, const void* ptrLine2);
int compareRightToLeft(const void* ptrLine1, const void* ptrLine2);
elStatus partition(stringInfo* massive, size_t left,
                        size_t right, const size_t middle, int (*compare)(const void*, const void*));
void quickSort(stringInfo* massive, size_t length, const size_t leftEdge, const size_t rightEdge,
                            int (*compare)(const void*, const void*));

int main(int argc, char* argv[]){
    cancelBuffering();

    struct files usedFiles = {};
    setFileNames(&usedFiles, MAX_PATH_LENGTH, argc, argv);

    struct codeInfo mainInfo = {};
    mainInfo.textOfEntirePoem = NULL;
    deletingResultFile(usedFiles.resultFile);

    // Read the text from the file
    readText(usedFiles, &mainInfo);

    // Set information about the textOfEntirePoem
    mainInfo.stringsInfoMassive = (stringInfo*)calloc(mainInfo.stringsCount, sizeof(stringInfo));
    assert(mainInfo.stringsInfoMassive);

    setStringsInfo(mainInfo.textOfEntirePoem, mainInfo.stringsInfoMassive, mainInfo.stringsCount);

    // My QuickSort from left to right
    quickSort(mainInfo.stringsInfoMassive, mainInfo.stringsCount, 0, mainInfo.stringsCount-1, compareLeftToRight);
    writeStringsToFile(usedFiles.resultFile, mainInfo.stringsInfoMassive, mainInfo.stringsCount,
                       "------- Part 1. My QuickSort from left to right");

    // Standard QSORT from right to left
    qsort(mainInfo.stringsInfoMassive, mainInfo.stringsCount, sizeof(stringInfo), compareRightToLeft);
    writeStringsToFile(usedFiles.resultFile, mainInfo.stringsInfoMassive, mainInfo.stringsCount,
                      "------- Part 2. Standard qsort from right to left");

    // The original text of the poem
    writeTextToFile(usedFiles.resultFile, mainInfo.textOfEntirePoem, mainInfo.stringsCount,
                    "------- Part 3. The original text of the poem");

    memoryDeallocation(mainInfo.textOfEntirePoem, mainInfo.stringsInfoMassive);

    return 0;
}

void setFileNames(struct files* usedFiles, const size_t maxPathLength, int argc, char* argv[]){

    if(argc != 3){
        printf("Input-Error");
        printf("%s %u %s", "Type: path, input file, output file.\nPaths must be no longer than ",
                            maxPathLength, "characters.\n");
        exit(1);
    }

    const char* PATH = "./used-files/";

    strncpy(usedFiles->path, PATH, maxPathLength);
    usedFiles->path[maxPathLength - 1] = '\0';

    strncpy(usedFiles->inputFile, usedFiles->path, maxPathLength);
    usedFiles->inputFile[maxPathLength - 1] = '\0';
    strncat(usedFiles->inputFile, argv[1], maxPathLength);

    strncpy(usedFiles->resultFile, usedFiles->path, maxPathLength);
    usedFiles->resultFile[maxPathLength - 1] = '\0';
    strncat(usedFiles->resultFile, argv[2], maxPathLength);

    printf("Path:        [%s]\n", usedFiles->path);
    printf("Input file:  [%s]\n", usedFiles->inputFile);
    printf("Result file: [%s]\n", usedFiles->resultFile);

    return;
}


void deletingResultFile(const char* fileName){
    int codeOfDeleting = unlink(fileName);
    if(codeOfDeleting < 0){
        printf("File '%s' does not exists", fileName);
    }
    return;
}

void cancelBuffering(){

    // Abandoning standard buffering since a custom buffer already exists.
    setvbuf(stdout, NULL, _IONBF, 0);

    return;
}

void memoryDeallocation(char* text, stringInfo* stringsInfoMassive){
    free(stringsInfoMassive);
    free(text);
}

void cleanFile(const char* fileName){
    /*
        Function: Clean file
        Returns: void
    */
    FILE* file = fopen(fileName, "w");
    assert(file);
    fclose(file);
}

void writeTextToFile(const char* fileName, char* text, size_t stringCount, const char* reason){
    /*
        Function: Writing text to a file
        Returns: void
    */

    FILE* file = fopen(fileName, "a");
    assert(file);

    // Write of the reason/title
    fprintf(file, "%s\n\n", reason);

    /*
        Function: Record pointer about each line
        Returns: void
    */

    size_t stringIndex = 0;
    char* currentPtr = text;

    while(currentPtr != NULL && stringIndex < stringCount){
        fprintf(file, "%s\n", currentPtr);//TODO fwrite
        stringIndex++;

        // Get new string pointer
        char* newString = strchr(currentPtr, '\0');
        if(newString == NULL) break;

        currentPtr = newString + 1;
    }

    fclose(file);

    return;
}

void writeStringsToFile(const char* fileName, stringInfo* stringsInfoMassive, size_t length, const char* reason){
    /*
        Function: Writing strings to a file
        Returns: void
    */

    FILE* file = fopen(fileName, "a");
    assert(file);

    // Write of the reason/title
    fprintf(file, "%s\n", reason);

    // Write text to a file line by line
    for (size_t i = 0; i < length; i++)
    {
        fprintf(file, "%s\n", stringsInfoMassive[i].stringPtr);
    }

    fclose(file);

    return;
}

char* createMemoryBlock(size_t fileSize){
    /*
        Function: Allocate of a dynamic memory block
        Returns: (char*) the address of the allocated block in memory
    */
        char* memory = (char*)calloc(fileSize + 1, sizeof(char));
        assert(memory);

        return memory;
}

void readTextIntoSingleBuffer(const char* fileName, char** text){
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
    *text = createMemoryBlock(fileSize);

    // Read file and write information to text
    fread(*text, fileSize, 1, file);
    (*text)[fileSize] = '\0';

    fclose(file);
}

void readText(files usedFiles, codeInfo* mainInfo){

    readTextIntoSingleBuffer(usedFiles.inputFile, &(mainInfo->textOfEntirePoem));
    mainInfo->stringsCount = calculateStringsCount(mainInfo->textOfEntirePoem);

    return;
}

void recordPtrStrings(char* text, stringInfo stringsInfoMassive[], const size_t stringCount){
    /*
        Function: Record pointer about each line
        Returns: void
    */

    size_t stringIndex = 0;
    char* currentPtr = text;

    while(currentPtr != NULL && stringIndex < stringCount){
        // Record string pointer
        stringsInfoMassive[stringIndex].stringPtr = currentPtr;

        // Get new string pointer
        char* newString = strchr(currentPtr, '\n');
        if(newString == NULL) break;
        stringsInfoMassive[stringIndex].stringSize = newString - currentPtr;

        currentPtr = newString + 1;
        *newString = '\0';
        stringIndex++;
    }

    return;
}

size_t calculateStringsCount(const char* text){
    /*
        Function: calculate count of strings
        Returns: (size_t) count of strings
    */

    size_t stringCount = 0;
    const char* symbolPtr = text;
    // Calculate count of strings in buffer
    while(*symbolPtr){
        if(*symbolPtr == '\n'){
            stringCount++;
        }
        symbolPtr++;
    }

    return stringCount;
}

void calculateStringsSizes(stringInfo stringsInfoMassive[], size_t stringsCount){
    /*
        Function: calculate sizes of strings and record them into structure
        Returns: void
    */

    // Calculate sizes of the last string
    stringsInfoMassive[stringsCount-1].stringSize = strlen(stringsInfoMassive[stringsCount-1].stringPtr);

    // Calculate sizes of other strings using their pointers
    for(size_t i = 0; i < stringsCount-1; i++){
            stringsInfoMassive[i].stringSize = stringsInfoMassive[i+1].stringPtr - stringsInfoMassive[i].stringPtr - 1;
    }

    return;
}

void setStringsInfo(char* text, stringInfo stringsInfoMassive[], size_t stringsCount){
    /*
        Function: Establish information about the lines of the poem
        Returns: void
    */
    recordPtrStrings(text, stringsInfoMassive, stringsCount);
    //calculateStringsSizes(stringsInfoMassive, stringsCount);

    return;
}

void swapLinesInfo(stringInfo* strInfo1, stringInfo* strInfo2){
    /*
        Function: Exchange of stringInfo-structures
        Returns: void
    */

    // Use temporary variable to swapping
    stringInfo temp = *strInfo1;
    *strInfo1 = *strInfo2;
    *strInfo2 = temp;

    return;
}

int compareLeftToRight(const void* ptrStringInfo1, const void* ptrStringInfo2){
    /*
        Function: Compare strings without punctuation and spaces from left to right
        Returns: (int) =0 if the strings are equal
                       >0 if the first is greater than the second
                       <0 if the second is greater than the first
    */

    assert(ptrStringInfo1);
    assert(ptrStringInfo2);

    // Get string values
    const stringInfo* strInfo1 = (const stringInfo*)ptrStringInfo1;
    const stringInfo* strInfo2 = (const stringInfo*)ptrStringInfo2;

    assert(strInfo1);
    assert(strInfo2);

    const char* str1 = (*strInfo1).stringPtr;
    const char* str2 = (*strInfo2).stringPtr;

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

int compareRightToLeft(const void* ptrStringInfo1, const void* ptrStringInfo2){
    /*
        Function: Compare strings without punctuation and spaces from right to left
        Returns: (int) =0 if the strings are equal
                       >0 if the first is greater than the second
                       <0 if the second is greater than the first
    */

    assert(ptrStringInfo1);
    assert(ptrStringInfo2);

    // Get string values
    const stringInfo* strInfo1 = (const stringInfo*)ptrStringInfo1;
    const stringInfo* strInfo2 = (const stringInfo*)ptrStringInfo2;

    assert(strInfo1);
    assert(strInfo2);

    const char* str1 = (*strInfo1).stringPtr;
    const char* str2 = (*strInfo2).stringPtr;

    // Indexes of last string symbols
    int strInd1 = strInfo1->stringSize - 1;
    int strInd2 = strInfo2->stringSize - 1;

    // Search for not punctuation and spaces in strings
    while(!isalpha(str1[strInd1]) && strInd1 > 0) strInd1--;
    while(!isalpha(str2[strInd2]) && strInd2 > 0) strInd2--;

    while(strInd1 >= 0 && strInd2 >= 0)
    {
        if (str1[strInd1] != str2[strInd2]) return str1[strInd1] - str2[strInd2];
        strInd1--;
        strInd2--;
        // Search for next not punctuation and spaces in strings
        while(!isalpha(str1[strInd1]) && strInd1 > 0) strInd1--;
        while(!isalpha(str2[strInd2]) && strInd2 > 0) strInd2--;
    }

    if (strInd1 < 0 && strInd2 < 0) return 0;
    if (strInd1 < 0) return -1;
    return 1;

}

elStatus partition(stringInfo* massive, size_t left,
                        size_t right, const size_t middle, int (*compare)(const void*, const void*))
{
    /*
        Function: Dividing the array into two parts, where the left part is less than middleEl
                    and the right part is greater
        Returns: (structure el_status) structure: left and right elements
    */

    assert(massive);

    const stringInfo middleEl = massive[middle];

    while(left <= right){
        while(compare(&massive[left], &middleEl) < 0) left++;
        while(compare(&middleEl, &massive[right]) < 0) right--;

        if(left <= right){
            swapLinesInfo(&massive[left], &massive[right]);

            left++;
            if(right > 0) right--;
        }

    }
    return elStatus {.left = left, .right = right};

}

void quickSort(stringInfo* massive, size_t length, const size_t leftEdge, const size_t rightEdge,
                            int (*compare)(const void*, const void*)){
    /*
        Function: Quick sort
        Returns: void
    */
    assert(massive);

    if(length == 0){
        printf("%s", "An empty array has been passed");
        return;
    }

    size_t middle = (rightEdge + leftEdge) / 2;
    elStatus edge = partition(massive, leftEdge, rightEdge, middle, compare);

    if(edge.right > leftEdge) quickSort(massive, length, leftEdge, edge.right, compare);
    if(rightEdge > edge.left) quickSort(massive, length, edge.left, rightEdge, compare);

    return;
}






