#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
/*
Structure that stores the csv data
*/

// Define the Column structure
typedef struct Column {
    int type; // 0 for categorical, 1 for integer, 2 for float
    union {
        char **strings; // For categorical data
        int *integers;  // For integer data
        float *floats;  // For float data
    } data;
} Column;


typedef struct DataFrame{
    int rows;
    int cols;
    char ** headings; // Column headings
    Column *columns; // Array of columns of different types
} DataFrame;

// Initialize Column structure
void initColumn(Column *column, int type, int size) {
    column->type = type;
    switch (type) {
        case 0:
            column->data.strings = malloc(size * sizeof(char *));
            if (column->data.strings == NULL) {
                printf("Memory allocation failed for strings in column\n");
                exit(1);
            }
            for (int i = 0; i < size; i++) {
                column->data.strings[i] = NULL; // Initialize each string pointer to NULL
            }
            break;
        case 1:
            column->data.integers = malloc(size * sizeof(int));
            if (column->data.integers == NULL) {
                printf("Memory allocation failed for integers in column\n");
                exit(1);
            }
            break;
        case 2:
            column->data.floats = malloc(size * sizeof(float));
            if (column->data.floats == NULL) {
                printf("Memory allocation failed for floats in column\n");
                exit(1);
            }
            break;
    }
}

// Free memory allocated for Column structure
void freeColumn(Column *column,int size) {
    switch (column->type) {
        case 0:
            for (int i = 0; i < size; i++) {
                free(column->data.strings[i]);
            }
            free(column->data.strings);
            break;
        case 1:
            free(column->data.integers);
            break;
        case 2:
            free(column->data.floats);
            break;
    }
}


// Function to read the CSV file and determine column types simultaneously
// TODO : Need to fix the issue of null values in the data
DataFrame read_csv(char *filename) {
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

    // Read the file line by line
    char line[1024];

    // Read the column headings
    if (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character if present
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, ",");
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

        // Initialize columns
        while (token) {
            df.headings[col] = (char *)malloc(strlen(token) + 1);
            if (df.headings[col] == NULL) {
                printf("String Memory allocation failed for heading column in column: %d\n", col);
                exit(1);
            }
            strcpy(df.headings[col], token);
            
            // Initialize each column as categorical
            initColumn(&df.columns[col], 0, 0); // Initialize with 0 rows initially

            token = strtok(NULL, ",");
            col++;
        }
        df.cols = col;
    }

    // Read the data and determine column types simultaneously
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character if present
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, ",");
        int col = 0;

        // Resize columns for new row
        for (int i = 0; i < df.cols; i++) {
            Column *column = &df.columns[i];
            column->data.strings = realloc(column->data.strings, (df.rows + 1) * sizeof(char *));
            if (column->data.strings == NULL) {
                printf("Memory allocation failed for data column %d in row %d\n", i, df.rows);
                exit(1);
            }
            column->data.strings[df.rows] = (char *)malloc(strlen(token) + 1);
            if (column->data.strings[df.rows] == NULL) {
                printf("String Memory allocation failed for data column %d in row %d\n", i, df.rows);
                exit(1);
            }
            strcpy(column->data.strings[df.rows], token);

            // Check if the column type should be numerical
            if (column->type == 0) { // Only check if it's currently categorical
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

                // Assign type based on column content
                if (is_integer) {
                    column->type = 1;
                } else if (is_float) {
                    column->type = 2;
                } else {
                    column->type = 0;
                }
            }

            token = strtok(NULL, ",");
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

// TODO:Function to convert Dataframe column to integer or float depending on the df.column.type
void convert_to_int_float(DataFrame *df) {
    printf("Converting to integer and float\n");
    for (int i = 0; i < df->cols; i++) {
        Column *column = &df->columns[i];
        if (column->type == 1) { // Integer
            column->data.integers = malloc(df->rows * sizeof(int));
            if (column->data.integers == NULL) {
                printf("Memory allocation failed for integer data\n");
                exit(1);
            }
            for (int j = 0; j < df->rows; j++) {
                column->data.integers[j] = atoi(column->data.strings[j]);
                free(column->data.strings[j]); // Free the original string data
            }
            free(column->data.strings); // Free the string pointers
            column->data.strings = NULL; // Set to NULL to avoid dangling pointers
        } else if (column->type == 2) { // Float
            column->data.floats = malloc(df->rows * sizeof(float));
            if (column->data.floats == NULL) {
                printf("Memory allocation failed for float data\n");
                exit(1);
            }
            for (int j = 0; j < df->rows; j++) {
                column->data.floats[j] = atof(column->data.strings[j]);
                free(column->data.strings[j]); // Free the original string data
            }
            free(column->data.strings); // Free the string pointers
            column->data.strings = NULL; // Set to NULL to avoid dangling pointers
        }
    }
    printf("Conversion successful\n");
}



// Function to print the DataFrame with number of rows and columns
void print_df(DataFrame df, int rows, int cols){
    printf("Index ");
    for(int i = 0; i < cols; i++){
        printf("%s ", df.headings[i]);
    }
    printf("\n");
    for(int i = 0; i < rows; i++){
        printf("%d ", i+1);
        for(int j = 0; j < cols; j++){
            printf("%s ", df.columns[j].data.strings[i]);
        }
        printf("\n");
    }
}

// Function to print dataframe column names , rows and columns
void print_rows_cols(DataFrame df){
    for(int i = 0; i < df.cols; i++){
        printf("%s ", df.headings[i]);
    }
    printf("Rows: %d\n", df.rows);
    printf("Columns: %d\n", df.cols);
}

// Function to free the memory allocated for the DataFrame
void free_df(DataFrame df) {
    // Free each column's data
    printf("Freeing memory\n");
    for (int i = 0; i < df.cols; i++) {
        freeColumn(&df.columns[i], df.rows); // Free column data
    }

    // Free the columns array
    free(df.columns);

    // Free the headings array
    for (int i = 0; i < df.cols; i++) {
        free(df.headings[i]);
    }
    free(df.headings);
    printf("Memory freed\n");
}

/*
Main function for testing
Comment out when running main.c
*/

int main(){
    DataFrame df = read_csv("Salary_Data.csv");
    print_rows_cols(df);
    print_df(df, df.rows, df.cols);
    convert_to_int_float(&df);
    free_df(df);
    return 0;

}