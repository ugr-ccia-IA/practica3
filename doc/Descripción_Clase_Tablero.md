# Descripción de la Clase Tablero

La clase `Tablero` es el núcleo del motor de juego "Ninja 9x9". Gestiona el estado de la partida, las reglas de la Trinidad (Módulo 3) y los efectos de las casillas especiales.

## Métodos de Control de Turnos

### `int getJugadorTurno() const`
Devuelve el identificador del jugador (`1` o `2`) al que le toca realizar el próximo movimiento según el reloj global.

### `int getFaseActual() const`
Devuelve el número de fase de la Trinidad actual. Equivale al número total de turnos de jugador que se han completado (empieza en 2). La fase solo avanza cuando un jugador agota todas sus piezas y cede el turno al rival.

### `int getTurnoActual() const`
Devuelve el número total de *piezas colocadas* en el tablero (también conocido como plies).
*   **Nota**: Empieza en **4**, asumiendo que ya se ha colocado el "Diamante Ninja" central inicial. Este contador aumenta con cada pieza puesta exitosamente, e incluye las piezas colocadas como resultado del bono verde.

### `int getMovimientosRestantes() const`
Devuelve el número de piezas que el jugador actual aún tiene derecho a colocar antes de que su turno termine y pase al oponente (parte de la secuencia 1-2-2-2).

### `bool ponerPieza(int fila, int columna, int jugador)`
Intenta colocar una pieza. Aplica automáticamente las reglas de adyacencia y el ciclo de Módulo 3. Procesa también los efectos de casillas rojas, verdes y amarillas.
*   **Retorno**: `true` si el movimiento es legal, `false` en caso contrario.

### `void pasarTurno()`
Avanza el cronograma global del juego sin colocar pieza. Se usa si un jugador no tiene movimientos válidos.

### `std::vector<Tablero> getSucesores() const`
Genera y devuelve todos los estados de tablero posibles a los que se puede llegar desde el estado actual con un único movimiento legal. Es el método fundamental para alimentar los algoritmos de búsqueda (MiniMax/Alfa-Beta).
*   **Nota**: Aplica internamente todas las reglas (Trinidad, Adyacencia y Efectos) y solo devuelve tableros resultantes de movimientos válidos.

## Métodos de Consulta y Evaluación

### `int getCelda(int fila, int columna) const`
Devuelve el contenido de una celda: `0` (vacío), `1` (Jugador 1) o `2` (Jugador 2).

### `TipoCelda getTipoCelda(int fila, int columna) const`
Devuelve la especialidad de la casilla: `NORMAL`, `ROJO`, `VERDE` o `AMARILLO`.

### `int comprobarGanador()`
Escanea el tablero en busca de una línea de longitud `n`. 
*   **Retorno**: ID del ganador, `0` si no hay, o `-1` si hay empate técnico.

### `int getGanadorDesempate() const`
Calcula el ganador por puntos cuando el tablero está lleno. Según las reglas vigentes, el ganador se decide exclusivamente por quién posee un mayor número de **líneas de 4 piezas**. Si el número de líneas de 4 es idéntico, se devuelve `-1` (empate).

### `int contarCombinaciones(int n, int jugador) const`
Cuenta cuántas líneas de longitud exacta `n` tiene el jugador especificado. Fundamental para la implementación de heurísticas.

### `bool tieneMovimientosValidos() const`
Verifica si el jugador actual tiene alguna casilla legal disponible según las reglas Ninja.

---

## Reglas Especiales Ninja

### La Trinidad (Módulo 3)
Solo se permite colocar piezas en casillas donde `(fila + columna) % 3 == (fase % 3)`. El residuo válido cambia cada vez que el reloj global avanza de fase.

### Casillas Especiales
*   **Verdes (Místicas)**: Bono de movimiento. Otorgan inmediatamente **+1 movimiento extra** en el turno actual. Permiten al jugador colocar 3 (o más) piezas seguidas si encadena varias casillas verdes.
*   **Rojas (Sabotaje)**: Trampa de piezas. Colocan una pieza del oponente en lugar de la propia.
*   **Amarillas (Bombas)**: Efecto de área. Limpian la fila y columna completas al ser activadas (la pieza que activa la bomba sobrevive).
