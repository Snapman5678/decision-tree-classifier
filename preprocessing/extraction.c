#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
Preprocessing module
-extraction.c - entrypoint
-hash_encoding.c - hash ebconding functions
*/
/*
Brief overview of extraction.c
This is the entry point to the preprocessing module.
All structure definitions are defined here.
All structures's memory allocation and deallocation functions are defined here.
This funcion reads the csv file and determines the column types simultaneously.
*/

/*
Structure that stores the csv data
*/

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

// Enum for encoded or not encoded
typedef enum {
    NOT_ENCODED = 0,
    ENCODED = 1,
} EncodingType;

// Structure that hold encoding data

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


// Initialize Column structure
void initColumn(Column *column, ColumnType type, int size) {
    switch (type) {
        case CATEGORICAL:
            column->data.strings = malloc(size * sizeof(char *));
            if (column->data.strings == NULL) {
                printf("Memory allocation failed for strings in column\n");
                exit(1);
            }
            for (int i = 0; i < size; i++) {
                column->data.strings[i] = NULL; // Initialize each string pointer to NULL
            }
            break;
        case INTEGER:
            column->data.integers = malloc(size * sizeof(int));
            if (column->data.integers == NULL) {
                printf("Memory allocation failed for integers in column\n");
                exit(1);
            }
            break;
        case FLOAT:
            column->data.floats = malloc(size * sizeof(float));
            if (column->data.floats == NULL) {
                printf("Memory allocation failed for floats in column\n");
                exit(1);
            }
            break;
    }
}

// Free memory allocated for Column structure
void freeColumn(Column *column, int type, int size) {
    switch (type) {
        case CATEGORICAL:
            for (int i = 0; i < size; i++) {
                free(column->data.strings[i]);
            }
            free(column->data.strings);
            break;
        case INTEGER:
            free(column->data.integers);
            break;
        case FLOAT:
            free(column->data.floats);
            break;
    }
}

// Fallback strsep implementation cause default strsep is not available in windows
char *strsep(char **stringp, const char *delim) {
    char *start = *stringp;
    char *p;

    if (!start)
        return NULL;

    p = strpbrk(start, delim);
    if (p) {
        *p = '\0';
        *stringp = p + 1;
    } else {
        *stringp = NULL;
    }

    return start;
}

