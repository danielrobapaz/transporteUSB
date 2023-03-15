#define bool int
#define true 1
#define false 0

typedef struct datos_hilo {
    int cap_bus;
    int gente_en_bus;
    int gente_en_parada;
    bool en_espera;
    int tiempo_en_camino;
    int duracion_viaje;
    int tiempo_en_espera;
} datos_hilos;


/*void pq aun no se que retornar xdd*/
int  rutina_hilo(int cap_bus, int gente_en_bus, int gente_en_parada, bool en_espera, int tiempo_en_camino, int duracion_viaje, int tiempo_en_espera) {
    if (!en_espera && tiempo_en_camino == duracion_viaje) {
        /*Se marca el bus como esperando en la parada*/
        en_espera = true;
        gente_en_parada = gente_en_parada - cap_bus; /*Se retira la gente de la parada*/
        /*Se resettea el tiempo de viaje*/
        tiempo_en_camino = 0;
        tiempo_en_espera = 1; /*Empieza a esperar en la parada*/
    } else if (en_espera && tiempo_en_espera == 10) { /*Termino de esperar los 10 min en la parada*/
        en_espera = false;
        tiempo_en_camino = 1;
        tiempo_en_espera = 0; /*Lo que dure el viaje de la ruta*/
    } else if (tiempo_en_camino < duracion_viaje) { /*El bus esta esperando o en carretera*/
        if (en_espera) { /*El bus esta esperando*/
            tiempo_en_espera++; /*Se avanza un minuto en el reloj de espera*/
            while (gente_en_bus < cap_bus && gente_en_parada > 0) {
                gente_en_bus++;
                gente_en_parada--; /*Se monta gente en el autobus*/
            }
        } else {
            tiempo_en_camino++; /*Se avanza en el recorrido*/
        }
    }
}

/*Dentro del proceso*/
void lo_del_proceso() {

}
