/**
 * CI3825: Sistemas de Operación
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Service_List_Functions.h"

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

/**
 * Corta una sección de un string con ayuda de strncpy.
 * 
 * Entrada:
 * - line: cadena de caracteres a ser cortada.
 * - i: posición de la cadena desde donde se
 *      cortará.
 * - n: cantidad de caracteres a cortar.
 * 
 * Salida:
 *      Apuntador a cadena de caracteres.
 */ 
char* cutLine(char* line, int i, int n) {
    char* str = malloc((n + 1) * sizeof(char));

    if (!str) {
        printf("Error: No se pudo reservar memoria para el arreglo.");
        exit(1);
    }

    strncpy(str, &line[i], n);    
    str[n] = 0;

    if (!str) {
        exit(1);
    }
    
    return str;
}

/**
 * Read each line of the filename, adds the schedules to a
 * list which then will be added to the service list as well
*/
void addService(char filename[], struct Service_List* list) {
    FILE *fp;
    fp = fopen(filename, "r");

    if (!fp) {
        printf("Error al abrir el archivo %s\n", filename);
        exit(1);
    }

    /*while(fgets(linea, sizeof(linea) + 1, fp)) {
        char* Route = cutLine(linea, 0, 3);
        char* schedules = cutLine(linea, 3, HASTA EL FINAL DE LA LINEA);

        // guardar schedules en Service
        // guardar Service en Nodo
        // insertar nodo a la lista

        i++;
    }*/

    fclose(fp);
}

int main(int argc, char **argv) {
    /* Obtener nombre de los archivos*/
    char* servicio = Filename("-s", argc, argv);
    /*char* carga = Filename("-c", argc, argv);
    float tiempo = atof(Filename("-t", argc, argv));*/

    Service_List *Service_List = Create_Service_List();
    addService(servicio, Service_List);

    return 0;
}