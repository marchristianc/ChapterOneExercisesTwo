#include <stdio.h>
#include <stdlib.h> //exit
#include <ctype.h> //atoi
#include <stdbool.h> 

bool openfiles(int argc, const char *argv[], FILE** fin, FILE** fout, int *n);
void closefiles(FILE* fin, FILE* fout);

int main(int argc, const char *argv[]){
    FILE* fin;
    FILE* fout;
    int n;
    bool ok = openfiles(argc, argv, &fin, &fout, &n);
    if(!ok){
        fprintf(stderr, "Invalid usage: ./fold input output [n]\n");
        exit(1);
    }

    int c;
    int ctr = 0;
    while((c = fgetc(fin)) != EOF){
        if(c == '\n'){
            fputc(c, fout);
            ctr = 0;
        }else if(c != ' ' && c != '\t' && ctr < n){
            ctr++;
            fputc(c, fout);
        }else if(ctr >= n){
            fputc('\n', fout);
            fputc(c, fout);
            ctr = 1;
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