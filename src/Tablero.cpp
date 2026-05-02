#include "Tablero.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief Carga el estado de un tablero desde un fichero de texto.
 * El formato del fichero debe incluir: filas, columnas, nParaGanar, fase de la trinidad, jugador actual
 * seguido de la matriz del tablero.
 * @param ruta Ruta al fichero .board
 * @return Un objeto Tablero inicializado con los datos del fichero.
 */
Tablero Tablero::cargarDesdeFichero(const std::string& ruta) {
    std::ifstream f(ruta);
    if (!f.is_open()) {
        std::cerr << "Error: No se pudo abrir el fichero de tablero " << ruta << std::endl;
        return Tablero(9, 9, 5); // Fallback por defecto
    }

    int fils, cols, nWin;
    f >> fils >> cols >> nWin;
    
    Tablero t(fils, cols, nWin);
    
    std::string lineaRestante;
    std::getline(f, lineaRestante); // Consumir el \n de la primera línea
    std::getline(f, lineaRestante); // Leer la segunda línea entera
    
    std::stringstream ss(lineaRestante);
    int fase = 2, jugador = 1, movs_rest = 2, turno_actual = 0;
    ss >> fase >> jugador >> movs_rest >> turno_actual;

    t.faseActual = fase;
    t.jugadorTurno = jugador;
    t.movimientosRestantes = movs_rest;
    t.turnoActual = turno_actual;

    for (int i = 0; i < fils; ++i) {
        for (int j = 0; j < cols; ++j) {
            int pieza;
            f >> pieza;
            t.rejilla[i][j] = pieza;
        }
    }

    f.close();
    return t;
}

/**
 * @brief Constructor de la clase Tablero.
 * Inicializa las dimensiones, el objetivo de victoria y las casillas especiales si es un tablero 9x9.
 * @param filas Número de filas.
 * @param cols Número de columnas.
 * @param nParaGanar Número de piezas en línea para ganar.
 */
Tablero::Tablero(int filas, int cols, int nParaGanar) 
    : filas(filas), columnas(cols), nParaGanar(nParaGanar) {
    
    bool modoNinja = (filas == 9 && columnas == 9 && nParaGanar == 5);
    
    // Inicialización de estado según el modo
    if (modoNinja) {
        turnoActual = 4;
        faseActual = 2;
        movimientosRestantes = 1;
    } else {
        turnoActual = 0;
        faseActual = 0;
        movimientosRestantes = 1;
    }

    jugadorTurno = 1;
    rejilla.assign(filas, std::vector<int>(cols, 0));
    especialidad.assign(filas, std::vector<TipoCelda>(cols, TipoCelda::NORMAL));

    // --- EL DIAMANTE DEL NINJA (Inicio Cuádruple) ---
    // Solo se aplica en el tablero estándar de 9x9 para mantener compatibilidad
    if (filas == 9 && columnas == 9 && nParaGanar == 5) {
        rejilla[4][0] = 1;
        rejilla[0][4] = 1;
        rejilla[4][8] = 2;
        rejilla[8][4] = 2;
    }

    // --- DISEÑO DEFINITIVO 2026: ANILLOS DE NINJA ---
    if (filas == 9 && columnas == 9 && nParaGanar == 5) {
        // 1. Centro y Cruz Central: ROJO (Sabotaje)
        int midF = filas / 2;
        int midC = columnas / 2;
        especialidad[midF][midC] = TipoCelda::ROJO;
        if (midF > 0) especialidad[midF - 1][midC] = TipoCelda::ROJO;
        if (midF < filas - 1) especialidad[midF + 1][midC] = TipoCelda::ROJO;
        if (midC > 0) especialidad[midF][midC - 1] = TipoCelda::ROJO;
        if (midC < columnas - 1) especialidad[midF][midC + 1] = TipoCelda::ROJO;

        // 2. Rombo Exterior: AMARILLO (Bomba)
        if (filas >= 7 && columnas >= 7) {
            if (midF >= 2) especialidad[midF - 2][midC] = TipoCelda::AMARILLO;
            if (midF <= filas - 3) especialidad[midF + 2][midC] = TipoCelda::AMARILLO;
            if (midC >= 2) especialidad[midF][midC - 2] = TipoCelda::AMARILLO;
            if (midC <= columnas - 3) especialidad[midF][midC + 2] = TipoCelda::AMARILLO;
        }

        // 3. Esquinas Interiores: VERDE (Bonus)
        if (filas > 2 && columnas > 2) {
            especialidad[1][1] = TipoCelda::VERDE;
            especialidad[1][columnas - 2] = TipoCelda::VERDE;
            especialidad[filas - 2][1] = TipoCelda::VERDE;
            especialidad[filas - 2][columnas - 2] = TipoCelda::VERDE;
        }
    }
}

