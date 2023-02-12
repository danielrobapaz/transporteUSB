/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera 16-11233
*/

/**
 * Estructura de tweet.
 * Almacena un puntero al usuario que lo creó, su ID,
 * otro al tweet per sé y otro al timestamp.
*/
typedef struct Tweet {
    char *Tweet;
    char *Username;
    int User_id;
    char *TimeStamp;
}
Tweet;