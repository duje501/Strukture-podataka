#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define TABLE_SIZE 11

typedef struct City {
    char name[MAX];
    int population;
    struct City* left;
    struct City* right;
} City;

typedef struct Country {
    char name[MAX];
    City* cityRoot;
    struct Country* next;
} Country;

int hash(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += (int)name[i];
    return sum % TABLE_SIZE;
}

City* insertCity(City* root, char* name, int population) {
    if (root == NULL) {
        City* newCity = malloc(sizeof(City));
        if (!newCity) return NULL;
        strcpy(newCity->name, name);
        newCity->population = population;
        newCity->left = newCity->right = NULL;
        return newCity;
    }

    if (population < root->population ||
        (population == root->population &&
            strcmp(name, root->name) < 0))
        root->left = insertCity(root->left, name, population);
    else
        root->right = insertCity(root->right, name, population);

    return root;
}

void printCities(City* root) {
    if (!root) return;
    printCities(root->left);
    printf("    %s (%d)\n", root->name, root->population);
    printCities(root->right);
}

Country* insertCountry(Country* head, char* name) {
    Country* newC = malloc(sizeof(Country));
    if (!newC) return head;

    strcpy(newC->name, name);
    newC->cityRoot = NULL;
    newC->next = NULL;

    if (!head || strcmp(name, head->name) < 0) {
        newC->next = head;
        return newC;
    }

    Country* temp = head;
    while (temp->next && strcmp(temp->next->name, name) < 0)
        temp = temp->next;

    newC->next = temp->next;
    temp->next = newC;
    return head;
}

Country* findCountry(Country* head, char* name) {
    while (head) {
        if (strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

void loadCities(char* filename, City** root) {
    FILE* f = fopen(filename, "r");
    if (!f) return;

    char name[MAX];
    int population;

    while (fscanf(f, " %[^,],%d", name, &population) == 2)
        *root = insertCity(*root, name, population);

    fclose(f);
}

void printHashTable(Country* table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("\n[%d]\n", i);
        Country* temp = table[i];
        while (temp) {
            printf("%s:\n", temp->name);
            printCities(temp->cityRoot);
            temp = temp->next;
        }
    }
}

int main() {
    FILE* file = fopen("countries.txt", "r");
    if (!file) {
        printf("Cannot open countries.txt\n");
        return 1;
    }

    Country* hashTable[TABLE_SIZE] = { NULL };
    char country[MAX], cityFile[MAX];

    while (fscanf(file, "%s %s", country, cityFile) == 2) {
        int index = hash(country);

        Country* c = findCountry(hashTable[index], country);
        if (!c) {
            hashTable[index] = insertCountry(hashTable[index], country);
            c = findCountry(hashTable[index], country);
        }

        loadCities(cityFile, &c->cityRoot);
    }

    fclose(file);
    printHashTable(hashTable);
    return 0;
}
