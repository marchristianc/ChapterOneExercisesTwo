#include <stdio.h>
#include <stdlib.h> //exit
#include <ctype.h> //atoi
#include <stdbool.h> 

bool openfiles(int argc, const char *argv[], FILE** fin, FILE** fout);
void closefiles(FILE* fin, FILE* fout);

int main(int argc, const char *argv[]){
    FILE* fin;
    FILE* fout;
    bool ok = openfiles(argc, argv, &fin, &fout);
    if(!ok){
        fprintf(stderr, "Invalid usage: ./comment input output\n");
        exit(1);
    }

    int c;
    int d;
    bool inString = false;
    bool inComment = false;
    bool inMultiComment = false;
    while((c = fgetc(fin)) != EOF){
        if(c == '\n' && !inMultiComment){
            inComment = false;
            fputc('\n', fout);
        }else if(c == '"'){
            inString = !inString;
            fputc(c, fout);
        }else if(c == '/' && !inString){
            d = fgetc(fin);
            if(d == '/')
                inComment = true;
            else if(d == '*')
                inMultiComment = true;
            else{
                fputc(c, fout);
                fputc(d, fout);
            }
        }else if(inMultiComment && c == '*'){
            if((d = fgetc(fin) == '/'))
                inMultiComment = false;
        }else if(!inComment && !inMultiComment){
            fputc(c, fout);
        }
    }

    closefiles(fin, fout);
    return 0;
}

bool openfiles(int argc, const char *argv[], FILE** fin, FILE** fout){
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
    return true;
}

void closefiles(FILE* fin, FILE* fout){
    fclose(fin);
    fclose(fout);
}