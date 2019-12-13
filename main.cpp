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
const int numToken = 55;
const int MAX_PLAYERS = 4;
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
    int playerTurn;
    tListToken pool;
	tPlayers players;
    tPoints points;
} tPlay;

int maxNumber = 0;

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

int maxNumTokens (tPlay play)
{
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

bool canPutLeft (string board, short int token) {
    return (board[1]-0x30==token);
}

bool canPutRight (string board, short int token) {
    return (board[board.size()-2]-0x30==token);
}


string putTokenLeft (string board, tToken token) {
    return tokenToStr(token.token1, token.token2) + board;
}

string putTokenRight (string board, tToken token) {
    return board + tokenToStr(token.token1, token.token2);
}


void deleteToken (tToken token, short int& numPlayerToken, short int numToken) {
    for(int i = numToken; i < numPlayerToken; i++) {
        token[i].token1 = token[i+1].token1;
        token[i].token2 = token[i+1].token2;
    }
    numPlayerToken--;
}

void drawTokens (tListToken pool, tToken token) {
	token.token1 = pool.listToken[cont-1].token1;
	token.token2 = pool.listToken[cont-1].token2;
	pool.cont--;
}

// Tablero
void showBoard (tPlay play, string board) {
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

short int chooseToken (tToken token, int numPlayerToken) {
    int chooseToken = 0;

    for(int i = 0; i < numPlayerToken; i++) {
        cout << "(" << i+1 << ") " << tokenToStr(token.listToken[i].token1, token.listToken[i].token2) << endl;
    }
    
    while (chooseToken < 1 || chooseToken > numPlayerToken) {
        cout << "Elije ficha: ";
        cin >> chooseToken;
    }
    return chooseToken-1;
}

short int chooseMax () {
    int chooseMax = 0;
    
    while (chooseMax < 6 || chooseMax > 9) {
        cout << "Variante del juego (entre 6 y 9): ";
        cin >> chooseMax;
    }
    return chooseMax;
}

bool chooseSave () {
    char option = ' ';
    
    while (option != 'S' && option != 'N') {
        cout << "¿Quiéres guardar antes de salir? (S/N)";
        cin >> option;
    }
    
    return option == 'S';
}

bool openFile () {
    char option = ' ';
    
    while (option != 'S' && option != 'N') {
        cout << "¿Quiéres abrir un archivo anterior? (S/N)";
        cin >> option;
    }
    
    return option == 'S';
}

void generatePool (tPlay play) {
    int k = 0;
    for ( int i = 0; i <= play.maxNumber; i++ ) {
        for ( int j = 0; j <= i; j++ ) {
            play.pool.listToken[k].token1 = i;
            play.pool.listToken[k].token2 = j;
            k++;
        }
    }
}

void disorderPool (tPlay play) {
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

bool readGame(tPlay play, string& board) {
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

void writeGame(tPlay play, string board) {
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


bool canDrawToken(string board, tListToken token, int numPlayerToken) {
    bool canPutLeftBool = true;
    bool canPutRightBool = true;

    for (int i = 0; i < numPlayerToken; i++) {
        canPutLeftBool = canPutLeft(board, token.listToken[i].token1, token.listToken[i].token2) || canPutLeft(board, token.listToken[i].token2, token.listToken[i].token1);
        canPutRightBool = canPutRight(board, token.listToken[i].token1, token.listToken[i].token2) || canPutRight(board, token.listToken[i].token2, token.listToken[i].token1);
        if (canPutLeftBool || canPutRightBool) {
            return false;
        }
    }
    return true;
}

bool isGameOver(tPlay& play) {
    for (int j = 0; j < play.numbersPlayers; j++) {
        // tiene fichas
        if (play.players[j].token1.length) {
            return true;
        }
        
        // puede colocar a la izquierda o a la derecha
        if (!canDrawToken(board, token, numPlayerToken)) {
            return true;
        }
    }

    return false;
}

bool strategy1(tPlay& play, int player) {

}

int playerTurn(tPlay& play) {

}

void init(tPlay& play, int& numPlayerToken) {
    maxNumber = chooseMax();

		generatePool(play);
		disorderPool(play);
		numPlayerToken = 7;
		play.pool.cont = maxNumTokens(play);

		for (int j = 0; j < play.numbersPlayers; j++) {
			for (int i = 0; i < numPlayerToken; i++) {
				play.players[j].listToken[i].token1 = play.pool.listToken[play.pool.cont-1].token1;
				play.players[j].listToken[i].token2 = play.pool.listToken[play.pool.cont-1].token2;
				play.players[j].cont++;
				play.pool.cont--;
			}
		}

		play.pool.cont--;
}

int main(int argc, const char * argv[]) {
    tPlay play;
    string board;
    tListToken token;
    int turn;
    int numPlayerToken;
    srand(time(NULL));

	if (!openFile() || !readGame(play, board)) {
        init(play, numPlayerToken);
		board = tokenToStr(play.listToken[play.pool.cont-1].token1,play.listToken[play.pool.cont-1].token2);
	}
    
    for (int option = 0; option != 4;) {
        showBoard(play, board);
        option = showMenu();
        short int chosen;

        if (isGameOver(play)) {
            cout << "¡Sin salida!" << endl;
            return 0;
        }

        turn = playerTurn();

        switch(option) {
            case 1:
                chosen = chooseToken(token, numPlayerToken);
                if (canPutLeft(board, play.players[turn].listToken[chosen].token1)) {
                    board = putTokenLeft(board, play.players[turn].listToken[chosen].token1);
                    deleteToken(token, numPlayerToken, chosen);
                    counter++;
                } else if (canPutLeft(board, play.players[turn].listToken[chosen].token2)) {
                    board = putTokenLeft(board, play.players[turn].listToken[chosen].token2);
                    deleteToken(token, numPlayerToken, chosen);
                    counter++;
                } else {
                    cout << " ERROR! :-( " << endl;
                }
                break;
            case 2: 
                chosen = chooseToken(token, numPlayerToken);
                if (canPutRight(board, play.players[turn].listToken[chosen].token1)) {
                    board = putTokenRight(board, play.players[turn].listToken[chosen].token1);
                    deleteToken(token, numPlayerToken, chosen);
                } else if (canPutRight(board, play.players[turn].listToken[chosen].token2)) {
                    board = putTokenRight(board, play.players[turn].listToken[chosen].token2);
                    deleteToken(token, numPlayerToken, chosen);
                } else {
                    cout << " ERROR! :-( " << endl;
                }
                break;
            case 3:
                if (canDrawToken(board, token, numPlayerToken)) {
                    numPlayerToken = numPlayerToken + 1;
                    drawTokens(pool, token[numPlayerToken-1]);
                } else {
                    cout << ">> Tienes fichas que puedes utilizar! :-) <<" << endl;
                }
                break;
        }
    }
    
    if (chooseSave()) {
        writeGame(play, board);
    }
    return 0;
}
