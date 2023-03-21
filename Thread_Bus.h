/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finamore
 * Valeria Vera Herrera
*/

/*Estructura para albergar la informacion requerida por los hilos*/

typedef struct thread_bus {
    int dispatched; /*Indica si el autobus ha sido desplegado o no*/
    int state; /*1 si va camino a la parada, 0 si esta esperando, -1 si va de regreso a la universidad*/
    int minutes_on_road; /*Cuenta la cantidad de minutos que el autobus lleva rodando*/
    int minutes_waited; /*Cantidad de minutos que el autobus lleva esperando*/
    int trip_duration
    int capacity; /*Capacidad total del autobus*/
    int on_board; /*Cantidad de personas actualmente en el autobus*/   
    pthread_mutex_t *parada;
    int *Estudiantes_En_Parada /*Apuntador al numero de personas en la parada*/
    int done; /*Indica si este autobus ya termino sus actividades*/
} Thread_Bus;