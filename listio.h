#define _POSIX_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#define SUCCESS 1
#define FAILURE 0

struct dataString{
    char *string;
    struct dataString *next;
};

struct dataHeader {
    char *name;
    int length;
    struct dataString *next;
};

struct returnStruct {
	int value;
	struct dataHeader *header;
};

/*Used to build a struct dataHeader that the user will
be populating, and return it's address within the returnStruct */
struct returnStruct buildHeader();


/*Sets the name of the passed struct Dataheader with the passed name value*/
int setName(struct dataHeader *header, char *name);


/*returns the name of the passed struct dataHeader*/
char *getName(struct dataHeader *header);


/*returns the length of the total number of characters for all the strings
stored within the passed struct dataHeader*/
int getLength(struct dataHeader *header);


/*Adds the given string to the linked list structure of the given struct dataHeader*/
int addString(struct dataHeader *header, char *str);


/*prints the title, length and all the strings stored within the linked list structure
of the passed struct dataHeader to stdout*/
int printString(struct dataHeader *header);


/*Calls the functions necessary for preforming the various tasks in formatting the 
strings contained in the linked list structure of the passed struct dataHeader*/
int processStrings(struct dataHeader *header);


/*Helper function for processStrings, takes in a string and a character. Any repeating 
characters of the passed char dupe are removed. Used by processStrings to handle 
multiple spaces and tabs*/
void processDuplicates(char *str, char dupe);


/*Helper function for processStrings, handles the replacing of \r and \n characters
with the appriate break and paragraph HTML tags. Returns a pointer to the reformatted string*/
char *processCRNLs(char *str);


/*Frees the struct dataHeader as well as all the struct dataString and their contained string
values all stored within a linked structure*/
int freeStructure(struct dataHeader *header);


/*Recursively goes thrugh the linked list structure to free all of the struct dataStrings from
tail to head.*/
void freeString(struct dataString *string);


/*writes out the struct dataHeader and its linked list content in the required format to a 
binary file of the name passed by the user*/
int writeStrings(char *filename, struct dataHeader *header);


/*returns a pointer to a struct dataHeader populated by reading a binary file of name given
by the user.*/
struct returnStruct readStrings(char *filename);


/*returns an int of the passed string length including the null terminator character*/
int trulen(char *str);


/*updates the length of the dataHeader, is used by processStrings after formatting changes
the length of the stored string values*/
void updateLength (struct dataHeader *header);


/*Takes the string segment(substring) defined by the passed int variables for
two different strings and returns the value of the strcmp() run on the segments*/
int strcmpseg (char*str1, int s1, int e1, char*str2, int s2, int e2);


/*returns a string, which is a segment of the passed string src from the passed character
locations ints start and end. used by processCRNLs when placing tags in the middle of a
string.*/
char *strcpyseg (char*src, int start, int end);


/*Returns a boolean value of true if the the passed char it a space, tab, newline or 
carriage return character, otherwise returns false*/
bool isWhitespace (char c);


/*returns the amount fo strings stored withing the struct dataHeaders linked list
structure*/
int countStrings(struct dataHeader *header);


/*returns a struct returnStruct setting its values to their failure specification
primarily used in order to avoid typing repeated code*/
struct returnStruct returnFail(struct returnStruct rtrn);


/*Closes the open file before returning returnFail to return the struct returnStruct
primarily used in order to avoid typing repeated code*/
struct returnStruct returnFileFail(struct returnStruct rtrn, FILE *fp);


/*Frees the reading char* before returning returnFileFail to handle the open file
primarily used in order to avoid typing repeated code*/
struct returnStruct returnReadFail(struct returnStruct rtrn, FILE *fp, char *wp);
