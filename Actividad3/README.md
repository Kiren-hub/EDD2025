✈️ EDD2025 - Actividad 3: Sistema de Pasajeros con Listas Enlazadas 🚀
 
🎯 Descripción
¡Bienvenidos al Actividad 3 del curso de Estructuras de Datos 2025! Este proyecto es un sistema en C que gestiona pasajeros de una aerolínea usando listas enlazadas y memoria dinámica. ¿El objetivo? Registrar pasajeros, manejar overbooking como los grandes y listar quién sube al avión primero. Todo al puro estilo "bajo nivel", sin complicaciones innecesarias. 🛫
✨ Funcionalidades clave

Capacidad del avión: Ingresa cuántos asientos tiene el avión.
Overbooking pro: Vende hasta un 10% más de tiquetes que la capacidad.
Datos de pasajeros: Guarda tipo de documento (CC, TI, PA), número y primer apellido.
Abordaje en orden: El primero en registrarse, primero sube (hasta llenar la capacidad).
Memoria dinámica: Usa malloc y free para una gestión eficiente.

🛠️ Requisitos

Compilador C (recomendado: gcc).
Librería math.h (para el cálculo de overbooking con ceil).
Terminal compatible (Windows, Linux, macOS).

🚀 Instalación y ejecución

Clona este repo:
git clone https://github.com/Kiren-hub/EDD2025.git


Entra a la carpeta:
cd EDD2025


Compila el código:
gcc Actividad3/gestionpasajeros.c -o gestionpasajeros -lm


¡Arranca el programa!
./gestionpasajeros



📋 Cómo usar

Ingresa la capacidad máxima del avión al iniciar.

Navega el menú interactivo:

1 ➡️ Registrar pasajero (tipo de documento, número, apellido).

2 ➡️ Ver la lista de pasajeros en orden de abordaje.

3 ➡️ Salir y liberar la memoria.


El programa respeta el límite de overbooking y te muestra quiénes abordan (los primeros hasta la capacidad).


📂 Estructura del repositorio
EDD2025/

└── Actividad3/

    ├── gestionpasajeros.c  # Código fuente en C
    
    └── README.md           # ¡Este archivo, puro estilo!

👨‍✈️ Autor

Nombre: David Aponte

GitHub: Kiren-hub

Correo: josedavid08new@gmail.com

📜 Licencia
Proyecto educativo sin licencia específica. Hecho con 💪 para el curso EDD2025.