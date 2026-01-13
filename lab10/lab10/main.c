#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

/* ---------- STRUCTURES ---------- */

typedef struct City {
    char name[MAX];
    int population;
    struct City* left;
    struct City* right;
} City;

typedef struct CityList {
    char name[MAX];
    int population;
    struct CityList* next;
} CityList;

typedef struct Country {
    char name[MAX];
    City* cityBST;           
    CityList* cityList;      
    struct Country* next;   
    struct Country* left;    
    struct Country* right;
} Country;

/* ---------- CITY BST FUNCTIONS ---------- */

City* insertCityBST(City* root, char* name, int population) {
    if (!root) {
        root = malloc(sizeof(City));
        strcpy(root->name, name);
        root->population = population;
        root->left = root->right = NULL;
        return root;
    }

    if (population < root->population ||
        (population == root->population &&
            strcmp(name, root->name) < 0))
        root->left = insertCityBST(root->left, name, population);
    else
        root->right = insertCityBST(root->right, name, population);

    return root;
}

void printCityBST(City* root) {
    if (!root) return;
    printCityBST(root->left);
    printf("    %s (%d)\n", root->name, root->population);
    printCityBST(root->right);
}

void searchCityBST(City* root, int minPopulation) {
    if (!root) return;
    searchCityBST(root->left, minPopulation);
    if (root->population > minPopulation)
        printf("    %s (%d)\n", root->name, root->population);
    searchCityBST(root->right, minPopulation);
}

/* ---------- CITY LIST FUNCTIONS ---------- */

CityList* insertCityList(CityList* head, char* name, int population) {
    CityList* newCity = malloc(sizeof(CityList));
    strcpy(newCity->name, name);
    newCity->population = population;
    newCity->next = NULL;

    if (!head || population < head->population ||
        (population == head->population &&
            strcmp(name, head->name) < 0)) {
        newCity->next = head;
        return newCity;
    }

    CityList* temp = head;
    while (temp->next &&
        (temp->next->population < population ||
            (temp->next->population == population &&
                strcmp(temp->next->name, name) < 0)))
        temp = temp->next;

    newCity->next = temp->next;
    temp->next = newCity;
    return head;
}

void printCityList(CityList* head) {
    while (head) {
        printf("    %s (%d)\n", head->name, head->population);
        head = head->next;
    }
}

void searchCityList(CityList* head, int minPopulation) {
    while (head) {
        if (head->population > minPopulation)
            printf("    %s (%d)\n", head->name, head->population);
        head = head->next;
    }
}

/* ---------- COUNTRY LINKED LIST (part a) ---------- */

Country* insertCountryList(Country* head, char* name) {
    Country* newCountry = malloc(sizeof(Country));
    strcpy(newCountry->name, name);
    newCountry->cityBST = NULL;
    newCountry->next = NULL;

    if (!head || strcmp(name, head->name) < 0) {
        newCountry->next = head;
        return newCountry;
    }

    Country* temp = head;
    while (temp->next && strcmp(temp->next->name, name) < 0)
        temp = temp->next;

    newCountry->next = temp->next;
    temp->next = newCountry;
    return head;
}

/* ---------- COUNTRY BST (part b) ---------- */

Country* insertCountryBST(Country* root, char* name) {
    if (!root) {
        root = malloc(sizeof(Country));
        strcpy(root->name, name);
        root->cityList = NULL;
        root->left = root->right = NULL;
        return root;
    }

    if (strcmp(name, root->name) < 0)
        root->left = insertCountryBST(root->left, name);
    else
        root->right = insertCountryBST(root->right, name);

    return root;
}

/* ---------- LOAD CITIES FROM FILE ---------- */

void loadCitiesBST(char* filename, City** root) {
    FILE* file = fopen(filename, "r");
    char name[MAX];
    int population;

    while (fscanf(file, " %[^,],%d", name, &population) == 2)
        *root = insertCityBST(*root, name, population);

    fclose(file);
}

void loadCitiesList(char* filename, CityList** head) {
    FILE* file = fopen(filename, "r");
    char name[MAX];
    int population;

    while (fscanf(file, " %[^,],%d", name, &population) == 2)
        *head = insertCityList(*head, name, population);

    fclose(file);
}

/* ---------- PRINT COUNTRIES ---------- */

void printCountryList(Country* head) {
    while (head) {
        printf("%s:\n", head->name);
        printCityBST(head->cityBST);
        head = head->next;
    }
}


void printCountryBST(Country* root) {
    if (!root) return;
    printCountryBST(root->left);
    printf("%s:\n", root->name);
    printCityList(root->cityList);
    printCountryBST(root->right);
}

/* ---------- MAIN ---------- */

int main() {
    FILE* file = fopen("countries.txt", "r");
    if (!file) {
        printf("Cannot open countries.txt\n");
        return 1;
    }

    char countryName[MAX], cityFile[MAX];
    int minPopulation;

    Country* countryList = NULL;
    Country* countryBST = NULL;

    while (fscanf(file, "%s %s", countryName, cityFile) == 2) {
        countryList = insertCountryList(countryList, countryName);
        loadCitiesBST(cityFile, &countryList->cityBST);

        countryBST = insertCountryBST(countryBST, countryName);
        loadCitiesList(cityFile, &countryBST->cityList);
    }
    fclose(file);

    printf("\n--- a) Linked list of countries + City BST ---\n");
    printCountryList(countryList);

    printf("\n--- b) Country BST + City linked list ---\n");
    printCountryBST(countryBST);

    printf("\nEnter minimum population: ");
    scanf("%d", &minPopulation);

    printf("\nSearch result (a):\n");
    searchCityBST(countryList->cityBST, minPopulation);

    printf("\nSearch result (b):\n");
    searchCityList(countryBST->cityList, minPopulation);

    return 0;
}

