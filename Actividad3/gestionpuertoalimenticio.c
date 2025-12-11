#include <stdio.h>
#include <stdlib.h>

// Definición de las estructuras de datos fundamentales.

// Estructura para los Pedidos en la Cola FIFO(Pedido va arriba porque NodoArbol la usa)
typedef struct Pedido {
    char nombre_destino[40];     // destino del pedido
    int cantidad_solicitada;     // cantidad pedida
    struct Pedido *siguiente;    // siguiente en la cola
} Pedido;

// 1. Estructura del nodo del Árbol ABB + AVL
typedef struct NodoArbol {
    int fecha_vencimiento;     // AAAAMMDD (clave del ABB + AVL)
    int stock_total;           // cantidad disponible del lote
    char nombre_producto[40];  // atun, carne, etc.

    // Cola FIFO (lista enlazada simple)
     Pedido *cola_head;            // primer pedido de la cola FIFO
    Pedido *cola_tail;            // ultimo pedido (para encolar rapido)

    //AVL y ABB
    struct NodoArbol *izq;             // hijo izquierdo
    struct NodoArbol *der;             // hijo derecho
    int altura;                   // altura del nodo (para AVL)
} NodoArbol;

// ==========================================
//       PROTOTIPOS DE FUNCIONES (Fix)
// ==========================================

// Para registrarProducto
NodoArbol* insertar(NodoArbol *raiz, int fecha, int stock, char nombre[]);
NodoArbol* buscarNodo(NodoArbol *actual, int fecha);

// Para eliminarLote
NodoArbol* eliminarNodoAVL(NodoArbol* raiz, int fecha);

// Raíz global del árbol
NodoArbol *raiz = NULL;

//
// ==========================================
//           FUNCIONES DE UTILIDAD
// ==========================================
//

// Función para obtener la altura de un nodo
int altura(NodoArbol *n) {
    if (n == NULL) return 0;
    return n->altura;
}
// Función para Calcular máximo entre dos enteros
int max(int a, int b) {
    return (a > b) ? a : b;
}
// Función para obtener el balance de un nodo(factor de balance)
int obtenerBalance(NodoArbol *n) {
    return (n == NULL) ? 0 : altura(n->izq) - altura(n->der);
}

//
// -------- ROTACIONES DEL AVL --------
//

// Rotación derecha para AVL
NodoArbol* rotacionDerecha(NodoArbol *y) {
    NodoArbol *x = y->izq;  // hijo izquierdo
    NodoArbol *T2 = x->der; // subárbol que se moverá

    // Realizar rotación
    x->der = y;
    y->izq = T2;

    // Actualizar alturas
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;

    // Nueva raíz después de la rotación
    return x;
}
// Rotación izquierda para AVL
NodoArbol* rotacionIzquierda(NodoArbol *x) {
    NodoArbol *y = x->der;  // hijo derecho
    NodoArbol *T2 = y->izq; // subárbol que se mueve

    // rotación
    y->izq = x;
    x->der = T2;

    // actualizar alturas
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;

    // nueva raíz
    return y;
}

//
// ==========================================
//              CREAR NODO
// ==========================================
//

// nodo para el ABB/AVL
NodoArbol* crearNodo(int fecha, int stock, char nombre[] ) {

    NodoArbol *nuevo = (NodoArbol*) malloc(sizeof(NodoArbol));

    nuevo->fecha_vencimiento = fecha;
    nuevo->stock_total = stock;

    // Copiar nombre manualmente
    int i = 0;
    while (nombre[i] != '\0' && i < 39) {
        nuevo->nombre_producto[i] = nombre[i];
        i++;
    }
    nuevo->nombre_producto[i] = '\0';

    // Cola vacía
    nuevo->cola_head = NULL;
    nuevo->cola_tail = NULL;

    // Hijos del árbol
    nuevo->izq = NULL;
    nuevo->der = NULL;

    // Para AVL
    nuevo->altura = 1; // nuevo nodo es inicialmente hoja

    return nuevo;
}

//
// ==========================================
//         INSERTAR (ABB + AVL)
// ==========================================
//

