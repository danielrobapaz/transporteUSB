/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

/**
 * Estructura nodo para la lista enlazada de servicios.
*/
typedef struct Route_Data {
    struct Service_Node *Servicio;
    struct Carga *Carga;
} Route_Data;