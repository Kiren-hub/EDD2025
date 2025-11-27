#include <stdio.h> // Para funciones de entrada/salida (printf, scanf)
#include <stdlib.h> // Para funciones de memoria dinámica (malloc, free)

// Máxima longitud de cadena para el destino
#define MAX_DESTINO 20

// 1. Estructura del Nodo (Pasajero)
typedef struct Pasajero {
    int documento;                     // Clave de búsqueda e indexación (Similar a 'valor' en tu base)
    char destino[MAX_DESTINO];        
    int tipo_pasaje;                   // 1: Ida, 2: Ida y Regreso
    
    struct Pasajero *izq;              
    struct Pasajero *der;              
} Nodo; // Usamos 'Nodo' en lugar de 'nodo' por convención, pero sigue tu lógica

// Raíz global del Árbol Binario de Búsqueda
Nodo *raiz = NULL; // Usamos 'Nodo' en lugar de 'nodo'

// --- Prototipos de Funciones ---
void registrarPasajero(); // Reemplaza a crearNodo
void insertarRec(Nodo *actual, Nodo *nuevo); // Reemplaza a posicionar
void imprimirPasajero(Nodo *n);

// Funciones de Recorrido
void recorridoInorden(Nodo *actual);    
void recorridoPreorden(Nodo *actual);   
void recorridoPostorden(Nodo *actual);  

// Funciones Adicionales
int contarPasajeros(Nodo *actual);      
Nodo* encontrarMinimo(Nodo* nodo);
Nodo* eliminarPasajero(Nodo* actual, int doc); 
void liberarArbol(Nodo* actual);


// ----------------------------------------------------------------------
// 6. MENU PRINCIPAL (Base de tu primer código)
// ----------------------------------------------------------------------