// Function to read the CSV file and determine column types simultaneously
DataFrame read_csv_initial(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // Initialize the DataFrame
    DataFrame df;
    df.rows = 0;
    df.cols = 0;
    df.headings = NULL;
    df.columns = NULL;
    df.types = NULL; // Initialize the types array
    df.maxWidths = NULL;
    df.encoding = NULL;
    df.labelMap = NULL;

    // Read the file line by line
    char line[1024];

    // Read the column headings
    if (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character if present
        line[strcspn(line, "\n")] = '\0';
        char *rest = line;
        char *token = strsep(&rest, ",");
        int col = 0;

        // Allocate memory for headings and columns
        df.headings = (char **)malloc(1024 * sizeof(char *));
        if (df.headings == NULL) {
            printf("Memory allocation failed for heading column\n");
            exit(1);
        }
        df.columns = (Column *)malloc(1024 * sizeof(Column)); // Allocate initial memory
        if (df.columns == NULL) {
            printf("Memory allocation failed for columns\n");
            exit(1);
        }
        df.types = (ColumnType *)malloc(1024 * sizeof(ColumnType)); // Allocate memory for types
        if (df.types == NULL) {
            printf("Memory allocation failed for column types\n");
            exit(1);
        }

        // Initialize columns
        while (token) {
            df.headings[col] = (char *)malloc(strlen(token) + 1);
            if (df.headings[col] == NULL) {
                printf("String memory allocation failed for heading column in column: %d\n", col);
                exit(1);
            }
            strcpy(df.headings[col], token);
            
            // Initialize each column as categorical
            initColumn(&df.columns[col], CATEGORICAL, 0); // Initialize with 0 rows initially
            df.types[col] = CATEGORICAL; // Set type to categorical initially

            token = strsep(&rest, ",");
            col++;
        }
        df.cols = col;
    }

    // Set encoding type as zero for all columns
    df.encoding = (EncodingType *)malloc(df.cols * sizeof(EncodingType));
    for(int i=0;i<df.cols;i++) {
        df.encoding[i] = NOT_ENCODED;
    }

    // Set label map as NULL for all columns
    df.labelMap = (LabelHashMap **)malloc(df.cols * sizeof(LabelHashMap *));
    for(int i=0;i<df.cols;i++) {
        df.labelMap[i] = NULL;
    }

    // Read the data and determine column types simultaneously
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character if present
        line[strcspn(line, "\n")] = '\0';
        char *rest = line;
        char *token = strsep(&rest, ",");
        int col = 0;

        // Resize columns for new row
        for (int i = 0; i < df.cols; i++) {
            Column *column = &df.columns[i];
            ColumnType type = df.types[i];
            column->data.strings = realloc(column->data.strings, (df.rows + 1) * sizeof(char *));
            if (column->data.strings == NULL) {
                printf("Memory allocation failed for data column %d in row %d\n", i, df.rows);
                exit(1);
            }
            column->data.strings[df.rows] = (char *)malloc(strlen(token) + 1);
            if (column->data.strings[df.rows] == NULL) {
                printf("String memory allocation failed for data column %d in row %d\n", i, df.rows);
                exit(1);
            }

            // checking if the token is empty and assigning NULL if not empty assigning the token
            if(token[0] == '\0'){
                strcpy(column->data.strings[df.rows], "NULL");
            } else {
                strcpy(column->data.strings[df.rows], token);
            }

            // Check if the column type should be numerical
            if (type == CATEGORICAL) { // Only check if it's currently categorical
                int is_integer = 1;
                int is_float = 1;

                // Check for integer
                for (int k = 0; token[k] != '\0'; k++) {
                    if (!isdigit(token[k]) && token[k] != '-') { // Allow negative sign
                        is_integer = 0;
                        break;
                    }
                }

                // Check for float
                int decimal_point_seen = 0;
                if (is_integer) {
                    for (int k = 0; token[k] != '\0'; k++) {
                        if (token[k] == '.') {
                            decimal_point_seen = 1;
                            break;
                        }
                    }
                    if (decimal_point_seen) {
                        is_integer = 0;
                        is_float = 1;
                    }
                }

                if (!is_integer) {
                    // Check if it's a valid float
                    int has_digits_before = 0;
                    int has_digits_after = 0;
                    for (int k = 0; token[k] != '\0'; k++) {
                        if (token[k] == '.') {
                            if (has_digits_before == 0) {
                                is_float = 0;
                                break;
                            }
                            decimal_point_seen = 1;
                        } else if (isdigit(token[k])) {
                            if (!decimal_point_seen) {
                                has_digits_before = 1;
                            } else {
                                has_digits_after = 1;
                            }
                        } else {
                            is_float = 0;
                            break;
                        }
                    }
                    if (!has_digits_before || !has_digits_after) {
                        is_float = 0;
                    }
                }

                // Check if the token is empty
                if(token[0] == '\0') {
                    is_integer = 0;
                    is_float = 0;
                }

                // Assign type based on column content
                if (is_integer) {
                    df.types[i] = INTEGER;
                } else if (is_float) {
                    df.types[i] = FLOAT;
                } else {
                    df.types[i] = CATEGORICAL;
                }
            }

            token = strsep(&rest, ",");
            col++;
        }

        // Move to the next row
        df.rows++;
    }

    // Resize columns to final row count
    for (int i = 0; i < df.cols; i++) {
        Column *column = &df.columns[i];
        column->data.strings = realloc(column->data.strings, (df.rows) * sizeof(char *));
        if (column->data.strings == NULL) {
            printf("Memory allocation failed for finalizing column %d\n", i);
            exit(1);
        }
    }

    // Close the file
    fclose(file);
    printf("File read successfully\n");

    return df;
}