/**
 * @brief Ejecuta el movimiento de colocar una pieza en el tablero.
 * Aplica validaciones de límites, adyacencia y trinidad, y procesa los efectos de casillas especiales.
 * @param f Fila destino.
 * @param c Columna destino.
 * @param jugador ID del jugador que realiza el movimiento (1 o 2).
 * @return true si el movimiento es legal y se ha realizado, false en caso contrario.
 */
bool Tablero::ponerPieza(int f, int c, int jugador) {
    if (f < 0 || f >= filas || c < 0 || c >= columnas || rejilla[f][c] != 0 || jugador != jugadorTurno) {
        return false;
    }

    // Regla de Adyacencia y Trinidad (Solo en Ninja 9x9)
    if (filas == 9 && columnas == 9 && nParaGanar == 5) {
        if (!esVacio() && !tieneAdyacente(f, c)) return false;
        if ((f + c) % 3 != (faseActual % 3)) return false;
    }

    TipoCelda tipo = especialidad[f][c];
    
    // EFECTO ROJO: Sabotaje (coloca ficha del rival)
    if (tipo == TipoCelda::ROJO) {
        rejilla[f][c] = (jugador == 1) ? 2 : 1;
    } else {
        rejilla[f][c] = jugador;
    }

    // EFECTO AMARILLO: Bomba (limpia fila y columna excepto la celda actual)
    if (tipo == TipoCelda::AMARILLO) {
        for (int i = 0; i < filas; ++i) {
            if (i != f) rejilla[i][c] = 0;
        }
        for (int j = 0; j < columnas; ++j) {
            if (j != c) rejilla[f][j] = 0;
        }
    }

    turnoActual++;
    movimientosRestantes--;

    // EFECTO VERDE: Gana un movimiento extra
    if (tipo == TipoCelda::VERDE) {
        movimientosRestantes += 1;
    }

    // Cambio de turno si se agotan movimientos o hay ganador
    if (movimientosRestantes <= 0 || comprobarGanador() != 0) {
        bool modoNinja = (filas == 9 && columnas == 9 && nParaGanar == 5);
        jugadorTurno = (jugadorTurno == 1) ? 2 : 1;
        movimientosRestantes = modoNinja ? 2 : 1; 
        faseActual++; // Avanzamos la fase al cambiar de jugador
    }

    return true;
}

/**
 * @brief Genera todos los estados sucesores válidos desde el tablero actual.
 * @return Un vector de objetos Tablero representando los estados hijos.
 */
