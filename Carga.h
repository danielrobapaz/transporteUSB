/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finamore
 * Valeria Vera Herrera
*/

/**
 * Estructura para almacenar cargas
*/
typedef struct Carga {
    char Cod[4];
    char Nombre[50];
    time_t *Recorr;
    int capacidades[8];
} Carga;