// Function to convert DataFrame columns to integers or floats depending on the type
void convert_to_int_float(DataFrame *df) {
    printf("Converting to integer and float\n");

    // Allocate a new array of Column to hold the converted data
    Column *new_columns = malloc(df->cols * sizeof(Column));
    if (new_columns == NULL) {
        printf("Memory allocation failed for new columns\n");
        exit(1);
    }

    // Iterate over each column
    for (int i = 0; i < df->cols; i++) {
        Column *old_column = &df->columns[i];
        Column *new_column = &new_columns[i];
        ColumnType type = df->types[i];

        // Initialize the new column with the correct type and size
        if (type == INTEGER) {
            initColumn(new_column, INTEGER, df->rows);
            for (int j = 0; j < df->rows; j++) {
                new_column->data.integers[j] = atoi(old_column->data.strings[j]);
            }
        } else if (type == FLOAT) {
            initColumn(new_column, FLOAT, df->rows);
            for (int j = 0; j < df->rows; j++) {
                new_column->data.floats[j] = atof(old_column->data.strings[j]);
            }
        } else {
            initColumn(new_column, CATEGORICAL, df->rows);
            for (int j = 0; j < df->rows; j++) {
                new_column->data.strings[j] = strdup(old_column->data.strings[j]);
                if (new_column->data.strings[j] == NULL) {
                    printf("Memory allocation failed for string copy\n");
                    exit(1);
                }
            }
        }
    }

    // Free old columns and update DataFrame
    for (int i = 0; i < df->cols; i++) {
        freeColumn(&df->columns[i], df->types[i], df->rows);
    }
    free(df->columns);
    df->columns = new_columns;
}



// Helper function to determine the width of a string
int getMaxWidthForString(char *str, int currentMax) {
    int len = strlen(str);
    return len > currentMax ? len : currentMax;
}

// Helper function to determine the width of an integer
int getMaxWidthForInteger(int value, int currentMax) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d", value);
    int len = strlen(buffer);
    return len > currentMax ? len : currentMax;
}

// Helper function to determine the width of a float
int getMaxWidthForFloat(float value, int currentMax) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.2f", value);
    int len = strlen(buffer);
    return len > currentMax ? len : currentMax;
}

int getMaxWidthForHeading(char *heading) {
    return strlen(heading);
}

// Function that takes max of 3 integers
int get_max_integer(int a, int b, int c) {
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}


// Function to compute maximum widths for columns
void computeMaxWidths(DataFrame *df) {
    df->maxWidths = malloc(df->cols * sizeof(int));
    if (df->maxWidths == NULL) {
        printf("Memory allocation failed for max widths\n");
        exit(1);
    }

    // Initialize maxWidths with 0
    for (int i = 0; i < df->cols; i++) {
        df->maxWidths[i] = 0;
    }

    // Compute the maximum width for each column based on its data
    for (int i = 0; i < df->cols; i++) {
        ColumnType type = df->types[i];
        int maxWidth = 0;
        if (type == CATEGORICAL) {
            for (int j = 0; j < df->rows; j++) {
                int width = getMaxWidthForString(df->columns[i].data.strings[j], 0);
                maxWidth = width > maxWidth ? width : maxWidth;
            }
        } else if (type == INTEGER) {
            for (int j = 0; j < df->rows; j++) {
                int width = getMaxWidthForInteger(df->columns[i].data.integers[j], 0);
                maxWidth = width > maxWidth ? width : maxWidth;
            }
        } else {
            for (int j = 0; j < df->rows; j++) {
                int width = getMaxWidthForFloat(df->columns[i].data.floats[j], 0);
                maxWidth = width > maxWidth ? width : maxWidth;
            }
        }

        // Update maxWidths with the maximum width found for data and heading
        df->maxWidths[i] = get_max_integer(
            df->maxWidths[i],
            getMaxWidthForHeading(df->headings[i]),
            maxWidth
        );

        // Include fixed width for column type descriptions
        int typeWidth = (type == INTEGER) ? INTEGER_WIDTH :
                        (type == FLOAT) ? FLOAT_WIDTH : CATEGORICAL_WIDTH;

        df->maxWidths[i] = get_max_integer(
            df->maxWidths[i],
            typeWidth,
            df->maxWidths[i]
        );
    }
}