NodoArbol* insertar(NodoArbol *raiz, int fecha, int stock, char nombre[]) {

    // 1. Inserción normal de ABB
    if (raiz == NULL) {
        return crearNodo(fecha, stock, nombre);
    }

    if (fecha < raiz->fecha_vencimiento) {
        raiz->izq = insertar(raiz->izq, fecha, stock, nombre);
    }
    else if (fecha > raiz->fecha_vencimiento) {
        raiz->der = insertar(raiz->der, fecha, stock, nombre);
    }
    else {
        printf("ERROR: Ya existe un producto con esa fecha.\n");
        return raiz;
    }

    // 2. Actualizar altura del nodo
    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));

    
    // 3. Obtener factor de balance (Balanceo AVL)
    int balance = obtenerBalance(raiz);

    // -------------------------------
    // 4. CASOS DE ROTACIÓN DEL AVL
    // -------------------------------

    // CASO LL (Izquierda - Izquierda)
    if (balance > 1 && fecha < raiz->izq->fecha_vencimiento) {
        return rotacionDerecha(raiz); //Rotacion derecha
    }

    // CASO RR (Derecha - Derecha)
    if (balance < -1 && fecha > raiz->der->fecha_vencimiento) {
        return rotacionIzquierda(raiz);// Rotacion izquierda
    }

    // CASO LR (Izquierda - Derecha)
    if (balance > 1 && fecha > raiz->izq->fecha_vencimiento) {
        raiz->izq = rotacionIzquierda(raiz->izq);
        return rotacionDerecha(raiz);// rotacion izquierda + derecha
    }

    // CASO RL (Derecha - Izquierda)
    if (balance < -1 && fecha < raiz->der->fecha_vencimiento) {
        raiz->der = rotacionDerecha(raiz->der);
        return rotacionIzquierda(raiz); // rotacion derecha + izquierda
    }

    // Si no hubo rotación, retornar la raíz normal
    return raiz;
}

//
// ==========================================
//      BUSCAR NODO MÁS CERCANO A VENCER
// ==========================================
//
NodoArbol* buscarMasCercano(NodoArbol *actual) {
    if (actual == NULL) return NULL;
// El nodo más cercano es el más a la izquierda
    while (actual->izq != NULL)
        actual = actual->izq;

    return actual;
}

//
// ==========================================
//              RECORRIDO INORDEN
// ==========================================
//
void inordenMostrar(NodoArbol *actual) {
    if (actual == NULL) return;

    inordenMostrar(actual->izq);

    printf("\n-------------------\n");
    printf("Fecha: %d\n", actual->fecha_vencimiento);
    printf("Producto: %s\n", actual->nombre_producto);
    printf("Stock: %d\n", actual->stock_total);

    Pedido *aux = actual->cola_head;
    if (aux == NULL) {
        printf("Pedidos en espera: 0\n");
    } else {
        printf("Pedidos en espera:\n");
        while (aux != NULL) {
            printf("  Destino: %s | Cantidad: %d\n",
                aux->nombre_destino,
                aux->cantidad_solicitada);
            aux = aux->siguiente;
        }
    }

    inordenMostrar(actual->der);
}


//
// ==========================================
//          FUNCIONES DEL MENÚ
// ==========================================
//

void registrarProducto() {
    int fecha, stock;
    char nombre[40];

    printf("\n=== REGISTRAR PRODUCTO ===\n");

    printf("Ingrese fecha de vencimiento (AAAAMMDD): ");
    scanf("%d", &fecha);

    // --- CORRECCIÓN: VALIDAR ANTES DE INSERTAR [cite: 35, 38] ---
    if (buscarNodo(raiz, fecha) != NULL) {
        printf("ERROR: Ya existe un lote con la fecha %d. No se puede procesar.\n", fecha);
        return; // Nos salimos de la función sin hacer nada más
    }
    // -------------------------------------------------------------

    printf("Ingrese nombre del producto: ");
    scanf("%s", nombre);

    printf("Ingrese cantidad de stock: ");
    scanf("%d", &stock);

    // Insertar en el AVL
    raiz = insertar(raiz, fecha, stock, nombre);

    printf("Producto registrado correctamente.\n");
}

void registrarPedido() {

    if (raiz == NULL) {
        printf("No hay productos registrados.\n");
        return;
    }

    NodoArbol *nodo = buscarMasCercano(raiz);

    printf("\nProducto asignado:\n");
    printf("Fecha: %d\n", nodo->fecha_vencimiento);
    printf("Producto: %s\n", nodo->nombre_producto);
    printf("Stock disponible: %d\n", nodo->stock_total);

    Pedido *nuevo = (Pedido*) malloc(sizeof(Pedido));

    printf("Ingrese destino: ");
    scanf("%s", nuevo->nombre_destino);

    printf("Ingrese cantidad solicitada: ");
    scanf("%d", &nuevo->cantidad_solicitada);

    nuevo->siguiente = NULL;

    // Encolar en FIFO
    if (nodo->cola_head == NULL) {
        nodo->cola_head = nuevo;
        nodo->cola_tail = nuevo;
    } else {
        nodo->cola_tail->siguiente = nuevo;
        nodo->cola_tail = nuevo;
    }

    // Descontar stock
    nodo->stock_total -= nuevo->cantidad_solicitada;

    printf("\nPedido registrado exitosamente.\n");
}

