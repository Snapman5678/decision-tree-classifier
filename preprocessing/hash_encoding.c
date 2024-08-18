#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "extraction.h"


// Function to initialize Label
void initLabel(Label *label) {
    label->value = NULL;
}

// function to initialize the hash map
void initLabelHashMap(LabelHashMap *labelMap, int max_rows) {
    labelMap->capacity = max_rows;
    labelMap->unique_labels = 0;
    labelMap->labels = malloc(max_rows * sizeof(Label));

    // Initialize each label with default values
    for (int i = 0; i < max_rows; i++) {
        initLabel(&labelMap->labels[i]);
    }
}

//  hash function using djb2 algorithm
unsigned long hashLabel(char *label, int capacity) {
    unsigned long hash = 5381;
    int c;

    while ((c = *label++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash % capacity;
}

// Second hash function for double hashing
unsigned long hashLabel2(char *label, int capacity) {
    unsigned long hash = 0;
    int c;

    while ((c = *label++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return (hash % (capacity - 1)) + 1;
}

// Function to insert a label into the hash map
void insertLabel(LabelHashMap *labelMap, char *label) {
    unsigned long index = hashLabel(label, labelMap->capacity);
    unsigned long stepSize = hashLabel2(label, labelMap->capacity);

    // Double hashing to find an empty slot or a duplicate
    while (labelMap->labels[index].value != NULL) {
        if (strcmp(labelMap->labels[index].value, label) == 0) {
            // If the same label is found, ignore the insertion
            return;
        }
        // Move to the next slot with a step size determined by the second hash
        index = (index + stepSize) % labelMap->capacity;
    }

    // Insert the label if it's not already present
    labelMap->labels[index].value = strdup(label);  // Allocate memory for the label
    labelMap->unique_labels++;
}

// Function to get the actual value of a label
int getLabelValue(LabelHashMap *labelMap, char *label) {
    unsigned long index = hashLabel(label, labelMap->capacity);
    unsigned long stepSize = hashLabel2(label, labelMap->capacity);

    // Double hashing to find the label
    while (labelMap->labels[index].value != NULL) {
        if (strcmp(labelMap->labels[index].value, label) == 0) {
            return index;
        }
        // Move to the next slot with a step size determined by the second hash
        index = (index + stepSize) % labelMap->capacity;
    }

    // Return -1 if the label is not found
    return -1;
}



// Function that converts a categorical column to numerical using label encoding
Column convert_column_to_encoded(Column *column, int rows, LabelHashMap *labelMap) {
    // Create a new column for the encoded values
    Column encoded_column;
    initColumn(&encoded_column, INTEGER, rows);

    // Iterate over the column and insert each label into the hash map
    for (int i = 0; i < rows; i++) {
        char *label = column->data.strings[i];  // Accessing the string part of the union
        insertLabel(labelMap, label);
    }

    // Iterate over the column and get the value of each label
    for (int i = 0; i < rows; i++) {
        char *label = column->data.strings[i];  // Accessing the string part of the union
        int value = getLabelValue(labelMap, label);
        encoded_column.data.integers[i] = value;  // Storing the value in the integer part of the union
    }

    return encoded_column;
}

// Function to convert a DataFrame to columns which are encoded
void hash_encode(DataFrame *df) {
    for (int i = 0; i < df->cols; i++) {
        if (df->encoding[i] == NOT_ENCODED && df->types[i] == CATEGORICAL) {
            // Initialize the LabelHashMap for this column
            df->labelMap[i] = (LabelHashMap *)malloc(sizeof(LabelHashMap));
            if (df->labelMap[i] == NULL) {
                // Handle memory allocation failure
                fprintf(stderr, "Failed to allocate memory for LabelHashMap\n");
                exit(EXIT_FAILURE);
            }
            initLabelHashMap(df->labelMap[i], df->rows);

            df->columns[i] = convert_column_to_encoded(&df->columns[i], df->rows, df->labelMap[i]);
            df->encoding[i] = ENCODED;
        }
    }
}


/*
Terminal command to run label_encoding.c with the extraction.h header file:
gcc hash_encoding.c extraction.c -o my_program
Comment out if running in main
*/

/*
int main(){
     DataFrame df = read_csv("../files/Data.csv");


    // Print the head of the DataFrame
    printHead(&df);

    //encode the categorical columns
    hash_encode(&df);

    // Print out unique labels count for each column
    for (int i = 0; i < df.cols; i++) {
        if (df.types[i] == CATEGORICAL) {
            printf("Column %s has %d unique labels\n", df.headings[i], df.labelMap[i]->unique_labels);
        }
    }


    // Print the head of the DataFrame after encoding
    printHead(&df);

    freeDataFrame(&df);
    return 0;

}
*/