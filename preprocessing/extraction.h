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

typedef enum {
    NOT_ENCODED = 0,
    ENCODED = 1,
} EncodingType;

// structure used to hold actual data compared to value which is being converted
typedef struct {
    char *value;
} Label;

// hash map used to store the actual data and the value which is being converted
typedef struct {
    Label *labels;
    int capacity;
    int unique_labels;
} LabelHashMap;

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
    EncodingType *encoding; // Array of encoding types for each column
    LabelHashMap **labelMap; // Array of label maps for each column if encoded
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
void printDataFrame(DataFrame *df, int rows, int cols,int start);
void printAll(DataFrame *df);
void printHead(DataFrame *df);
void printTail(DataFrame *df);
void freeDataFrame(DataFrame *df);
DataFrame read_csv(char *filename);


#endif // EXTRACTION_H
