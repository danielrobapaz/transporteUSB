/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#define _GNU_SOURCE
#define READ_END 0
#define WRITE_END 1
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "Service_List_Functions.h"
#include "Sch_List_Functions.h"
#include "Sch_Node.h"
#include "Service_Node.h"
#include "Carga.h"
#include "Route_Data.h"
#include "Thread_Bus.h"

#define MAX_HOURS 8
#define MAX_LEN 100
#define REGRESO -1
#define ESPERANDO 0
#define IDA 1


/**
 * Busca el nombre de un archivo dependiendo de los flags.
 * En caso de no conseguirlo, retorna valores default:
 * -s: ERROR
 * -c: carga.csv
 * -t: 0.25
*/
char* Get_Argument(char* type, int argc, char **argv) {
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

    /*Se adquiere la informacion recibida como argumento*/
    struct Thread_Bus my_bus = (struct Thread_Bus*)args;

    if (my_bus->dispatched) {  /*Solo se trabaja con autobuses despachados*/
        if (my_bus->state == 1) {
            /*Caso ida a la universidad*/
            if (my_bus->minutes_on_road == my_bus->trip_duration) {
                /*Si se llego a la parada se intenta acaparar la parada*/
                if (!pthread_mutex_trylock(my_bus->parada)) {
                    my_bus->state = 0; /*Se empieza a esperar*/
                    while (my_bus->on_board < *(my_bus->Estudiantes_En_Parada) && *(my_bus->Estudiantes_En_Parada) > 0) {
                        my_bus->on_board++;
                        *(my_bus->Estudiantes_En_Parada)--; /*Se monta gente en el autobus*/
                    }
                }
                return NULL; /*RETORNAR INFORMACION RELEVANTE*/
            } else {
                /*Si aun se esta rodando se aumenta el tiempo circulando*/
                my_bus->minutes_on_road++;
                return NULL; /*RETORNAR INFORMACION RELEVANTE*/
            }
        } else if (my_bus->state == 0) {
            /*Caso esperando en la parada*/
            if (my_bus->minutes_waited == 10) {
                my_bus->state = -1; /*Se empieza a volver*/
                my_bus->minutes_on_road = 0;
                if (pthread_mutex_unlock(my_bus->parada)) { /*Se libera el mutex en la parada*/
                    printf("Error liberando mutex.\n");
                    exit(1);
                }
                return NULL; /*RETORNAR INFORMACION RELEVANTE*/
            } else {
                /*Si aun se esta esperando se aumenta el tiempo en espera*/
                my_bus->minutes_waited++;
                /*Se monta a la gente que aun quepa en el autobus*/
                while (my_bus->on_board < *(my_bus->Estudiantes_En_Parada) && *(my_bus->Estudiantes_En_Parada) > 0) {
                    my_bus->on_board++;
                    *(my_bus->Estudiantes_En_Parada)--;
                }
                return NULL; /*RETORNAR INFORMACION RELEVANTE*/
            }
        } else if (my_bus->state == -1) {
            /*Caso camino de vuelta a la universidad*/
            if (my_bus->minutes_on_road == my_bus->trip_duration) {
                /*DESPACHAR GENTE EN LA UNIVERSIDAD*/
                return NULL; /*RETORNAR INFORMACION RELEVANTE*/
            } else {
                my_bus->minutes_on_road++;
                return NULL; /*RETORNAR INFORMACION RELEVANTE*/
            }
        }
    }
    return NULL; /*Si el autobus no estaba desplegado se retorna NULL*/
}

