#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Nodo {
    char tipoDocumento[3];  // CC, TI, PA
    char numeroDocumento[20];
    char primerApellido[50];
    struct Nodo* siguiente;
} Nodo;

int esTipoDocumentoValido(char* tipo) {
    return (tipo[0] == 'C' && tipo[1] == 'C' && tipo[2] == '\0') ||
           (tipo[0] == 'T' && tipo[1] == 'I' && tipo[2] == '\0') ||
           (tipo[0] == 'P' && tipo[1] == 'A' && tipo[2] == '\0');
}

Nodo* crearNodo(char tipoDocumento[3], char numeroDocumento[20], char primerApellido[50]) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        exit(1);
    }
    for (int i = 0; i < 3; i++) nuevo->tipoDocumento[i] = tipoDocumento[i];
    for (int i = 0; i < 20; i++) nuevo->numeroDocumento[i] = numeroDocumento[i];
    for (int i = 0; i < 50; i++) nuevo->primerApellido[i] = primerApellido[i];
    nuevo->siguiente = NULL;
    return nuevo;
}

void insertarPasajero(Nodo** cabeza, char tipoDocumento[3], char numeroDocumento[20], char primerApellido[50]) {
    Nodo* nuevo = crearNodo(tipoDocumento, numeroDocumento, primerApellido);
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

int contarPasajeros(Nodo* cabeza) {
    int count = 0;
    Nodo* temp = cabeza;
    while (temp != NULL) {
        count++;
        temp = temp->siguiente;
    }
    return count;
}

void imprimirLista(Nodo* cabeza, int capacidad) {
    Nodo* temp = cabeza;
    int i = 1;
    printf("\nLista de pasajeros (en orden de abordaje):\n");
    while (temp != NULL) {
        printf("Pasajero %d: %s %s - %s\n", 
               i, temp->tipoDocumento, temp->numeroDocumento, temp->primerApellido);
        if (i == capacidad) {
            printf("--- Fin de pasajeros que pueden abordar ---\n");
        }
        temp = temp->siguiente;
        i++;
    }
}

void liberarLista(Nodo** cabeza) {
    Nodo* temp;
    while (*cabeza != NULL) {
        temp = *cabeza;
        *cabeza = (*cabeza)->siguiente;
        free(temp);
    }
}

int main() {
    int capacidad;
    printf("Ingrese la capacidad maxima de asientos del avion: ");
    scanf("%d", &capacidad);
    while (getchar() != '\n');  // Limpiar buffer

    int limiteOverbooking = (int)ceil(capacidad * 1.10);
    printf("Se pueden vender hasta %d tiquetes (incluyendo overbooking).\n", limiteOverbooking);

    Nodo* listaPasajeros = NULL;
    int opcion;

    do {
        printf("\nMenu:\n");
        printf("1. Registrar pasajero\n");
        printf("2. Mostrar lista de pasajeros\n");
        printf("3. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);
        while (getchar() != '\n');  // Limpiar buffer

        if (opcion == 1) {
            if (contarPasajeros(listaPasajeros) >= limiteOverbooking) {
                printf("Limite de overbooking alcanzado.\n");
                continue;
            }
            char tipoDocumento[3], numeroDocumento[20], primerApellido[50];
            printf("Ingrese tipo de documento (CC, TI, PA): ");
            scanf("%2s", tipoDocumento);
            while (getchar() != '\n');  // Limpiar buffer
            if (!esTipoDocumentoValido(tipoDocumento)) {
                printf("Tipo de documento invalido.\n");
                continue;
            }
            printf("Ingrese numero de documento: ");
            scanf("%19s", numeroDocumento);
            while (getchar() != '\n');  // Limpiar buffer
            printf("Ingrese primer apellido: ");
            scanf("%49s", primerApellido);
            while (getchar() != '\n');  // Limpiar buffer
            insertarPasajero(&listaPasajeros, tipoDocumento, numeroDocumento, primerApellido);
            printf("Pasajero registrado.\n");
        } else if (opcion == 2) {
            imprimirLista(listaPasajeros, capacidad);
        } else if (opcion == 3) {
            printf("Saliendo del programa...\n");
            liberarLista(&listaPasajeros);
        } else {
            printf("Opcion invalida.\n");
        }
    } while (opcion != 3);

    return 0;
}