std::vector<Tablero> Tablero::getSucesores() const {
    std::vector<Tablero> sucesores;
    
    // Si ya hay un ganador, no hay sucesores
    if (comprobarGanador() != 0) return sucesores;

    bool modoNinja = (filas == 9 && columnas == 9 && nParaGanar == 5);
    int residuoValido = modoNinja ? (faseActual % 3) : -1;
    bool vacio = esVacio();

    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            if (rejilla[f][c] == 0) {
                bool movimientoValido = false;
                if (modoNinja) {
                    if ((f + c) % 3 == residuoValido && (vacio || tieneAdyacente(f, c))) {
                        movimientoValido = true;
                    }
                } else {
                    movimientoValido = true;
                }

                if (movimientoValido) {
                    Tablero hijo = *this;
                    if (hijo.ponerPieza(f, c, jugadorTurno)) {
                        sucesores.push_back(hijo);
                    }
                }
            }
        }
    }
    return sucesores;
}

/**
 * @brief Genera todos los estados sucesores válidos desde el tablero actual junto al movimiento realizado para alcanzarlo.
 * @return Un vector de de pares <Tablero, movimiento (par de enteros)> representando los estados hijos.
 */
std::vector<std::pair<Tablero, std::pair<int,int>>> Tablero::getSucesoresConMovimientos() const {
    std::vector<std::pair<Tablero, std::pair<int,int>>>  sucesores;
    
    // Si ya hay un ganador, no hay sucesores
    if (comprobarGanador() != 0) return sucesores;

    bool modoNinja = (filas == 9 && columnas == 9 && nParaGanar == 5);
    int residuoValido = modoNinja ? (faseActual % 3) : -1;
    bool vacio = esVacio();

    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            if (rejilla[f][c] == 0) {
                bool movimientoValido = false;
                if (modoNinja) {
                    if ((f + c) % 3 == residuoValido && (vacio || tieneAdyacente(f, c))) {
                        movimientoValido = true;
                    }
                } else {
                    movimientoValido = true;
                }

                if (movimientoValido) {
                    Tablero hijo = *this;
                    if (hijo.ponerPieza(f, c, jugadorTurno)) {
                        sucesores.push_back({hijo, {f, c}});
                    }
                }
            }
        }
    }
    return sucesores;
}


/**
 * @brief Escanea el tablero para encontrar si algún jugador ha completado una línea ganadora.
 * @return ID del jugador ganador, 0 si no hay ganador, o -1 si hay empate por tablero lleno.
 */
int Tablero::comprobarGanador() const {
    lineaGanadora.clear();
    const int df[] = {0, 1, 1, 1};
    const int dc[] = {1, 0, 1, -1};

    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            int jugador = rejilla[f][c];
            if (jugador == 0) continue;

            for (int d = 0; d < 4; ++d) {
                int contador = 1;
                std::vector<std::pair<int, int>> posibleLinea = {{f, c}};
                for (int i = 1; i < nParaGanar; ++i) {
                    int nf = f + df[d] * i;
                    int nc = c + dc[d] * i;

                    if (nf >= 0 && nf < filas && nc >= 0 && nc < columnas && rejilla[nf][nc] == jugador) {
                        contador++;
                        posibleLinea.push_back({nf, nc});
                    } else {
                        break;
                    }
                }

                if (contador >= nParaGanar) {
                    lineaGanadora = posibleLinea;
                    return jugador;
                }
            }
        }
    }
    
    // Si no hay ganador directo y el tablero está lleno, aplicar desempate
    if (estaLleno()) {
        return getGanadorDesempate();
    }

    return 0;
}

/**
 * @brief Cuenta cuántas secuencias de piezas de una longitud dada posee un jugador.
 * @param longitud Longitud de la línea a buscar.
 * @param jugador ID del jugador (1 o 2).
 * @return Número total de combinaciones encontradas.
 */
int Tablero::contarCombinaciones(int longitud, int jugador) const {
    int total = 0;
    const int df[] = {0, 1, 1, 1};
    const int dc[] = {1, 0, 1, -1};

    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            for (int d = 0; d < 4; ++d) {
                int contador = 0;
                for (int i = 0; i < longitud; ++i) {
                    int nf = f + df[d] * i;
                    int nc = c + dc[d] * i;
                    if (nf >= 0 && nf < filas && nc >= 0 && nc < columnas && rejilla[nf][nc] == jugador) {
                        contador++;
                    } else {
                        break;
                    }
                }
                if (contador == longitud) total++;
            }
        }
    }
    return total;
}

