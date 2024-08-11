#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_TRAINING_SAMPLES_f 6
#define UNIQUE_LABELS_f 3
#define CLASSES_f 3
#define MAX_TESTING_DATA_f 5

#define MAX_TRAINING_SAMPLES_h 12
#define UNIQUE_LABELS_h 3
#define CLASSES_h 5
#define MAX_TESTING_DATA_h 5

#define MAX_STRING_LENGTH 25

// Define a structure for the hash map for colors
typedef struct
{
    char *color;
    int code;
} ColorHashMap_f;

// Define a structure for the hash map for labels
typedef struct
{
    char *label;
    int code;
} LabelHashMap_f;

typedef struct
{
    char color[MAX_STRING_LENGTH];
    int diameter;
    char label[MAX_STRING_LENGTH];
} Row_f;

// Define a structure for the hash map for symptoms
typedef struct
{
    char *symptom1;
    int code;
} Symptom1HashMap_h;

// Define a structure for the hash map for symptoms
typedef struct
{
    char *symptom2;
    int code;
} Symptom2HashMap_h;

// Define a structure for the hash map for the disease names
typedef struct
{
    char *disease_name;
    int code;
} DiseaseNameHashMap_h;

typedef struct
{
    char symptom1[MAX_STRING_LENGTH];
    int severity_level;
    int duration_of_symptoms; //in days
    char symptom2[MAX_STRING_LENGTH];
    char disease_name[MAX_STRING_LENGTH];
} Row_h;

// Function to initialize the color hash map
void initColorHashMap_f(ColorHashMap_f *hashMap)
{
    // Initialize color codes (adjust as needed)
    hashMap[0].color = "Green";
    hashMap[0].code = 1;

    hashMap[1].color = "Yellow";
    hashMap[1].code = 2;

    hashMap[2].color = "Red";
    hashMap[2].code = 3;
}

// Function to initialize the label hash map
void initLabelHashMap_f(LabelHashMap_f *hashMap)
{
    // Initialize label codes (adjust as needed)
    hashMap[0].label = "Apple";
    hashMap[0].code = 1;

    hashMap[1].label = "Grape";
    hashMap[1].code = 2;

    hashMap[2].label = "Lemon";
    hashMap[2].code = 3;
}

// Function to find the code for a given color
int getColorCode_f(ColorHashMap_f *hashMap, const char *color)
{
    for (int i = 0; i < 3; i++)
    { // Assuming 3 color codes
        if (strcmp(hashMap[i].color, color) == 0)
        {
            return hashMap[i].code;
        }
    }
    // Return a default code if the color is not found (adjust as needed)
    return 0;
}

// Function to find the code for a given label
int getLabelCode_f(LabelHashMap_f *hashMap, const char *label)
{
    for (int i = 0; i < 3; i++)
    { // Assuming 3 label codes
        if (strcmp(hashMap[i].label, label) == 0)
        {
            return hashMap[i].code;
        }
    }
    // Return a default code if the label is not found (adjust as needed)
    return 0;
}

// Function to encode the Row structure and return a 2D array
int **encodeRow_f(Row_f *row, ColorHashMap_f *colorMap, LabelHashMap_f *labelMap)
{
    int **encoded_data = (int **)malloc(MAX_TRAINING_SAMPLES_f * sizeof(int *));

    for (int i = 0; i < MAX_TRAINING_SAMPLES_f; i++)
    {
        encoded_data[i] = (int *)malloc(3 * sizeof(int));
        encoded_data[i][0] = getColorCode_f(colorMap, row[i].color);
        encoded_data[i][1] = row[i].diameter;
        encoded_data[i][2] = getLabelCode_f(labelMap, row[i].label);
    }

    return encoded_data;
}

// Function to initialize the color hash map
void initSymptom1HashMap_h(Symptom1HashMap_h *hashMap)
{
    // Initialize color codes (adjust as needed)
    hashMap[0].symptom1 = "Hightemperature";
    hashMap[0].code = 1;

    hashMap[1].symptom1 = "Cough";
    hashMap[1].code = 2;

    hashMap[2].symptom1 = "Fatigue";
    hashMap[2].code = 3;
}

