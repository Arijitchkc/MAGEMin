#include "c_func.h"
<<<<<<< HEAD
#include <string.h>
#include <ctype.h>
#include <stdio.h>


char * trim_space(char *str) 
{
    char *end;
    /* skip leading whitespace */
    while (isspace(*str)) {
        str = str + 1;
    }
    /* remove trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end = end - 1;
    }
    /* write null character */
    *(end+1) = '\0';
    return str;
}

void perform_calculations(SomeStruct *data) 
{
    // Do calculations
    data->c = data->a * data->b;
    char * word1="Trye";
    char * word2="ghg";

    printf("%s", word1);
    
    strcpy(data->name[0],trim_space(word1));
    strcpy(data->name[1],trim_space(word2)); 
    // data.str[0]="OMG";
    // data.str[1]="hi";
=======

void perform_calculations(SomeStruct *data) {
    // Do calculations
    data->c = data->a * data->b; 
>>>>>>> 9e30f2a (function call from cpp function works)
}
