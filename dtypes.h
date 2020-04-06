#ifndef _DTYPES_H_
#define _DTYPES_H_

#include "csvreader.h"
int DATA_IsInt(const char * str);
int DATA_IsDouble(const char * str);
int DATA_CheckType(const char * str,CSV_CELL * cell,int type,int * size);
char * unpackSTR(const char * str);

int DATA_PrintCell(CSV_CELL *cell,int size);

#endif // _DTYPES_H_
