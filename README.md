# Sorting the lines of A. S. Pushkin’s poem “Eugene Onegin"
## Author: Ekaterina Kraeva
### The code is written in the C language. It contains comments with documentation.

The program reads the text of “Eugene Onegin” from the file and writes it to a buffer (the text variable). Memory is dynamically allocated for the buffer using calloc. To make the strings in the buffer look standard and ensure they end with ‘\0’, ‘\n’ is replaced with ‘\0’. The information is written to an array of structures, which contains the length of each line and a pointer to the beginning of the line in the buffer. The length of each string is calculated as the difference between the pointers.

The following information is output to the resulting file:

### Part 1.
The lines of Eugene Onegin are sorted using the implemented quicksort algorithm. The lines are sorted from left to right, ignoring spaces and punctuation marks.

### Part 2.
The lines of Eugene Onegin are sorted using the standard quicksort algorithm. The lines are sorted from right to left, ignoring spaces and punctuation marks.

### Part 3.
The original version of Pushkin’s poem is displayed.

Standard input and output buffering has also been disabled. At the end of the program, the dynamically allocated memory is freed.


## Assembling

The code compiles using standard compilers
```bash
g++ string-sorting.cpp -o string-sorting.exe
```

Run via
```bash
.\string-sorting.exe onegin.txt result.txt
```

## Features that were added after the review:
1. Create a general structure for the main data +
2. Remove cleanFile and delete the file using unlink +
3. It would be better to break it down into functions: clearing dynamic memory, etc. +
4. Implement input of file names from the console +


## quickSort
My function of quick sorting massive of strings implemented recursively. It returns structure. It uses function "partition" for division elements for current middle element. The algorithm is standard.

## Example of work
Test-file
```bash
brebebeqrg

btwebrqe

afsagsa

kuylutlg

ykum,,h
```



