//
//  main.cpp
//  practica-1-domino
//
//  Created by Carolina Chamorro on 12/10/2019.
//  Copyright © 2019 Carolina Chamorro. All rights reserved.
//

#include <time.h>
#include <cstdlib>
#include <list>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>



using namespace std;

string board;
const int numToken = 55;
const int MAX_PLAYERS = 4;
const int INITIAL_TOKENS = 7;
bool muestraFichasMaquina = true;

typedef struct {
    short int token1;
    short int token2;
} tToken;

typedef tToken tArrayToken[numToken];

typedef struct {
    tArrayToken listToken;
    int cont;
} tListToken;

typedef tListToken tPlayers[MAX_PLAYERS];

typedef int tPoints[MAX_PLAYERS];

typedef struct {
    int numbersPlayers;
    int maxNumber;
    tListToken pool;
	tPlayers players;
    tPoints points;
} tPlay;


// functions for The Domino
string toStr (int n);
int maxNumTokens (tPlay play);
string tokenToStr (short int left, short int right);
string poolToStr (tListToken pool);
bool canPutLeft (short int token);
bool canPutRight (short int token);
void putTokenLeft (short int token1, short int token2);
void putTokenRight (short int token1, short int token2);
void deleteToken (tListToken& token, short int numToken);
void drawTokens (tListToken& pool, tListToken& player);
void showBoard (tPlay play);
int showMenu ();
short int aleat (int n);
short int chooseToken (tListToken token);
short int question_INTER (short int min, short int max, string text);
void generatePool (tPlay& play);
void disorderPool (tPlay& play);
void readListToken (ifstream& archivo, tListToken& listToken);
bool readGame(tPlay play);
void writeListToken(ofstream& archivo, tListToken& listToken);
void writeGame(tPlay play);
bool canDrawToken(tListToken tokens);
bool isGameOver(tPlay& play);
bool putToken(tPlay& play, int player, int token);
bool strategy1(tPlay& play, int player);
bool strategy2(tPlay& play, int player);
int playerTurn(tPlay& play, int& index);
void init(tPlay& play, int& player);
bool realPlayerOption(tPlay& play);
void showWinner(int winner);
void showPoints(tPlay play);
void updatePoints(tPlay & play);
bool question_S_N (string text);

void borrar( void ){
	system("cls||clear");	
}


// main function
int main(int argc, const char * argv[]) {
    borrar();
    tPlay play;
    int turn = -1;
	int winner = -1;
	bool needInit = false;

    srand(time(NULL));

	if (!question_S_N("¿Quieres abrir un archivo anterior? (S/N)") || !readGame(play)) {
		needInit = true;
		play.maxNumber = question_INTER(6, 9, "Variante del juego (entre 6 y 9): ");
		play.numbersPlayers = question_INTER(2, 4, "Elige número de jugadores(entre 2 y 4): ");
		for(int i = 0; i < play.numbersPlayers; i++) {
			play.points[i] = 0;
		}
    }
    
    do {
		if (needInit) {
			init(play, turn);
		}
		needInit = true;

        for(bool end = false; !end; turn = (turn+1)%play.numbersPlayers) {
            switch(turn) {
                case 0:
                    end = !realPlayerOption(play);
                    break;
                case 1:
                    while(!strategy2(play, turn)) {
                        if (play.pool.cont == 0) {
							break;
                        }
                        
                        drawTokens(play.pool, play.players[turn]);
                    }
                    break;
                default:
                    while(!strategy1(play, turn)) {
                        if (play.pool.cont == 0) {
							break;
                        }
                        
                        drawTokens(play.pool, play.players[turn]);
                    }
                    break;
            }
			if (play.players[turn].cont == 0) {
				updatePoints(play);
				 end = true;
				 winner = turn;
			}
        }

        showWinner(winner);
        showPoints(play);

    } while (question_S_N("¿Quieres jugar otra ronda? (S/N)"));


    
    if (question_S_N("¿Quiéres guardar antes de salir? (S/N)")) {
        writeGame(play);
    }
    return 0;
}

string toStr (int n) {
    switch (n) {
        case 0:
            return "0";
        case 1:
            return "1";
        case 2:
            return "2";
        case 3:
            return "3";
        case 4:
            return "4";
        case 5:
            return "5";
        case 6:
            return "6";
        case 7:
            return "7";
        case 8:
            return "8";
        case 9:
            return "9";
        default:
            return "ERROR";
    }
}

