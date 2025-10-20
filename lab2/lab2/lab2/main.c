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

Person* addAtBeginning(Person* head);
Person* addAtEnd(Person* head);
void printList(Person* head);
Person* findByLastName(Person* head, const char* lastName);
Person* deleteByLastName(Person* head, const char* lastName);

int main() {
    Person* head = NULL;
    char searchLastName[MAX_LENGTH];
    char deleteLastName[MAX_LENGTH];

    // 1️⃣ Add two people at the beginning
    printf("Adding first person at beginning:\n");
    head = addAtBeginning(head);

    printf("\nAdding second person at beginning:\n");
    head = addAtBeginning(head);

    // 2️⃣ Add one person at the end
    printf("\nAdding one person at the end:\n");
    head = addAtEnd(head);

    // 3️⃣ Print the full list
    printf("\nCurrent list:\n");
    printList(head);

    // 4️⃣ Search by last name
    printf("\nEnter last name to search: ");
    scanf("%s", searchLastName);
    Person* found = findByLastName(head, searchLastName);
    if (found)
        printf("Found: %s %s (%d)\n", found->firstName, found->lastName, found->birthYear);
    else
        printf("Person with last name '%s' not found.\n", searchLastName);

    // 5️⃣ Delete person by last name
    printf("\nEnter last name to delete: ");
    scanf("%s", deleteLastName);
    head = deleteByLastName(head, deleteLastName);

    // 6️⃣ Print list again
    printf("\nList after deletion:\n");
    printList(head);

    // 7️⃣ Manual memory cleanup (instead of freeList)
    Person* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}

Person* addAtBeginning(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Memory allocation failed!\n");
        return head;
    }

    printf("Enter first name: ");
    scanf("%s", newPerson->firstName);
    printf("Enter last name: ");
    scanf("%s", newPerson->lastName);
    printf("Enter birth year: ");
    scanf("%d", &newPerson->birthYear);

    newPerson->next = head;
    return newPerson;
}

Person* addAtEnd(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Memory allocation failed!\n");
        return head;
    }

    printf("Enter first name: ");
    scanf("%s", newPerson->firstName);
    printf("Enter last name: ");
    scanf("%s", newPerson->lastName);
    printf("Enter birth year: ");
    scanf("%d", &newPerson->birthYear);
    newPerson->next = NULL;

    if (head == NULL)
        return newPerson;

    Person* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newPerson;
    return head;
}

void printList(Person* head) {
    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    Person* temp = head;
    while (temp != NULL) {
        printf("%s %s (%d)\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
}

Person* findByLastName(Person* head, const char* lastName) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->lastName, lastName) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

Person* deleteByLastName(Person* head, const char* lastName) {
    if (head == NULL)
        return NULL;

    Person* temp = head;
    Person* prev = NULL;

    while (temp != NULL && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Person with last name '%s' not found.\n", lastName);
        return head;
    }

    if (prev == NULL)
        head = temp->next;  // deleting first element
    else
        prev->next = temp->next;

    printf("Person '%s' deleted from list.\n", lastName);
    free(temp);
    return head;
}