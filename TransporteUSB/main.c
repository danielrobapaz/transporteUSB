/**
 * CI3825: Sistemas de Operación
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Service_List_Functions.h"
#include "Sch_List_Functions.h"
#include "Sch_Node.h"
#include "Carga.h"

#define MAX_HOURS 8
#define MAX_LEN 100

/**
 * Busca el nombre de un archivo dependiendo de los flags.
 * En caso de no conseguirlo, retorna valores default:
 * -s: ERROR
 * -c: carga.csv
 * -t: 0.25
*/
char* Get_Filename(char* type, int argc, char **argv) {
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
void add_Service(char filename[], struct Service_List* serv_list) {
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

/**
 * Takes string and creates a new time_t struct
*/
time_t *create_Hour(char* time) {
    time_t *new_Hour = malloc(sizeof(time_t));
    struct tm *date;
    time_t now;
    date = localtime(&now);

    if (!new_Hour) {
        printf("Error: No se pudo reservar memoria.\n");
        exit(1);
    }

    date->tm_hour = atoi(strtok(time, ":"));
    date->tm_min = atoi(strtok(NULL, ","));
    *new_Hour = mktime(date);

    return new_Hour;
}

/**
 * Takes values and creates a new Carga struct
*/
Carga *create_Carga(char* cod, char* nombre, char* recorr, int cap0,
    int cap1, int cap2, int cap3, int cap4, int cap5, int cap6, int cap7) {

    Carga *new_Carga = malloc(sizeof(Carga));

    if (!new_Carga) {
        printf("Error: No se pudo reservar memoria.\n");
        exit(1);
    }

    new_Carga->Cod = cod;
    new_Carga->Nombre = nombre;
    new_Carga->Recorr = create_Hour(recorr);
    new_Carga->Cap0 = cap0;
    new_Carga->Cap1 = cap1;
    new_Carga->Cap2 = cap2;
    new_Carga->Cap3 = cap3;
    new_Carga->Cap4 = cap4;
    new_Carga->Cap5 = cap5;
    new_Carga->Cap6 = cap6;
    new_Carga->Cap7 = cap7;

    return new_Carga;
}

/**
 * Read each line of the filename, the hours on the first
 * line will be added to the "horarios" int array, meanwhile
 * the data on the rest of the lines will be added to an array
 * of Carga structs
*/
void add_Carga(char filename[], struct Carga *carga, int *horarios) {
    FILE *fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;

    fp = fopen(filename, "r");

    if (!fp) {
        printf("Error al abrir el archivo %s\n", filename);
        exit(1);
    }

    /* Read file line by line */
    while ((read = getline(&line, &len, fp)) != -1) {
        if (i == 0) {
            /* Guardar horas en horarios[]*/
            int j = 0;
            char* hour = strtok(line, ",");
            while (j < 10) {
                int k = 0;
                hour = strtok(NULL, ",");
                if (j >= 3) {
                    horarios[k] = atoi(hour);
                    k++;
                }
                j++;
            }
        } else {
            /* Guardar en carga[] */
            /*char *cod = strtok(line, ",");
            char *nombre = strtok(NULL, ",");
            char *recorr = strtok(NULL, ",");
            int cap0 = atoi(strtok(NULL, ","));
            int cap1 = atoi(strtok(NULL, ","));
            int cap2 = atoi(strtok(NULL, ","));
            int cap3 = atoi(strtok(NULL, ","));
            int cap4 = atoi(strtok(NULL, ","));
            int cap5 = atoi(strtok(NULL, ","));
            int cap6 = atoi(strtok(NULL, ","));
            int cap7 = atoi(strtok(NULL, ","));

            Carga *flt = create_Carga(cod, nombre, recorr, cap0,
            cap1, cap2, cap3, cap4, cap5, cap6, cap7);

            carga[i - 1] = flt;*/
        }

        i++;
    }

    fclose(fp);
}

int main(int argc, char **argv) {
    /* Obtener nombre de los archivos*/
    char* Servicio_Filename = Get_Filename("-s", argc, argv);
    char* Carga_Filename = Get_Filename("-c", argc, argv);
    /*float tiempo = atof(Get_Filename("-t", argc, argv));*/

    int Horarios[MAX_HOURS];
    Carga Carga_Al_Sistema[MAX_LEN];
    Service_List *Service_List = Create_Service_List();

    add_Service(Servicio_Filename, Service_List);
    add_Carga(Carga_Filename, Carga_Al_Sistema, Horarios);

    return 0;
}
