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
    tArrayToken pool;
	tPlayers players;
    tPoints points;
} tPlay;

int maxNumber = 0;

string toStr(int n) {
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

int maxNumTokens(int maxNumber)
{
	int sum = 0;
	for (int i = 1; i <= maxNumber+1; i++)
		sum += i;

	return sum;
}

// Token
string tokenToStr(short int left, short int right) {
    string str = "|" + toStr(left) + "-" + toStr(right) + "|";
    return str;
}

// painting pool
string poolToStr(tListToken pool) {
    string str;
    
    for ( int i = 0; i < pool.cont; i++ ) {
        str = str + tokenToStr(pool.listToken.token1[i], pool.listToken.token2[i]);
    }
    return str;
}

bool canPutLeft(string board, short int tokenN1, short int tokenN2) {
    return (board[1]-0x30==tokenN2);
}

bool canPutRight(string board, short int tokenN1, short int tokenN2) {
    return (board[board.size()-2]-0x30==tokenN1);
}


string putTokenLeft(string board, short int tokenN1, short int tokenN2) {
    return tokenToStr(tokenN1, tokenN2) + board;
}

string putTokenRight(string board, short int tokenN1, short int tokenN2) {
    return board + tokenToStr(tokenN1, tokenN2);
}


void deleteToken (tArrayToken tokenN1, tArrayToken tokenN2, short int& numPlayerToken, short int numToken) {
    for(int i = numToken; i < numPlayerToken; i++) {
        tokenN1[i] = tokenN1[i+1];
        tokenN2[i] = tokenN2[i+1];
    }
    numPlayerToken--;
}

void drawTokens (tArrayToken pool1, tArrayToken pool2, short int& cont, short int& tokenN1, short int& tokenN2) {
	tokenN1 = pool1[cont-1];
	tokenN2 = pool2[cont-1];
	cont--;
}

//Lee las fichas del siguiente renglón del archivo de entrada:
void readToken(ifstream& entrada, tListaFichas& listaFichas) {

}


// Tablero
void showBoard(tListToken token, tPlay play, string board, int numCounter, int numStolen) {
    cout << " -------------------- " << endl;
    cout << "|       TABLERO      |" << endl;
    cout << " -------------------- " << endl;
    cout << board << endl;
    cout << "Fichas colocadas: " << numCounter << " - Fichas robadas: " << numStolen << endl;
    cout << "Fichas del jugador: " << poolToStr(token) << endl;
	if (muestraFichasMaquina)
	{
		for (int i = 0; i < play.numbersPlayers - 1; i++)
		{
			cout << "Maquina#" << i + 1 << ": " << poolToStr();
		}
	}
	else
	{

	}
    return;
}

// Menú
int showMenu() {
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
short int aleat(int n) {
    return rand()%(n+1);
}

short int chooseToken(tArrayToken tokenN1, tArrayToken tokenN2, int numPlayerToken) {
    int chooseToken = 0;

    for(int i = 0; i < numPlayerToken; i++) {
        cout << "(" << i+1 << ") " << tokenToStr(tokenN1[i], tokenN2[i]) << endl;
    }
    
    while (chooseToken < 1 || chooseToken > numPlayerToken) {
        cout << "Elije ficha: ";
        cin >> chooseToken;
    }
    return chooseToken-1;
}

short int chooseMax() {
    int chooseMax = 0;
    
    while (chooseMax < 6 || chooseMax > 9) {
        cout << "Variante del juego (entre 6 y 9): ";
        cin >> chooseMax;
    }
    return chooseMax;
}

bool chooseSave() {
    char option = ' ';
    
    while (option != 'S' && option != 'N') {
        cout << "¿Quiéres guardar antes de salir? (S/N)";
        cin >> option;
    }
    
    return option == 'S';
}

bool openFile() {
    char option = ' ';
    
    while (option != 'S' && option != 'N') {
        cout << "¿Quiéres abrir un archivo anterior? (S/N)";
        cin >> option;
    }
    
    return option == 'S';
}

void generatePool(tArrayToken pool1, tArrayToken pool2, int maxNumber) {
    int k = 0;
    for ( int i = 0; i <= maxNumber; i++ ) {
        for ( int j = 0; j <= i; j++ ) {
            pool1[ k ] = i;
            pool2[ k ] = j;
            k++;
        }
    }
}

void disorderPool(tArrayToken pool1, tArrayToken pool2) {
    int idx;
    short int tmp1, tmp2;
        for (int i = maxNumTokens(maxNumber) - 1; i >= 0; i--) {
            idx = rand() % (i + 1);
            if (i != idx) {
            tmp1 = pool1[i];
            tmp2 = pool2[i];
            pool1[i] = pool1[idx];
            pool2[i] = pool2[idx];
            pool1[idx] = tmp1;
            pool2[idx] = tmp2;
        }
    }
}

string convertArrayTokenToString(tArrayToken xs, short int maxNumber) {
    string returnstring;
    for (int temp = 0; temp < maxNumber; temp++)
        returnstring += toStr(xs[temp]);
    return returnstring;
}

void convertStringToArray(string el, int num, tArrayToken arr) {
    for (int i = 0; i < num; ++i) {
        arr[i] = el[i] - '0';
    }
}

void readListToken(ifstream& archivo, tListToken& listToken) {
    string _el
    archivo >> _el
    convertStringToArray(_el, numPoolToken, listToken.listToken.tokenN1);
    
}

bool collectData(tArrayToken pool1, tArrayToken pool2, short int& numPlayerToken, string& board, tArrayToken tokenN1, tArrayToken tokenN2, short int& numPoolToken, int& counter, int& stolen) {
    ifstream archivo;
    archivo.open("domino_save.txt", ios::in);
    if (!archivo.is_open()) {
        cout << "¡No se ha podido abrir el archivo!" << endl;
        return false;
    }
    string _numPoolToken, _numPlayerToken;

    archivo >> board;
    archivo >> _numPlayerToken;
    archivo >> _numPoolToken;
    readListToken(archivo, listToken);
    readListToken(archivo, listToken);
    archivo >> counter;
    archivo >> stolen;
    archivo >> maxNumber;

    numPlayerToken = stoi(_numPlayerToken);
    numPoolToken = stoi(_numPoolToken);

    archivo.close();
    
    return true;
}

void saveCollectData(tArrayToken pool1, tArrayToken pool2, short int numPlayerToken, string board, tArrayToken tokenN1, tArrayToken tokenN2, int numPoolToken, int counter, int stolen) {
    ofstream file;
    file.open("domino_save.txt", ios::out);
    if (file.is_open()) {
    file << board << '\n'
        << numPlayerToken << '\n'
        << numPoolToken << '\n'
        << convertArrayTokenToString(pool1, maxNumTokens(maxNumber)) << '\n'
        << convertArrayTokenToString(pool2, maxNumTokens(maxNumber)) << '\n'
        << convertArrayTokenToString(tokenN1, numPlayerToken) << '\n'
        << convertArrayTokenToString(tokenN2, numPlayerToken) << '\n'
        << counter << '\n'
        << stolen << '\n'
        << maxNumber << '\n';

        file.close();

    } else {
        cout << "¡No se ha podido abrir el archivo!" << endl;
    }
}


bool canDrawToken(string board, tArrayToken tokenN1, tArrayToken tokenN2, int numPlayerToken) {
    bool canPutLeftBool = true;
    bool canPutRightBool = true;

    for (int i = 0; i < numPlayerToken; i++) {
        canPutLeftBool = canPutLeft(board, tokenN1[i], tokenN2[i]) || canPutLeft(board, tokenN2[i], tokenN1[i]);
        canPutRightBool = canPutRight(board, tokenN1[i], tokenN2[i]) || canPutRight(board, tokenN2[i], tokenN1[i]);
        if (canPutLeftBool || canPutRightBool) {
            return false;
        }
    }
    return true;
}

int main(int argc, const char * argv[]) {
    int counter = 0;
    int stolen = 0;
    short int numPlayerToken = 0;
    short int numPoolToken = 0;
    tArrayToken tokenN1;
    tArrayToken tokenN2;
    string board;
    tArrayToken pool1;
    tArrayToken pool2;
    
    
    srand(time(NULL));


	if (!openFile() || !collectData(pool1, pool2, numPlayerToken, board, tokenN1, tokenN2, numPoolToken, counter, stolen)) {

		maxNumber = chooseMax();

		generatePool(pool1, pool2, maxNumber);
		disorderPool(pool1, pool2);
		numPlayerToken = 7;
		numPoolToken = maxNumTokens(maxNumber);

		for (int i = 0; i < numPlayerToken; i++) {
			tokenN1[i] = pool1[numPoolToken-1];
			tokenN2[i] = pool2[numPoolToken-1];
			numPoolToken--;
		}

		board = tokenToStr(pool1[numPoolToken-1],pool2[numPoolToken-1]);
		numPoolToken--;
	}
    
    for (int option = 0; option != 4;) {
        showBoard(tokenN1, tokenN2, board, counter, stolen, numPlayerToken);
        option = showMenu();
        short int chosen;

        switch(option){
            case 1:
                chosen = chooseToken(tokenN1, tokenN2, numPlayerToken);
                if (canPutLeft(board, tokenN1[chosen], tokenN2[chosen])) {
                    board = putTokenLeft(board, tokenN1[chosen], tokenN2[chosen]);
                    deleteToken(tokenN1, tokenN2, numPlayerToken, chosen);
                    counter++;
                } else if (canPutLeft(board, tokenN2[chosen], tokenN1[chosen])) {
                    board = putTokenLeft(board, tokenN2[chosen], tokenN1[chosen]);
                    deleteToken(tokenN1, tokenN2, numPlayerToken, chosen);
                    counter++;
                } else {
                    cout << " ERROR! :-( " << endl;
                }
                break;
            case 2: 
                chosen = chooseToken(tokenN1, tokenN2, numPlayerToken);
                if (canPutRight(board, tokenN1[chosen], tokenN2[chosen])) {
                    board = putTokenRight(board, tokenN1[chosen], tokenN2[chosen]);
                    deleteToken(tokenN1, tokenN2, numPlayerToken, chosen);
                    counter++;
                } else if (canPutRight(board, tokenN2[chosen], tokenN1[chosen])) {
                    board = putTokenRight(board, tokenN2[chosen], tokenN1[chosen]);
                    deleteToken(tokenN1, tokenN2, numPlayerToken, chosen);
                    counter++;
                } else {
                    cout << " ERROR! :-( " << endl;
                }
                break;
            case 3:
                if (canDrawToken(board, tokenN1, tokenN2, numPlayerToken)) {
                    numPlayerToken = numPlayerToken + 1;
                    drawTokens(pool1, pool2, numPoolToken, tokenN1[numPlayerToken-1], tokenN2[numPlayerToken-1]);
                    stolen++;
                } else {
                    cout << ">> Tienes fichas que puedes utilizar! :-) <<" << endl;
                }
                break;
        }
        if (counter == maxNumTokens(maxNumber)-1)
        {
            cout << "Enhorabuena" << endl;
            break;
        }
        else if (counter + numPlayerToken == maxNumTokens(maxNumber)-1)
        {
            cout << "Has perdido" << endl;
			break;
		}
    }
    
    if (chooseSave()) {
        saveCollectData(pool1, pool2, numPlayerToken, board, tokenN1, tokenN2, numPoolToken, counter, stolen);
    }
    return 0;
}
