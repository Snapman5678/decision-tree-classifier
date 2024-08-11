#ifndef DECISION_TREE_CLASSIFIER_H
#define DECISION_TREE_CLASSIFIER_H

// Include necessary standard headers
#include <stdbool.h>

// Define constants
#define MAX_TRAINING_SAMPLES_f 6
#define UNIQUE_LABELS_f 3
#define CLASSES_f 3
#define MAX_TESTING_DATA_f 5

#define MAX_TRAINING_SAMPLES_h 12
#define UNIQUE_LABELS_h 3
#define CLASSES_h 5
#define MAX_TESTING_DATA_h 5

#define MAX_STRING_LENGTH 25

// Define the structures
typedef struct {
    char *color;
    int code;
} ColorHashMap_f;

typedef struct {
    char *label;
    int code;
} LabelHashMap_f;

typedef struct {
    char color[MAX_STRING_LENGTH];
    int diameter;
    char label[MAX_STRING_LENGTH];
} Row_f;

typedef struct {
    char *symptom1;
    int code;
} Symptom1HashMap_h;

typedef struct {
    char *symptom2;
    int code;
} Symptom2HashMap_h;

typedef struct {
    char *disease_name;
    int code;
} DiseaseNameHashMap_h;

typedef struct {
    char symptom1[MAX_STRING_LENGTH];
    int severity_level;
    int duration_of_symptoms; // in days
    char symptom2[MAX_STRING_LENGTH];
    char disease_name[MAX_STRING_LENGTH];
} Row_h;

typedef struct TreeNode {
    int feature;   // Feature index for the split (1st or 3rd column)
    int threshold; // Threshold value for the split
    int label;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Function declarations
void initColorHashMap_f(ColorHashMap_f *hashMap);
void initLabelHashMap_f(LabelHashMap_f *hashMap);
int getColorCode_f(ColorHashMap_f *hashMap, const char *color);
int getLabelCode_f(LabelHashMap_f *hashMap, const char *label);
int **encodeRow_f(Row_f *row, ColorHashMap_f *colorMap, LabelHashMap_f *labelMap);
void initSymptom1HashMap_h(Symptom1HashMap_h *hashMap);
void initSymptom2HashMap_h(Symptom2HashMap_h *hashMap);
void initDiseaseNameHashMap_h(DiseaseNameHashMap_h *hashMap);
int getSymptom1Code_h(Symptom1HashMap_h *hashMap, const char *symptom1);
int getSymptom2Code_h(Symptom2HashMap_h *hashMap, const char *symptom2);
int getDiseaseNameCode_h(DiseaseNameHashMap_h *hashMap, const char *disease_name);
int **encodeRow_h(Row_h *row, Symptom1HashMap_h *symptom1Map, Symptom2HashMap_h *symptom2Map, DiseaseNameHashMap_h *disease_nameMap);
TreeNode *createTreeNode();
double giniImpurity(int **dataset, int num_samples, int unique_labels, int num_features);
void splitDataset(int **dataset, int numRows, int numCols, int feature, int threshold, int *left_count, int ***leftData, int *right_count, int ***rightData, int datatype);
double infoGain(int **leftData, int left_count, int **rightData, int right_count, double current_uncertainty, int l_unique_labels, int r_unique_labels, int num_features);
int countUniqueLabels(int **dataset, int num_samples, int num_features);
int findBestSplit_f(int **dataset, int numRows, int numCols, int *feature, int *threshold, int *left_count, int ***leftData, int *right_count, int ***rightData);
int findBestSplit_h(int **dataset, int numRows, int numCols, int *feature, int *threshold, int *left_count, int ***leftData, int *right_count, int ***rightData);
TreeNode *buildDecisionTree(int **dataset, int numRows, int numCols, int(*findBestSplitCallback)(int **, int, int, int *, int *, int *, int ***, int *, int ***));
int predict(TreeNode *root, int *sample);
void capitalizeFirstLetter(char *str);
int isValidColor(const char *color);
int isValidLabel(const char *label);
int isValidSymptom(const char *symptom);
int isValidDiseaseName(const char *disease_name);
void getValidStringInput(char *input, int maxLength, int (*isValid)(const char *),void (*printOptions)());
void printFruitLabels();
void printDiseaseLabels();
void printSymptoms();
void printColors();


#endif // DECISION_TREE_CLASSIFIER_H
