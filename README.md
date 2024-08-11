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

3. **Build the project using the provided `Makefile`:**

    ```bash
    make
    ```

## Running the Program

1. **Run the compiled executable:**

    ```bash
    ./main
    ```

2. Follow the on-screen instructions to choose between predicting fruit or disease classes. Input testing data as prompted.

### Input Format

#### For Fruits Dataset

- **Color:** Enter a string representing the color of the fruit from the following options only ("Green", "Red", "Yellow").
- **Diameter:** Enter an integer representing the diameter of the fruit ("Any integer").
- **Label:** Enter a string representing the label of the fruit from the following options only ("Apple", "Grape", "Lemon").

#### For Diseases Dataset

- **Symptom 1:** Enter a string representing the first symptom from the following options only ("Hightemperature", "Fatigue", "Cough").
- **Severity Level:** Enter an integer representing the severity level ("Any integer").
- **Duration of Symptoms:** Enter an integer representing the duration of symptoms in days ("Any integer").
- **Symptom 2:** Enter a string representing the second symptom from the following options only ("Dryskin", "Headache", "Sorethroat").
- **Disease Name:** Enter a string representing the name of the disease from the following options only ("Fever", "Commoncold", "Flu").

### Output

The program will print the predicted class labels along with the actual labels for the provided testing data.

### Exiting the Program

To exit the program, choose option 3 in the menu.

## Makefile

The `Makefile` provided allows for compiling and cleaning the project. Use the following commands:

- **To compile the project:**

    ```bash
    make
    ```

- **To clean up the build artifacts:**

    ```bash
    make clean
    ```

**Note for Windows Users:**
The `make clean` command may not work on Windows systems due to differences in command syntax. Instead, manually delete the following files:

- `main`
- `main.o`
- `classification/decision_tree_classifier.o`

To remove these files, you can use the following commands in Command Prompt:

```cmd
del main
del main.o
del classification\decision_tree_classifier.o
