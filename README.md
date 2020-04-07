# csv-reader
a c library to read csv file

## how it's work

1. Initialize the struct `CSV_Init()`
2. Set the name of the targeted file `CSV_SetName(csv_struct,"mydata/Book1.csv")`
3. Read the data `CSV_Read(csv_struct)`

## next Targerted features

1. exception types (if columns error or different type)
2. print with ranges
3. get 2D integer array from CSV (specify columns)
4. get 2D double array from CSV (specify columns)
5. get struct array from CSV (specify columns)
6. set and get error function
7. add test code files
8. add documentation
