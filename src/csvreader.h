#ifndef _CSVREADER_H_
#define _CSVREADER_H_

#include <stdio.h>

#define EXPORT __declspec(dllexport)


//the value of the cell depending on it's data type
typedef union
{
    int i;
    double d;
    char * s;
} CELL_VAL;

//the cells
typedef struct
{
    int type;
    CELL_VAL val;
}CSV_CELL;

typedef struct csv_row
{
    CSV_CELL * cell;
    struct csv_row * next;
}CSV_ROW;

typedef struct
{
    char * filepath;    //the full path and name of the CSV file
    int columns;        //count of columns
    int rows;           //count of rows
    char ** head;       //the header name
    CSV_ROW * row;      //the row of the table

    int *headSize;      //the width of the column
    int * pre_type;     //if contain predeclared type {by default auto types}
    int typeException;  // boolean if columns allows exception of data type {if data type not auto}
    char * errorMSG;    //if the program have some errors

//defining later
    char * dateFormat;  // the format of the date (REGEX)
    char * timeFormat;  // the format of the time (REGEX)
} CSV_DATA;

/*
 * flags of data type that should be predicted
 */
#define TYPE_AUTO      0
#define TYPE_INT       1
#define TYPE_DOUBLE    2
#define TYPE_STRING    3
#define TYPE_DATE      4
#define TYPE_TIME      5



/*
 * CSV_Init()
 * initialize the data structure
 * returns the pointer of the data
 * returns NULL if fail
*/
EXPORT CSV_DATA * CSV_Init();

/*
 * CSV_SetName()
 * Set the path and the name of the CSV file
 * data is the pointer of the used CSV Data Object
 * fn is the file name and path
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_SetName(CSV_DATA * data, const char * fn);

/*
 * CSV_SetTypes()
 * Set the types of the CSV data columns
 * data is the pointer of the used CSV Data Object
 * numbers is the count pf the columns
 * and the rest are the integer flags of the types
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_SetTypes(CSV_DATA * data, int numbers,...);

/*
 * CSV_Read()
 * Start reading the data from the file after initialize all necessary data
 * data is the pointer of the used CSV Data Object
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_Read(CSV_DATA *data);

/*
 * CSV_Print()
 * print all rows of the file
 * data is the pointer of the used CSV Data Object
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_Print(CSV_DATA *data);

/*
 * CSV_PrintHead()
 * print the first 5 rows of the file
 * data is the pointer of the used CSV Data Object
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_PrintHead(CSV_DATA *data);

#endif // _CSVREADER_H_
