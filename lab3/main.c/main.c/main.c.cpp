#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

typedef struct _Person {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    int birthYear;
    struct _Person* next;
} Person;

// Prototipi funkcija
Person* addAtBeginning(Person* head);
Person* addAtEnd(Person* head);
Person* addAfter(Person* head, const char* lastName);
Person* addBefore(Person* head, const char* lastName);
Person* sortByLastName(Person* head);
void printList(Person* head);
Person* findByLastName(Person* head, const char* lastName);
Person* deleteByLastName(Person* head, const char* lastName);
void writeToFile(Person* head, const char* fileName);
Person* readFromFile(const char* fileName);

// Kreiranje nove osobe
Person* createPerson() {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Greška pri alokaciji memorije!\n");
        return NULL;
    }

    printf("Unesi ime: ");
    scanf("%s", newPerson->firstName);
    printf("Unesi prezime: ");
    scanf("%s", newPerson->lastName);
    printf("Unesi godinu rođenja: ");
    scanf("%d", &newPerson->birthYear);
    newPerson->next = NULL;

    return newPerson;
}

int main() {
    Person* head = NULL;

    printf("Dodavanje dvoje osoba na početak:\n");
    head = addAtBeginning(head);
    head = addAtBeginning(head);

    printf("\nDodavanje jedne osobe na kraj:\n");
    head = addAtEnd(head);

    printf("\nTrenutna lista:\n");
    printList(head);

    printf("\nDodavanje nove osobe IZA određene osobe:\n");
    char afterLastName[MAX_LENGTH];
    printf("Unesi prezime osobe iza koje želiš dodati novu osobu: ");
    scanf("%s", afterLastName);
    head = addAfter(head, afterLastName);

    printf("\nDodavanje nove osobe ISPRED određene osobe:\n");
    char beforeLastName[MAX_LENGTH];
    printf("Unesi prezime osobe ispred koje želiš dodati novu osobu: ");
    scanf("%s", beforeLastName);
    head = addBefore(head, beforeLastName);

    printf("\nLista nakon dodavanja elemenata:\n");
    printList(head);

    printf("\nSortiranje liste po prezimenima...\n");
    head = sortByLastName(head);

    printf("\nSortirana lista:\n");
    printList(head);

    printf("\nBrisanje osobe iz liste:\n");
    char delLastName[MAX_LENGTH];
    printf("Unesi prezime osobe koju želiš obrisati: ");
    scanf("%s", delLastName);
    head = deleteByLastName(head, delLastName);

    printf("\nLista nakon brisanja:\n");
    printList(head);

    printf("\nUpis liste u datoteku 'osobe.txt'...\n");
    writeToFile(head, "osobe.txt");

    printf("\nUčitavanje liste iz datoteke 'osobe.txt'...\n");
    Person* fileList = readFromFile("osobe.txt");

    printf("\nLista učitana iz datoteke:\n");
    printList(fileList);

    // Oslobađanje memorije
    Person* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
    while (fileList) {
        temp = fileList;
        fileList = fileList->next;
        free(temp);
    }

    printf("\nProgram je završio s radom.\n");
    return 0;
}

// Funkcije

Person* addAtBeginning(Person* head) {
    Person* newPerson = createPerson();
    if (!newPerson) return head;
    newPerson->next = head;
    return newPerson;
}

Person* addAtEnd(Person* head) {
    Person* newPerson = createPerson();
    if (!newPerson) return head;
    if (head == NULL) return newPerson;

    Person* temp = head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newPerson;
    return head;
}

void printList(Person* head) {
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return;
    }
    Person* temp = head;
    while (temp) {
        printf("%s %s (%d)\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
}

Person* findByLastName(Person* head, const char* lastName) {
    while (head) {
        if (strcmp(head->lastName, lastName) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

Person* deleteByLastName(Person* head, const char* lastName) {
    if (!head) return NULL;
    Person* temp = head;
    Person* prev = NULL;

    while (temp && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Osoba nije pronađena.\n");
        return head;
    }

    if (!prev)
        head = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    printf("Osoba je obrisana.\n");
    return head;
}

Person* addAfter(Person* head, const char* lastName) {
    Person* found = findByLastName(head, lastName);
    if (!found) {
        printf("Osoba nije pronađena.\n");
        return head;
    }
    Person* newPerson = createPerson();
    newPerson->next = found->next;
    found->next = newPerson;
    return head;
}

Person* addBefore(Person* head, const char* lastName) {
    if (!head) return NULL;

    if (strcmp(head->lastName, lastName) == 0)
        return addAtBeginning(head);

    Person* prev = NULL;
    Person* curr = head;
    while (curr && strcmp(curr->lastName, lastName) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        printf("Osoba nije pronađena.\n");
        return head;
    }

    Person* newPerson = createPerson();
    newPerson->next = curr;
    prev->next = newPerson;
    return head;
}

Person* sortByLastName(Person* head) {
    if (!head || !head->next) return head;

    int swapped;
    Person* ptr1;
    Person* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->lastName, ptr1->next->lastName) > 0) {
                char tempFirst[MAX_LENGTH], tempLast[MAX_LENGTH];
                int tempYear;

                strcpy(tempFirst, ptr1->firstName);
                strcpy(tempLast, ptr1->lastName);
                tempYear = ptr1->birthYear;

                strcpy(ptr1->firstName, ptr1->next->firstName);
                strcpy(ptr1->lastName, ptr1->next->lastName);
                ptr1->birthYear = ptr1->next->birthYear;

                strcpy(ptr1->next->firstName, tempFirst);
                strcpy(ptr1->next->lastName, tempLast);
                ptr1->next->birthYear = tempYear;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    return head;
}

void writeToFile(Person* head, const char* fileName) {
    FILE* f = fopen(fileName, "w");
    if (!f) {
        printf("Greška pri otvaranju datoteke!\n");
        return;
    }

    while (head) {
        fprintf(f, "%s %s %d\n", head->firstName, head->lastName, head->birthYear);
        head = head->next;
    }

    fclose(f);
    printf("Lista je zapisana u datoteku '%s'.\n", fileName);
}

Person* readFromFile(const char* fileName) {
    FILE* f = fopen(fileName, "r");
    if (!f) {
        printf("Greška pri otvaranju datoteke!\n");
        return NULL;
    }

    Person* head = NULL;
    Person* last = NULL;
    Person temp;

    while (fscanf(f, "%s %s %d", temp.firstName, temp.lastName, &temp.birthYear) == 3) {
        Person* newPerson = (Person*)malloc(sizeof(Person));
        strcpy(newPerson->firstName, temp.firstName);
        strcpy(newPerson->lastName, temp.lastName);
        newPerson->birthYear = temp.birthYear;
        newPerson->next = NULL;

        if (!head)
            head = newPerson;
        else
            last->next = newPerson;
        last = newPerson;
    }

    fclose(f);
    printf("Lista je učitana iz datoteke '%s'.\n", fileName);
    return head;
}


 