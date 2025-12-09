#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>



typedef struct Polinom {
    int koef;
    int exp;
    struct Polinom* next;
} Polinom;

Polinom* createNode(int koef, int exp) {
    Polinom* newNode = (Polinom*)malloc(sizeof(Polinom));
    newNode->koef = koef;
    newNode->exp = exp;
    newNode->next = NULL;
    return newNode;
}

void insertSorted(Polinom* head, int koef, int exp) {
    if (koef == 0) return;

    Polinom* prev = head;
    Polinom* curr = head->next;

    while (curr != NULL && curr->exp > exp) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL && curr->exp == exp) {
        curr->koef += koef;
        if (curr->koef == 0) {
            prev->next = curr->next;
            free(curr);
        }
    }
    else {
        Polinom* newNode = createNode(koef, exp);
        newNode->next = curr;
        prev->next = newNode;
    }
}

void readPolynom(FILE* fp, Polinom* head) {
    int koef, exp;
    char buffer[256];
    fgets(buffer, sizeof(buffer), fp);
    char* ptr = buffer;

    while (sscanf(ptr, "%d %d", &koef, &exp) == 2) {
        insertSorted(head, koef, exp);
        while (*ptr != ' ' && *ptr != '\0') ptr++;
        while (*ptr == ' ') ptr++;
        while (*ptr != ' ' && *ptr != '\0') ptr++;
        while (*ptr == ' ') ptr++;
    }
}

Polinom* addPoly(Polinom* p1, Polinom* p2) {
    Polinom* res = createNode(0, 0);
    Polinom* t1 = p1->next;
    Polinom* t2 = p2->next;

    while (t1 != NULL) {
        insertSorted(res, t1->koef, t1->exp);
        t1 = t1->next;
    }

    while (t2 != NULL) {
        insertSorted(res, t2->koef, t2->exp);
        t2 = t2->next;
    }

    return res;
}

Polinom* multiplyPoly(Polinom* p1, Polinom* p2) {
    Polinom* res = createNode(0, 0);
    for (Polinom* t1 = p1->next; t1 != NULL; t1 = t1->next) {
        for (Polinom* t2 = p2->next; t2 != NULL; t2 = t2->next) {
            insertSorted(res, t1->koef * t2->koef, t1->exp + t2->exp);
        }
    }
    return res;
}

void printPoly(Polinom* p) {
    Polinom* t = p->next;
    if (!t) {
        printf("0\n");
        return;
    }

    while (t != NULL) {
        printf("%dx^%d ", t->koef, t->exp);
        t = t->next;
        if (t != NULL) printf("+ ");
    }
    printf("\n");
}

int main() {
    FILE* fp = fopen("polinomi.txt", "r");
    if (!fp) return 1;

    Polinom* P1 = createNode(0, 0);
    Polinom* P2 = createNode(0, 0);

    readPolynom(fp, P1);
    readPolynom(fp, P2);
    fclose(fp);

    printf("P1(x) = ");
    printPoly(P1);
    printf("P2(x) = ");
    printPoly(P2);

    Polinom* S = addPoly(P1, P2);
    Polinom* M = multiplyPoly(P1, P2);

    printf("\nS(x) = ");
    printPoly(S);

    printf("M(x) = ");
    printPoly(M);

    return 0;
}