int main(int argc, char **argv) {
    /* Obtener nombre de los archivos*/
    char* Servicio_Filename = Get_Argument("-s", argc, argv);
    char* Carga_Filename = Get_Argument("-c", argc, argv);
    float tiempo = atof(Get_Argument("-t", argc, argv));
    int i, j, Father_PID;
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

    printf("Antes de iniciar el ciclo\n");
    for (i = 0; i < route_count; i++) {
        Routes[i].Servicio = navigator;
        for (j = 0; j < route_count; j++) {
            if (!strcmp(Carga_Al_Sistema[j]->Cod, navigator->Service->Route)) {
                Routes[i].Carga = Carga_Al_Sistema[j];
            }
        }
        navigator = navigator->Next;
    }
    printf("Despues de terminar el ciclo\n");

    /*array que guardara los pid de los procesos hijos*/
    int Child_PID[route_count];
    Father_PID = getpid();

    /*Se crea el arreglo de pipes*/
    int pipes[route_count][2];

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
        Sch_Node *Next_Bus = Route_Service->Schedule->Head; /*Apuntador al siguiente autobus que va a departir*/
        pthread_mutex_t parada; /*Mutex que regula acceso a recoger estudiantes*/
        time_t current_time = 28800; /*4AM hora Venezuela, por ahora la simulacion va a iniciar a esa hora*/
        int Num_Busses = 0; 
        int Camino_Duracion = localtime(Routes[my_route].Carga->Recorr)->tm_min; /*Duracion del recorrido en minutos*/
        int Next_Student_Load = 0; /*Posicion en el arreglo de carga de la siguiente ola de estudiantes a llegar*/
        int Done_Busses = 0;
        int Current_Thread = 0;
        int Estudiantes_En_Parada = 0;

        /* Calculamos el numero de autobuses de cada ruta */
        while (Next_Bus != NULL) {
            Num_Busses++;
            Next_Bus = Next_Bus->Next;
        }

        Next_Bus = Route_Service->Schedule->Head;
        /*Each bus will be handled by a separate thread*/
        Thread_Bus Autobuses[Num_Busses];
        pthread_t Busses_Threads[Num_Busses];

        /*Ciclo principal de la simulación*/
        while (Done_Busses < Num_Busses) {
            /*Si a esta hora sale algún autobus, sale el autobus apuntado por Next_Bus con el hilo Current_Thread*/
            if (localtime(current_time)->tm_hour == localtime(Next_Bus->Schedule->Time)->tm_hour
            && localtime(current_time)->tm_min == localtime(Next_Bus->Schedule->Time)->tm_min) {
                /*Se genera el objeto Thread_Bus que funcionara como argumento del procedimiento*/
                Autobuses[Current_Thread].dispatched = 1;
                Autobuses[Current_Thread].state = 1;
                Autobuses[Current_Thread].minutes_on_road = 0;
                Autobuses[Current_Thread].minutes_waited = 0;
                Autobuses[Current_Thread].capacity = Next_Bus->Schedule->Capacity;
                Autobuses[Current_Thread].on_board = 0;
                Autobuses[Current_Thread].parada = &parada;
                Autobuses[Current_Trhead].Estudiantes_En_Parada = &Estudiantes_En_Parada;
                Autobuses[Current_Thread].done = 0;

                /*Se mueve al siguiente hilo y autobus a despachar*/
                Next_Bus = Next_Bus->Next;
                Current_Thread++;
            }

            /*Se llama la funcion de thread para cada autobus*/
            for (j = 0; j < Num_Busses; j++) {
                if (pthread_create(&Busses_Threads[j], Bus_Simulation, &Autobuses[j])) {
                    printf("Error durante la creación de hilo.\n");
                    exit(1);
                }
            }

            /*PARTE DONDE EL PROCESO HIJO ENVIA INFORMACION RELEVANTE DE TODOS LOS AUTOBUSES AL PROCESO PADRE*/
        }
    } else {
        /*Parte del padre*/

        /*EL PADRE RECIBE LA INFORMACION ENVIADA POR SUS HIJOS Y SINCRONIZA LA EJECUCION DEPENDIENDO DE LA HORA*/
    }


    /*El padre espera a que todos sus hijos terminen para evitar zombies*/
    while (wait(NULL) > 0) {
        ;
    }

    return 0;
}
