#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <direct.h>

// Structure for storing filenames
struct files {
        const char* testFile;
        const char* oneginFile;
        const char* resultFile;
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

// Prototypes
void cleanFile(const char* fileName);
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

int main(){
    // Cancel buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    // Set the file names
    struct files usedFiles = {};
    usedFiles.testFile = "used-files/test.txt";
    usedFiles.oneginFile = "used-files/onegin.txt";
    usedFiles.resultFile = "used-files/result.txt";

    // Variable to store the text of the entire poem
    char* text = NULL;

    // Clean resultFile
    cleanFile(usedFiles.resultFile);

    // Read the text from the file
    readTextIntoSingleBuffer(usedFiles.oneginFile, &text);
    size_t stringsCount = calculateStringsCount(text);

    // Set information about the text
    stringInfo* stringsInfoMassive = (stringInfo*)calloc(stringsCount, sizeof(stringInfo));
    assert(stringsInfoMassive);
    //printf("%u",stringsCount);
    setStringsInfo(text, stringsInfoMassive, stringsCount);

    // My QuickSort from left to right
    quickSort(stringsInfoMassive, stringsCount, 0, stringsCount-1, compareLeftToRight);
    writeStringsToFile(usedFiles.resultFile, stringsInfoMassive, stringsCount, "------- Part 1. My QuickSort from left to right");

    // Standard QSORT from right to left
    qsort(stringsInfoMassive, stringsCount, sizeof(stringInfo), compareRightToLeft);
    writeStringsToFile(usedFiles.resultFile, stringsInfoMassive, stringsCount, "------- Part 2. Standard qsort from right to left");

    // The original text of the poem
    writeTextToFile(usedFiles.resultFile, text, stringsCount, "------- Part 3. The original text of the poem");

    //Memory deallocation
    free(stringsInfoMassive);
    free(text);

    return 0;
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
    fprintf(file, "\n%s\n\n", reason);

    /*
        Function: Record pointer about each line
        Returns: void
    */

    size_t stringIndex = 0;
    char* currentPtr = text;

    while(currentPtr != NULL && stringIndex < stringCount){
        fprintf(file, "%s\n", currentPtr);;
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
    fprintf(file, "\n%s\n\n", reason);

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
        stringIndex++;

        // Get new string pointer
        char* newString = strchr(currentPtr, '\n');
        if(newString == NULL) break;

        currentPtr = newString + 1;
        *newString = '\0';
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
    calculateStringsSizes(stringsInfoMassive, stringsCount);

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

    // Indexes of string symbols
    int strInd1 = 0;
    int strInd2 = 0;

    // Reach the end of the strins
    while(str1[strInd1+1] != '\0') strInd1++;
    while(str2[strInd2+1] != '\0') strInd2++;

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






