# Decision Tree Classifier

This repository contains C code for a decision tree classifier that predicts classes for two different datasets: fruits and diseases.

## Prerequisites

Ensure you have a C compiler installed on your system (e.g., GCC).

## Getting Started

1. **Clone the repository to your local machine:**

    ```bash
    git clone https://github.com/Snapman5678/decision-tree-classifier.git
    ```

2. **Navigate to the project directory:**

    ```bash
    cd decision-tree-classifier
    ```
    
## Running the Program

1. **Compile the C code using your preferred compiler (e.g., GCC):**

    ```bash
    gcc decision_tree_classifier.c -o decision_tree_classifier
    ```

2. **Run the compiled executable:**

    ```bash
    ./decision_tree_classifier
    ```

3. Follow the on-screen instructions to choose between predicting fruit or disease classes. Input testing data as prompted.

### Input Format

#### For Fruits Dataset

- **Color:** Enter a string representing the color of the fruit from the following options only ("Green","Red","Yellow").
- **Diameter:** Enter an integer representing the diameter of the fruit ("Any integer").
- **Label:** Enter a string representing the label of the fruit from the following options only ("Apple","Grape","Lemon").

#### For Diseases Dataset

- **Symptom 1:** Enter a string representing the first symptom from the following options only ("Hightemperature","Fatigue","Cough").
- **Severity Level:** Enter an integer representing the severity level ("Any integer").
- **Duration of Symptoms:** Enter an integer representing the duration of symptoms in days ("Any integer").
- **Symptom 2:** Enter a string representing the second symptom from the following options only ("Dryskin","Headache","Sorethroat").
- **Disease Name:** Enter a string representing the name of the disease from the following options only ("Fever","Commoncold","Flu").

### Output

The program will print the predicted class labels along with the actual labels for the provided testing data.

### Exiting the Program

To exit the program, choose option 3 in the menu.
