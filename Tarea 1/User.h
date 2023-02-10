/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

/**
 * Estructura para almacenar usuarios.
 * En ella, se guarda un apuntador al handle
 * del usuario y su ID, un apuntador al hash
 * de su contraseña, un apuntador a la lista
 * de tweets creados por el usuario en cuestión
 * y otro apuntador a la lista de personas a las que
 * el usuario sigue.
*/
typedef struct User {
    char *Handle;
    int User_Id;
    char *Password;
    struct Tweets_List *Tweets;
    struct User_List *Following;
}
User;