void printDataFrame(DataFrame *df,int rows,int cols,int start) {
    // Print column headings
    printf("%-*s", df->maxWidths[0] + 2, "Index"); // Space for row index
    for (int j = 0; j < cols; j++) {
        printf("%-*s", df->maxWidths[j] + 2, df->headings[j]); // Add some padding
    }
    printf("\n");

    // Print column types
    printf("%-*s", df->maxWidths[0] + 2, "Integer"); // Space for index type
    for (int j = 0; j < cols; j++) {
        if (df->types[j] == INTEGER) {
            printf("%-*s", df->maxWidths[j] + 2, "Integer"); // Add some padding
        } else if (df->types[j] == FLOAT) {
            printf("%-*s", df->maxWidths[j] + 2, "Float"); // Add some padding
        } else {
            printf("%-*s", df->maxWidths[j] + 2, "Categorical"); // Add some padding
        }
    }
    printf("\n");

    // Print rows
    for (int i = start; i < rows; i++) {
        printf("%-*d", df->maxWidths[0] + 2, i + 1); // Print row number with padding
        for (int j = 0; j < cols; j++) {
            ColumnType type = df->types[j];
            EncodingType encoding = df->encoding[j];
            if (type == INTEGER || encoding == ENCODED) {
                printf("%-*d", df->maxWidths[j] + 2, df->columns[j].data.integers[i]);
            } else if (type == FLOAT) {
                printf("%-*.*f", df->maxWidths[j] + 2, 2, df->columns[j].data.floats[i]);
            } else {
                printf("%-*s", df->maxWidths[j] + 2, df->columns[j].data.strings[i]);
            }
        }
        printf("\n");
    }
}

//Function to print all the rows of the DataFrame
void printAll(DataFrame *df) {
    printDataFrame(df, df->rows, df->cols,0);
}

//Function to print head or 10 lines of the DataFrame
void printHead(DataFrame *df) {
    printDataFrame(df, 10, df->cols,0);
}

//Function to print tail or 10 lines of the DataFrame
void printTail(DataFrame *df) {
    printDataFrame(df, df->rows, df->cols,df->rows-10);
}

// Function to free Label memory allocated
void freeLabel(Label *label) {
    if (label->value != NULL) {
        free(label->value);
    }
}

// Function to free the memory allocated for the hash map
void freeLabelHashMap(LabelHashMap *labelMap) {
    for (int i = 0; i < labelMap->capacity; i++) {
        freeLabel(&labelMap->labels[i]);
    }

    free(labelMap->labels);
}


// Free memory allocated for DataFrame
void freeDataFrame(DataFrame *df) {
    for (int i = 0; i < df->cols; i++) {
        if(df->encoding[i] == ENCODED) {
            freeColumn(&df->columns[i], INTEGER, df->rows);
        } else {
        freeColumn(&df->columns[i], df->types[i], df->rows);
        }
    }
    free(df->columns);
    for (int i = 0; i < df->cols; i++) {
        free(df->headings[i]);
    }
    // Free the LabelHashMaps
    for (int i = 0; i < df->cols; i++) {
        if (df->labelMap[i] != NULL) {
            freeLabelHashMap(df->labelMap[i]);
            free(df->labelMap[i]);
        }
    }
    free(df->labelMap);
    free(df->headings);
    free(df->types);
    free(df->maxWidths);
}

DataFrame read_csv(char *filename) {
    DataFrame df = read_csv_initial(filename);
    convert_to_int_float(&df);
    computeMaxWidths(&df);
    return df;
}


/*
Make sure the csv file doesnt have missing values as the code does not handle missing values

Comment out this part if running in main
*/
/*
int main() {
    // Example usage
    DataFrame df = read_csv("../files/Salary_Data.csv");

    // Print the DataFrame with rows and columns
    printDataFrame(&df,df.rows,df.cols,0);

    // Print the head of the DataFrame
    printHead(&df);

    // Print the tail of the DataFrame
    printTail(&df);

    freeDataFrame(&df);
    return 0;
}
*/