void reporteInorden() {
    printf("\n=== REPORTE COMPLETO (INORDEN) ===\n");
    inordenMostrar(raiz);
}

NodoArbol* buscarNodo(NodoArbol *actual, int fecha) {
    if (actual == NULL) return NULL;

    if (fecha < actual->fecha_vencimiento)
        return buscarNodo(actual->izq, fecha);

    else if (fecha > actual->fecha_vencimiento)
        return buscarNodo(actual->der, fecha);

    else
        return actual; // Encontrado
}

/* Cancelar pedido (Case 3.1) - VERSIÓN MANUAL (Sin string.h)
   Busca el pedido por nombre de destino comparando char por char,
   desencola, libera memoria y devuelve stock.
*/
void cancelarPedido() {
    int fecha;
    char destino[40]; // Variable para guardar el nombre a buscar
    
    printf("\n=== CANCELAR PEDIDO ESPECIFICO ===\n");
    printf("Ingrese la fecha de vencimiento del lote (AAAAMMDD): ");
    scanf("%d", &fecha);

    // 1. Buscar el lote en el árbol
    NodoArbol *nodo = buscarNodo(raiz, fecha);
    if (nodo == NULL) {
        printf("No se encontro un lote con esa fecha.\n");
        return;
    }

    if (nodo->cola_head == NULL) {
        printf("No hay pedidos para cancelar en este lote.\n");
        return;
    }

    // Mostrar pedidos disponibles
    printf("\nPedidos en este lote:\n");
    Pedido *temp = nodo->cola_head;
    int contador = 1;
    while (temp != NULL) {
        printf("%d. Destino: %s | Cantidad: %d\n", 
               contador, temp->nombre_destino, temp->cantidad_solicitada);
        temp = temp->siguiente;
        contador++;
    }

    // 2. Pedir el nombre del destino a eliminar
    printf("Ingrese el nombre del destino a cancelar (Ej: Nuqui): ");
    scanf("%s", destino);

    // Punteros para recorrer la cola
    Pedido *anterior = NULL;
    Pedido *actual = nodo->cola_head;
    int encontrado = 0;

    // 3. Recorrer la cola buscando el nombre MANUALMENTE
    while (actual != NULL) {
        // Comparar destinos manualmente
        int i = 0;
        int iguales = 1;
        while (destino[i] != '\0' || actual->nombre_destino[i] != '\0') {
            if (destino[i] != actual->nombre_destino[i]) {
                iguales = 0;
                break;
            }
            i++;
        }

        if (iguales) {
            encontrado = 1;
            break;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    if (!encontrado) {
        printf("No se encontro un pedido con ese destino.\n");
        return;
    }

    // Eliminar el pedido de la cola
    if (anterior == NULL) {
        // Es el primero
        nodo->cola_head = actual->siguiente;
        if (nodo->cola_head == NULL) {
            nodo->cola_tail = NULL;
        }
    } else {
        // Esta en medio o al final
        anterior->siguiente = actual->siguiente;
        if (actual == nodo->cola_tail) {
            nodo->cola_tail = anterior;
        }
    }

    // Devolver el stock
    nodo->stock_total += actual->cantidad_solicitada;

    printf("\nPedido cancelado exitosamente.\n");
    printf("Destino: %s\n", actual->nombre_destino);
    printf("Se devolvio al stock: %d unidades\n", actual->cantidad_solicitada);

    free(actual);
}
/* Eliminar lote completo (Case 3.2)
   Pide la fecha del lote, elimina el nodo del árbol AVL,
   libera toda la memoria asociada (incluyendo la cola FIFO).
*/    
NodoArbol* eliminarNodoAVL(NodoArbol* raiz, int fecha) {

    if (raiz == NULL)
        return NULL;

    // Buscar nodo por ABB
    if (fecha < raiz->fecha_vencimiento) {
        raiz->izq = eliminarNodoAVL(raiz->izq, fecha);
    }
    else if (fecha > raiz->fecha_vencimiento) {
        raiz->der = eliminarNodoAVL(raiz->der, fecha);
    }
    else {
        // ===============================
        //        CASO ENCONTRADO
        // ===============================

        // Caso 1: 0 o 1 hijo
        if (raiz->izq == NULL || raiz->der == NULL) {

            NodoArbol *hijo = (raiz->izq != NULL) ? raiz->izq : raiz->der;

            // Liberar la cola FIFO del nodo actual
            Pedido *p = raiz->cola_head;
            while (p != NULL) {
                Pedido *borrar = p;
                p = p->siguiente;
                free(borrar);
            }

            // Sin hijos
            if (hijo == NULL) {
                free(raiz);
                return NULL;
            }

            // Un hijo: sustituir el nodo eliminado por el hijo
            NodoArbol *temp = hijo;
            free(raiz);
            return temp;
        }
        else {
            // === Caso 2 hijos ===

            // Sucesor inorden (menor del subárbol derecho)
            NodoArbol* sucesor = buscarMasCercano(raiz->der);

            // Copiamos datos del sucesor
            raiz->fecha_vencimiento = sucesor->fecha_vencimiento;
            raiz->stock_total = sucesor->stock_total;

            // Copiar nombre
            int i = 0;
            while (sucesor->nombre_producto[i] != '\0') {
                raiz->nombre_producto[i] = sucesor->nombre_producto[i];
                i++;
            }
            raiz->nombre_producto[i] = '\0';

            // Copiar cola FIFO
            raiz->cola_head = sucesor->cola_head;
            raiz->cola_tail = sucesor->cola_tail;

            // Desvinculamos la cola del sucesor para que al eliminarlo 
            // NO borre la lista de pedidos que acabamos de mover.
            sucesor->cola_head = NULL; 
            sucesor->cola_tail = NULL;

            // Eliminar sucesor
            raiz->der = eliminarNodoAVL(raiz->der, sucesor->fecha_vencimiento);
        }
    }

    if (raiz == NULL)
        return NULL;

    // ===== Reutilizamos funciones existentes =====
    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));

    int balance = obtenerBalance(raiz);

    // Casos de rotación AVL (YA CREADOS)
    if (balance > 1 && obtenerBalance(raiz->izq) >= 0)
        return rotacionDerecha(raiz);

    if (balance > 1 && obtenerBalance(raiz->izq) < 0) {
        raiz->izq = rotacionIzquierda(raiz->izq);
        return rotacionDerecha(raiz);
    }

    if (balance < -1 && obtenerBalance(raiz->der) <= 0)
        return rotacionIzquierda(raiz);

    if (balance < -1 && obtenerBalance(raiz->der) > 0) {
        raiz->der = rotacionDerecha(raiz->der);
        return rotacionIzquierda(raiz);
    }

    return raiz;
}

