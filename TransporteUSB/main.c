/**
 * CI3825: Sistemas de Operación
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List_Functions.h"

/**
 * Busca el nombre de un archivo dependiendo de los flags.
 * En caso de no conseguirlo, retorna el default:
 * -s: ERROR
 * -c: carga.csv
 * -t: 0.25
*/
char* Filename(char* type, int argc, char **argv) {
    int i = 1;
    int found = 0;
    while (i < argc) {
        if (!strcmp(type, argv[i])) {
            return argv[i + 1];
        }
        i++;
    }

    if (!found) {
        if (!strcmp("-s", type)) {
            printf("ERROR");
            exit(1);
        } else if (!strcmp("-c", type)) {
            return "carga.csv";
        } else if (!strcmp("-t", type)) {
            return "0.25";
        }
    }

    return NULL;
}

void addService(char filename[], struct List* list) {
    /*FILE *fp;
    fp = fopen(filename, "r");*/
}

int main(int argc, char **argv) {
    /* Obtener nombre de los archivos*/
    /*char* servicio = Filename("-s", argc, argv);
    char* carga = Filename("-c", argc, argv);
    float tiempo = atof(Filename("-t", argc, argv));


    List *Service_List = Create_List();
    addService(servicio, Service_List);*/

    return 0;
}