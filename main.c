#include <stdio.h>
#include <stdlib.h>
#include "classification/decision_tree_classifier.h"

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


int main()
{
    int choice;
    do
    {
        printf("Please enter a choice\n");
        printf("1. Predict Fruit 2. Predict Disease 3. Exit\n");
        scanf("%d", &choice);

        if (choice == 1)
        {
            // ...
            Row_f training_data[MAX_TRAINING_SAMPLES_f] = {
                {"Green", 10, "Apple"},
                {"Red", 7, "Apple"},
                {"Green", 1, "Grape"},
                {"Red", 3, "Grape"},
                {"Yellow", 4, "Lemon"},
                {"Green", 6, "Lemon"}
            };
    
        //test encoding for fruit data
        ColorHashMap_f colorMap_f[3];
        LabelHashMap_f labelMap_f[3];
        initColorHashMap_f(colorMap_f);
        initLabelHashMap_f(labelMap_f);

        int **encoded_data_f = encodeRow_f(training_data, colorMap_f, labelMap_f);

        //test the build decision tree function
        TreeNode *root = buildDecisionTree(encoded_data_f, MAX_TRAINING_SAMPLES_f, 3, findBestSplit_f);

            Row_f testing_data[MAX_TESTING_DATA_f];

            // Input testing data for fruits from the user
            for (int i = 0; i < MAX_TESTING_DATA_f; i++)
            {
                printf("Enter testing data for fruit #%d:\n", i + 1);
                printf("Color: ");
                scanf("%s", testing_data[i].color);
                printf("Diameter: ");
                scanf("%d", &testing_data[i].diameter);
                printf("Label: ");
                scanf("%s", testing_data[i].label);
            }

            // Encode testing data
            int **encoded_testing_data = encodeRow_f(testing_data, colorMap_f, labelMap_f);

            // Predict and print the results
            for (int i = 0; i < MAX_TESTING_DATA_f; i++)
            {
                int result = predict(root, encoded_testing_data[i]);
                printf("Predicted class label: %s Actual Value: %s\n", labelMap_f[result - 1].label, testing_data[i].label);
            }
        }
    else if(choice==2)
    {

    // //testing encoding for disease data
    Row_h training_data[MAX_TRAINING_SAMPLES_h] = {
                {"Hightemperature", 5, 7, "Dryskin", "Fever"},
                {"Fatigue", 6, 5,"Headache","Fever"},
                {"Hightemperature", 3, 3,"Headache","Fever"},
                {"Fatigue", 4, 4, "Dryskin", "Fever"},
                {"Cough", 3, 4, "Sorethroat", "Commoncold"},
                {"Fatigue", 1, 3, "Sorethroat", "Commoncold"},
                {"Cough", 2, 2, "Sorethroat", "Commoncold"},
                {"Cough", 1, 1, "Sorethroat", "Commoncold"},
                {"Fatigue", 4, 7, "Headache", "Flu"},
                {"Hightemperature", 6, 6, "Dryskin","Flu"},
                {"Hightemperature", 5, 4,"Headache","Flu"},
                {"Fatigue", 4, 9, "Dryskin", "Flu"}
            };
    // Create color and label hash maps
    Symptom1HashMap_h symptom1Map[3];
    Symptom2HashMap_h symptom2Map[3];
    DiseaseNameHashMap_h disease_nameMap[3];

    initSymptom1HashMap_h(symptom1Map);
    initSymptom2HashMap_h(symptom2Map);
    initDiseaseNameHashMap_h(disease_nameMap);

    // Call the encodeRow function to encode the Row structures
    int **encoded_data = encodeRow_h(training_data, symptom1Map, symptom2Map, disease_nameMap);

    //test the build decision tree function
    TreeNode *root = buildDecisionTree(encoded_data, MAX_TRAINING_SAMPLES_h, 5, findBestSplit_h);

    //test the predict class label function
    // Row_h testing_data[MAX_TESTING_DATA_h] = {
    //             {"Hightemperature", 5, 7, "Dryskin", "Fever"},
    //             {"Hightemperature", 5, 7, "Dryskin", "Fever"},
    //             {"Cough", 4, 5, "Sorethroat", "Commoncold"},
    //             {"Fatigue", 3, 3, "Headache", "Flu"},
    //             {"Hightemperature",3,2,"Headache","Fever"}
    //         };
    Row_h testing_data[MAX_TESTING_DATA_h];

    // Input testing data for fruits from the user
    for (int i = 0; i < MAX_TESTING_DATA_h; i++)
    {
        printf("Enter testing data for disease #%d:\n", i + 1);
        printf("Symptom 1: ");
        scanf("%s", testing_data[i].symptom1);
        printf("Severity Level: ");
        scanf("%d", &testing_data[i].severity_level);
        printf("Duration of Symptoms: ");
        scanf("%d", &testing_data[i].duration_of_symptoms);
        printf("Symptom 2: ");
        scanf("%s", testing_data[i].symptom2);
        printf("Disease Name: ");
        scanf("%s", testing_data[i].disease_name);
    }
    
    int **encoded_testing_data = encodeRow_h(testing_data, symptom1Map, symptom2Map, disease_nameMap);
    //print the encoded testing data without predict

    for (int i = 0; i < MAX_TESTING_DATA_h; i++)
    {
        int result = predict(root, encoded_testing_data[i]);
        printf("Predicted class label: %s Actual Value: %s\n", disease_nameMap[result - 1].disease_name, testing_data[i].disease_name);
    }
    }
    else if(choice==3)
    {
        exit(0);
    }
    }while(choice>=1 && choice<=3);
}