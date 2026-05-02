# Propuesta de Seminario: Práctica 3 (Curso 2025/2026)
## Juego: 5-en-Raya Táctico (Modo Competición 9x9)

Esta propuesta adapta la estructura del seminario del año pasado al nuevo juego, manteniendo la coherencia visual y pedagógica.

---

### Diapositiva 1: Portada
- **Título**: Práctica 3: Búsqueda con Adversario.
- **Subtítulo**: El 5-en-Raya Táctico (Modo Competición 9x9).
- **Información**: Grado en Ingeniería Informática / IA. Universidad de Granada.
- **Imagen**: Un tablero de 9x9 con algunas piezas colocadas y efectos visuales (bombas, casillas verdes/rojas).

### Diapositiva 2: Índice
1. Introducción Histórica y Evolución.
2. Reglas del Modo Competición 9x9.
3. El Software y el Agente.
4. Diseño y Desarrollo (Algoritmos).
5. Evaluación y Entrega.

---

### Diapositiva 3: Introducción - Del 3-en-Raya al 5-en-Raya
- Breve historia del Tic-Tac-Toe.
- El problema de la resolución trivial (siempre empate).
- ¿Por qué el Modo Competición? Aumento del espacio de estados y complejidad estratégica.
- **Imagen**: Comparativa visual entre un tablero de 3x3 (simple) y el de 9x9 (complejo).

### Diapositiva 4: Reglas (I) - Tablero y Objetivo
- Tablero de 9x9.
- Objetivo: Alinear **5 piezas consecutivas** (horizontal, vertical o diagonal).
- Configuración inicial: 4 piezas pre-colocadas en el centro (conectividad inicial).
- **Imagen**: Captura del tablero inicial con las 4 piezas centrales (2 de cada color).

### Diapositiva 5: Reglas (II) - Secuencia de Turnos (1-2-2-2)
- El flujo de la partida no es simétrico.
- Turno 1: Jugador 1 coloca **1 pieza**.
- Turnos sucesivos: Cada jugador coloca **2 piezas**.
- Permite crear amenazas dobles y defensas complejas en un solo turno.
- **Imagen**: Diagrama de flujo o cronología de turnos mostrando (1) -> (2) -> (2) -> ...

### Diapositiva 6: Reglas (III) - La Regla de la Trinidad
- División matemática del tablero.
- Restricción: `(fila + columna) % 3 == faseActual % 3`.
- La fase avanza al cambiar de turno.
- **Imagen**: El tablero de 9x9 coloreado con un patrón de "tablero de ajedrez triple" (tres colores/símbolos) para visualizar las casillas permitidas en cada fase.

### Diapositiva 7: Reglas (IV) - Regla de Adyacencia
- Obligación de colocar piezas adyacentes a las ya existentes.
- Evita el juego "disperso" y fomenta el conflicto directo.
- Excepción: Primer movimiento.
- **Imagen**: Un tablero con una "zona de influencia" resaltada alrededor de las piezas actuales donde es legal colocar.

### Diapositiva 8: Reglas (V) - Casillas Especiales
- **Verdes (Místicas)**: +1 movimiento extra inmediato.
- **Rojas (Sabotaje)**: La pieza se convierte al color del oponente.
- **Amarillas (Bomba)**: Limpia fila y columna (excepto la bomba).
- **Imagen**: Iconos representativos de cada casilla y un ejemplo visual de una explosión de bomba (limpiando líneas).

---

### Diapositiva 9: El Software
- Repositorio GitHub: `https://github.com/ugr-ccia-IA/practica3`.
- Opciones de ejecución:
    - Modo Gráfico (Visualización y debug).
    - Modo Batch (Entrenamiento y competición rápida).
- **Imagen**: Captura de pantalla del simulador gráfico funcionando.

### Diapositiva 10: Implementación del Agente
- Archivos clave: `AgenteEstudiante.cpp` y `AgenteEstudiante.hpp`.
- Clase `AgenteEstudiante`.
- No modificar el resto del motor (ninja_engine).
- **Imagen**: Captura de código (snippet) de la estructura de la clase `AgenteEstudiante`.

---

### Diapositiva 11: Algoritmos - MiniMax y Poda Alfa-Beta
- Base del agente deliberativo.
- **MiniMax**: Exploración de todo el árbol.
- **Poda Alfa-Beta**: Optimización necesaria debido al gran factor de ramificación (agravado por colocar 2 piezas/turno).
- **Imagen**: Diagrama del árbol de búsqueda con ramas podadas (tachadas).

### Diapositiva 12: Algoritmos - Resolución de Finales (Status)
- Implementación de `Status(...)`.
- Uso de Backtracking para determinar si un estado es victoria/derrota/empate matemático.
- Ayuda a tomar decisiones perfectas en el "endgame".
- **Imagen**: Un tablero simplificado (p.e. 4x4) mostrando una situación de victoria forzada detectada por Status.

### Diapositiva 13: Mejoras Avanzadas (Opcional/Valorado)
- **Profundización Iterativa**: Para gestionar el tiempo de respuesta.
- **Ordenación de Movimientos**: Priorizar casillas especiales o amenazas de línea.
- **Heurística Robusta**: Evaluación de líneas abiertas, control del centro y gestión de casillas especiales.
- **Imagen**: Gráfico comparativo de nodos explorados con y sin ordenación de movimientos.

---

### Diapositiva 14: La Heurística
- El corazón de la IA.
- ¿Cómo evaluar un tablero de 9x9?
    - Valor de las líneas de 2, 3, 4 piezas.
    - Cercanía a casillas bomba/verdes.
    - Evitar casillas rojas.
- Niveles de dificultad: Ninja 1 (Básico), Ninja 2 (Avanzado), Ninja 3 (Maestro), Ninja 4 (Desafío Final).
- **Imagen**: Gráfico de barras o "radar" comparando las prioridades de la heurística (Ataque vs Defensa vs Control de Casillas).

---

### Diapositiva 15: Evaluación
- Calificación de 0 a 10:
    - **Memoria (0-2 pts)**: Claridad, análisis y tablas.
    - **Calidad Algorítmica (0-2 pts)**: Implementación correcta de Poda y Status.
    - **Eficacia (0-6 pts)**: Ganar a los 4 Ninjas (J1 y J2). En caso de tablero lleno, el desempate se decide por número de líneas de 4.
- **Imagen**: Tabla ejemplo de resultados (Ninja 1: Win, Ninja 2: Win, Ninja 3: Win, Ninja 4: Loss...).

### Diapositiva 16: Entrega y Fechas
- **Fecha Límite**: 31 de Mayo de 2026 (23:00h).
- Entrega individual.
- Formato: ZIP con `.cpp`, `.hpp` y Memoria PDF.
- **¡OJO!**: Política estricta de copia (suspense automático).
- **Imagen**: Icono de un calendario marcando la fecha y un recordatorio de "Originalidad".

---

### Resumen de Imágenes Sugeridas (Checklist):
1. [ ] **Tablero 3x3 vs 9x9**: Para ilustrar la complejidad.
2. [ ] **Patrón de la Trinidad**: Colorear el tablero para entender `(f+c)%3`.
3. [ ] **Diagrama de Turnos**: Para explicar el 1-2-2-2.
4. [ ] **Efecto Bomba**: Antes y después de colocar en una casilla amarilla.
5. [ ] **Árbol de Poda**: Clásico diagrama de IA.
6. [ ] **Interfaz Gráfica**: Pantallazo del software real.
