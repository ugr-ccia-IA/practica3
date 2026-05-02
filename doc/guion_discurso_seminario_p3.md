# Guion de la Presentación: Práctica 3 - Búsqueda con Adversario
## Juego: 5-en-Raya Táctico (Modo Competición 9x9)

Este documento contiene el discurso sugerido para cada diapositiva del seminario.

---

### Diapositiva 1: Portada
"Estamos en la presentación de la tercera práctica de Inteligencia Artificial. Hoy vamos a presentar el proyecto final del bloque de búsqueda con adversario. Dejamos atrás los entornos deterministas simples para adentrarnos en la competición pura con el '5-en-Raya Táctico'. En esta sesión vamos a desglosar las reglas de esta variante compleja y las técnicas algorítmicas que tendréis que implementar para que vuestro agente sea capaz de derrotar a los cuatro niveles de ninjas que hemos preparado."

### Diapositiva 2: Índice
"La sesión se divide en cinco bloques principales. Empezaremos con una breve contextualización histórica para entender por qué este juego es un reto computacional. Después, analizaremos en detalle las reglas del Modo Competición 9x9. En el tercer bloque veremos el software y la estructura de clases. El cuarto bloque será el más técnico, donde repasaremos los algoritmos de búsqueda y optimización. Finalmente, cerraremos con los criterios de evaluación y las fechas clave."

### Diapositiva 3: Introducción - Del 3-en-Raya al 5-en-Raya
"Todos conocemos las 'Tres en Raya'. Es un juego milenario que, aunque es útil para aprender lógica básica, tiene un problema fundamental en computación: está resuelto. Con juego perfecto, la partida siempre acaba en empate. El espacio de estados es minúsculo. Para esta práctica, hemos evolucionado el concepto hacia el '5-en-Raya Táctico'. Al aumentar el tablero a 9x9 y exigir 5 piezas alineadas, el factor de ramificación explota. Ya no es posible explorar todo el árbol de forma trivial; aquí es donde entra en juego vuestra capacidad para diseñar heurísticas y optimizar la búsqueda."

### Diapositiva 4: Reglas (I) - Tablero y Objetivo
"Entremos en materia. El tablero estándar es de 9x9 casillas. El objetivo es simple: ser el primero en alinear 5 piezas consecutivas de tu color, ya sea en horizontal, vertical o diagonal. Pero atención al inicio: la partida no comienza con el tablero vacío. Para forzar la interacción desde el primer segundo, el sistema coloca 4 piezas en el centro (dos de cada jugador). Esto garantiza que la regla de adyacencia que veremos luego se pueda aplicar desde el primer movimiento."

### Diapositiva 5: Reglas (II) - Secuencia de Turnos (1-2-2-2)
"Aquí es donde el juego rompe con la simetría tradicional. El flujo de turnos sigue un esquema 1-2-2-2. El Jugador 1, para compensar la ventaja de empezar, solo coloca UNA pieza en su primer turno. A partir de ahí, cada jugador colocará DOS piezas por turno. Esto cambia radicalmente la estrategia: en un solo turno puedes pasar de estar defendiendo a crear una amenaza doble que tu oponente no pueda bloquear. Tened esto muy en cuenta al diseñar vuestro minimax, porque un 'nodo' de vuestra búsqueda debe considerar estos movimientos dobles."

### Diapositiva 6: Reglas (III) - La Regla de la Trinidad
"Para evitar que el juego se convierta en una colocación arbitraria, introducimos la 'Regla de la Trinidad'. El tablero está dividido matemáticamente en tres fases. Solo podéis colocar una pieza si la suma de su fila y columna, al dividirla por tres, da el resto de la fase actual. Como la fase avanza en cada turno, esto crea un patrón dinámico de casillas permitidas y prohibidas. Es vital que vuestra IA consulte el método `getFaseActual()` para no intentar movimientos inválidos."

### Diapositiva 7: Reglas (IV) - Regla de Adyacencia
"Además de la Trinidad, existe la regla de adyacencia. Excepto en la configuración inicial, toda pieza que pongáis debe estar tocando (en cualquiera de las 8 direcciones) a una pieza que ya esté en el tablero. Esto impide que los jugadores 'se escapen' a esquinas aisladas del tablero y los obliga a luchar por el control del centro y de las líneas ya creadas. El conflicto es constante y directo."

### Diapositiva 8: Reglas (V) - Casillas Especiales
"Para darle el toque táctico final, el tablero contiene tres tipos de casillas especiales. Las verdes o 'Místicas' os regalan un movimiento extra; son oro puro para cerrar una línea de 5. Las rojas de 'Sabotaje' son trampas: si ponéis vuestra pieza ahí, el sistema le cambiará el color al de vuestro rival. Por último, las amarillas son 'Bombas': al activarlas, limpian toda su fila y columna. Saber cuándo usar una bomba para destruir un ataque del rival o cuándo arriesgarse a una casilla roja para bloquear un camino es lo que diferenciará a un agente normal de uno excelente."

