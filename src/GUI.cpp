#include "GUI.hpp"
#include <cmath>
#include <iostream>
#include <string>

int GUI::ventanaAncho = 1200; 
int GUI::ventanaAlto = 800;
int GUI::filasTablero = 3;
int GUI::colsTablero = 3;
ControladorJuego* GUI::controlador = nullptr;
const Tablero* GUI::tableroActual = nullptr;

void GUI::inicializar(int argc, char** argv, int filas, int cols, ControladorJuego* ctrl) {
    filasTablero = filas;
    colsTablero = cols;
    controlador = ctrl;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(ventanaAncho, ventanaAlto);
    glutCreateWindow("N-en-raya Premium - Practica 3");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
}

void GUI::idle() {
    if (controlador && !controlador->esFinDeJuego()) {
        // Llamamos a paso si es turno de IA O si hay sugerencia/resolución en curso
        if (controlador->esTurnoDeIA() || controlador->esSugerenciaEnCurso() || controlador->esResolucionEnCurso()) {
            controlador->paso();
        }
    }
    glutPostRedisplay();
}

void GUI::ejecutar() {
    glutMainLoop();
}

void renderText(float x, float y, void* font, const std::string& text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void GUI::display() {
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    dibujarFondoElegante();
    dibujarTablero();

    if (controlador) {
        const Tablero& t = controlador->getTablero();
        bool modoNinja = (filasTablero == 9 && colsTablero == 9 && t.getNParaGanar() == 5);
        for (int f = 0; f < filasTablero; ++f) {
            for (int c = 0; c < colsTablero; ++c) {
                int jugador = t.getCelda(f, c);
                if (jugador != 0) {
                    dibujarPieza(f, c, jugador);
                }
            }
        }

        if (controlador->getGanador() != 0 && controlador->getGanador() != -1) {
            int ganador = controlador->getGanador();
            const auto& lineaGanadora = t.getLineaGanadora();
            
            std::vector<std::vector<std::pair<int, int>>> todasLasLineas;
            
            if (!lineaGanadora.empty()) {
                todasLasLineas.push_back(lineaGanadora);
            } else if (t.estaLleno()) {
                // Ganador por puntos: resaltamos líneas de 5, y si no hay, las de 4
                todasLasLineas = t.buscarTodasLasLineas(5, ganador);
                if (todasLasLineas.empty()) {
                    todasLasLineas = t.buscarTodasLasLineas(4, ganador);
                }
            }

            for (const auto& linea : todasLasLineas) {
                glLineWidth(10.0f);
                glBegin(GL_LINE_STRIP);
                glColor4f(1.0f, 1.0f, 1.0f, 0.4f); 
                for (const auto& p : linea) {
                    glVertex2f(p.second + 0.5f, p.first + 0.5f);
                }
                glEnd();

                glLineWidth(16.0f);
                glBegin(GL_LINE_STRIP);
                if (ganador == 1) glColor4f(0.0f, 1.0f, 1.0f, 0.2f);
                else glColor4f(1.0f, 0.2f, 1.0f, 0.2f);
                for (const auto& p : linea) {
                    glVertex2f(p.second + 0.5f, p.first + 0.5f);
                }
                glEnd();
            }
        }

        // --- Panel Lateral de Información (Glassmorphism) ---
        glColor4f(0.1f, 0.1f, 0.25f, 0.5f);
        glBegin(GL_QUADS);
        glVertex2f(colsTablero + 0.1f, 0.1f);
        glVertex2f(colsTablero + 1.4f, 0.1f);
        glVertex2f(colsTablero + 1.4f, filasTablero - 0.1f);
        glVertex2f(colsTablero + 0.1f, filasTablero - 0.1f);
        glEnd();

        // Texto informativo refinado (Ajustado para 3x3)
        float y_offset_base = (filasTablero == 3) ? -0.1f : 0.0f;
        float y_config   = (filasTablero == 3) ? 0.3f  : 0.4f;
        float y_objetivo = (filasTablero == 3) ? 0.6f  : 0.7f;
        float y_estado   = (filasTablero == 3) ? 1.0f  : 1.3f;
        float y_turno    = (filasTablero == 3) ? 1.25f : 1.6f;
        float y_crono    = (filasTablero == 3) ? 1.45f : 1.9f;
        float y_ganador  = (filasTablero == 3) ? 1.7f  : 2.3f;
        float y_total1   = (filasTablero == 3) ? 1.95f : 2.7f;
        float y_total2   = (filasTablero == 3) ? 2.1f  : 2.9f;

        renderText(colsTablero + 0.25f, y_config, GLUT_BITMAP_HELVETICA_12, "CONFIGURACION", 0.5f, 0.7f, 1.0f);
        renderText(colsTablero + 0.25f, y_objetivo, GLUT_BITMAP_HELVETICA_18, "Objetivo: " + std::to_string(t.getNParaGanar()), 1.0f, 1.0f, 1.0f);
        
        std::string turnoStr = "Turno: J" + std::to_string(controlador->getTurnoActual());
        if (controlador->esTurnoDeIA()) turnoStr += " (IA)";
        else turnoStr += " (TU)";
        
        renderText(colsTablero + 0.25f, y_estado, GLUT_BITMAP_HELVETICA_12, "ESTADO ACTUAL:", 0.5f, 0.7f, 1.0f);
        float tr = (controlador->getTurnoActual() == 1) ? 0.0f : 1.0f;
        float tg = (controlador->getTurnoActual() == 1) ? 1.0f : 0.4f;
        float tb = (controlador->getTurnoActual() == 1) ? 1.0f : 1.0f;
        renderText(colsTablero + 0.25f, y_turno, GLUT_BITMAP_HELVETICA_18, turnoStr, tr, tg, tb);

        // --- Cronómetros ---
        char buffer[32];
        sprintf(buffer, "Turno: %.1fs", controlador->getTiempoTurnoActual());
        renderText(colsTablero + 0.35f, y_crono, GLUT_BITMAP_HELVETICA_12, buffer, 0.8f, 0.8f, 0.8f);

        sprintf(buffer, "Total J1: %.1fs", controlador->getTiempoAcumulado(1));
        renderText(colsTablero + 0.25f, y_total1, GLUT_BITMAP_HELVETICA_10, buffer, 0.0f, 1.0f, 1.0f);
        sprintf(buffer, "Total J2: %.1fs", controlador->getTiempoAcumulado(2));
        renderText(colsTablero + 0.25f, y_total2, GLUT_BITMAP_HELVETICA_10, buffer, 1.0f, 0.4f, 1.0f);

        // --- Marcador de Puntos (Tiebreakers) ---
        if (modoNinja) {
            float y_puntos_base = y_total2 + 0.5f;
            renderText(colsTablero + 0.25f, y_puntos_base, GLUT_BITMAP_HELVETICA_12, "PUNTUACION (5|4):", 0.5f, 0.7f, 1.0f);
            
            std::string pts1 = "J1: " + std::to_string(t.contarCombinaciones(5, 1)) + " | " + std::to_string(t.contarCombinaciones(4, 1));
            std::string pts2 = "J2: " + std::to_string(t.contarCombinaciones(5, 2)) + " | " + std::to_string(t.contarCombinaciones(4, 2));
            renderText(colsTablero + 0.25f, y_puntos_base + 0.3f, GLUT_BITMAP_HELVETICA_12, pts1, 0.0f, 1.0f, 1.0f);
            renderText(colsTablero + 0.25f, y_puntos_base + 0.6f, GLUT_BITMAP_HELVETICA_12, pts2, 1.0f, 0.4f, 1.0f);
        }

        if (controlador->getGanador() != 0) {
            std::string winStr = (controlador->getGanador() == -1) ? "EMPATE" : "J" + std::to_string(controlador->getGanador()) + " GANA";
            if (controlador->getGanador() != -1 && t.getLineaGanadora().empty()) winStr += " (PTS)";
            renderText(colsTablero + 0.2f, y_ganador, GLUT_BITMAP_TIMES_ROMAN_24, winStr, 1.0f, 0.9f, 0.0f);
        }

        // --- Botones Interactivos ---
        // Botón REINICIAR
        float btnX = colsTablero + 0.3f;
        float btnW = 0.9f;
        float btnH = 0.3f;
        float btnY_R = filasTablero - 0.8f;
        float btnY_N = filasTablero - 2.0f;
        float btnY_Res = filasTablero - 3.2f;

        // Botón STATUS
        if (!controlador->esFinDeJuego()) {
            bool ocupadoRes = controlador->esResolucionEnCurso();
            if (ocupadoRes) glColor4f(0.6f, 0.0f, 1.0f, 0.6f); // Púrpura intenso
            else glColor4f(0.4f, 0.0f, 0.8f, 0.4f); // Púrpura traslúcido
            
            glBegin(GL_QUADS);
            glVertex2f(btnX, btnY_Res); glVertex2f(btnX + btnW, btnY_Res);
            glVertex2f(btnX + btnW, btnY_Res + btnH); glVertex2f(btnX, btnY_Res + btnH);
            glEnd();

            std::string resTxt = ocupadoRes ? "Thinking..." : "Status";
            renderText(btnX + 0.08f, btnY_Res + 0.2f, GLUT_BITMAP_HELVETICA_18, resTxt, 1, 1, 1);
            
            // Mostrar resultado de la resolución si existe
            std::string verdict = controlador->getUltimoResultadoResolucion();
            if (!verdict.empty()) {
                renderText(btnX, btnY_Res + 0.45f, GLUT_BITMAP_HELVETICA_12, "Resultado: " + verdict, 1, 1, 0);
            }
        }

        // Botón NEXT (Sugerencia)
        if (!controlador->esTurnoDeIA() && !controlador->esFinDeJuego()) {
            bool ocupado = controlador->esSugerenciaEnCurso();
            if (ocupado) glColor4f(1.0f, 0.4f, 0.0f, 0.6f); // Naranja intenso si está ocupado
            else glColor4f(1.0f, 0.8f, 0.0f, 0.4f); // Dorado traslúcido normal
            
            glBegin(GL_QUADS);
            glVertex2f(btnX, btnY_N); glVertex2f(btnX + btnW, btnY_N);
            glVertex2f(btnX + btnW, btnY_N + btnH); glVertex2f(btnX, btnY_N + btnH);
            glEnd();

            if (ocupado) glColor4f(1.0f, 0.6f, 0.0f, 1.0f);
            else glColor4f(1.0f, 0.9f, 0.5f, 0.8f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(btnX, btnY_N); glVertex2f(btnX + btnW, btnY_N);
            glVertex2f(btnX + btnW, btnY_N + btnH); glVertex2f(btnX, btnY_N + btnH);
            glEnd();

            std::string txt = ocupado ? "Thinking..." : "Next Mov";
            float x_off = ocupado ? 0.05f : 0.08f;
            renderText(btnX + x_off, btnY_N + 0.2f, GLUT_BITMAP_HELVETICA_18, txt, 1, 1, 1);
        }
        
        glColor4f(0.0f, 0.5f, 1.0f, 0.3f);
        glBegin(GL_QUADS);
        glVertex2f(btnX, btnY_R); glVertex2f(btnX + btnW, btnY_R);
        glVertex2f(btnX + btnW, btnY_R + btnH); glVertex2f(btnX, btnY_R + btnH);
        glEnd();
        glColor4f(0.0f, 1.0f, 1.0f, 0.6f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(btnX, btnY_R); glVertex2f(btnX + btnW, btnY_R);
        glVertex2f(btnX + btnW, btnY_R + btnH); glVertex2f(btnX, btnY_R + btnH);
        glEnd();
        renderText(btnX + 0.1f, btnY_R + 0.2f, GLUT_BITMAP_HELVETICA_12, "REINICIAR", 1, 1, 1);

        // Botón SALIR
        float btnY_S = filasTablero - 0.4f;
        glColor4f(1.0f, 0.2f, 0.2f, 0.3f);
        glBegin(GL_QUADS);
        glVertex2f(btnX, btnY_S); glVertex2f(btnX + btnW, btnY_S);
        glVertex2f(btnX + btnW, btnY_S + btnH); glVertex2f(btnX, btnY_S + btnH);
        glEnd();
        glColor4f(1.0f, 0.5f, 0.5f, 0.6f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(btnX, btnY_S); glVertex2f(btnX + btnW, btnY_S);
        glVertex2f(btnX + btnW, btnY_S + btnH); glVertex2f(btnX, btnY_S + btnH);
        glEnd();
        renderText(btnX + 0.2f, btnY_S + 0.2f, GLUT_BITMAP_HELVETICA_12, "SALIR", 1, 1, 1);
    }

    glutSwapBuffers();
}

void GUI::reshape(int w, int h) {
    ventanaAncho = w;
    ventanaAlto = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, colsTablero + 1.5f, filasTablero, 0); 
    glMatrixMode(GL_MODELVIEW);
}

void GUI::dibujarFondoElegante() {
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.04f, 0.04f, 0.15f, 1.0f);
    glVertex2f(colsTablero / 2.0f, filasTablero / 2.0f);
    glColor4f(0.01f, 0.01f, 0.05f, 1.0f);
    for (int i = 0; i <= 36; ++i) {
        float ang = i * 10.0f * 3.14159f / 180.0f;
        glVertex2f(colsTablero/2.0f + cos(ang) * colsTablero * 2, filasTablero/2.0f + sin(ang) * filasTablero * 2);
    }
    glEnd();
}

void GUI::dibujarTablero() {
    const Tablero& t = controlador->getTablero();
    bool modoNinja = (filasTablero == 9 && colsTablero == 9 && t.getNParaGanar() == 5);
    int residuoValido = t.getFaseActual() % 3;

    for (int f = 0; f < filasTablero; ++f) {
        for (int c = 0; c < colsTablero; ++c) {
            Tablero::TipoCelda tipo = t.getTipoCelda(f, c);
            bool esResiduoCorrecto = !modoNinja || ((f + c) % 3 == residuoValido);
            bool esAdyacenteValida = !modoNinja || t.esVacio() || t.tieneAdyacente(f, c);
            bool esJugable = esResiduoCorrecto && esAdyacenteValida && t.getCelda(f, c) == 0;
            
            glBegin(GL_QUADS);
            if (esJugable) {
                // Colores vibrantes para casillas jugables
                if (tipo == Tablero::TipoCelda::VERDE) glColor4f(0.2f, 1.0f, 0.4f, 0.6f);
                else if (tipo == Tablero::TipoCelda::ROJO) glColor4f(1.0f, 0.2f, 0.2f, 0.6f);
                else if (tipo == Tablero::TipoCelda::AMARILLO) glColor4f(1.0f, 1.0f, 0.4f, 0.6f);
                else glColor4f(0.3f, 0.7f, 1.0f, 0.3f); 
            } else {
                // Colores apagados para casillas no jugables
                if (tipo == Tablero::TipoCelda::VERDE) glColor4f(0.0f, 0.3f, 0.1f, 0.15f);
                else if (tipo == Tablero::TipoCelda::ROJO) glColor4f(0.3f, 0.0f, 0.0f, 0.15f);
                else if (tipo == Tablero::TipoCelda::AMARILLO) glColor4f(0.3f, 0.3f, 0.0f, 0.15f);
                else if (esResiduoCorrecto) glColor4f(0.2f, 0.3f, 0.5f, 0.12f); // Residuo correcto pero no adyacente
                else glColor4f(0.02f, 0.02f, 0.05f, 0.05f); 
            }
            
            float m = 0.02f;
            glVertex2f(c + m, f + m);
            glVertex2f(c + 1 - m, f + m);
            glVertex2f(c + 1 - m, f + 1 - m);
            glVertex2f(c + m, f + 1 - m);
            glEnd();

            // Borde resaltado tipo "neon" para las jugables
            if (esJugable) {
                glLineWidth(2.5f);
                if (tipo == Tablero::TipoCelda::NORMAL) glColor4f(0.0f, 0.8f, 1.0f, 0.5f);
                else glColor4f(1.0f, 1.0f, 1.0f, 0.7f); // Borde blanco brillante para especiales
                
                glBegin(GL_LINE_LOOP);
                glVertex2f(c + m, f + m); glVertex2f(c + 1 - m, f + m);
                glVertex2f(c + 1 - m, f + 1 - m); glVertex2f(c + m, f + 1 - m);
                glEnd();
            }
        }
    }

    glLineWidth(1.5f);
    glColor4f(0.0f, 0.7f, 1.0f, 0.3f);
    glBegin(GL_LINES);
    for (int i = 0; i <= colsTablero; ++i) {glVertex2f(i, 0); glVertex2f(i, filasTablero);}
    for (int i = 0; i <= filasTablero; ++i) {glVertex2f(0, i); glVertex2f(colsTablero, i);}
    glEnd();
}

void GUI::dibujarPieza(int f, int c, int jugador) {
    float x = c + 0.5f;
    float y = f + 0.5f;
    float radio = 0.35f;

    if (jugador == 1) { // Cyan X
        glLineWidth(6.0f);
        glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(x-radio, y-radio); glVertex2f(x+radio, y+radio);
        glVertex2f(x+radio, y-radio); glVertex2f(x-radio, y+radio);
        glEnd();
        dibujarEfectoNeon(x, y, 0.0f, 1.0f, 1.0f);
    } else { // Magenta O
        glLineWidth(6.0f);
        glColor4f(1.0f, 0.2f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        for(int i=0; i<40; ++i){
            float a = i*9.0f*3.14159f/180.0f;
            glVertex2f(x+cos(a)*radio, y+sin(a)*radio);
        }
        glEnd();
        dibujarEfectoNeon(x, y, 1.0f, 0.2f, 1.0f);
    }
}

void GUI::dibujarEfectoNeon(float x, float y, float r, float g, float b) {
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(r, g, b, 0.15f);
    glVertex2f(x, y);
    glColor4f(r, g, b, 0.0f);
    for (int i = 0; i <= 36; ++i) {
        float a = i*10.0f*3.14159f/180.0f;
        glVertex2f(x+cos(a)*0.7f, y+sin(a)*0.7f);
    }
    glEnd();
}

void GUI::mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float relX = (float)x / ventanaAncho * (colsTablero + 1.5f);
        float relY = (float)y / ventanaAlto * filasTablero;
        
        if (relX < colsTablero) {
            int c = (int)relX;
            int f = (int)relY;
            if (controlador) {
                // Solo permitimos clicks si es turno de un humano
                if (!controlador->esTurnoDeIA()) {
                    controlador->procesarMovimiento(f, c);
                }
            }
        } else {
            // Detección de botones en el panel lateral
            float btnX = colsTablero + 0.3f;
            float btnW = 0.9f;
            float btnH = 0.3f;
            
            // Reiniciar
            if (relX >= btnX && relX <= btnX + btnW && relY >= filasTablero - 0.8f && relY <= filasTablero - 0.5f) {
                if (controlador) controlador->reiniciar();
            }
            // Sugerencia (NEXT)
            if (relX >= btnX && relX <= btnX + btnW && relY >= filasTablero - 2.0f && relY <= filasTablero - 1.7f) {
                if (controlador) controlador->ejecutarSugerencia();
            }
            // Resolución (STATUS)
            if (relX >= btnX && relX <= btnX + btnW && relY >= filasTablero - 3.2f && relY <= filasTablero - 2.9f) {
                if (controlador) controlador->ejecutarResolucion();
            }
            // Salir
            if (relX >= btnX && relX <= btnX + btnW && relY >= filasTablero - 0.4f && relY <= filasTablero - 0.1f) {
                exit(0);
            }
        }
    }
}

void GUI::keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    if (key == 'r' || key == 'R') {
        if (controlador) controlador->reiniciar();
    }
}
