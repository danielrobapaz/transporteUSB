/**
 * CI3825: Sistemas de Operación
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Service_List_Functions.h"
#include "Sch_List_Functions.h"
#include "Sch_Node.h"

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
            printf("ERROR: No se encontró archivo de servicio. \n");
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
 * Read each line of the filename, adds the schedules to a
 * list which then will be added to the service list as well
*/
void addService(char filename[], struct Service_List* serv_list) {
    FILE *fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");

    if (!fp) {
        printf("Error al abrir el archivo %s\n", filename);
        exit(1);
    }

    /* Read file line by line */
    while ((read = getline(&line, &len, fp)) != -1) {
        Sch_List *horarios = Create_Sch_List();

        char* route = strtok(line, " ");

        while(line != NULL) {
            if (strcmp(route, line)) {
                char* hour = strtok(line, ":");
                char* min = strtok(NULL, "(");
                char* cap = strtok(NULL, ") ");

                if (hour && min && cap) {
                    Schedule *sch = create_Schedule(atoi(hour), atoi(min), atoi(cap));
                    add_Sch_Node(horarios, sch);
                }
            }
            line = strtok(NULL, "");

            /* Agregar lista enlazada de horarios a la lista de servicios*/
            if (!line) {
                Service *serv = create_Service(route, horarios);
                add_Service_Node(serv_list, serv);
            }
        }
    }

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