// Function to initialize the label hash map
void initSymptom2HashMap_h(Symptom2HashMap_h *hashMap)
{
    // Initialize label codes (adjust as needed)
    hashMap[0].symptom2 = "Dryskin";
    hashMap[0].code = 1;

    hashMap[1].symptom2 = "Sorethroat";
    hashMap[1].code = 2;

    hashMap[2].symptom2 = "Headache";
    hashMap[2].code = 3;
}

void initDiseaseNameHashMap_h(DiseaseNameHashMap_h *hashMap)
{
    // Initialize label codes (adjust as needed)
    hashMap[0].disease_name = "Fever";
    hashMap[0].code = 1;

    hashMap[1].disease_name = "Commoncold";
    hashMap[1].code = 2;

    hashMap[2].disease_name = "Flu";
    hashMap[2].code = 3;
}

// Function to find the code for a given symptom
int getSymptom1Code_h(Symptom1HashMap_h *hashMap, const char *symptom1)
{
    for (int i = 0; i < 3; i++)
    { // Assuming 3 color codes
        if (strcmp(hashMap[i].symptom1, symptom1) == 0)
        {
            return hashMap[i].code;
        }
    }
    // Return a default code if the color is not found (adjust as needed)
    return 0;
}

// Function to find the code for a given symptom
int getSymptom2Code_h(Symptom2HashMap_h *hashMap, const char *symptom2)
{
    for (int i = 0; i < 3; i++)
    { // Assuming 3 color codes
        if (strcmp(hashMap[i].symptom2, symptom2) == 0)
        {
            return hashMap[i].code;
        }
    }
    // Return a default code if the color is not found (adjust as needed)
    return 0;
}

// Function to find the code for a given label
int getDiseaseNameCode_h(DiseaseNameHashMap_h *hashMap, const char *disease_name)
{
    for (int i = 0; i < 3; i++)
    { // Assuming 3 label codes
        if (strcmp(hashMap[i].disease_name, disease_name) == 0)
        {
            return hashMap[i].code;
        }
    }
    // Return a default code if the label is not found (adjust as needed)
    return 0;
}

// Function to encode the Row structure and return a 2D array
int **encodeRow_h(Row_h *row, Symptom1HashMap_h *symptom1Map, Symptom2HashMap_h *symptom2Map, DiseaseNameHashMap_h *disease_nameMap)
{
    int **encoded_data = (int **)malloc(MAX_TRAINING_SAMPLES_h * sizeof(int *));

    for (int i = 0; i < MAX_TRAINING_SAMPLES_h; i++)
    {
        encoded_data[i] = (int *)malloc(5 * sizeof(int));
        encoded_data[i][0] = getSymptom1Code_h(symptom1Map, row[i].symptom1);
        encoded_data[i][1] = row[i].severity_level;
        encoded_data[i][2] = row[i].duration_of_symptoms;
        encoded_data[i][3] = getSymptom2Code_h(symptom2Map, row[i].symptom2);
        encoded_data[i][4] = getDiseaseNameCode_h(disease_nameMap, row[i].disease_name);
    }

    return encoded_data;
}

typedef struct TreeNode
{
    int feature;   // Feature index for the split (1st or 3rd column)
    int threshold; // Threshold value for the split
    int label;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Function to create a new decision tree node
TreeNode *createTreeNode()
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->left = NULL;
    node->right = NULL;
    node->label = -1; // Initialize as a non-leaf node
    return node;
}

double giniImpurity(int **dataset, int num_samples, int unique_labels, int num_features)
{    if (unique_labels == 1) {
        return 0.0;  // If there's only one unique label, Gini impurity is 0.
    }
    // Dynamically allocate memory for label_counts array
    int *label_counts = (int *)malloc(unique_labels * sizeof(int));
    if (label_counts == NULL) {
        // Handle allocation failure
        return -1.0; // You can choose an appropriate error value
    }

    for (int i = 0; i < unique_labels; i++) {
        label_counts[i] = 0;
    }

    // Count the occurrences of each label in the dataset
    for (int i = 0; i < num_samples; i++) {
        label_counts[dataset[i][num_features - 1] - 1]++;
    }

    // Calculate Gini impurity
    double impurity = 1.0;
    for (int i = 0; i < unique_labels; i++) {
        double prob = (double)label_counts[i] / num_samples;
        impurity -= prob * prob;
    }

    // Free the dynamically allocated memory
    free(label_counts);
    return impurity;
}

