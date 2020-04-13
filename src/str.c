#include "str.h"

#include <string.h>
#include <stdlib.h>

#include "dtypes.h"

/*
 * get the string array of the header names
 */
char **GET_HeadName(int * clms,int ** hSize,const char * str)
{
    int size;
    int shift=0,i,n;
    char ** head=NULL;
    if(* clms==0)
    {
        * clms=STRCountComma(str) + 1;
    }

    size = *clms;
    head=(char **)malloc(sizeof(char*)*size);
    if(head==NULL)
    {
        return NULL;
    }

    * hSize = (int *) calloc(size,sizeof(int));
    if(hSize==NULL)
    {
        return NULL;
    }

    for(i=0;i<size;i++)
    {
        n=STRFirstComma(str+shift);
        head[i]=(char *)malloc(sizeof(char)*(n+1));

        strncpy(head[i],str+shift,n);
        head[i][n]='\0';//null terminate
        shift += (n+1);
        //set length of the name
        hSize[0][i]=n+4;
    }

    return head;

}

/*
 * get the row clmsof data
 */
CSV_ROW * GET_Row(const char * str, int ** pty,int ** size, int clms)
{
    int i,shift=0,n;
    CSV_ROW * row=NULL;
    char tmp[1024];

    if(*pty==NULL)
    {
        *pty= (int *) malloc(sizeof(int)*clms);
        if(*pty==NULL)
        {
            return NULL;
        }

        for(i=0;i<clms;i++)
        {
            pty[0][i]=TYPE_AUTO;
        }
    }

    if(clms!=STRCountComma(str)+1)//if row and columns not matched size
    {
        return NULL;
    }

    row=(CSV_ROW *)calloc(1,sizeof(CSV_ROW));
    if(row==NULL)
    {
        return NULL;
    }

    row->cell=(CSV_CELL *) malloc(sizeof(CSV_CELL)*clms);
    if(row->cell==NULL)
    {
        return NULL;
    }

    //get the values from the string
    for(i=0;i<clms;i++)
    {
        n=STRFirstComma(str+shift);
        strncpy(tmp,str+shift,n);
        tmp[n]='\0';//null terminate
        shift += (n+1);

        //check type
        if(!DATA_CheckType(tmp,&row->cell[i],pty[0][i],&size[0][i]))
        {
            return NULL;
        }
    }
    return row;
}

/*
 * count how many comma's in the string
 */
int STRCountComma(const char * str)
{
    int i,c=0,len=strlen(str);
    int quoteC=0,dQuoteC=0;
    for(i=0;i<len;i++)
    {
        //when we found a comma outside quotes
        if(str[i]==',' && quoteC%2==0 && dQuoteC%2==0)
            c++;
        else if(str[i]=='\'')
        {
            quoteC++;
        }
        else if(str[i]=='\"')
        {
            dQuoteC++;
        }
    }
    return c;
}
/*
 * get the first comma found in the string
 */
int STRFirstComma(const char * str)
{
    int i;
    int quoteC=0,dQuoteC=0;
    for(i=0;i<strlen(str);i++)
    {
        if((str[i]==',' || str[i]=='\n' || str[i]=='\r') && quoteC%2==0 && dQuoteC%2==0)//new lines treated the same as comma
            break;
        else if(str[i]=='\'')
        {
            quoteC++;
        }
        else if(str[i]=='\"')
        {
            dQuoteC++;
        }
    }
    return i;
}
