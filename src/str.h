#ifndef _STR_H__
#define _STR_H__

#include "csvreader.h"

char **GET_HeadName(int * clms,int ** hSize,const char * str);
CSV_ROW * GET_Row(const char * str, int ** pty,int ** size, int clms);

int STRCountComma(const char * str);
int STRFirstComma(const char * str);

#endif // _STR_H__