// Function to split the dataset into two subsets
void splitDataset(int **dataset, int numRows, int numCols, int feature, int threshold, int *left_count, int ***leftData, int *right_count, int ***rightData, int datatype)
{

    // Initialize left and right counts
    *left_count = 0;
    *right_count = 0;

    // Initalize left and right data arrays
    *leftData = (int **)malloc(numRows * sizeof(int *));
    *rightData = (int **)malloc(numRows * sizeof(int *));

    if (datatype == 1)
    {
        for (int i = 0; i < numRows; i++)
        {
            if (dataset[i][feature] >= threshold)
            {
                // Copy the data to the left subset
                (*leftData)[*left_count] = (int *)malloc(numCols * sizeof(int));
                memcpy((*leftData)[*left_count], dataset[i], numCols * sizeof(int));
                (*left_count)++;
            }
            else
            {
                // Copy the data to the right subset
                (*rightData)[*right_count] = (int *)malloc(numCols * sizeof(int));
                memcpy((*rightData)[*right_count], dataset[i], numCols * sizeof(int));
                (*right_count)++;
            }
        }
    }
    else if (datatype == 0)
    {
        for (int i = 0; i < numRows; i++)
        {
            if (dataset[i][feature] == threshold)
            {
                // Copy the data to the left subset
                (*leftData)[*left_count] = (int *)malloc(numCols * sizeof(int));
                memcpy((*leftData)[*left_count], dataset[i], numCols * sizeof(int));
                (*left_count)++;
            }
            else
            {
                // Copy the data to the right subset
                (*rightData)[*right_count] = (int *)malloc(numCols * sizeof(int));
                memcpy((*rightData)[*right_count], dataset[i], numCols * sizeof(int));
                (*right_count)++;
            }
        }
    }
}

// Function to calculate the Information Gain
double infoGain(int **leftData, int left_count, int **rightData, int right_count, double current_uncertainty,int l_unique_labels,int r_unique_labels,int num_features)
{
    // Information Gain = uncertainty before - (weighted average) * uncertainty after
    double p = (double)left_count / (left_count + right_count);
    double info_gain = current_uncertainty - p * giniImpurity(leftData, left_count,l_unique_labels,num_features) - (1 - p) * giniImpurity(rightData, right_count,r_unique_labels,num_features);
    return info_gain;
}

// Function to calculate the number of unique labels in a dataset
int countUniqueLabels(int **dataset, int num_samples, int num_features)
{
    int *unique_labels = (int *)malloc(num_samples * sizeof(int));
    int unique_count = 0;

    for (int i = 0; i < num_samples; i++)
    {
        int label = dataset[i][num_features - 1];
        bool isUnique = true;

        // Check if the label is already in the unique_labels array
        for (int j = 0; j < unique_count; j++)
        {
            if (label == unique_labels[j])
            {
                isUnique = false;
                break;
            }
        }

        // If the label is unique, add it to the unique_labels array
        if (isUnique)
        {
            unique_labels[unique_count] = label;
            unique_count++;
        }
    }

    free(unique_labels);
    return unique_count;
}

