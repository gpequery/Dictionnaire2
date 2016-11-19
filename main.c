#include <stdio.h>
#include <stdlib.h>

#include "gestrech.c"

#define color(param) printf("\033[%sm",param)

void clearConsole(char*);   //Affichage en tete (char* le titre a afficher)
void accueil();             //Gestion des dicos
void menu(int);             //Apres selection d'un dico (int: numDico)
void searchWord(char*, char*, int);

enum {SEARCH_WORD = 1, ACCUEIL, EXIT};

int main() {
    accueil();
    return 0;
}

void clearConsole(char* dico) {
    system("cls");
    color("35");
    printf("Dictionnaire %s\n", dico);
    printf("--------------");

    int size = my_strlen(dico);
    int i;
    for ( i = 0; i < size; i ++ ) {
        printf("-");
    }
    printf("\n\n");
    color("37");
}

void accueil() {
    clearConsole("accueil");

    int countDico = getLesDicos();
    color("36");
    printf("\n");

    printf("%d. Quitter\n\n\n", countDico+1);

    int action;
    color("37");
    printf("Selectionner un dictionnaire: ");
    scanf("%d", &action);


    if (action == countDico + 1 ) {
        system("exit");
    } else {
        if ( action > 0 && action <= countDico) {  //Si sélection d'un dico existant (go menu)
            menu(action);
        } else {
            accueil();                          //Si sélection d'un int non valide (retour accueil)
        }
    }
}

void menu(int numDico) {
    char* nomDico = getNomDico(numDico);
    char inputWord[25];
    int inputSeuil;

    clearConsole(nomDico);

    color("36");
    printf("\n\n1. Rechercher un mot\n");
    printf("2. Retour\n");
    printf("3. Quitter\n\n");

    color("37");
    int action;
    printf("Choisir une action : ");
    scanf("%d", &action);

    switch (action) {
        case SEARCH_WORD :
            searchWord(nomDico, "", -1);
            break;
        case ACCUEIL :
            accueil();
            break;
        case EXIT :
            system("exit");
            break;
        default :
            menu(numDico);
    }
}

void searchWord(char* nomDico, char* word, int seuil) {
    clearConsole(nomDico);

    color("37");
    printf("Liste de mot proche :\n");

    if (seuil >= 0) {
        int i;
        int size;
        char** tab = getWordSeuil(nomDico, word, seuil, &size);

        printf("\n");
        color("36");
        for (i = 0; i < size; i++) {
            if (my_strcmp(word, tab[i]) == 0) {
                color("35");
            }
            printf("%s", tab[i]);
            if (i != size-1) {
                printf(" - ");
            }
            if (my_strcmp(word, tab[i]) == 0) {
                color("36");
            }
        }
        printf("\n");
    }


    char inputWord[25];
    int inputSeuil;

    color("37");
    printf("\nSaisir un mot : ");
    scanf("%s", &inputWord);
    printf("Saisir le seuil : ");
    scanf("%d", &inputSeuil);

    printf("\n\nFIIIIIII\n\n");
    searchWord(nomDico, my_tolower(inputWord), inputSeuil);
}

