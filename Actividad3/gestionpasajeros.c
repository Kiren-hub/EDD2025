#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Definición de la estructura para los datos de los pasajeros
typedef struct {
    char tipoDocumento[3];  // CC, TI o PA (2 caracteres + null)
    char numeroDocumento[20];  // Número de documento
    char primerApellido[50];  // Primer apellido
} Pasajero;

// Definición del nodo para la lista enlazada
typedef struct Nodo {
    Pasajero datos;
    struct Nodo* siguiente;
} Nodo;

// Función para leer un string manualmente (sin usar funciones de string.h)
void leerString(char* buffer, int maxLen) {
    int i = 0;
    char c;
    // Limpiar el buffer de entrada
    while ((c = getchar()) != '\n' && c != EOF);
    // Leer hasta maxLen-1 para dejar espacio al null
    while (i < maxLen - 1 && (c = getchar()) != '\n' && c != EOF) {
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0';  // Terminar con null
}

// Función para validar tipo de documento manualmente
int esTipoDocumentoValido(char* tipo) {
    // Validar si es "CC", "TI" o "PA"
    if (tipo[0] == 'C' && tipo[1] == 'C' && tipo[2] == '\0') return 1;
    if (tipo[0] == 'T' && tipo[1] == 'I' && tipo[2] == '\0') return 1;
    if (tipo[0] == 'P' && tipo[1] == 'A' && tipo[2] == '\0') return 1;
    return 0;
}

// Función para crear un nuevo nodo
Nodo* crearNodo(Pasajero p) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        exit(1);
    }
    nuevo->datos = p;
    nuevo->siguiente = NULL;
    return nuevo;
}

// Función para insertar un pasajero al final de la lista
void insertarPasajero(Nodo** cabeza, Pasajero p) {
    Nodo* nuevo = crearNodo(p);
    if (*cabeza == NULL) {
        *cabeza = nuevo;
    } else {
        Nodo* temp = *cabeza;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
}

// Función para contar el número de pasajeros
int contarPasajeros(Nodo* cabeza) {
    int count = 0;
    Nodo* temp = cabeza;
    while (temp != NULL) {
        count++;
        temp = temp->siguiente;
    }
    return count;
}

// Función para imprimir la lista de pasajeros
void imprimirLista(Nodo* cabeza, int capacidad) {
    Nodo* temp = cabeza;
    int i = 1;
    printf("\nLista de pasajeros registrados (en orden de abordaje):\n");
    while (temp != NULL) {
        printf("Pasajero %d: %s %s - %s\n", i, temp->datos.tipoDocumento, 
               temp->datos.numeroDocumento, temp->datos.primerApellido);
        if (i == capacidad) {
            printf("--- Fin de pasajeros que pueden abordar (capacidad maxima) ---\n");
        }
        temp = temp->siguiente;
        i++;
    }
}

// Función principal
int main() {
    int capacidad;
    printf("Ingrese la capacidad maxima de asientos del avion: ");
    scanf("%d", &capacidad);
    
    // Calcular el límite de overbooking (capacidad + 10%)
    int limiteOverbooking = (int)ceil(capacidad * 1.10);
    printf("Se pueden vender hasta %d tiquetes (incluyendo overbooking).\n", limiteOverbooking);
    
    Nodo* listaPasajeros = NULL;  // Cabeza de la lista enlazada
    
    int opcion;
    do {
        printf("\nMenu:\n");
        printf("1. Registrar pasajero\n");
        printf("2. Mostrar lista de pasajeros\n");
        printf("3. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);
        
        if (opcion == 1) {
            int numPasajeros = contarPasajeros(listaPasajeros);
            if (numPasajeros >= limiteOverbooking) {
                printf("Limite de overbooking alcanzado. No se pueden registrar mas pasajeros.\n");
                continue;
            }
            
            Pasajero nuevoPasajero;
            printf("Ingrese tipo de documento (CC, TI, PA): ");
            leerString(nuevoPasajero.tipoDocumento, 3);
            // Validar tipo de documento
            if (!esTipoDocumentoValido(nuevoPasajero.tipoDocumento)) {
                printf("Tipo de documento invalido. Debe ser CC, TI o PA.\n");
                continue;
            }
            
            printf("Ingrese numero de documento: ");
            leerString(nuevoPasajero.numeroDocumento, 20);
            
            printf("Ingrese primer apellido: ");
            leerString(nuevoPasajero.primerApellido, 50);
            
            insertarPasajero(&listaPasajeros, nuevoPasajero);
            printf("Pasajero registrado exitosamente.\n");
        } else if (opcion == 2) {
            imprimirLista(listaPasajeros, capacidad);
        } else if (opcion == 3) {
            printf("Saliendo del programa...\n");
        } else {
            printf("Opcion invalida.\n");
        }
    } while (opcion != 3);
    
    // Liberar memoria de la lista enlazada
    Nodo* temp;
    while (listaPasajeros != NULL) {
        temp = listaPasajeros;
        listaPasajeros = listaPasajeros->siguiente;
        free(temp);
    }
    
    return 0;
}