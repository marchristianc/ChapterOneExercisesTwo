#include <stdio.h>
#include <stdlib.h> //exit
#include <ctype.h> //atoi
#include <stdbool.h> 

bool openfiles(int argc, const char *argv[], FILE** fin, FILE** fout, int *n);
void closefiles(FILE* fin, FILE* fout);

void entab(FILE* fin, FILE* fout, int tab_interval){
    int counter = 1, c;
    while((c = fgetc(fin)) != EOF && c == ' ') { counter++; }
    int ntabs = counter / tab_interval;
    int nspaces = counter % tab_interval;
    while(ntabs-- > 0) { fputc('\t', fout); }
    while(nspaces-- > 0) { fputc(' ', fout); }
    fputc(c, fout);
}

int main(int argc, const char *argv[]){
    FILE* fin;
    FILE* fout;
    int n; //DEFAULT: 10 for tabs
    bool ok = openfiles(argc, argv, &fin, &fout, &n);
    if(!ok){
        fprintf(stderr, "Invalid usage: ./entab input output [n]\n");
        exit(1);
    }

    int c;
    while((c = fgetc(fin)) != EOF){
        if(c != ' ') { fputc(c, fout); }
        else{
            entab(fin, fout, n);
        }
    }

    closefiles(fin, fout);
    return 0;
}

bool openfiles(int argc, const char *argv[], FILE** fin, FILE** fout, int *n){
    if(argc < 3)
        return false;
    *fin = fopen(argv[1], "r");
    if(*fin == NULL)
        return false;
    *fout = fopen(argv[2], "w");
    if(*fout == NULL){
        fclose(*fin);
        return false;
    }
    *n = (argc == 4) ? atoi(argv[3]) : 10;
    return true;
}

void closefiles(FILE* fin, FILE* fout){
    fclose(fin);
    fclose(fout);
}