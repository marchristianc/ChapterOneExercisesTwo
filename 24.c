#include <stdio.h>
#include <stdlib.h> //exit
#include <ctype.h> //atoi
#include <stdbool.h> 

bool openfiles(int argc, const char *argv[], FILE** fin);
void closefiles(FILE* fin);

int main(int argc, const char *argv[]){
    FILE* fin;
    bool ok = openfiles(argc, argv, &fin);
    if(!ok){
        fprintf(stderr, "Invalid usage: ./syntax input\n");
        exit(1);
    }

    /*parantheses, brackets, braces, quotes (single and double), escape seq, 
        comments */

    int c;
    int paraCount = 0;
    int brackCount = 0;
    int braceCount = 0;
    int singleQCount = 0;
    int doubleQCount = 0;
    bool inComment = false;
    bool inMultiComment = false;
    while((c = fgetc(fin)) != EOF){

        //escaped strings in doublequote and singlequote
        if(doubleQCount%2 != 0 || singleQCount%2 != 0){
            if(c == '\\'){
                c = fgetc(fin); //skip the escaped string
                continue;
            }
        }

        if(!inComment && !inMultiComment){
            //make sure we're not inside a double quote
            if(c == '\'' && doubleQCount%2 == 0)
                singleQCount++;
            //make sure we're not inside a single quote
            if(c == '"' && singleQCount%2 == 0)
                doubleQCount++;

            //make sure we're not inside a double or single quote
            if(doubleQCount%2 != 0 || singleQCount%2 != 0)
                continue;
        }

        if(doubleQCount%2 == 0 && singleQCount%2 == 0){
            if(!inMultiComment && c == '\n')
                inComment = false;
            else if(c == '*' && inMultiComment){
                if((c = fgetc(fin)) == '/')
                    inMultiComment = false;
            }else if(c == '/'){
                c = fgetc(fin);
                if(c == '/')
                    inComment = true;
                else if(c == '*')
                    inMultiComment = true;
            }
            
            //make sure we're not inside a comment
            if(inComment || inMultiComment)
                continue;
        }
        
        if(c == '(')
            paraCount++;
        if(c == ')')
            paraCount--;
        if(c == '{')
            brackCount++;
        if(c == '}')
            brackCount--;
        if(c == '[')
            braceCount++;
        if(c == ']')
            braceCount--;
    }

    closefiles(fin);

    if(paraCount != 0)
        printf("Parenthesis Mismatch!\n");
    if(brackCount != 0)
        printf("Brackets Mismatch!\n");
    if(braceCount != 0)
        printf("Braces Mismatch!\n");
    if(singleQCount%2 != 0)
        printf("Single Quotes Mismatch!\n");
    if(doubleQCount%2 != 0)
        printf("Double Quotes Mismatch!\n");
    
    if(paraCount == 0 && brackCount == 0 && braceCount == 0 
        && singleQCount%2 == 0 && doubleQCount%2 == 0)
        printf("Syntax is correct!\n");
    return 0;
}

bool openfiles(int argc, const char *argv[], FILE** fin){
    if(argc < 2)
        return false;
    *fin = fopen(argv[1], "r");
    if(*fin == NULL)
        return false;
    return true;
}

void closefiles(FILE* fin){
    fclose(fin);
}