void eliminarLote() {
    int fecha;
    printf("\nIngrese la fecha del lote que desea eliminar: ");
    scanf("%d", &fecha);

    if (buscarNodo(raiz, fecha) == NULL) {
        printf("No existe un lote con esa fecha.\n");
        return;
    }

    raiz = eliminarNodoAVL(raiz, fecha);
    printf("Lote eliminado correctamente.\n");
}
// ----------------------------------------------------
// FUNCIÓN PRINCIPAL (MAIN)
// ----------------------------------------------------
int main() {

int opcion;

    do {
        printf("\n=====================================\n");
        printf("      SISTEMA DE GESTION DE LOTES     \n");
        printf("=====================================\n");
        printf("1. Registrar Producto (Insertar AVL)\n");
        printf("2. Registrar Pedido de Despacho\n");
        printf("3. Cancelaciones\n");
        printf("   3.1 Cancelar un pedido\n");
        printf("   3.2 Eliminar lote completo\n");
        printf("4. Reporte de Estado (Inorden)\n");
        printf("5. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {

            case 1:
                registrarProducto();
                break;

            case 2:
                registrarPedido();
                break;

            case 3: {
                int sub;

                printf("\n--- CANCELACIONES ---\n");
                printf("1. Cancelar pedido\n");
                printf("2. Eliminar lote completo\n");
                printf("Seleccione una opcion: ");
                scanf("%d", &sub);

                if (sub == 1) {
                    cancelarPedido();
                } else if (sub == 2) {
                    eliminarLote();

                } else {
                    printf("Opcion no valida.\n");
                }
                break;
            }

            case 4:
                reporteInorden(); 
                break;

            case 5:
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 5);

    return 0;
}