/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#define _GNU_SOURCE
#define READ_END 0
#define WRITE_END 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>

#include "Service_List_Functions.h"
#include "Sch_List_Functions.h"
#include "Sch_Node.h"
#include "Service_Node.h"
#include "Carga.h"
#include "Route_Data.h"

#define MAX_HOURS 8
#define MAX_LEN 100
sem_t pipe_sem;

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
time_t *create_Hour(char* input_time) {
    time_t *new_Hour = malloc(sizeof(time_t));
    struct tm *date;
    time_t now = time(NULL);
    date = localtime(&now);

    if (!new_Hour) {
        printf("Error: No se pudo reservar memoria.\n");
        exit(1);
    }

    date->tm_hour = atoi(strtok(input_time, ":"));
    date->tm_min = atoi(strtok(NULL, ","));
    *new_Hour = mktime(date);

    return new_Hour;
}

/**
 * Takes values and creates a new Carga struct
*/
Carga *create_Carga(char* cod, char* nombre, char* recorr, int capacidades[MAX_HOURS]) {

    Carga *new_Carga = malloc(sizeof(Carga));
    int i;

    if (!new_Carga) {
        printf("Error: No se pudo reservar memoria.\n");
        exit(1);
    }

    strcpy(new_Carga->Cod, cod);
    strcpy(new_Carga->Nombre, nombre);
    new_Carga->Recorr = create_Hour(recorr);
    for(i = 0; i < MAX_HOURS; i++) {
        new_Carga->capacidades[i] = capacidades[i];
    }

    return new_Carga;
}

/**
 * Read each line of the filename, the hours on the first
 * line will be added to the "horarios" int array, meanwhile
 * the data on the rest of the lines will be added to an array
 * of Carga structs
*/
void add_Carga(char filename[], struct Carga *carga[], int *horarios) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    int j;

    fp = fopen(filename, "r");

    if (!fp) {
        printf("Error al abrir el archivo %s\n", filename);
        exit(1);
    }

    /* Read file line by line */
    while ((read = getline(&line, &len, fp)) != -1) {
        if (i == 0) {
            /* Guardar horas en horarios[]*/
            j = 0;
            char* hour = strtok(line, ",");
            int k = 0;
            while (j < 10) {
                hour = strtok(NULL, ",");
                if (j >= 2) {
                    horarios[k] = atoi(hour);
                    k++;
                }
                j++;
            }
        } else {
            /* Guardar en carga[] */
            char *cod = strtok(line, ",");
            char *nombre = strtok(NULL, ",");
            char *recorr = strtok(NULL, ",");
            int cap[MAX_HOURS];
            for (j = 0; j < MAX_HOURS; j++) {
                cap[j] = atoi(strtok(NULL, ","));
            }
            carga[i-1] = create_Carga(cod, nombre, recorr, cap);
        }
        i++;
    }

    fclose(fp);
}

/** 
 * Funcion que ejecutara cada hilo
 **/
void *Bus_Simulation(void *args) {
    struct Schedule *sch = (Schedule*)args;
    struct tm *Time = localtime(&sch->Time);
    int hour = Time->tm_hour;
    int min = Time->tm_min;
    printf("soy un bus con capacidad %d y salgo a las %d:%d\n", sch->Capacity, hour, min);

    return NULL;
}

int main(int argc, char **argv) {
    /* Obtener nombre de los archivos*/
    char* Servicio_Filename = Get_Filename("-s", argc, argv);
    char* Carga_Filename = Get_Filename("-c", argc, argv);
    float tiempo = atof(Get_Filename("-t", argc, argv));
    int i, Father_PID;
    int route_count;
    int my_route;               /*Apuntador al objeto servicio que le corresponde a cada proceso*/
    Service_Node *navigator;    /*Variable auxiliar para desplazarse por la lista de servicios*/

    int Horarios[MAX_HOURS];
    Carga *Carga_Al_Sistema[MAX_LEN];
    Service_List *Servicio = Create_Service_List();

    add_Service(Servicio_Filename, Servicio);
    add_Carga(Carga_Filename, Carga_Al_Sistema, Horarios);

    /*Se cuenta la cantidad de rutas en el sitema para determinar el número de
    hijos a crear*/

    navigator = Servicio->Head;
    route_count = 0;
    while (navigator != NULL) {
        route_count++;
        navigator = navigator->Next;
    }

    Route_Data Routes[route_count];
    /* Llenamos el servicio y la arga de cada ruta */
    navigator = Servicio->Head;
    for (i = 0; i < route_count; i++) {
        Routes[i].Servicio = navigator;
        Routes[i].Carga = Carga_Al_Sistema[i];
        navigator = navigator->Next;
    }

    /*array que guardara los pid de los procesos hijos*/
    int Child_PID[route_count];
    Father_PID = getpid();

    /*Se crea el arreglo de pipes*/
    int pipes[route_count][2];
    
    /*Se inicializa el semaforo encargado de sincronizar los procesos hijos en el establecimiento de pipes*/
    if (sem_init(&pipe_sem, 1, 1)) {
        printf("Error en inicialización de semáforo\n");
        exit(1);
    }

    
    navigator = Servicio->Head; /*Se usara mas adelante para enviar informacion por los pipes*/
    for (i = 0; i < route_count; i++) {
        if (pipe(pipes[i])) {
            printf("Error en la creación del pipe.\n");
            exit(1);
        }
        int ch_pid = fork();
        if (ch_pid) {
            Child_PID[i] = ch_pid;
            /*Caso del padre*/
            if (write(pipes[i][WRITE_END], &i, sizeof(int)) == -1) {
                printf("Error escribiendo al pipe.\n");
                exit(1);
            }
            close(pipes[i][WRITE_END]);
            navigator = navigator->Next;
        } else {
            /*Caso del hijo*/
            if (read(pipes[i][READ_END], &my_route, sizeof(int)) == -1) {
                printf("Error leyendo del pipe.\n");
                exit(1);
            }
            break; /*Salirse del ciclo para evitar generar más hijos*/
        }
    }
    
    /* inicio de la simulacion */
    if (getpid() != Father_PID){
        Service *Route_Service = Routes[my_route].Servicio->Service;
        Sch_Node *Curr_Bus = Route_Service->Schedule->Head;

        int Num_Busses = 0;

        /* Calculamos el numero de autobuses de cada ruta */
        while (Curr_Bus != NULL) {
            Num_Busses++;
            Curr_Bus = Curr_Bus->Next;
        }

        printf("Ruta %s tiene %d buses\n", Route_Service->Route, Num_Busses);

        /* Creamos cada hilo de cada bus */
        pthread_t Busses_Threads[Num_Busses];
        Curr_Bus = Route_Service->Schedule->Head;
        for (i = 0; i < Num_Busses; i++) {
            if (pthread_create(&Busses_Threads[i], NULL, Bus_Simulation, (void*)Curr_Bus->Schedule) != 0) {
                printf("Error creando hilo\n");
                exit(1);
            }
            Curr_Bus = Curr_Bus->Next;
        }

        for (i = 0; i < Num_Busses; i++) {
            if (pthread_join(Busses_Threads[i], NULL) != 0) {
                printf("Error retornando hilo \n");
                exit(1);
            }
        }

    }
    /*El padre espera a que todos sus hijos terminen para evitar zombies*/
    while (wait(NULL) > 0) {
        ;
    }

    return 0;
}
