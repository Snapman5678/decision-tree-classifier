#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "extraction.h"

// structure used to hold actual data compared to value which is being converted
typedef struct {
    char *value;
    int actual;
} Label;

// hash map used to store the actual data and the value which is being converted
typedef struct {
    Label *labels;
    int capacity;
    int unique_labels;
} LabelHashMap;

// Function to initialize Label
void initLabel(Label *label) {
    label->value = NULL;
    label->actual = -1;
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
void insertLabel(LabelHashMap *labelMap, char *label, int actual) {
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
    labelMap->labels[index].value = label;
    labelMap->labels[index].actual = actual;
    labelMap->unique_labels++;
}


// Function to get the actual value of a label
int getLabelValue(LabelHashMap *labelMap, char *label) {
    unsigned long index = hashLabel(label, labelMap->capacity);
    unsigned long stepSize = hashLabel2(label, labelMap->capacity);

    // Double hashing to find the label
    while (labelMap->labels[index].value != NULL) {
        if (strcmp(labelMap->labels[index].value, label) == 0) {
            return labelMap->labels[index].actual;
        }
        // Move to the next slot with a step size determined by the second hash
        index = (index + stepSize) % labelMap->capacity;
    }

    // Return -1 if the label is not found
    return -1;
}



// Function to free the memory allocated for the hash map
void freeLabelHashMap(LabelHashMap *labelMap) {
    for (int i = 0; i < labelMap->capacity; i++) {
        if (labelMap->labels[i].value != NULL) {
            free(labelMap->labels[i].value);
        }
    }

    free(labelMap->labels);
}


int main(){
     DataFrame df = read_csv("../files/Salary_Data.csv");


    // Print the head of the DataFrame
    printHead(&df);

    freeDataFrame(&df);
    return 0;

}