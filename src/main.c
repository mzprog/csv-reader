#include "csvreader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "str.h"
#include "dtypes.h"

/*
 * CSV_Init()
 * initialize the data structure
 * returns the pointer of the data
 * returns NULL if fail
*/
EXPORT CSV_DATA * CSV_Init()
{
    //allocate the variable
    CSV_DATA * dt= (CSV_DATA *)calloc(1,sizeof(CSV_DATA));
    if(dt==NULL)
    {
        return NULL;
    }

    return dt;
}

/*
 * CSV_SetName()
 * Set the path and the name of the CSV file
 * data is the pointer of the used CSV Data Object
 * fn is the file name and path
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_SetName(CSV_DATA * data, const char * fn)
{
    data->filepath=(char *)malloc(strlen(fn)+1);
    if(data->filepath==NULL)
    {
        return 0;
    }
    strcpy(data->filepath,fn);
    return 1;
}

/*
 * CSV_SetTypes()
 * Set the types of the CSV data columns
 * data is the pointer of the used CSV Data Object
 * numbers is the count pf the columns
 * and the rest are the integer flags of the types
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_SetTypes(CSV_DATA * data, int numbers,...)
{
    va_list valist;
    int i;
    int *tps=(int *)malloc(sizeof(int)*numbers);
    if(tps==NULL)
    {
        return 0;
    }
    //get all types
    for (i= 0; i < numbers; i++)
    {
        tps[i] = va_arg(valist, int);
    }
    /* clean memory reserved for valist */
    va_end(valist);

    //assign to the data
    if(data->pre_type)
    {
        free(data->pre_type);
    }
    data->pre_type=tps;
    data->columns=numbers;
    return 1;
}

/*
 * CSV_Read()
 * Start reading the data from the file after initialize all necessary data
 * data is the pointer of the used CSV Data Object
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_Read(CSV_DATA *data)
{
    char tmp[2048];
    CSV_ROW *row,*lastRow;
    CSV_ROW baitRow;

    if(data==NULL)
    {
        return 0;
    }
    FILE * fptr = fopen (data->filepath, "r");
    if(fptr==NULL)
    {
        return 0;
    }
    if(fgets(tmp,2048,fptr)==NULL)
        return 0;

    data->head=GET_HeadName(&data->columns,tmp);
    if(data->head==NULL)
    {
        return 0;
    }
//puts("1");
    //get the rows
   // baitRow.next=&data->row;
    lastRow=&baitRow;
    //allocate the head size with 0's
    if(data->headSize==NULL)
    {
        data->headSize=(int *) calloc(data->columns,sizeof(int));
        if(data->head==NULL)
        {
            return 0;
        }
    }


    while(fgets(tmp,2048,fptr))
    {
//puts("1.1");
        row=GET_Row(tmp,&data->pre_type,&data->headSize,data->columns);
//puts("1.2");
        if(row)
        {
            lastRow->next=row;
            lastRow=row;
        }
        else
        {
            return 0;
        }
    }
    data->row=baitRow.next;

    fclose(fptr);
    return 1;
}
/*
 * CSV_Print()
 * print all rows of the file
 * data is the pointer of the used CSV Data Object
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_Print(CSV_DATA *data)
{
    int i;
    int colC=data->columns;
    CSV_ROW *row;
    char tmp[256];//for sprintf

    if(data->columns<1)
        return 0;
    //head names
    for(i=0;i<data->columns;i++)
    {
        sprintf(tmp,"%%-%ds",data->headSize[i]);
        printf(tmp,data->head[i]);
    }
    puts("");//new line
    //first 5 rows
    row=data->row;
    while( row!=NULL)
    {
        for(i=0;i<colC;i++)
        {
            DATA_PrintCell(&row->cell[i],data->headSize[i]);//print the value the get a new tab

        }
        puts("");
        row=row->next;

    }

    return 1;
}
/*
 * CSV_PrintHead()
 * print the first 5 rows of the file
 * data is the pointer of the used CSV Data Object
 * Returns 1 if success
 * Returns 0 if fail
*/
EXPORT int CSV_PrintHead(CSV_DATA *data)
{
    int i,j;
    int colC=data->columns;
    char tmp[256];//for sprintf
    CSV_ROW *row;
    if(data->columns<1)
        return 0;
    //head names
    for(i=0;i<data->columns;i++)
    {
        sprintf(tmp,"%%-%ds",data->headSize[i]);
        printf(tmp,data->head[i]);
    }
    puts("");//new line
    //first 5 rows
    row=data->row;
    for(i=0;i<5 && row!=NULL;i++)
    {
        for(j=0;j<colC;j++)
        {
            DATA_PrintCell(&row->cell[j],data->headSize[j]);//print the value the get a new tab
            //printf("\t\t");
        }
        puts("");
        row=row->next;

    }

    return 1;
}
