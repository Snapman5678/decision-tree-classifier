#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
/*
Structure that stores the csv data
*/
typedef struct DataFrame{
    int rows;
    int cols;
    char ** headings;
    char ***data;
    int *type; // 0 for categorical and 1 for numerical 2 and 2 for float
} DataFrame;

// Function to read the CSV file and determine column types simultaneously
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
    df.data = NULL;
    df.headings = NULL;
    df.type = NULL;

    // Read the file line by line
    char line[1024];

    // Read the column headings
    if (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character if present
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, ",");
        int col = 0;

        // Allocate memory for headings and types
        df.headings = (char **)malloc(1024 * sizeof(char *));
        if (df.headings == NULL) {
            printf("Memory allocation failed for heading column\n");
            exit(1);
        }
        df.type = (int *)malloc(1024 * sizeof(int)); // Allocate initial memory
        if (df.type == NULL) {
            printf("Memory allocation failed for column types\n");
            exit(1);
        }

        // Initialize type to categorical
        for (int i = 0; i < 1024; i++) {
            df.type[i] = 0;
        }

        while (token) {
            df.headings[col] = (char *)malloc(strlen(token) + 1);
            if (df.headings[col] == NULL) {
                printf("String Memory allocation failed for heading column in column: %d\n", col);
                exit(1);
            }
            strcpy(df.headings[col], token);
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

        // Resize df.data for new row
        df.data = (char ***)realloc(df.data, (df.rows + 1) * sizeof(char **));
        if (df.data == NULL) {
            printf("Memory allocation failed for data row : %d\n", df.rows);
            exit(1);
        }
        df.data[df.rows] = (char **)malloc(df.cols * sizeof(char *));
        if (df.data[df.rows] == NULL) {
            printf("Memory allocation failed for data column %d in data row : %d\n", df.cols, df.rows);
            exit(1);
        }

        while (token) {
            df.data[df.rows][col] = (char *)malloc(strlen(token) + 1);
            if (df.data[df.rows][col] == NULL) {
                printf("String Memory allocation failed for data column %d in data row : %d\n", col, df.rows);
                exit(1);
            }
            strcpy(df.data[df.rows][col], token);

            // Check if the column type should be numerical
            if (df.type[col] == 0) { // Only check if it's currently categorical
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
                    df.type[col] = 1;
                } else if (is_float) {
                    df.type[col] = 2;
                } else {
                    df.type[col] = 0;
                }
            }

            token = strtok(NULL, ",");
            col++;
        }

        // Move to the next row
        df.rows++;
    }

    // Close the file
    fclose(file);

    return df;
}

// Function to convert Dataframe column to integer or float depending on the df.type



// Function to print the DataFrame with number of rows and columns
void print_df(DataFrame df,int rows,int cols){
    for(int i = 0; i < cols; i++){
        printf("%s ", df.headings[i]);
    }
    printf("\n");
    for (int i = 0; i < df.cols; i++) {
        printf("(%s) ",df.type[i] == 0 ? "Categorical" : df.type[i] == 1 ? "Integer" : "Float");
    }
    printf("\n");
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%s ", df.data[i][j]);
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
void free_df(DataFrame df){
    for(int i = 0; i < df.rows; i++){
        for(int j = 0; j < df.cols; j++){
            free(df.data[i][j]);
        }
        free(df.data[i]);
    }
    free(df.data);
    for(int i = 0; i < df.cols; i++){
        free(df.headings[i]);
    }
    free(df.headings);
    free(df.type);
}

/*
Main function for testing
Comment out when running main.c
*/

int main(){
    DataFrame df = read_csv("Salary_Data.csv");
    print_df(df,df.rows,df.cols);
    print_rows_cols(df);
    return 0;


}