int maxNumTokens (tPlay play) {
	int sum = 0;
	for (int i = 1; i <= play.maxNumber+1; i++)
		sum += i;

	return sum;
}

// Token
string tokenToStr (short int left, short int right) {
    string str = "|" + toStr(left) + "-" + toStr(right) + "|";
    return str;
}

// painting pool
string poolToStr (tListToken pool) {
    string str;
    
    for ( int i = 0; i < pool.cont; i++ ) {
        str = str + tokenToStr(pool.listToken[i].token1, pool.listToken[i].token2);
    }
    return str;
}

bool canPutLeft (short int token) {
    return (board[1]-0x30==token);
}

bool canPutRight (short int token) {
    return (board[board.size()-2]-0x30==token);
}


void putTokenLeft (short int token1, short int token2) {
    board = tokenToStr(token1, token2) + board;
}

void putTokenRight (short int token1, short int token2) {
    board = board + tokenToStr(token1, token2);
}


void deleteToken (tListToken& token, short int numToken) {
    for(int i = numToken; i < token.cont; i++) {
        token.listToken[i].token1 = token.listToken[i+1].token1;
        token.listToken[i].token2 = token.listToken[i+1].token2;
    }
    token.cont--;
}

void drawTokens (tListToken& pool, tListToken& player) {
	pool.cont--;
	player.listToken[player.cont].token1 = pool.listToken[pool.cont].token1;
	player.listToken[player.cont].token2 = pool.listToken[pool.cont].token2;
    player.cont++;
}

// Tablero
void showBoard (tPlay play) {
	cout << endl;
    cout << " -------------------- " << endl;
    cout << "|       TABLERO      |" << endl;
    cout << " -------------------- " << endl;
    cout << board << endl << endl;

	if (muestraFichasMaquina) {
		for (int i = 0; i < play.numbersPlayers - 1; i++) {
			cout << "Maquina#" << i + 1 << ": " << poolToStr(play.players[i+1]) << endl;
		}

	}
	else {
		for (int i = 0; i < play.numbersPlayers - 1; i++) {
			cout << "Maquina#" << i + 1 << ": " << play.players[i+1].cont << " fichas." << endl;
		}
	}

    cout << endl << "Jugador: " << poolToStr(play.players[0]) << endl;

    return;
}

// Menú
int showMenu () {
    char chooseOption = '0';
    
    while (chooseOption < '1' || chooseOption > '4') {
        cout << " -------------------- " << endl;
        cout << "|  MENÚ DE OPCIONES  |" << endl;
        cout << " -------------------- " << endl;
        cout << "1. Poner ficha por la izquierda" << endl;
        cout << "2. Poner ficha por la derecha" << endl;
        cout << "3. Robar ficha nueva" << endl;
        cout << "4. Salir" << endl;
        
        cout << "Elija opción: ";
        cin >> chooseOption;
    }
    
    return chooseOption-0x30;
}

// aleat
short int aleat (int n) {
    return rand()%(n+1);
}

short int chooseToken (tListToken token) {
    int chooseToken = 0;

    for(int i = 0; i < token.cont; i++) {
        cout << "(" << i+1 << ") " << tokenToStr(token.listToken[i].token1, token.listToken[i].token2) << endl;
    }
    
    while (chooseToken < 1 || chooseToken > token.cont) {
        cout << "Elije ficha: ";
        cin >> chooseToken;
    }
    borrar();
    return chooseToken-1;
}


short int question_INTER (short int min, short int max, string text) {
    int element = 0;
    
    while (element < min || element > max) {
        cout << text;
        cin >> element;
    }
    borrar();
    return element;
}


void generatePool (tPlay& play) {
    int k = 0;
    for ( int i = 0; i <= play.maxNumber; i++ ) {
        for ( int j = 0; j <= i; j++ ) {
            play.pool.listToken[k].token1 = i;
            play.pool.listToken[k].token2 = j;
            k++;
        }
    }
}

void disorderPool (tPlay& play) {
    int idx;
    short int tmp1, tmp2;
        for (int i = maxNumTokens(play) - 1; i >= 0; i--) {
            idx = rand() % (i + 1);
            if (i != idx) {
            tmp1 = play.pool.listToken[i].token1;
            tmp2 = play.pool.listToken[i].token2;
            play.pool.listToken[i].token1 = play.pool.listToken[idx].token1;
            play.pool.listToken[i].token2 = play.pool.listToken[idx].token2;
            play.pool.listToken[idx].token1 = tmp1;
            play.pool.listToken[idx].token2 = tmp2;
        }
    }
}

