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
#include <sys/mman.h>

#include "Service_List_Functions.h"
#include "Sch_List_Functions.h"
#include "Sch_Node.h"
#include "Service_Node.h"
#include "Carga.h"
#include "Route_Data.h"
#include "Thread_Bus.h"
#include "Thread_Return.h"

#define MAX_HOURS 8
#define MAX_LEN 100


/**
 * Funcion que recibe un numero de milisegundos durante los cuales el proceso llamante va a dormir.
 * En caso de error retorna -1.
*/
int milli_sleep(int m_time) {
    return usleep(m_time*1000);
}

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
    Thread_Bus *my_bus = (Thread_Bus *) args;

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
            } else {
                /*Si aun se esta rodando se aumenta el tiempo circulando*/
                my_bus->minutes_on_road++;
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
            } else {
                /*Si aun se esta esperando se aumenta el tiempo en espera*/
                my_bus->minutes_waited++;
                /*Se monta a la gente que aun quepa en el autobus*/
                while (my_bus->on_board < *(my_bus->Estudiantes_En_Parada) && *(my_bus->Estudiantes_En_Parada) > 0) {
                    my_bus->on_board++;
                    *(my_bus->Estudiantes_En_Parada)--;
                }
            }
        } else if (my_bus->state == -1) {
            /*Caso camino de vuelta a la universidad*/
            if (my_bus->minutes_on_road == my_bus->trip_duration) {
                /*DESPACHAR GENTE EN LA UNIVERSIDAD*/
            } else {
                my_bus->minutes_on_road++;
            }
        }
        return NULL;
    }
    return NULL;
}