int main(){
    int opc, doc_eliminar;
    
    // Función para liberar memoria al salir
    atexit((void(*)())liberarArbol); 
    
    do {
        printf("\n--- MENU PRINCIPAL - ARBOL DE TIQUETES ---\n");
        printf("1. Registrar Pasajero (Insercion)\n");
        printf("2. Listado Inorden (Ordenado)\n");
        printf("3. Listado Preorden\n");
        printf("4. Listado Postorden\n");
        printf("5. Contar total de pasajeros\n");
        printf("6. Eliminar pasajero\n");
        printf("7. Salir\n");
        printf("Elija una opcion: ");
        
        if (scanf("%d", &opc) != 1) {
             while (getchar() != '\n'); 
             printf("Opcion invalida. Por favor, ingrese un numero.\n");
             continue;
        }
        while (getchar() != '\n'); 
        
        switch (opc) {
            case 1:
                registrarPasajero();
                break;
            case 2:
                printf("\n--- LISTADO INORDEN (Orden Ascendente por Documento) ---\n");
                if (raiz == NULL) { printf("Arbol vacio.\n"); } else { recorridoInorden(raiz); }
                break;
            case 3:
                printf("\n--- LISTADO PREORDEN ---\n");
                if (raiz == NULL) { printf("Arbol vacio.\n"); } else { recorridoPreorden(raiz); }
                break;
            case 4:
                printf("\n--- LISTADO POSTORDEN ---\n");
                if (raiz == NULL) { printf("Arbol vacio.\n"); } else { recorridoPostorden(raiz); }
                break;
            case 5:
                printf("\nTotal de pasajeros registrados: %d\n", contarPasajeros(raiz));
                break;
            case 6:
                printf("Ingrese Documento del pasajero a eliminar: ");
                if (scanf("%d", &doc_eliminar) == 1) {
                    Nodo* resultado = eliminarPasajero(raiz, doc_eliminar);
                    if (resultado != raiz) {
                        raiz = resultado; // Actualiza la raíz si fue eliminada
                    }
                    printf("Proceso de eliminacion de pasajero finalizado.\n");
                } else {
                    printf("Entrada invalida para el documento.\n");
                }
                while (getchar() != '\n'); 
                break;
            case 7:
                printf("Saliendo del programa. Liberando memoria...\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while(opc != 7);
    
    return 0;
}


// ----------------------------------------------------------------------
// FUNCIONES DE CREACIÓN E INSERCIÓN
// ----------------------------------------------------------------------

/**
 * @brief Crea un nuevo nodo Pasajero, y lo inserta en el ABB.
 * Es la versión expandida de tu función crearNodo, ahora con entrada de datos completa.
 */
void registrarPasajero() {
    int doc, tipo;
    char dest[MAX_DESTINO];

    printf("--- REGISTRAR NUEVO PASAJERO ---\n");
    printf("Ingrese Documento (clave de busqueda): ");
    if(scanf("%d", &doc) != 1) { printf("Error de lectura.\n"); while(getchar() != '\n'); return; }
    while (getchar() != '\n');
    
    printf("Ingrese Destino (ej. Timbiqui): ");
    scanf("%19s", dest); while (getchar() != '\n');
    
    int entradaValida = 0;
    do {
        printf("Ingrese Tipo de Pasaje (1: Ida, 2: Ida y Regreso): ");
        if(scanf("%d", &tipo) != 1) { printf("Error de lectura.\n"); while(getchar() != '\n'); continue; }
        while (getchar() != '\n');
        if (tipo == 1 || tipo == 2) {
            entradaValida = 1;
        } else {
            printf("Error: Opcion invalida. Debe ser 1 o 2.\n");
        }
    } while (!entradaValida);
    
    // Crear el nodo e inicializar sus campos
    Nodo *nuevo = (Nodo *) malloc (sizeof(Nodo));
    if (nuevo == NULL) { printf("Error: No hay memoria.\n"); return; }
    
    nuevo->documento = doc;
    nuevo->tipo_pasaje = tipo;
    
    for (int i = 0; i < MAX_DESTINO; i++) {
        nuevo->destino[i] = dest[i];
        if (dest[i] == '\0') break; 
    }
    
    nuevo->izq = nuevo->der = NULL;
    
    // Lógica de Inserción
    if (raiz == NULL) {
        raiz = nuevo;
        printf("Pasajero registrado como RAIZ del arbol.\n");
    } else {
        insertarRec(raiz, nuevo);
    }
}

/**
 * @brief Lógica recursiva de inserción (análoga a tu 'posicionar').
 * * Usa 'documento' como clave.
 */
void insertarRec(Nodo *actual, Nodo *nuevo) {
    if (nuevo->documento == actual->documento) {
        printf("Error: Ya existe un pasajero con documento %d. Insercion ignorada.\n", nuevo->documento);
        free(nuevo); 
        return;
    }

    if (nuevo->documento < actual->documento) {
        if (actual->izq == NULL) {
            actual->izq = nuevo; 
            printf("Pasajero registrado exitosamente.\n");
        } else {
            insertarRec(actual->izq, nuevo); 
        }
    } else { // Si es mayor
        if (actual->der == NULL) {
            actual->der = nuevo; 
            printf("Pasajero registrado exitosamente.\n");
        } else {
            insertarRec(actual->der, nuevo); 
        }
    }
}

// ----------------------------------------------------------------------
// FUNCIONES DE RECORRIDO Y CONTEO
// ----------------------------------------------------------------------

void imprimirPasajero(Nodo *n) {
    const char* tipo_str = (n->tipo_pasaje == 1) ? "Ida" : "Ida y Regreso";
    printf("Doc: %-10d | Destino: %-15s | Pasaje: %s\n", 
           n->documento, n->destino, tipo_str);
}

// a) Inorden: (Izquierda -> Raíz -> Derecha)
void recorridoInorden(Nodo *actual) {
    if (actual != NULL) {
        recorridoInorden(actual->izq);    
        imprimirPasajero(actual);         
        recorridoInorden(actual->der);    
    }
}

// b) Preorden: (Raíz -> Izquierda -> Derecha)
void recorridoPreorden(Nodo *actual) {
    if (actual != NULL) {
        imprimirPasajero(actual);         
        recorridoPreorden(actual->izq);    
        recorridoPreorden(actual->der);    
    }
}

// c) Postorden: (Izquierda -> Derecha -> Raíz)
void recorridoPostorden(Nodo *actual) {
    if (actual != NULL) {
        recorridoPostorden(actual->izq);    
        recorridoPostorden(actual->der);    
        imprimirPasajero(actual);         
    }
}

// Conteo Total
int contarPasajeros(Nodo *actual) {
    if (actual == NULL) {
        return 0; 
    }
    return 1 + contarPasajeros(actual->izq) + contarPasajeros(actual->der);
}

// ----------------------------------------------------------------------
// FUNCIONES DE ELIMINACIÓN Y UTILIDAD
// ----------------------------------------------------------------------

Nodo* encontrarMinimo(Nodo* nodo) {
    Nodo* actual = nodo;
    while (actual && actual->izq != NULL) {
        actual = actual->izq;
    }
    return actual;
}

// Eliminación de un pasajero (Función Recursiva)
Nodo* eliminarPasajero(Nodo* actual, int doc) {
    if (actual == NULL) {
        printf("Pasajero con documento %d no encontrado.\n", doc);
        return actual; 
    }

    if (doc < actual->documento) {
        actual->izq = eliminarPasajero(actual->izq, doc);
    } else if (doc > actual->documento) {
        actual->der = eliminarPasajero(actual->der, doc);
    } else {
        // Nodo encontrado: Manejar los 3 casos

        // Caso 1 y 2: Sin hijo o con un solo hijo
        if (actual->izq == NULL) {
            Nodo* temp = actual->der;
            free(actual);
            printf("Pasajero %d eliminado.\n", doc);
            return temp;
        } else if (actual->der == NULL) {
            Nodo* temp = actual->izq;
            free(actual);
            printf("Pasajero %d eliminado.\n", doc);
            return temp;
        }

        // Caso 3: Dos hijos. Reemplazar por el sucesor Inorden
        Nodo* temp = encontrarMinimo(actual->der); 

        // Copiar data del sucesor al nodo actual
        actual->documento = temp->documento;
        actual->tipo_pasaje = temp->tipo_pasaje;
        for (int i = 0; i < MAX_DESTINO; i++) {
            actual->destino[i] = temp->destino[i];
            if (temp->destino[i] == '\0') break; 
        }

        // Eliminar el sucesor
        actual->der = eliminarPasajero(actual->der, temp->documento);
    }
    return actual;
}

// Función para liberar toda la memoria del árbol 
void liberarArbol(Nodo* actual) {
    if (actual != NULL) {
        liberarArbol(actual->izq); 
        liberarArbol(actual->der); 
        free(actual); 
    }
}