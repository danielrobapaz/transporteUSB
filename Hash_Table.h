/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

/**
 * Estructura correspondiente a la tabla de hash.
 * Contiene un arreglo de 20 listas de tipo User_List.
 * Se aceptan ideas de como hacer que el arreglo no
 * tenga un tamaño fijo y se pueda declarar el tamaño
 * del arreglo después.
*/
typedef struct Hash_Table {
    struct User_List list_array[20];
} Hash_Table;