/*############################################################################MAIN############################################################################################################*/
int main(int argc, char **argv) {

    /*VARIABLES DESTINADAS A OBTENCIÓN DE INFORMACIÓN GLOBAL*/
    char* Servicio_Filename = Get_Argument("-s", argc, argv); /* Obtener nombre de los archivos*/
    char* Carga_Filename = Get_Argument("-c", argc, argv);
    float tiempo = atof(Get_Argument("-t", argc, argv));
    int time_delta = (int)tiempo*1000; /*cantidad de milisegundos que dura una iteracion*/
    int i, j, Father_PID;
    int route_count;
    int my_route;               /*Apuntador al objeto servicio que le corresponde a cada proceso*/
    /*Se desvinculan los semaforos en caso de que existan*/
    sem_unlink("parent_to_children");
    sem_unlink("children_to_parent");
    sem_unlink("write");
    sem_t *parent_to_children = sem_open("parent_to_children", O_CREAT, 0644, 0);
    sem_t *children_to_parent = sem_open("children_to_parent", O_CREAT, 0644, 0); /*Semaforos para coordinacion*/
    sem_t *write_to_sh_mem = sem_open("write", O_CREAT, 0644, 1);
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

    Thread_Bus *Autobuses = (Thread_Bus *) mmap(NULL, (sizeof(Thread_Bus))*(route_count*50), 
    PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); /*Arreglo que se llevara a memoria compartida para ser modificado por los hilos y leido por el padre*/

    for (i = 0; i < route_count*50; i++) {
        Autobuses[i].dispatched = 0;
    }

    int *paradas = mmap(NULL, sizeof(int)*route_count, 
    PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    /* Llenamos el servicio y la arga de cada ruta */
    navigator = Servicio->Head;

    for (i = 0; i < route_count; i++) {
        Routes[i].Servicio = navigator;
        for (j = 0; j < route_count; j++) {
            if (!strcmp(Carga_Al_Sistema[j]->Cod, navigator->Service->Route)) {
                Routes[i].Carga = Carga_Al_Sistema[j];
            }
        }
        navigator = navigator->Next;
    }

    /*array que guardara los pid de los procesos hijos*/
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
    if (getpid() != Father_PID) {
        Service *Route_Service = Routes[my_route].Servicio->Service;
        Sch_Node *Next_Bus = Route_Service->Schedule->Head; /*Apuntador al siguiente autobus que va a departir*/
        pthread_mutex_t parada; /*Mutex que regula acceso a recoger estudiantes*/
        time_t current_time = 28800; /*4AM hora Venezuela, por ahora la simulacion va a iniciar a esa hora*/
        int Num_Busses = 0; 
        int Camino_Duracion = localtime(Routes[my_route].Carga->Recorr)->tm_min + (localtime(Routes[my_route].Carga->Recorr)->tm_hour*60); /*Duracion del recorrido en minutos*/
        printf("Hola, mi ruta es %s y mi ruta dura %d min\n", Route_Service->Route, Camino_Duracion);
        int Next_Student_Load = 0; /*Posicion en el arreglo de carga de la siguiente ola de estudiantes a llegar*/
        int Done_Busses = 0;
        int Current_Thread = 0;
        int start_index = 100*my_route;     /*Indice donde comienzan los objetos de tipo Thread_Bus que le pertenecen al proceso en Autobuses.*/

        /* Calculamos el numero de autobuses de cada ruta */
        while (Next_Bus != NULL) {
            Num_Busses++;
            Next_Bus = Next_Bus->Next;
        }

        Next_Bus = Route_Service->Schedule->Head;
        pthread_t Threads[Num_Busses];  /*Hilos pertenecientes al proceso hijo*/
        sem_wait(write_to_sh_mem);
        Autobuses[start_index + Num_Busses].dispatched = -1; /*Marca para el proceso padre*/
        sem_post(write_to_sh_mem);


        /*Ciclo principal de la simulación*/
        while (1) {
            /*Si a la hora actual llega gente a la parada se suma el valor a Estudiantes_En_Parada*/
            if (localtime(&current_time)->tm_min == 0 && localtime(&current_time)->tm_hour == 6 + Next_Student_Load) {
                paradas[my_route] += Routes[my_route].Carga->capacidades[Next_Student_Load];
                Next_Student_Load++;
            }
            /*Si a esta hora sale algún autobus, sale el autobus apuntado por Next_Bus con el hilo Current_Thread*/
            if (localtime(&current_time)->tm_hour == localtime(&(Next_Bus->Schedule->Time))->tm_hour
            && localtime(&current_time)->tm_min == localtime(&(Next_Bus->Schedule->Time))->tm_min) {
                /*Se genera el objeto Thread_Bus que funcionara como argumento del procedimiento*/
                sem_wait(write_to_sh_mem);
                Autobuses[Current_Thread + start_index].dispatched = 1;
                Autobuses[Current_Thread + start_index].state = 1;
                Autobuses[Current_Thread + start_index].minutes_on_road = 0;
                Autobuses[Current_Thread + start_index].minutes_waited = 0;
                Autobuses[Current_Thread + start_index].capacity = Next_Bus->Schedule->Capacity;
                Autobuses[Current_Thread + start_index].on_board = 0;
                Autobuses[Current_Thread + start_index].parada = &parada;
                Autobuses[Current_Thread + start_index].Estudiantes_En_Parada = &paradas[my_route];
                Autobuses[Current_Thread + start_index].done = 0;
                sem_post(write_to_sh_mem);

                /*Se mueve al siguiente hilo y autobus a despachar*/
                Next_Bus = Next_Bus->Next;
                Current_Thread++;
            }
            /*Se llama la funcion de thread para cada autobus*/
            for (i = 0; i < Num_Busses; i++) {
                if (pthread_create(&Threads[i], NULL, &Bus_Simulation, &Autobuses[start_index + i])) {
                    printf("Error durante la creación de hilo.\n");
                    exit(1);
                }
            }

            /*Se hace join a los threads enviados*/
            for (i = 0; i < Num_Busses; i++) {
                if (pthread_join(Threads[i], NULL)) {       /*Las modificaciones estan almacenadas en el objeto modificado por el hilo*/
                    printf("Error haciendo join de thread.\n");
                    exit(1);
                }
            }

            current_time += 60;
            sem_post(children_to_parent);   /*Se indica al padre que este proceso ya termino su iteracion*/
            sem_wait(parent_to_children);   /*Se espera a que el padre de permiso de pasar a la siguiente iteracion*/


            /*PARTE DONDE EL PROCESO HIJO ENVIA INFORMACION RELEVANTE DE TODOS LOS AUTOBUSES AL PROCESO PADRE*/
        }
    } else {
        time_t current_time_parent = 28800;
        while (1) {
            for (i = 0; i < route_count; i++) {
                sem_wait(children_to_parent);   /*Se espera a que todos los hijos hayan terminado sus iteraciones*/
            }
            printf("Done waiting.\n");
            for (i = 0; i < route_count; i++) {
                j = 0;
                
                system("clear");  /*limpiar pantalla*/
                /*printf("%s: %d", Routes[i].Servicio->Service->Route, paradas[i]);*/
                fflush(stdout);
                while (Autobuses[(100*i) + j].dispatched != -1) {
                    if (Autobuses[(100*i) + j].dispatched == 1) {
                        printf("%d, ", Autobuses[j].minutes_on_road);
                        fflush(stdout);
                    }
                    j++;
                }
                printf("\n");
            }

            current_time_parent += 60;
            if (milli_sleep(time_delta)) {
                printf("Error durmiendo.\n");
                exit(1);
            }

            for (i = 0; i < route_count; i++) {
                sem_post(parent_to_children); /*Dar el visto bueno a los hijos de pasar a la siguiente iteracion*/
            }

        }
        
    }

    /*El padre espera a que todos sus hijos terminen para evitar zombies*/
    while (wait(NULL) > 0) {
        ;
    }

    return 0;
}
