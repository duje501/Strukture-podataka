#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Artikal {
    char naziv[100];
    int kolicina;
    float cijena;
    struct Artikal* next;
} Artikal;

typedef struct Racun {
    char datum[20];
    Artikal* artikli;
    struct Racun* next;
} Racun;

void ubaci_artikal(Artikal** head, Artikal* novi) {
    if (*head == NULL || strcmp(novi->naziv, (*head)->naziv) < 0) {
        novi->next = *head;
        *head = novi;
        return;
    }
    Artikal* t = *head;
    while (t->next && strcmp(novi->naziv, t->next->naziv) > 0) t = t->next;
    novi->next = t->next;
    t->next = novi;
}

void ubaci_racun(Racun** head, Racun* novi) {
    if (*head == NULL || strcmp(novi->datum, (*head)->datum) < 0) {
        novi->next = *head;
        *head = novi;
        return;
    }
    Racun* t = *head;
    while (t->next && strcmp(novi->datum, t->next->datum) > 0) t = t->next;
    novi->next = t->next;
    t->next = novi;
}

int main() {
    FILE* f = fopen("racuni.txt", "r");
    if (!f) return 1;

    Racun* lista = NULL;
    char imeDatoteke[200];

    while (fscanf(f, "%s", imeDatoteke) == 1) {
        FILE* r = fopen(imeDatoteke, "r");
        if (!r) continue;

        Racun* novi = malloc(sizeof(Racun));
        novi->artikli = NULL;
        novi->next = NULL;
        fgets(novi->datum, 20, r);
        novi->datum[strcspn(novi->datum, "\n")] = 0;

        char linija[300];
        while (fgets(linija, 300, r)) {
            Artikal* a = malloc(sizeof(Artikal));
            a->next = NULL;
            sscanf(linija, "%[^,],%d,%f", a->naziv, &a->kolicina, &a->cijena);
            ubaci_artikal(&novi->artikli, a);
        }
        fclose(r);

        ubaci_racun(&lista, novi);
    }
    fclose(f);

    float ukupno = 0;
    Racun* r = lista;
    while (r) {
        Artikal* a = r->artikli;
        while (a) {
            ukupno += a->kolicina * a->cijena;
            a = a->next;
        }
        r = r->next;
    }

    printf("Ukupno potroseno: %.2f\n", ukupno);
    return 0;
}