void readListToken (ifstream& archivo, tListToken& listToken) {
	for (int i = 0; i < listToken.cont; i++) {
		archivo >> listToken.listToken[i].token1;
		archivo >> listToken.listToken[i].token2;
	}
    
}

bool readGame(tPlay play) {
    ifstream archivo;
    archivo.open("domino_save.txt", ios::in);
    if (!archivo.is_open()) {
        cout << "¡No se ha podido abrir el archivo!" << endl;
        return false;
    }

	archivo >> play.numbersPlayers;
    archivo >> play.maxNumber;
    archivo >> board;
	archivo >> play.pool.cont ;
    readListToken(archivo, play.pool);

	for (int i = 0; i < play.numbersPlayers; i++) {
		archivo >> play.players[i].cont;
		readListToken(archivo, play.players[i]);
		archivo >> play.points[i];
	}

    archivo.close();
    
    return true;
}

void writeListToken(ofstream& archivo, tListToken& listToken) {
	for (int i = 0; i < listToken.cont; i++) {
		archivo << listToken.listToken[i].token1 << " " << listToken.listToken[i].token2 << " ";
	}
	archivo << endl;
}

void writeGame(tPlay play) {
    ofstream file;
    file.open("domino_save.txt", ios::out);

    if (file.is_open()) {
    file << play.numbersPlayers << " " << play.maxNumber <<endl
		<< board << endl
        << play.pool.cont << endl;
		writeListToken(file, play.pool);
		for (int i = 0; i < play.numbersPlayers; i++) {
			file << play.players[i].cont << endl;
			writeListToken(file, play.players[i]);
			file << play.points[i] << endl;
		}

        file.close();

    } else {
        cout << "¡No se ha podido abrir el archivo!" << endl;
    }
}


bool canDrawToken(tListToken tokens) {
    bool canPutLeftBool = true;
    bool canPutRightBool = true;

    for (int i = 0; i < tokens.cont; i++) {
        canPutLeftBool = canPutLeft(tokens.listToken[i].token1) || canPutLeft(tokens.listToken[i].token2);
        canPutRightBool = canPutRight(tokens.listToken[i].token1) || canPutRight(tokens.listToken[i].token2);
        if (canPutLeftBool || canPutRightBool) {
            return false;
        }
    }
    return true;
}

bool isGameOver(tPlay& play) {
    for (int j = 0; j < play.numbersPlayers; j++) {
        // puede colocar a la izquierda o a la derecha
        if (!canDrawToken(play.players[j])) {
            return false;
        }
    }

    return play.pool.cont == 0;
}

bool putToken(tPlay& play, int player, int token) {
     if (canPutLeft(play.players[player].listToken[token].token1)) {
            putTokenLeft(play.players[player].listToken[token].token2, play.players[player].listToken[token].token1);
			deleteToken(play.players[player], token);
            return true;
        } else if(canPutLeft(play.players[player].listToken[token].token2)) {
            putTokenLeft(play.players[player].listToken[token].token1, play.players[player].listToken[token].token2);
			deleteToken(play.players[player], token);
            return true;
        } else if ( canPutRight(play.players[player].listToken[token].token1)) {
            putTokenRight(play.players[player].listToken[token].token1, play.players[player].listToken[token].token2);
			deleteToken(play.players[player], token);
            return true;
        } else if (canPutRight(play.players[player].listToken[token].token2)) {
            putTokenRight(play.players[player].listToken[token].token2, play.players[player].listToken[token].token1);
			deleteToken(play.players[player], token);
            return true;
        }
        return false;
}

// Estrategia para las maquinas
bool strategy1(tPlay& play, int player) {

    for (int i = 0; i < play.players[player].cont; i++) {
        if (putToken(play, player, i)) {
            return true;
        }
    }
    return false;
}

// Estrategia para las maquinas
bool strategy2(tPlay& play, int player) {
	int maxPoints = -1;
	int bestTokenIndex = -1;
	short int token1, token2;

	for (int i = 0; i < play.players[player].cont; i++) {
		token1 = play.players[player].listToken[i].token1;
		token2 = play.players[player].listToken[i].token2;
		if (canPutLeft(token1) || canPutLeft(token2)
				|| canPutRight(token1) || canPutRight(token2)) {
			if (token1 + token2 > maxPoints) {
				bestTokenIndex = i;
				maxPoints = token1 + token2;
			}
		}
	}

	if (bestTokenIndex != -1) {
		putToken(play, player, bestTokenIndex);
		return true;
	} else {
		return false;
	}
}