// Function to find the best split for the dataset
int findBestSplit_f(int **dataset, int numRows, int numCols, int *feature, int *threshold, int *left_count, int ***leftData, int *right_count, int ***rightData)
{
    double best_information_gain = -1.0;
    int best_feature = -1;
    int best_threshold = 0;
    int datatype;

    // Calculate the Gini impurity of the current node
    double current_uncertainty = giniImpurity(dataset, numRows, UNIQUE_LABELS_f, numCols);

    // Initialize variables to keep track of the best counts and datasets
    int best_left_count = 0;
    int best_right_count = 0;
    int **best_leftData = NULL;
    int **best_rightData = NULL;

    // Iterate through all the features
    for (int i = 0; i < numCols - 1; i++)
    {
        // Iterate through all the values of the current feature
        for (int j = 0; j < numRows; j++)
        {
            int left_count, right_count;
            int **leftData;
            int **rightData;
            if (i == 0)
            {
                datatype = 0;
            }
            else if (i == 1)
            {
                datatype = 1;
            }
            splitDataset(dataset, numRows, numCols, i, dataset[j][i], &left_count, &leftData, &right_count, &rightData, datatype);
            double information_gain = infoGain(leftData, left_count, rightData, right_count, current_uncertainty, UNIQUE_LABELS_f, UNIQUE_LABELS_f, numCols);

            if (information_gain > best_information_gain)
            {
                best_information_gain = information_gain;
                best_feature = i;
                best_threshold = dataset[j][i];
                best_left_count = left_count;
                best_right_count = right_count;
                best_leftData = leftData;
                best_rightData = rightData;
            }
        }
    }

    // Assign the best results to the output parameters
    *feature = best_feature;
    *threshold = best_threshold;
    *left_count = best_left_count;
    *right_count = best_right_count;
    *leftData = best_leftData;
    *rightData = best_rightData;
    return 1;
}

// Function to find the best split for the dataset
int findBestSplit_h(int **dataset, int numRows, int numCols, int *feature, int *threshold, int *left_count, int ***leftData, int *right_count, int ***rightData)
{
    double best_information_gain = -1.0;
    int best_feature = -1;
    int best_threshold = 0;

    // Calculate the Gini impurity of the current node
    double current_uncertainty = giniImpurity(dataset, numRows, UNIQUE_LABELS_h, numCols);

    // Initialize variables to keep track of the best counts and datasets
    int best_left_count = 0;
    int best_right_count = 0;
    int **best_leftData = NULL;
    int **best_rightData = NULL;

    // Iterate through all the features
    for (int i = 0; i < numCols - 1; i++)
    {
        // Iterate through all the values of the current feature
        for (int j = 0; j < numRows; j++)
        {
            int left_count, right_count;
            int **leftData;
            int **rightData;
            int datatype;

            if (i == 0 || i == 3)
            {
                datatype = 0;
            }
            else if (i == 1 || i == 2)
            {
                datatype = 1;
            }
            splitDataset(dataset, numRows, numCols, i, dataset[j][i], &left_count, &leftData, &right_count, &rightData, datatype);
            double information_gain = infoGain(leftData, left_count, rightData, right_count, current_uncertainty, UNIQUE_LABELS_h, UNIQUE_LABELS_h, numCols);

            if (information_gain > best_information_gain)
            {
                best_information_gain = information_gain;
                best_feature = i;
                best_threshold = dataset[j][i];
                best_left_count = left_count;
                best_right_count = right_count;
                best_leftData = leftData;
                best_rightData = rightData;
            }
        }
    }

    // Assign the best results to the output parameters
    *feature = best_feature;
    *threshold = best_threshold;
    *left_count = best_left_count;
    *right_count = best_right_count;
    *leftData = best_leftData;
    *rightData = best_rightData;
    return 1;
}

