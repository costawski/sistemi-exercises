#ifndef SCRUTINIO_H_INCLUDED
#define SCRUTINIO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTI 20
#define LEN_STUDENTE 30 + 1
#define TOT_MATERIE 5
#define TRUE 1
#define FALSE 0

void creaValutazioniStudenteFile(float valutazioni[MAX_STUDENTI][TOT_MATERIE], int pos, float medie[MAX_STUDENTI], char esito[MAX_STUDENTI]);

float float_rand(float min, float max){
    float val;
    //srand(time(NULL));
    val = rand() / (float) RAND_MAX;  // [0, 1.0]
    return min + (val * (max - min));
}
int nuovoStudente(char studenti[MAX_STUDENTI][LEN_STUDENTE], float valutazioni[MAX_STUDENTI][TOT_MATERIE],
                  int pos, float medie[MAX_STUDENTI], char esito[MAX_STUDENTI]) {
    int i, caricato = FALSE, contIn = 0;
    float v;

    medie[pos] = 0;

    if(pos < MAX_STUDENTI){
        printf("\nInserisci nome: ");
        gets(studenti[pos]);

        for(i=0; i<TOT_MATERIE; i++){
            v = float_rand(2, 10);
            valutazioni[pos][i] = v;
            medie[pos] += v;

            if(v < 6) {
                contIn++;
            }
        }

        if(contIn > 3) {
            esito[pos] = 'N';
        }
        else if(contIn >= 1 || contIn <= 3) {
            esito[pos] = 'R';
        }
        else {
            esito[pos] = 'A';
        }

        medie[pos] /= TOT_MATERIE;
        caricato = TRUE;
    }

    return caricato;
}
void stampaValutazioni(char studenti[MAX_STUDENTI][LEN_STUDENTE], float valutazioni[MAX_STUDENTI][TOT_MATERIE],
                       int pos, float medie[MAX_STUDENTI], char esito[MAX_STUDENTI]) {
    int i;
    printf("\nINFO\tTPSIT\tSIS\tITA\tMAT\tMEDIA\tESITO\tALUNNO/A\n");
    for(i=0; i<pos; i++){
        printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%c\t%s\t\n",
               valutazioni[i][0], valutazioni[i][1], valutazioni[i][2],
               valutazioni[i][3], valutazioni[i][4], medie[i], esito[i], studenti[i]);
    }
}
void ordinaStudenti(char studenti[MAX_STUDENTI][LEN_STUDENTE], float valutazioni[MAX_STUDENTI][TOT_MATERIE],
                    int pos, float medie[MAX_STUDENTI], char esito[MAX_STUDENTI]) {
    int posMin;
    float aus;
    char ausStr[LEN_STUDENTE], ausC;

    for(int i = 0; i <= pos - 2; i++) {
        posMin = i;

        for(int j = i + 1; j <= pos - 1; j++) {
            if(strcmp(studenti[posMin], studenti[j]) > 0) {
                posMin = j;
            }
        }

        if(posMin != i) {
            strcpy(ausStr, studenti[i]);
            strcpy(studenti[i], studenti[posMin]);
            strcpy(studenti[posMin], ausStr);

            for(int n = 0; n < TOT_MATERIE; n++) {
                aus = valutazioni[i][n];
                valutazioni[i][n] = valutazioni[posMin][n];
                valutazioni[posMin][n] = aus;
            }

            aus = medie[i];
            medie[i] = medie[posMin];
            medie[posMin] = aus;

            ausC = esito[i];
            esito[i] = esito[posMin];
            esito[posMin] = ausC;
        }
    }
}
void scrutinio (char studenti[MAX_STUDENTI][LEN_STUDENTE], float valutazioni[MAX_STUDENTI][TOT_MATERIE],
                    int pos, char esito[MAX_STUDENTI]) {
    for(int i = 0; i < pos; i++) {
        printf("%s\t%c\t", studenti[i], esito[i]);

        for(int j = 0; j < TOT_MATERIE; j++) {
            if(valutazioni[i][j] < 6) {
                switch(j) {
                case 0:
                    printf("INFO ");
                    break;
                case 1:
                    printf("TPSIT ");
                    break;
                case 2:
                    printf("SIS ");
                    break;
                case 3:
                    printf("ITA ");
                    break;
                case 4:
                    printf("MAT ");
                    break;
                }
            }
        }

        printf("\n");
    }
}
void nuoveValutazioni(float valutazioni[MAX_STUDENTI][TOT_MATERIE], float medie[MAX_STUDENTI], char esito[MAX_STUDENTI], int totStudenti) {
    int nInsuff, somma;
    float v;

    for(int i = 0; i < totStudenti; i++) {
        somma = 0;
        nInsuff = 0;

        for(int j = 0; j < TOT_MATERIE; j++){
            v = float_rand(2, 10);
            valutazioni[i][j] = v;
            somma += v;

            if(v < 6) {
                nInsuff++;
            }
        }

        if(nInsuff > 3) {
            esito[i] = 'N';
        }
        else if(nInsuff >= 1 || nInsuff <= 3) {
            esito[i] = 'R';
        }
        else {
            esito[i] = 'A';
        }

        medie[i] = (somma / totStudenti);
    }
}
void stampaScrutinio(char file_name[], char studenti[MAX_STUDENTI][LEN_STUDENTE], float valutazioni[MAX_STUDENTI][TOT_MATERIE],
                       int totStud, float medie[MAX_STUDENTI], char esito[MAX_STUDENTI]) {
    FILE* fp;
    // "r" => read => lettura
    // "w" => write => scrittua
    // "a" => append => scrittura (senza sovr. dati precedenti)
    // "rb", "wb", "ab", ...
    fp = fopen(file_name, "w");

    if(fp != NULL) {
        for(int i = 0; i < totStud; i++){
            fprintf(fp, "%s;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%c;\n",
               studenti[i], valutazioni[i][0], valutazioni[i][1], valutazioni[i][2],
               valutazioni[i][3], valutazioni[i][4], medie[i], esito[i]);
        }
    }
    else {
        printf("\nImpossibile aprire il file");
    }

    fclose(fp);
}
int leggiFile(char file_name[], char studenti[MAX_STUDENTI][LEN_STUDENTE], float valutazioni[MAX_STUDENTI][TOT_MATERIE],
                       int pos, float medie[MAX_STUDENTI], char esito[MAX_STUDENTI]) { //deve essere funzione
    FILE* fp;
    char cogn[LEN_STUDENTE];

    fp = fopen(file_name, "r");

    if(fp != NULL){
        while(!feof(fp)) {
            fscanf(fp, "%s", cogn);
            //printf("%s\n", cogn);
            //cogn => studenti[i];
            strcpy(studenti[pos], cogn);
            creaValutazioniStudenteFile(valutazioni, pos, medie, esito);
            pos++;
        }
    }
    else
        printf("Impossibile aprire il file\n");

    fclose(fp);

    return pos;
}
void creaValutazioniStudenteFile(float valutazioni[MAX_STUDENTI][TOT_MATERIE], int pos, float medie[MAX_STUDENTI], char esito[MAX_STUDENTI]) {
    int i, contIn = 0;
    float v;

    medie[pos] = 0;

    if(pos < MAX_STUDENTI){
        for(i=0; i<TOT_MATERIE; i++){
            v = float_rand(2, 10);
            valutazioni[pos][i] = v;
            medie[pos] += v;

            if(v < 6) {
                contIn++;
            }
        }

        if(contIn > 3) {
            esito[pos] = 'N';
        }
        else if(contIn >= 1 || contIn <= 3) {
            esito[pos] = 'R';
        }
        else {
            esito[pos] = 'A';
        }

        medie[pos] /= TOT_MATERIE;
    }
}

#endif // SCRUTINIO_H_INCLUDED
