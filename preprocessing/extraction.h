#ifndef EXTRACTION_H
#define EXTRACTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define fixed widths for column types
#define INTEGER_WIDTH 7
#define FLOAT_WIDTH 7
#define CATEGORICAL_WIDTH 12

// Enum for column types
typedef enum {
    CATEGORICAL = 0,
    INTEGER = 1,
    FLOAT = 2
} ColumnType;

// Define the Column structure
typedef struct Column {
    union {
        char **strings; // For categorical data
        int *integers;  // For integer data
        float *floats;  // For float data
    } data;
} Column;

// Define the DataFrame structure
typedef struct DataFrame {
    int rows;
    int cols;
    char **headings; // Column headings
    Column *columns; // Array of columns of different types
    ColumnType *types; // Array of column types
    int *maxWidths; // Array of maximum widths for each column
} DataFrame;

// Function prototypes
void initColumn(Column *column, ColumnType type, int size);
void freeColumn(Column *column, int type, int size);
DataFrame read_csv_initial(char *filename);
void convert_to_int_float(DataFrame *df);
int getMaxWidthForString(char *str, int currentMax);
int getMaxWidthForInteger(int value, int currentMax);
int getMaxWidthForFloat(float value, int currentMax);
int getMaxWidthForHeading(char *heading);
int get_max_integer(int a, int b, int c);
void computeMaxWidths(DataFrame *df);
void printDataFrame(DataFrame *df, int rows, int cols);
void freeDataFrame(DataFrame *df);
DataFrame read_csv(char *filename);

#endif // EXTRACTION_H
