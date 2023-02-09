#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Estructuras, procedimientos y funciones relevantes a las listas enlazadas de la tabla de hash.*/

typedef struct user {
    int user_id;
    char *handle;
    /*Aqui va apuntador a la lista de tweets*/
} user_t;

/*Estructura nodo para la lista enlazada de la tabla de Hash.
Almacena un puntero a un objeto de tipo usuario y otro puntero
que apunta a algun otro nodo de la lista de la que forma parte.*/
typedef struct hash_node {
    user_t *user;
    struct hash_node *next;
} hash_node_t;

/*Estructura para la lista enlazada de la tabla de hash.
Almacena un puntero apuntando al nodo cabeza de la cola y otro
apuntando al nodo cola de la misma.*/
typedef struct hash_list {
    hash_node_t *head;
    hash_node_t *tail;
} hash_list_t;

/*Inicializa la lista enlazada de la tabla de hash apuntada por el
apuntador list.*/
void hash_list_init(hash_list_t *list) {
    (*list).head = NULL;
    (*list).tail = NULL;
}


/*Procedimiento que agrega un nodo nuevo a una lista enlazada de tipo 
hash_list. El nodo agregado apuntará a un usuario de handle "handle" e
identificación id. Este será agregado al final de la cola.*/
void add_hash_node(hash_list_t *list, user_t *user) {

    hash_node_t *new_node = malloc(sizeof(hash_node_t));
    new_node->user = user;
    new_node->next = NULL;

    /*Si la lista está vacía se inicializa la cabeza y la cola con el nodo ingresado*/
    if ((*list).head == NULL) {
        (*list).head = new_node;
        (*list).tail = new_node;
    }
    /*En caso contrario se agrega el nodo al final de la cola*/
    else {
        (*(*list).tail).next = new_node;
        (*list).tail = new_node;
    }
}

/*Función que indica si el usuario de handle "handle" está presente o no en la
lista hash_list apuntada por list.*/
int is_in_hash_list(hash_list_t *list, char *handle) {
    hash_node_t *tmp = (*list).head;
    while (tmp != NULL) {
        if (!strcmp((*(*tmp).user).handle, handle)) {
            return 1;
        }
        tmp = (*tmp).next;
    }
    return 0;
}



/*Estructuras, funciones y procedimientos relevantes a la tabla de hash.*/


/*Estructura correspondiente a la tabla de hash. Contiene un arreglo de 20 listas de 
tipo hash_list. Se aceptan ideas de como hacer que el arreglo no tenga un tamaño fijo y
se pueda declarar el tamaño del arreglo después uwu*/
typedef struct hash_table {
    hash_list_t list_array[20];   /*apuntador a un arreglo de objetos de tipo hash_list*/
} hash_table_t;


/*Funcion de hash para los handles. POR AHORA solo suma los ascii de los caracteres del handle
y devuelve el resultado. Cuando encuentre un hash mejor lo implemento.*/
int hash_function(char *handle) {
    int i; /*variable auxiliar*/
    int sum = 0;

    for (i=0; i < strlen(handle); i++) {
        sum += handle[i];
    }
    return sum%20;
}

/*Procedimiento que agrega un nuevo elemento a la tabla de Hash. */
void add_elem(hash_table_t *table, user_t *user) {
    /*se calcula el hash value del usuatio apuntado por user*/
    int hash_val = hash_function(user->handle);
    add_hash_node(&(table->list_array)[hash_val], user); /*Se agrega el ususario a la tabla de Hash*/
}

int is_in_hash_table(hash_table_t *table, char *handle) {
    int hash_val = hash_function(handle);
    return (is_in_hash_list(&(table->list_array[hash_val]), handle));
}

/*Funcion que busca al usuario de nombre handle y retorna el objeto de tipo
user_t que le corresponde. Se asume que el usuario existe y está presente en 
la lista apuntada por list.*/
user_t hash_search(hash_table_t *table, char *handle) {

    int hash_val = hash_function(handle);
    hash_list_t *list = &(table->list_array[hash_val]);
    hash_node_t *tmp = (*list).head;
    while (!strcmp((*(*tmp).user).handle, handle)) {
        tmp = (*tmp).next;
    }
    return (*(*tmp).user);
}

void hash_table_init(hash_table_t *table) {
    int i;
    for (i = 0; i < 20; i++) {
        hash_list_init(&(table->list_array[i]));
    }
}

int main(void) {
    user_t usr1, usr2, usr3;
    usr1.handle = "@realBenShapiro";
    usr2.handle = "@elunmosk";
    usr3.handle = "@iguaido";
    usr1.user_id = 1;
    usr2.user_id = 2;
    usr3.user_id = 3;
    hash_table_t table;
    hash_table_init(&table);
    add_elem(&table, &usr1);
    add_elem(&table, &usr2);
    add_elem(&table, &usr3);

    int i;
    printf("Prueba iteración sobre tabla de hash\n");
    for (i = 0; i < 20; i++) {
        hash_node_t *tmp = (table.list_array[i]).head;
        while (tmp != NULL) {
            user_t *usr_ptr = tmp->user;
            printf("handle: %s\n", usr_ptr->handle);
            printf("user_id: %d\n", usr_ptr->user_id);
            printf("Hash value: %d\n", hash_function(usr_ptr->handle));
            printf("\n");
            tmp = tmp->next;
        }
    }
    printf("Prueba de busqueda\n");
    printf("Is @iguaido in table: %d\n", is_in_hash_table(&table, "@iguaido"));
    printf("Is @bioloco in table: %d\n", is_in_hash_table(&table, "@bioloco"));
}