// organiza los turnos
int playerTurn(tPlay& play, int& index) {
    int maxDoubleToken = -1;
    int firstPlayer = -1;
    for (int j = 0; j < play.numbersPlayers; j++) {
        for (int i = 0; i < play.players[j].cont; i++) {
            if(play.players[j].listToken[i].token1 == play.players[j].listToken[i].token2) {
                if ( maxDoubleToken < play.players[j].listToken[i].token1) {
                    maxDoubleToken = play.players[j].listToken[i].token1;
                    firstPlayer = j;
					index = i;
                }
            }
        }
    }

    return firstPlayer;
}

void init(tPlay& play, int& player) {

    int index;
    generatePool(play);
    disorderPool(play);
    play.pool.cont = maxNumTokens(play);

    for (int j = 0; j < play.numbersPlayers; j++) {
		play.players[j].cont = 0;
        for (int i = 0; i < INITIAL_TOKENS; i++) {
            play.players[j].listToken[i].token1 = play.pool.listToken[play.pool.cont-1].token1;
            play.players[j].listToken[i].token2 = play.pool.listToken[play.pool.cont-1].token2;
            play.players[j].cont++;
            play.pool.cont--;
        }
    }
	if (play.pool.cont == 0) {
		cout << "No hay más tokens para robar. No puede empezar la partida." << endl;
		exit(EXIT_FAILURE);
	}
    play.pool.cont--;
    board = tokenToStr(play.pool.listToken[play.pool.cont].token1,play.pool.listToken[play.pool.cont].token2);

    player = playerTurn(play, index);

    if (player == 0) {
        return;
    }

    if (player == -1) {
        init(play, player);
    }

    putToken(play, player, index);
	player = (player+1)%play.numbersPlayers;
}


bool realPlayerOption(tPlay& play) {
    for (int option = 0; option != 4;) {
        showBoard(play);
        option = showMenu();
        short int chosen;

        if (isGameOver(play)) {
            cout << "¡Sin salida!" << endl;
            return false;
        }

        switch(option) {
            case 1:
                chosen = chooseToken(play.players[0]);
                if (canPutLeft(play.players[0].listToken[chosen].token1)) {
                    putTokenLeft(play.players[0].listToken[chosen].token2, play.players[0].listToken[chosen].token1);
                    deleteToken(play.players[0], chosen);

                    return true;
                } else if (canPutLeft(play.players[0].listToken[chosen].token2)) {
                    putTokenLeft(play.players[0].listToken[chosen].token1, play.players[0].listToken[chosen].token2);
                    deleteToken(play.players[0], chosen);

                    return true;
                } else {
                    cout << " ERROR! :-( " << endl;
                }
                break;
            case 2: 
                chosen = chooseToken(play.players[0]);
                if (canPutRight(play.players[0].listToken[chosen].token1)) {
                    putTokenRight(play.players[0].listToken[chosen].token1, play.players[0].listToken[chosen].token2);
                    deleteToken(play.players[0], chosen);

                    return true;
                } else if (canPutRight(play.players[0].listToken[chosen].token2)) {
                    putTokenRight(play.players[0].listToken[chosen].token2, play.players[0].listToken[chosen].token1);
                    deleteToken(play.players[0], chosen);

                    return true;
                } else {
                    cout << " ERROR! :-( " << endl;
                }
                break;
            case 3:
                if (canDrawToken(play.players[0])) {
                    if (play.pool.cont == 0) {
                        return true;
                    }

                    drawTokens(play.pool, play.players[0]);
                } else {
                    cout << ">> Tienes fichas que puedes utilizar! :-) <<" << endl;
                }
                break;
        }

    }

    return false;
}

void showWinner(int winner) {
            cout << " el jugador " << winner << " ha sido el ganador de la ronda" << endl;
}

void showPoints(tPlay play) {
    for (int i = 1; i < play.numbersPlayers; i++) {
        cout << "Maquina#" << i << ": " << play.points[i] << endl;
	}
        cout << "Jugador: " << play.points[0] << endl;
}

void updatePoints(tPlay & play) {
	for (int i = 0; i < play.numbersPlayers; i++) {
		for (int j = 0; j < play.players[i].cont; j++) {
			play.points[i] += play.players[i].listToken[j].token1 + play.players[i].listToken[j].token2;
		}
	}
}

bool question_S_N (string text) {
    char option = ' ';
    
    while (option != 'S' && option != 'N') {
        cout << text;
        cin >> option;
    }

    borrar();
    return option == 'S';
}