### Diapositiva 9: El Software
"El software está disponible en el GitHub oficial de la asignatura. Tenéis dos formas de ejecutarlo. El modo gráfico es vuestra mejor herramienta de depuración: veréis las explosiones de las bombas y podréis analizar visualmente por qué vuestra IA toma ciertas decisiones. Por otro lado, el modo batch es ideal para realizar pruebas de rendimiento y jugar cientos de partidas rápidas contra los ninjas para obtener estadísticas de victoria."

### Diapositiva 10: Implementación del Agente
"Vuestro trabajo se centra exclusivamente en dos archivos: `AgenteEstudiante.cpp` y su cabecera. En la clase `AgenteEstudiante` encontraréis el método `think`, que es el cerebro de vuestra IA. Tenéis total libertad para añadir métodos privados o estructuras auxiliares, pero recordad: no modifiquéis el motor del juego ni el resto de archivos, ya que vuestra entrega se compilará con el motor original en nuestro servidor de evaluación."

### Diapositiva 11: Algoritmos - MiniMax y Poda Alfa-Beta
"El núcleo del agente debe ser un algoritmo MiniMax con Poda Alfa-Beta. Dado que el factor de ramificación es alto (por la posibilidad de poner 2 piezas), la poda es absolutamente obligatoria. Sin ella, vuestro agente apenas podrá profundizar 2 o 3 niveles antes de agotar el tiempo. Con una buena poda Alfa-Beta y una gestión eficiente de los nodos, deberíais aspirar a profundidades de 6 o más para competir seriamente contra los niveles superiores."

### Diapositiva 12: Algoritmos - Resolución de Finales (Status)
"A diferencia de las versiones anteriores, este año pedimos implementar el método `Status`. Es una función basada en backtracking que debe ser capaz de resolver de forma exacta los finales de partida. Cuando queden pocos huecos o la victoria sea inminente, `Status` debe confirmar matemáticamente si el estado actual es una victoria, derrota o empate forzado. Es vuestro 'seguro de vida' para no cometer errores en los últimos movimientos."

### Diapositiva 13: Mejoras Avanzadas (Opcional/Valorado)
"Para aquellos que queráis la máxima nota, recomendamos dos mejoras. Primero, la 'Profundización Iterativa', que os permite aprovechar al máximo el tiempo de cómputo sin riesgo de que la partida se detenga por timeout. Segundo, la 'Ordenación de Movimientos': si exploráis primero las casillas más prometedoras (como las verdes o las que completan líneas), la poda Alfa-Beta será mucho más agresiva y eficiente, permitiéndoos ver 'más allá' en el futuro del juego."

### Diapositiva 14: La Heurística
"La heurística es donde realmente vais a ganar o perder la práctica. No basta con contar cuántas piezas tenéis. Debéis valorar las amenazas: ¿cuántas líneas de 4 piezas abiertas tengo? ¿cuántas tiene el rival? ¿estoy cerca de una casilla roja? Recordad que los Ninjas tienen niveles de dificultad progresivos. Desde el Ninja 1, que es asequible, hasta el Ninja 4, que es el desafío final y requiere una visión estratégica muy aguda. Vuestra función de evaluación debe ser capaz de cuantificar qué tan 'prometedor' es un estado intermedio para sobrevivir a los niveles superiores."

### Diapositiva 15: Evaluación
"La práctica se califica sobre 10 puntos. 2 puntos corresponden a la memoria, donde debéis explicar vuestro diseño y mostrar tablas de resultados. Otros 2 puntos valoran la calidad del código y la correcta implementación de Alfa-Beta y Status. Los 6 puntos restantes dependen de la eficacia: jugaréis un total de 8 partidas contra los 4 niveles de ninjas (como J1 y J2). Recordad que si el tablero se llena, el sistema decidirá el ganador contando exclusivamente quién tiene más líneas de 4 piezas; si hay empate en ese recuento, la partida será un empate técnico."

### Diapositiva 16: Entrega y Fechas
"Para terminar, anotad bien la fecha: domingo 31 de mayo a las 23:00 horas. La entrega es individual y se hace a través de la plataforma web en un único fichero ZIP. Un recordatorio muy importante: tenemos sistemas de detección de plagio muy avanzados que comparan vuestro código con el de años anteriores y el de vuestros compañeros. La originalidad es innegociable. ¡Mucha suerte y que gane el mejor algoritmo!"
