# INTELIGENCIA ARTIFICIAL
## E.T.S. de Ingenierías Informática y de Telecomunicación
### Grado en Ingeniería Informática / Inteligencia Artificial

# Práctica 3: Búsqueda con Adversario (Juegos)
## El 5-en-Raya Táctico (Modo Competición 9x9)

**DEPARTAMENTO DE CIENCIAS DE LA COMPUTACIÓN E INTELIGENCIA ARTIFICIAL**
**UNIVERSIDAD DE GRANADA**
**Curso 2025-2026**

---

## 1. Introducción
La tercera práctica de la asignatura consiste en el diseño e implementación de técnicas de búsqueda con adversario en un entorno de juegos. En esta ocasión, trabajaremos con una versión avanzada y altamente estratégica del clásico juego de las "Tres en Raya", denominada **5-en-Raya Táctico (Modo Competición 9x9)**.

A diferencia del juego tradicional (que resulta en empate bajo juego perfecto), esta variante introduce restricciones de colocación, turnos asimétricos y casillas con efectos especiales que amplían enormemente el espacio de estados y el desafío computacional.

El objetivo es aplicar el algoritmo de **Poda Alfa-Beta** y técnicas de optimización para dotar de comportamiento inteligente deliberativo a un agente capaz de competir contra oponentes automáticos (los "Ninjas").

## 2. Requisitos
Para poder realizar la práctica, es necesario disponer de:
1.  **Conocimientos de C++**: Clases, punteros, recursividad y gestión de memoria.
2.  **Librerías Gráficas**: OpenGL y GLUT (para la visualización del tablero).
    - *Ubuntu*: `sudo apt install freeglut3-dev`
3.  **Sistema de Construcción**: CMake (versión mínima 3.10).

## 3. Objetivo de la Práctica
El/la alumno/a deberá implementar la lógica de decisión de un agente inteligente en el archivo `AgenteEstudiante.cpp`. Los objetivos técnicos incluyen:
-   Implementación del algoritmo **MiniMax con Poda Alfa-Beta**.
-   Diseño de una **Función Heurística** robusta para evaluar estados no terminales.
-   Implementación del **Algoritmo Status** para la resolución completa de finales de partida.

Opcionalmente, se valorará la inclusión de mejoras avanzadas como **Profundización Iterativa (Iterative Deepening)** y Ordenación de Movimientos.

---

## 4. Reglas del Juego (Modo Competición 9x9)
Para facilitar la comprensión del flujo de la partida, a continuación se detallan de forma numerada las reglas que rigen el **5-en-Raya Táctico**:

1. **Objetivo del Juego**: Participan dos jugadores (Jugador 1 y Jugador 2). El objetivo es ser el primero en conseguir alinear **5 piezas consecutivas** de su propio color en línea recta (horizontal, vertical o diagonal).
2. **Tablero e Inicio**: El tablero estándar es de **9x9** casillas. Para garantizar la conectividad desde el primer instante, la partida no empieza vacía: se inicia con **4 piezas pre-colocadas** en el centro (2 del J1 y 2 del J2).
3. **Secuencia de Turnos (1-2-2-2)**: El flujo de colocación es asimétrico. En el Turno 1, el Jugador 1 coloca únicamente **1 pieza**. En todos los turnos sucesivos, los jugadores se alternan colocando un máximo de **2 piezas consecutivas** por turno.
4. **La Regla de la Trinidad**: El espacio de juego se divide en tres fases matemáticas. Un jugador solo puede colocar una pieza en una casilla `(fila, columna)` si el sumatorio de sus coordenadas cumple: `(fila + columna) % 3 == faseActual % 3`. La `faseActual` avanza +1 cuando el jugador agota sus movimientos y cede el turno.
5. **La Regla de Adyacencia**: Exceptuando la configuración inicial, toda pieza que se coloque en el tablero debe situarse obligatoriamente en una casilla vacía que sea **adyacente** (en cualquiera de las 8 direcciones) a una pieza ya existente.
6. **🟢 Casillas Verdes (Místicas)**: Cuando una pieza aterriza en una casilla verde, el jugador gana de forma inmediata **+1 movimiento extra** para usar en su turno actual.
7. **🔴 Casillas Rojas (Sabotaje)**: Estas casillas actúan como trampas. Si un jugador sitúa su pieza aquí, el tablero la absorberá y la transformará automáticamente en una pieza del **color del oponente**.
8. **🟡 Casillas Amarillas (Bomba)**: Al colocar una pieza en esta celda, se produce una detonación que **elimina** (vuelve a dejar vacías) todas las celdas de la misma fila y la misma columna, respetando únicamente la posición de la bomba.
9. **Resolución por Empate (Tie-break)**: Si el tablero se satura por completo sin que nadie logre el objetivo, el sistema otorgará la victoria al jugador que posea un mayor número de **líneas de 4 piezas**. En caso de igualdad en el recuento de líneas de 4, la partida se considerará empate.



---

## 5. Estructura del Código y Desarrollo
Los estudiantes trabajarán exclusivamente sobre la clase `AgenteEstudiante`.

### 5.1. Métodos a Implementar
-   `think(const Tablero& tablero)`: Punto de entrada de la IA.
-   `minimax(...)`: Implementación recursiva del algoritmo Minimax puro (sin poda).
-   `alfaBeta(...)`: Implementación recursiva de la poda Alfa-Beta.
-   `heuristica(const Tablero& tablero)`: Función de evaluación (debe devolver un `double`).
-   `Status(...)`: Resolución completa mediante backtracking.


### 5.2. Utilidades Proporcionadas
-   `getSucesores(const Tablero& t)`: Devuelve un vector con todos los estados válidos respetando la Trinidad y la Adyacencia.
-   `tablero.getJugadorTurno()`: Indica a quién le toca colocar la siguiente pieza (¡Crucial debido a la secuencia 1-2-2-2!).
-   `tablero.getFaseActual()`: Devuelve la fase actual de la Trinidad (para verificar el residuo).
-   `tablero.getMovimientosRestantes()`: Indica cuántas piezas le quedan por colocar al jugador en el turno actual.


---

## 6. Evaluación y Entrega
La calificación (0 a 10 puntos) se distribuirá de la siguiente forma:
-   **Memoria de Prácticas (0-2 puntos)**: Explicación de heurísticas, tablas de resultados y análisis de diseño. (¡Prohibido incluir código!).
-   **Calidad Algorítmica (0-2 puntos)**: Correcta implementación de Alfa-Beta y Status.
-   **Eficacia Competitiva (0-6 puntos)**: Enfrentamiento contra 3 niveles de Ninjas. 1 punto por victoria (jugando como J1 y J2).

**Fecha Límite**: Domingo 31 de Mayo de 2026 a las 23:00h.
