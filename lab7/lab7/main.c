#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dir {
    char name[50];
    struct Dir* child;
    struct Dir* sibling;
    struct Dir* parent;
} Dir;

Dir* createDir(char* name, Dir* parent) {
    Dir* newDir = (Dir*)malloc(sizeof(Dir));
    strcpy(newDir->name, name);
    newDir->child = NULL;
    newDir->sibling = NULL;
    newDir->parent = parent;
    return newDir;
}

void makeDir(Dir* current) {
    char name[50];
    printf("Ime direktorija: ");
    scanf("%s", name);

    Dir* newDir = createDir(name, current);

    if (current->child == NULL) {
        current->child = newDir;
    }
    else {
        Dir* temp = current->child;
        while (temp->sibling != NULL)
            temp = temp->sibling;
        temp->sibling = newDir;
    }
}

Dir* changeDir(Dir* current) {
    char name[50];
    printf("Ime direktorija: ");
    scanf("%s", name);

    Dir* temp = current->child;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->sibling;
    }

    printf("Direktorij ne postoji!\n");
    return current;
}

void listDir(Dir* current) {
    Dir* temp = current->child;
    if (temp == NULL) {
        printf("Direktorij je prazan.\n");
        return;
    }

    while (temp != NULL) {
        printf("%s\n", temp->name);
        temp = temp->sibling;
    }
}

int main() {
    Dir* root = createDir("C:", NULL);
    Dir* current = root;
    int choice;

    while (1) {
        printf("\n[%s]> \n", current->name);
        printf("1 - md\n");
        printf("2 - cd dir\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - izlaz\n");
        printf("Izbor: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            makeDir(current);
            break;
        case 2:
            current = changeDir(current);
            break;
        case 3:
            if (current->parent != NULL)
                current = current->parent;
            else
                printf("Vec ste u root direktoriju!\n");
            break;
        case 4:
            listDir(current);
            break;
        case 5:
            return 0;
        default:
            printf("Nepoznata opcija!\n");
        }
    }
}
