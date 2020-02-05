#include <stdio.h>
#include <stdlib.h> //exit
#include <ctype.h> //atoi
#include <stdbool.h> 

bool openfiles(int argc, const char *argv[], FILE** fin, FILE** fout, int *n);
void closefiles(FILE* fin, FILE* fout);

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
    int spaces = 0;
    int charPresent = 0;
    while((c = fgetc(fin)) != EOF){
        if(c == ' '){
            spaces++;
        }else{
            //calculate if we can fit any tabs into the space count
            //tab_size - one character present
            for(; spaces >= n-charPresent; spaces -= n)
                fputc('\t', fout);

            //remaining spaces
            for(; spaces > 0; spaces--)
                fputc(' ', fout);

            charPresent = 0;
            if(c != '\t')
                charPresent++;
            fputc(c, fout);
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