#include <stdio.h>
#include <stdlib.h>
#include "classification/decision_tree_classifier.h"
#include "preprocessing/extraction.h"

/*
The entry point to the machine learning algorithms.

1.  It reads the data from the file.

2.  The second step is to implement encoding of the data. This is done by converting the data into a format that can be used by the machine learning algorithms.
    -- These algorithms can be found in the encoding folder.

3. The third step is to implement the machine learning algorithms.
    -- These algorithms can be found in the machine_learning folder.

4. The fourth step is to implement the evaluation of the machine learning algorithms.
    -- These algorithms can be found in the evaluation folder.

** For now only encoding and classification algorithms are implemented. The evaluation algorithms will be implemented in the future. **
** The main function currently uses test.csv as the data source. This will be changed in the future to allow for user input. **
*/

/*
This is main for initial version of decision tree clasifier where fruits and diseases are classified.
There is not much choice for types of fruits and diseases as encoding was fixed.
*/

// This main is to test extracion from preprocessing/extraction.h
int main() {
    DataFrame df = read_csv("files/Salary_Data.csv");
    convert_to_int_float(&df);
    printDataFrame(&df, df.rows, df.cols);
    freeDataFrame(&df);
    return 0;
}

// This main is for decision tree classifier where fruits and diseases are classified.
/*
int main() {
    int choice;
    do {
        printf("Please enter a choice\n");
        printf("1. Predict Fruit 2. Predict Disease 3. Exit\n");
        scanf("%d", &choice);
        while (getchar() != '\n');  // Clear newline character

        if (choice == 1) {
            Row_f training_data[MAX_TRAINING_SAMPLES_f] = {
                {"Green", 10, "Apple"},
                {"Red", 7, "Apple"},
                {"Green", 1, "Grape"},
                {"Red", 3, "Grape"},
                {"Yellow", 4, "Lemon"},
                {"Green", 6, "Lemon"}
            };

            ColorHashMap_f colorMap_f[3];
            LabelHashMap_f labelMap_f[3];
            initColorHashMap_f(colorMap_f);
            initLabelHashMap_f(labelMap_f);

            int **encoded_data_f = encodeRow_f(training_data, colorMap_f, labelMap_f);

            TreeNode *root = buildDecisionTree(encoded_data_f, MAX_TRAINING_SAMPLES_f, 3, findBestSplit_f);

            Row_f testing_data[MAX_TESTING_DATA_f];

            for (int i = 0; i < MAX_TESTING_DATA_f; i++) {
                printf("Enter testing data for fruit #%d:\n", i + 1);

                printf("Color: ");
                getValidStringInput(testing_data[i].color, MAX_STRING_LENGTH, isValidColor,printColors);

                printf("Diameter (integer): ");
                while (scanf("%d", &testing_data[i].diameter) != 1) {
                    printf("Invalid input. Please enter an integer value for diameter: ");
                    while (getchar() != '\n');  // Clear invalid input
                }
                while (getchar() != '\n');  // Clear newline character

                printf("Label: ");
                getValidStringInput(testing_data[i].label, MAX_STRING_LENGTH, isValidLabel,printFruitLabels);
            }

            int **encoded_testing_data = encodeRow_f(testing_data, colorMap_f, labelMap_f);

            for (int i = 0; i < MAX_TESTING_DATA_f; i++) {
                int result = predict(root, encoded_testing_data[i]);
                printf("Predicted class label: %s Actual Value: %s\n", labelMap_f[result - 1].label, testing_data[i].label);
            }
        } else if (choice == 2) {
            Row_h training_data[MAX_TRAINING_SAMPLES_h] = {
                {"Hightemperature", 5, 7, "Dryskin", "Fever"},
                {"Fatigue", 6, 5, "Headache", "Fever"},
                {"Hightemperature", 3, 3, "Headache", "Fever"},
                {"Fatigue", 4, 4, "Dryskin", "Fever"},
                {"Cough", 3, 4, "Sorethroat", "Commoncold"},
                {"Fatigue", 1, 3, "Sorethroat", "Commoncold"},
                {"Cough", 2, 2, "Sorethroat", "Commoncold"},
                {"Cough", 1, 1, "Sorethroat", "Commoncold"},
                {"Fatigue", 4, 7, "Headache", "Flu"},
                {"Hightemperature", 6, 6, "Dryskin", "Flu"},
                {"Hightemperature", 5, 4, "Headache", "Flu"},
                {"Fatigue", 4, 9, "Dryskin", "Flu"}
            };

            Symptom1HashMap_h symptom1Map_h[3];
            Symptom2HashMap_h symptom2Map_h[3];
            DiseaseNameHashMap_h disease_nameMap_h[3];

            initSymptom1HashMap_h(symptom1Map_h);
            initSymptom2HashMap_h(symptom2Map_h);
            initDiseaseNameHashMap_h(disease_nameMap_h);

            int **encoded_data_h = encodeRow_h(training_data, symptom1Map_h, symptom2Map_h, disease_nameMap_h);

            TreeNode *root = buildDecisionTree(encoded_data_h, MAX_TRAINING_SAMPLES_h, 5, findBestSplit_h);

            Row_h testing_data[MAX_TESTING_DATA_h];

            for (int i = 0; i < MAX_TESTING_DATA_h; i++) {
                printf("Enter testing data for disease #%d:\n", i + 1);

                printf("Symptom 1: ");
                getValidStringInput(testing_data[i].symptom1, MAX_STRING_LENGTH, isValidSymptom,printSymptoms);

                printf("Severity Level (1 to 10): ");
                while (scanf("%d", &testing_data[i].severity_level) != 1) {
                    printf("Invalid input. Please enter an integer value for severity level: ");
                    while (getchar() != '\n');  // Clear invalid input
                }
                while (getchar() != '\n');  // Clear newline character

                printf("Duration of Symptoms (Days): ");
                while (scanf("%d", &testing_data[i].duration_of_symptoms) != 1) {
                    printf("Invalid input. Please enter an integer value for duration of symptoms: ");
                    while (getchar() != '\n');  // Clear invalid input
                }
                while (getchar() != '\n');  // Clear newline character

                printf("Symptom 2: ");
                getValidStringInput(testing_data[i].symptom2, MAX_STRING_LENGTH, isValidSymptom,printSymptoms);

                printf("Disease Name: ");
                getValidStringInput(testing_data[i].disease_name, MAX_STRING_LENGTH, isValidDiseaseName,printDiseaseLabels);
            }

            int **encoded_testing_data = encodeRow_h(testing_data, symptom1Map_h, symptom2Map_h, disease_nameMap_h);

            for (int i = 0; i < MAX_TESTING_DATA_h; i++) {
                int result = predict(root, encoded_testing_data[i]);
                printf("Predicted class label: %s Actual Value: %s\n", disease_nameMap_h[result - 1].disease_name, testing_data[i].disease_name);
            }
        } else if (choice == 3) {
            exit(0);
        }
    } while (choice >= 1 && choice <= 3);

    return 0;
}
*/