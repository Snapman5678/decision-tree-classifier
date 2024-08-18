#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "extraction.h"

void initLabel(Label *label);
void initLabelHashMap(LabelHashMap *labelMap, int max_rows);
unsigned long hashLabel(char *label, int capacity);
unsigned long hashLabel2(char *label, int capacity);
void insertLabel(LabelHashMap *labelMap, char *label);
int getLabelValue(LabelHashMap *labelMap, char *label);
Column convert_column_to_encoded(Column *column, int rows, LabelHashMap *labelMap);
void hash_encode(DataFrame *df);