// Function to build the decision tree
TreeNode *buildDecisionTree(int **dataset, int numRows, int numCols, int(*findBestSplitCallback)(int **, int, int, int *, int *, int *, int ***, int *, int ***))
{
    // Create a new node
    TreeNode *node = createTreeNode();

    // Find the best split using the provided callback function
    int feature, threshold, left_count, right_count;
    int **leftData;
    int **rightData;

    if (findBestSplitCallback(dataset, numRows, numCols, &feature, &threshold, &left_count, &leftData, &right_count, &rightData))
    {
        // Check if the information gain is 0
        if (feature == -1)
        {
            int num_unique_labels = countUniqueLabels(dataset, numRows, numCols);

            if (num_unique_labels == 1)
            {
                // Set the node as a leaf node and store the unique label
                node->label = dataset[0][numCols - 1];
            }
            else if(num_unique_labels>1)
            {
                //find the majority label
                int *label_counts = (int *)malloc(num_unique_labels * sizeof(int));
                if (label_counts == NULL) {
                    // Handle allocation failure
                    return NULL; // You can choose an appropriate error value
                }

                for (int i = 0; i < num_unique_labels; i++) {
                    label_counts[i] = 0;
                }

                // Count the occurrences of each label in the dataset
                for (int i = 0; i < numRows; i++) {
                    label_counts[dataset[i][numCols - 1] - 1]++;
                }

                int max = label_counts[0];
                int max_index = 0;
                for (int i = 1; i < num_unique_labels; i++) {
                    if (label_counts[i] > max) {
                        max = label_counts[i];
                        max_index = i;
                    }
                }

                // Set the node as a leaf node and store the unique label
                node->label = max_index+1;

                // Free the dynamically allocated memory
                free(label_counts);
                
            }

            return node;
        }

        // Set the node's feature and threshold
        node->feature = feature;
        node->threshold = threshold;
        printf("Feature: %d\n", feature);
        printf("Threshold: %d\n", threshold);
        printf("Left count: %d\n", left_count);
        printf("Right count: %d\n", right_count);
        printf("Left data:\n");
        for (int i = 0; i < left_count; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                printf("%d ", leftData[i][j]);
            }
            printf("\n");
        }
        printf("Right data:\n");
        for (int i = 0; i < right_count; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                printf("%d ", rightData[i][j]);
            }
            printf("\n");
        }


        // Recursively build the left and right subtrees
        node->left = buildDecisionTree(leftData, left_count, numCols, findBestSplitCallback);
        node->right = buildDecisionTree(rightData, right_count, numCols, findBestSplitCallback);
    }

    return node;
}

// Function to predict the class label for a given sample
int predict(TreeNode *root, int *sample)
{
    // Traverse the tree until a leaf node is found
    while (root->label == -1)
    {
        if (sample[root->feature] >= root->threshold)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
    return root->label;
}

/*
Below are the added functions so that user doesnt put wrong data manually
*/

// Function to capitalize the first letter and lowercase the rest
void capitalizeFirstLetter(char *str) {
    if (str[0]) {
        str[0] = toupper((unsigned char)str[0]);
    }
    for (int i = 1; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Function to check if a color or label is valid
int isValidColor(const char *color) {
    return strcmp(color, "Green") == 0 || strcmp(color, "Red") == 0 || strcmp(color, "Yellow") == 0;
}

int isValidLabel(const char *label) {
    return strcmp(label, "Apple") == 0 || strcmp(label, "Grape") == 0 || strcmp(label, "Lemon") == 0;
}


// Function to check if a symptom or disease name is valid
int isValidSymptom(const char *symptom) {
    return strcmp(symptom, "Hightemperature") == 0 || strcmp(symptom, "Fatigue") == 0 || strcmp(symptom, "Cough") == 0 ||
           strcmp(symptom, "Dryskin") == 0 || strcmp(symptom, "Headache") == 0 || strcmp(symptom, "Sorethroat") == 0;
}

int isValidDiseaseName(const char *disease_name) {
    return strcmp(disease_name, "Fever") == 0 || strcmp(disease_name, "Commoncold") == 0 || strcmp(disease_name, "Flu") == 0;
}

// Function that gets valid string input from the user that points a function to check if the input is valid
void getValidStringInput(char *input, int maxLength, int (*isValid)(const char *),void (*printOptions)()){
    while (1) {
        fgets(input, maxLength, stdin);
        input[strcspn(input, "\n")] = 0;  

        capitalizeFirstLetter(input);

        if (isValid(input)) {
            break;
        } else {
            printOptions();
            printf("Please enter a valid value: ");
        }
    }
}

//Functions to print the labels
void printFruitLabels() {
    printf("Fruit labels: Apple, Grape, Lemon\n");
}

void printDiseaseLabels() {
    printf("Disease labels: Fever, Commoncold, Flu\n");
}

void printSymptoms() {
    printf("Symptoms: Hightemperature, Fatigue, Cough\n");
}

void printColors() {
    printf("Colors: Green, Red, Yellow\n");
}