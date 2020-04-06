#include "dtypes.h"

#include <string.h>
#include <stdlib.h>


int DATA_IsInt(const char * str)
{
    int i=0;
    int len=strlen(str);

    //if found a sign skip it
    if(str[0]=='-' || str[0]=='+')
        i++;
    for(;i<len;i++)
    {
        if(str[i]<'0' || str[i]>'9')
        {
            return 0;
        }
    }
    return 1;
}


int DATA_IsDouble(const char * str)
{
    int i=0,dotC=0;
    int len=strlen(str);

    //if found a sign skip it
    if(str[0]=='-' || str[0]=='+')
        i++;
    for(;i<len;i++)
    {
        if(str[i]<'0' || str[i]>'9')
        {
            if(str[i]=='.' && dotC==0)
            {
                dotC++;
            }
            else
            {
                return 0;
            }
        }
    }
    return 1;
}


int DATA_CheckType(const char * str,CSV_CELL * cell,int type,int * size)
{
    int sizet;
    if(cell==NULL)
    {
        return 0;
    }
    switch(type)
    {
        case TYPE_INT:
            if(!DATA_IsInt(str))
            {
                return 0;
            }
            cell->type=type;
            cell->val.i=atoi(str);
            sizet=strlen(str);
            break;

        case TYPE_DOUBLE:
            if(!DATA_IsDouble(str))
            {
                return 0;
            }
            cell->type=type;
            cell->val.d=atof(str);
            sizet=strlen(str);
            break;


        case TYPE_DATE:
        case TYPE_TIME:
            /*
             * date and time data type are not ready yet
             * so we are switching it back to string
             */
        case TYPE_STRING:
            cell->type=type;
            cell->val.s = (char *) malloc(sizeof(char)*(strlen(str)+1));
            if(cell->val.s==NULL)
            {
                return 0;
            }
            strcpy(cell->val.s,str);
            sizet=strlen(str);
            break;

        case TYPE_AUTO://automatically get the type of the data
            if(DATA_IsInt(str))
            {
                cell->type=TYPE_INT;
                cell->val.i=atoi(str);
                sizet=strlen(str);
            }
            else if(DATA_IsDouble(str))
            {
                cell->type=TYPE_DOUBLE;
                cell->val.d=atof(str);
                sizet=strlen(str);
            }
            else
            {
                //just recognized as a string
                cell->type=TYPE_STRING;
                cell->val.s=(char *) malloc(sizeof(char)*(strlen(str)+1));
                if(cell->val.s==NULL)
                {
                    return 0;
                }
                strcpy(cell->val.s,unpackSTR(str));
                sizet=strlen(cell->val.s);
            }
    }
///puts("d1");
    if(sizet+4>*size)
    {
        *size=sizet+4;
    }
//puts("d2");
    return 1;
}


char * unpackSTR(const char * str)
{
    int i=0,j=0;
    int len=strlen(str);
    char * ret=(char *) calloc(len+1,sizeof(char));

    int bq=-5,bdq=-5;
    int isQ=0,isD=0;

    if(ret==NULL)
    {
        return ret;
    }
    if(str[0]=='\'')
    {
        i++;
        isQ=1;
    }
    else if(str[0]=='\"')
    {
        i++;
        isD=1;
    }

    for(;i<len;i++)
    {
        if(str[i]=='\'')
        {
            if(bq==i-1)
            {
                ret[j++]=str[i];
            }
            else if(i==len-1)
            {
                isQ=1-isQ;//maybe we should make it as an error
            }
            else
            {
                bq=i;
            }
        }
        if(str[i]=='\"')
        {
            if(bdq==i-1)
            {
                ret[j++]=str[i];
            }
            else if(i==len-1)
            {
                isD=1-isD;//maybe we should make it as an error
            }
            else
            {
                bdq=i;
            }
        }
        else
        {
            ret[j++]=str[i];
        }

    }
    if(isQ  || isD)
    {
        //error opened or no closing  quotes
        free(ret);
        return NULL;
    }
    return ret;
}

int DATA_PrintCell(CSV_CELL *cell,int size)
{
    char tmp[256];
    if(cell->type==TYPE_INT)
    {
        sprintf(tmp,"%%-%dd",size);
        printf(tmp,cell->val.i);
    }
    else if(cell->type==TYPE_DOUBLE)
    {
        sprintf(tmp,"%%-%dg",size);
        printf(tmp,cell->val.d);
    }
    else //if(cell->type==TYPE_STRING)
    {
        sprintf(tmp,"%%-%ds",size);
        printf(tmp,cell->val.s);
    }
    return 1;
}
