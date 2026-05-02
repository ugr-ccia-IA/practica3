# Guía del Entorno Gráfico - Ninja 9x9 (Práctica 3)

La interfaz gráfica de esta práctica ha sido diseñada utilizando **OpenGL** con una estética *Premium* basada en *Glassmorphism* y efectos de neón. Este documento describe los elementos visuales, el panel de control y las funcionalidades disponibles.

## 1. El Tablero de Juego
El tablero es una rejilla de tamaño variable (por defecto 9x9). Las celdas se comportan según la **Regla de la Trinidad** y las casillas especiales.

### Elementos Visuales:
*   **Fichas Cian (X)**: Pertenecen al Jugador 1.
*   **Fichas Magenta (O)**: Pertenecen al Jugador 2.
*   **Efecto Neón**: Las fichas recién colocadas emiten un resplandor del color del jugador.
*   **Línea Ganadora**: Cuando un jugador gana, se dibuja una línea gruesa traslúcida uniendo las piezas que forman el "N-en-raya".

### Tipos de Casillas:
*   **Negras/Azul oscuro**: Casillas normales.
*   **Verdes (Místicas)**: Permiten repetir turno.
*   **Rojas (Sabotaje)**: Colocan una ficha del oponente en lugar de la propia.
*   **Amarillas (Bomba)**: Limpian la fila y columna al ser activadas.

---

## 2. Panel Lateral de Información
Situado a la derecha, este panel traslúcido muestra el estado en tiempo real de la partida y la IA.

### Bloque de Configuración:
*   **Objetivo**: Indica cuántas piezas en raya se necesitan para ganar (ej: `Objetivo: 5`).
*   **Turno**: Indica a quién le toca mover (`J1` o `J2`) y si ese jugador es una **IA** o un **Humano (TU)**.

### Bloque de Estado Actual:
*   **Cronómetro de Turno**: Tiempo que lleva el jugador actual pensando su jugada.
*   **Tiempos Acumulados**: Tiempo total consumido por cada jugador a lo largo de toda la partida.
*   **Marcador de Puntos (Tiebreaker)**: En caso de que el tablero se llene, se muestran las líneas de 5 y 4 conseguidas por cada jugador (ej: `J1: 2 | 5` significa 2 líneas de cinco y 5 líneas de cuatro).

---

## 3. Botones de Control

### 🟢 Botón `Next Mov` (Sugerencia)
Este botón es una herramienta de asistencia y depuración para el jugador humano.
*   **Funcionalidad**: Al pulsarlo, el sistema invoca una instancia temporal de la IA del estudiante. Esta IA analizará el tablero con la profundidad configurada (`-d`) y realizará el mejor movimiento posible automáticamente.
*   **Estado "Thinking..."**: Mientras la IA calcula el movimiento, el botón cambiará a color naranja y mostrará el texto "Thinking...". Durante este proceso, la interfaz permanece activa pero no se permiten nuevos movimientos hasta que la IA termine.
*   **Uso Recomendado**: Úsalo para ver qué decisión tomaría tu heurística en una posición comprometida.

### 🟣 Botón `Status` (Análisis Completo)
Es la herramienta más potente de análisis estratégico del software.
*   **Funcionalidad**: Inicia una búsqueda exhaustiva (Full Search) para resolver el juego desde el estado actual. A diferencia del botón `Next Mov`, este no usa heurísticas, sino que intenta encontrar el resultado teórico perfecto si ambos jugadores jugaran de forma óptima.
*   **Feedback Visual**: Una vez calculado, mostrará sobre el botón el veredicto (ej: `GANA J1` o `EMPATE`) y las coordenadas de la casilla que lleva a ese resultado.
*   **Limitación**: Debido a la explosión combinatoria, este botón solo es efectivo en tableros pequeños o finales de partida muy avanzados. En tableros de 9x9 vacíos, el tiempo de cálculo puede ser excesivo.

### 🔵 Botón `REINICIAR`
*   Limpia el tablero por completo.
*   Resetea los cronómetros de tiempo acumulado.
*   Vuelve el turno al Jugador 1.
*   *Atajo de teclado*: Tecla `R`.

### 🔴 Botón `SALIR`
*   Cierra la aplicación de forma segura.
*   *Atajo de teclado*: Tecla `ESC`.

---

## 4. Ejemplos de Uso por Consola

### Ejemplo 1: Humano vs IA con asistencia
```bash
./n_en_raya -p1 humano -p2 inteligente -d 5 -id2 1
```
*Aquí puedes jugar como J1 y usar el botón **Next Mov** para que la IA (con heurística 1) te sugiera jugadas contra la IA del J2.*

### Ejemplo 2: Duelo de Heurísticas (Benchmark visual)
```bash
./n_en_raya -p1 inteligente -id1 1 -p2 inteligente -id2 2 -t 3.0
```
*Verás a las dos IAs enfrentarse en tiempo real. La columna derecha te permitirá comparar quién consume más tiempo y quién va ganando en el marcador de puntos.*

### Ejemplo 3: Tablero Grande
```bash
./n_en_raya -f 12 -c 12 -n 6
```
*Ajusta el entorno a un tablero de 12x12 donde hay que conectar 6 piezas.*