/**
 * @brief Determina el ganador final basándose en el recuento de líneas (Tie-break).
 * Prioriza líneas de 5, luego de 4 y finalmente de 3.
 * @return 1 o 2 para el ganador, -1 si el empate persiste.
 */
int Tablero::getGanadorDesempate() const {
    // El sistema de desempate por puntos solo se aplica en Modo Competición 9x9 n=5
    bool modoNinja = (filas == 9 && columnas == 9 && nParaGanar == 5);
    if (!modoNinja) return -1;

    int p1_4 = contarCombinaciones(4, 1);
    int p2_4 = contarCombinaciones(4, 2);

    if (p1_4 > p2_4) return 1;
    if (p2_4 > p1_4) return 2;

    return -1;
}

/**
 * @brief Localiza todas las líneas de piezas de una longitud dada para un jugador.
 * Útil para el resaltado visual en la interfaz gráfica.
 * @param longitud Longitud de las líneas a buscar.
 * @param jugador ID del jugador.
 * @return Vector con vectores de coordenadas que forman cada línea.
 */
std::vector<std::vector<std::pair<int, int>>> Tablero::buscarTodasLasLineas(int longitud, int jugador) const {
    std::vector<std::vector<std::pair<int, int>>> resultados;
    const int df[] = {0, 1, 1, 1};
    const int dc[] = {1, 0, 1, -1};

    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            for (int d = 0; d < 4; ++d) {
                std::vector<std::pair<int, int>> posible;
                int contador = 0;
                for (int i = 0; i < longitud; ++i) {
                    int nf = f + df[d] * i;
                    int nc = c + dc[d] * i;
                    if (nf >= 0 && nf < filas && nc >= 0 && nc < columnas && rejilla[nf][nc] == jugador) {
                        contador++;
                        posible.push_back({nf, nc});
                    } else {
                        break;
                    }
                }
                if (contador == longitud) resultados.push_back(posible);
            }
        }
    }
    return resultados;
}

/**
 * @brief Comprueba si todas las celdas del tablero están ocupadas.
 */
bool Tablero::estaLleno() const {
    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            if (rejilla[f][c] == 0) return false;
        }
    }
    return true;
}

/**
 * @brief Comprueba si el tablero no contiene ninguna pieza colocada.
 */
bool Tablero::esVacio() const {
    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            if (rejilla[f][c] != 0) return false;
        }
    }
    return true;
}

/**
 * @brief Determina si una celda dada tiene al menos un vecino ocupado por una pieza.
 * @param f Fila.
 * @param c Columna.
 * @return true si hay adyacencia, false si está aislada.
 */
bool Tablero::tieneAdyacente(int f, int c) const {
    for (int df = -1; df <= 1; ++df) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (df == 0 && dc == 0) continue;
            int nf = f + df;
            int nc = c + dc;
            if (nf >= 0 && nf < filas && nc >= 0 && nc < columnas) {
                if (rejilla[nf][nc] != 0) return true;
            }
        }
    }
    return false;
}

/**
 * @brief Comprueba si el jugador que tiene el turno puede realizar algún movimiento legal.
 * Tiene en cuenta las reglas de la Trinidad y la Adyacencia en modo Ninja.
 * @return true si existe al menos una jugada válida.
 */
bool Tablero::tieneMovimientosValidos() const {
    bool modoNinja = (filas == 9 && columnas == 9 && nParaGanar == 5);
    int residuoValido = modoNinja ? (faseActual % 3) : -1;
    bool vacio = esVacio();

    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            if (rejilla[f][c] == 0) {
                if (modoNinja) {
                    if ((f + c) % 3 == residuoValido && (vacio || tieneAdyacente(f, c))) return true;
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}
