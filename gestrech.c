#include <dirent.h>

#define color(param) printf("\033[%sm",param)

int my_strlen(char*);
int my_strcmp(char*, char*);
char* my_strcat(char*, char*);              // Malloc !
char* my_tolower(char* word);               // Malloc !
int getLesDicos();                          // Retourne le nombre de dico & les affiches
char* getNomDico(int);                      // Retourne le nom du dico sans le .txt (int:numDico)
int countWordInFile(char*);                 // Retourne le nombre de mot dans le fichier char*
char** getWords(char*, char*, int);
char** getWordSeuil(char*, char*, int, int*);
int getDiff(char*, char*);
short isNewLetter(char*, char, int);
short letterInOtherWord(char, char*, int);


struct CountLettre {
    char lettre;
    int count;
};
typedef struct CountLettre CountLettre;


int my_strlen(char* word) {
    int count = 0;

    while ( word[count] != '\0' ) {
        count ++;
    }

    return count;
}

int my_strcmp(char* wordA, char* wordB) {
    int count = 0;
    int i = 0;
    int nbLettreA = my_strlen(wordA);
    int nbLettreB = my_strlen(wordB);
    int diff = nbLettreB - nbLettreA;

    while (wordA[i] != '\0') {
        if (wordA[i] != wordB[i] ) {
            count ++;
        }
        i++;
    }

    if (diff > 0 ) {
        count += diff;
    }

    return count;
}

char* my_tolower(char* word) {
    char* lower = malloc(sizeof(char)*(my_strlen(word)+1));
    int i;

    for (i=0; i< my_strlen(word); i++) {
        if (word[i] >= 65 && word[i] <= 90) {
            lower[i] = word[i] + 32;
        } else {
            lower[i] = word[i];
        }
    }
    lower[i] = '\0';

    return lower;
}

char* my_strcat(char* wordA, char* wordB) {
    int lenthA = my_strlen(wordA);
    int lenthB = my_strlen(wordB);
    char* result = malloc(sizeof(char) * (lenthA + lenthB + 1));

    int a = 0;
    int b = 0;
    int i;

    for (i = 0; i < lenthA + lenthB; i ++) {
            if (a < lenthA) {
                result[i] = wordA[a];
                a++;
            } else {
                result[i] = wordB[b];
                b++;
            }
    }
    result[i] = '\0';

    return result;
}


int getLesDicos() {
    DIR* directory = opendir("LesDictionnaires");
    struct dirent* file = NULL;
    int count = 0;

    char* name;
    printf("Les dictionnaires disponible: \n\n");
    color("33");
    if ( directory != NULL ) {
        while ( file = readdir(directory) ) {
            if ( (my_strcmp(file->d_name, ".") != 0) && (my_strcmp(file->d_name, "..") != 0) ) {
                name = file->d_name;
                name[my_strlen(name)-4] = '\0';
                count++;
                printf("%d. %s\n", count, name);
            }
        }
        closedir(directory);
    }
    return count;
}

char* getNomDico(int numDico)  {
    DIR* directory = opendir("LesDictionnaires");
    struct dirent* file = NULL;
    int count = 1;

    char* name;

    if ( directory != NULL ) {
        while ( file = readdir(directory) ) {
            if ( (my_strcmp(file->d_name, ".") != 0) && (my_strcmp(file->d_name, "..") != 0) ) {
                if ( count == numDico ) {
                    name = file->d_name;
                    name[my_strlen(name)-4] = '\0';
                    break;
                }
                count ++;
            }
        }
        closedir(directory);
    }
    return name;
}

int countWordInFile(char* fileName) {
    char *str = malloc(sizeof(char) * 25);
    int countWord = 0;

    FILE* file = fopen(fileName, "r");
    while (!feof(file)) {
        fscanf(file, "%s", str);
        countWord ++;
    }
    free(str);
    return countWord;
}

char** getWordSeuil(char* nomDico, char* word, int seuil, int* size) {
    char* pathDico = "";
    pathDico = my_strcat(pathDico, "LesDictionnaires/");
    pathDico = my_strcat(pathDico, nomDico);
    pathDico = my_strcat(pathDico, ".txt");

    *size = countWordInFile(pathDico);
    char** result = malloc(sizeof(char[25]) * *size);
    int i = 0;
    int diff;

    FILE* dico = fopen(pathDico, "r");
    if (dico) {
        while(!feof(dico)) {
            char* currentWord = malloc(sizeof(char) * 25);
            fscanf(dico, "%s", currentWord);
            diff = getDiff(currentWord, my_tolower(word));
            if (diff <= seuil){
                result[i] = currentWord;
                i ++;
            }
        }
    }
    *size = i;
    return result;
}

int getDiff(char* a, char* b) {
    int result = 0;
    int i;
    int j;
    int lengthA = my_strlen(a);
    int lengthB = my_strlen(b);
    int count;
    int nbLettreA = 0;
    int nbLettreB = 0;

    CountLettre lettresA[lengthA];
    CountLettre lettresB[lengthB];
    //Count de chaque lettre de a dans la structure
    for (i = 0; i < lengthA; i ++) {
        count = 0;
        if ( isNewLetter(a, a[i], i) ) {
            for (j = 0; j < lengthA; j ++) {
                if ( a[i] == a[j] ) {
                    count ++;
                }
            }
            lettresA[nbLettreA].count = count;
            lettresA[nbLettreA].lettre = a[i];
            nbLettreA ++;
        }
    }

    //Count de chaque lettre de b dans la structure
    for (i = 0; i < lengthB; i ++) {
        count = 0;
        if ( isNewLetter(b, b[i], i) ) {
            for (j = 0; j < lengthB; j ++) {
                if ( b[i] == b[j] ) {
                    count ++;
                }
            }
            lettresB[nbLettreB].count = count;
            lettresB[nbLettreB].lettre = b[i];
            nbLettreB ++;
        }
    }



    if (nbLettreA >= nbLettreB) {
        for (i = 0; i < nbLettreA; i ++) {
            if (!letterInOtherWord(lettresA[i].lettre, b, nbLettreA)) {
                result += lettresA[i].count;
            } else {
                for (j = 0; j < nbLettreB; j ++) {
                    if (lettresA[i].lettre == lettresB[j].lettre) {
                        if (lettresA[i].count >= lettresB[j].count) {
                            result += lettresA[i].count - lettresB[j].count;
                        } else {
                            result += lettresB[j].count - lettresA[i].count;
                        }
                    }
                }
            }
        }
    } else {
        for (i = 0; i < nbLettreB; i ++) {
            if (!letterInOtherWord(lettresB[i].lettre, a, nbLettreB)) {
                result += lettresB[i].count;
            } else {
                for (j = 0; j < nbLettreA; j ++) {
                    if (lettresB[i].lettre == lettresA[j].lettre) {
                        if (lettresB[i].count >= lettresA[j].count) {
                            result += lettresB[i].count - lettresA[j].count;
                        } else {
                            result += lettresA[j].count - lettresB[i].count;
                        }
                    }
                }
            }
        }
    }

    return result;
}

short isNewLetter(char* a, char letter, int i) {
    short result;
    int j;

    //printf("%c, %d\n", letter, i);
    result = 1;
    for (j = 0; j < i; j ++) {
        if (a[j] == letter) {
            result = 0;
            break;
        }
    }

    return result;
}

short letterInOtherWord(char lettre, char* word, int lengthWord) {
    short result = 0;
    int i;

    for ( i = 0 ; i < lengthWord; i ++) {
        if ( word[i] == lettre ) {
            result = 1;
        }
    }

    return result;
}


