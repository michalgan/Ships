#include <iostream>
#include "Ship.h"
#include <string.h>
using namespace std;

const int BUFFER_SIZE = 20;

struct Game{

    Game(){
        this->board = new char * [DEFAULT_BOARD_Y];
        for (int i = 0; i < DEFAULT_BOARD_Y; i++) {
            board[i] = new char[DEFAULT_BOARD_X];
            for (int j = 0; j < DEFAULT_BOARD_X; j++) {
                board[i][j] = ' ';

            }

        }
        this->maxNumberOfShips = new int * [DEFAULT_NUMBER_OF_PLAYERS];
        this->shipCounter = new int * [DEFAULT_NUMBER_OF_PLAYERS];
        this->fleet = new Ship *** [DEFAULT_NUMBER_OF_PLAYERS];
        this->playerIDs = new char[DEFAULT_NUMBER_OF_PLAYERS];
        this->playerIDs[0] = 'A';
        this->playerIDs[1] = 'B';
        this->currentPlayerIndex = -1;
        this->boardY = DEFAULT_BOARD_Y;
        this->boardX = DEFAULT_BOARD_X;
        this->partsRemaining = new int[DEFAULT_NUMBER_OF_PLAYERS];
        this->startPositionY = new int[DEFAULT_NUMBER_OF_PLAYERS];
        this->startPositionX = new int[DEFAULT_NUMBER_OF_PLAYERS];
        this->endPositionY = new int[DEFAULT_NUMBER_OF_PLAYERS];
        this->endPositionX = new int[DEFAULT_NUMBER_OF_PLAYERS];
        this->commandBlockOpen = false;
        this->gameEnd = false;
        for (int i = 0; i < DEFAULT_NUMBER_OF_PLAYERS; i++) {
            this->fleet[i] = new Ship ** [NUMBER_OF_TYPES_OF_SHIPS];
            this->partsRemaining[i] = 0;
            this->shipCounter[i] = new int[NUMBER_OF_TYPES_OF_SHIPS];
            this->maxNumberOfShips[i] = new int[NUMBER_OF_TYPES_OF_SHIPS];
            for (int j = 0; j < NUMBER_OF_TYPES_OF_SHIPS; j++) {
                this->fleet[i][j] = new Ship * [4];
                this->shipCounter[i][j] = 0;

            }


        }
        this->startPositionY[0] = 0;
        this->startPositionX[0] = 0;
        this->startPositionY[1] = 11;
        this->startPositionX[1] = 0;

        this->endPositionY[0] = 10;
        this->endPositionX[0] = 10;
        this->endPositionY[1] = 21;
        this->endPositionX[1] = 10;

    }
    void initMaxNumbersOfShips(){
        for (int i = 0; i < DEFAULT_NUMBER_OF_PLAYERS; i++) {
            //printf("Player %c fleet size:", playerIDs[i] );
            for (int j = 0; j < NUMBER_OF_TYPES_OF_SHIPS; j++) {
                this->maxNumberOfShips[i][j] = j+1;
                //printf(" %d", this->maxNumberOfShips[i][j]);

            }
            //printf("\n");

        }

    }

    void initMaxNumbersOfShips(int * numbersByType){
        for (int i = 0; i < NUMBER_OF_TYPES_OF_SHIPS; i++) {
            this->maxNumberOfShips[currentPlayerIndex][i] = numbersByType[i];
            this->fleet[currentPlayerIndex][i] = new Ship * [numbersByType[i]];

        }

    }

    bool isTileOutsideTheBoard(int y, int x) {
        return y < 0 || x < 0 || y >= this->boardY || x >= this->boardX;

    }


    bool checkNextPlayer(int nextPlayerIndex){
        if(currentPlayerIndex == nextPlayerIndex){
            cout<<"INVALID OPERATION \""<<command<< "\": THE OTHER PLAYER EXPECTED"<<endl;
            exit(0);

        }
        return false;

    }


    void inputCommands(){
        while(!gameEnd){
            if(feof(stdin) != 0){
                break;

            }
            scanf("%s", command);
            if(!strcmp(command, "[playerA]") ){
                if(!commandBlockOpen){
                    int playerIndex = findPlayerIndexByID('A');
                    checkNextPlayer(playerIndex);
                    commandBlockOpen = true;
                    this->currentPlayerIndex = playerIndex;
                    this->otherPlayerIndex = findPlayerIndexByID('B');

                }
                else{
                    commandBlockOpen = false;

                }

            }
            else if(!strcmp(command, "[playerB]")){
                if(!commandBlockOpen){
                    int playerIndex = findPlayerIndexByID('B');
                    checkNextPlayer(playerIndex);
                    commandBlockOpen = true;
                    this->currentPlayerIndex = playerIndex;
                    this->otherPlayerIndex = findPlayerIndexByID('A');

                }
                else{
                    commandBlockOpen = false;

                }


            }
                //INIT_POSITION A 0 0 5 5
            else if(!strcmp(command, "INIT_POSITION")){
                char playerID;
                cin>>playerID;
                currentPlayerIndex = findPlayerIndexByID(playerID);
                int startY, startX, endY, endX;
                cin>>startY>>startX>>endY>>endX;
                initPosition(startY, startX, endY, endX);

            }
            else if(!strcmp(command, "PRINT")){
                this->printGameState();

            }
            else if(!strcmp(command, "PLACE_SHIP")){
                int y, x, id;
                char direction;
                char type[3];
                cin>>y>>x>>direction>>id>>type;
                this->placeShip(y,x, direction, id, type);

            }
            else if(!strcmp(command, "SHIP")){
                int y, x, id;
                char playerID, direction;
                char type[3];
                cin>>playerID>>y>>x>>direction>>id>>type;
                currentPlayerIndex = findPlayerIndexByID(playerID);
                Ship * ship = this->placeShip(y,x, direction, id, type);
                char * partStates = new char[ship->length];
                cin>>partStates;
                ship->setStates(partStates);
                for (int i = 0; i < ship->length; i++) {
                    if (partStates[i] == '0'){
                        this->board[ship->coords[i][0]][ship->coords[i][1]] = 'x';
                        partsRemaining[currentPlayerIndex] -= 1;

                    }

                }

            }
            else if(!strcmp(command, "REEF")){
                int y, x;
                cin>>y>>x;
                setReef(y,x);


            }
            else if(!strcmp(command, "SHOOT")){
                int y, x;
                cin>>y>>x;
                shoot(y,x);

            }
            else if(!strcmp(command, "SET_FLEET")){
                char playerID;
                int * numbersByType = new int[NUMBER_OF_TYPES_OF_SHIPS];
                cin>>playerID;
                int numberOfShipsCurrentType;
                currentPlayerIndex = findPlayerIndexByID(playerID);
                for (int i = 0; i <NUMBER_OF_TYPES_OF_SHIPS; i++) {
                    cin>>numberOfShipsCurrentType;
                    fleet[currentPlayerIndex][i] = new Ship * [numberOfShipsCurrentType];
                    numbersByType[i] = numberOfShipsCurrentType;

                }
                initMaxNumbersOfShips(numbersByType);

            }
            //MOVE 0 CRU R
            /*else if(strcmp(command, "MOVE")){
                int id;
                char type[3], direction;
                //move(id, type, direction);
            }*/

        }


    }
    void initPosition(int yStart, int xStart, int yEdn, int xEnd){
        startPositionY[currentPlayerIndex] = yStart;
        startPositionX[currentPlayerIndex] = xStart;
        endPositionY[currentPlayerIndex] = yEdn;
        endPositionX[currentPlayerIndex] = xEnd;

    }


    int findWinnerIndex(){
        if(partsRemaining[0] == 0){
            return 1;

        }
        else if(partsRemaining[1] == 0){
            return 0;

        }
        else{
            return -1;

        }

    }

    void printGameState(){
        for (int i = 0; i < this->boardY; i++) {
            for (int j = 0; j < this->boardX; ++j) {
                cout<<this->board[i][j];

            }
            cout<<endl;

        }
        cout<<"PARTS REMAINING:: ";
        for (int i = 0; i < DEFAULT_NUMBER_OF_PLAYERS; i++) {
            cout<<this->playerIDs[i]<<" : "<<this->partsRemaining[i]<<' ';

        }
        cout<<endl;

    }
    int findPlayerIndexByID(char id){
        for (int i = 0; i < DEFAULT_NUMBER_OF_PLAYERS; i++) {
            if(playerIDs[i] == id){
                return i;

            }

        }
        return -1;

    }

    bool isTileInStartPosition(int y, int x){

        return (y >= this->startPositionY[this->currentPlayerIndex] && y <= this->endPositionY[this->currentPlayerIndex]
                && x >= this->startPositionX[this->currentPlayerIndex] && x <= this->endPositionX[this->currentPlayerIndex]);

    }
    static int findTypeIndex(char * type){
        if(!strcmp(type,"CAR")){
            return 0;

        }
        else if(!strcmp(type,"BAT")){
            return 1;

        }
        else if(!strcmp(type,"CRU")){
            return 2;

        }
        else if(!strcmp(type,"DES")){
            return 3;

        }
        return -1;

    }

    bool classAllClassSet(char * type){
        int typeIndex = findTypeIndex(type);
        return this->shipCounter[currentPlayerIndex][typeIndex] >= this->maxNumberOfShips[currentPlayerIndex][typeIndex];


    }

    bool allShipsPlaced(){
        for (int i = 0; i < NUMBER_OF_TYPES_OF_SHIPS; i++) {
            if(this->shipCounter[currentPlayerIndex][i] < maxNumberOfShips[currentPlayerIndex][i]){
                return false;

            }

        }
        return true;

    }

    void shoot(int y, int x){
        if (!allShipsPlaced()){
            cout<<"INVALID OPERATION \"SHOOT "<<y<<' '<<x<<"\": NOT ALL SHIPS PLACED"<<endl;
            exit(0);

        }
        if(isTileOutsideTheBoard(y, x)){
            cout<<"INVALID OPERATION \"SHOOT "<<y<<' '<<x<< "\": FIELD DOES NOT EXIST"<<endl;
            return;

        }
        if(this->board[y][x] == '+'){
            this->board[y][x] = 'x';
            partsRemaining[otherPlayerIndex] --;

        }
        if(findWinnerIndex() != -1){
            int winnerIndex = findWinnerIndex();
            cout<<playerIDs[winnerIndex]<<" won"<<endl;
            gameEnd = true;

        }

    }

    void setReef(int y, int x){
        this->board[y][x] = '#';

    }

    bool isReefOnTile(int y, int x){
        return board[y][x] == '#';

    }

    bool isShipOnTile(int y, int x){
        return board[y][x] == '+' || board[y][x] == 'x';

    }

    bool isShipOnCollision(int y, int x, char direction){
        switch(direction){
            case 'N':
                y -= 1;
                break;
            case 'W':
                x -= 1;
                break;
            case 'S':
                y += 1;
                break;
            case 'E':
                x += 1;
                break;

        }
        if(isTileOutsideTheBoard(y, x)){
            return false;

        }
        else{
            return board[y][x] == 'x' || board[y][x] == '+';

        }

    }

    /*void move(int id, char * type, char direction){
        Ship * ship = fleet[currentPlayerIndex][findTypeIndex(type)][id];

    }

    void checkMove(Ship * ship, char direction){
        int tileY = ship->headY, tileX = ship->headX;
        switch(direction){
            case 'F':
                switch(ship->direction){
                    case 'N':
                        tileY
                        break;
                    case 'W':
                        break;
                    case 'S':
                        break;
                    case 'E':
                        break;
                }
                break;
            case 'L':
                break;
            case 'R':
                break;

        }
    }*/



    Ship * placeShip(int headY, int headX, char direction, int id,  char * type) {
        Ship * ship = new Ship(headY, headX, direction, type);
        int  tileY, tileX;
        int typeIndex = findTypeIndex(type);
        for (int i = 0; i < ship->length ; i++) {
            tileY = headY;
            tileX = headX;
            switch(direction){
                case 'N':
                    tileY += i;
                    break;
                case 'W':
                    tileX += i;
                    break;
                case 'S':
                    tileY -= i;
                    break;
                case 'E':
                    tileX -= i;
                    break;
                default:
                    cout<<"DIRECTION_ERROR!"<<endl;

            }
            if(!this->isTileInStartPosition(tileY, tileX)){
                cout<<"INVALID OPERATION PLACE_SHIP "<<headY<<' '<<headX<<' '<<direction<<' '<<id<<' '<<type<<": NOT IN STARTING POSITION"<<endl;
                exit(0);

            }
            else if(this->classAllClassSet(type) && fleet[currentPlayerIndex][typeIndex][id] == nullptr){
                cout<<"INVALID OPERATION PLACE_SHIP "<<headY<<' '<<headX<<' '<<direction<<' '<<id<<' '<<type<<": ALL SHIPS OF THE CLASS ALREADY SET"<<endl;
                exit(0);

            }
            else if(fleet[currentPlayerIndex][typeIndex][id] != nullptr){
                cout<<"INVALID OPERATION PLACE_SHIP "<<headY<<' '<<headX<<' '<<direction<<' '<<id<<' '<<type<<": SHIP ALREADY PRESENT"<<endl;
                exit(0);

            }
            else if(isReefOnTile(tileY, tileX)){
                printf("INVALID OPERATION \"PLACE_SHIP %d %d %c %d %s\": PLACING SHIP ON REEF\n", headY, headX, direction, id, type);
                exit(0);

            }
            else if(isShipOnTile(tileY, tileX) || isShipOnCollision(tileY, tileX, direction) ){
                printf("INVALID OPERATION \"PLACE_SHIP %d %d %c %d %s\": PLACING SHIP TOO CLOSE TO OTHER SHIP\n", headY, headX, direction, id, type);
                exit(0);

            }

        }
        for (int i = 0; i < ship->length ; i++) {
            tileY = headY;
            tileX = headX;
            switch(direction){
                case 'N':
                    tileY += i;
                    break;
                case 'W':
                    tileX += i;
                    break;
                case 'S':
                    tileY -= i;
                    break;
                case 'E':
                    tileX -= i;
                    break;
                default:
                    cout<<"DIRECTION_ERROR!"<<endl;

            }

            ship->coords[i][0] = tileY;
            ship->coords[i][1] = tileX;
            if(!isTileOutsideTheBoard(tileY, tileX)){
                this->board[tileY][tileX] = '+';

            }


        }
        fleet[currentPlayerIndex][typeIndex][id] = ship;
        this->shipCounter[currentPlayerIndex][typeIndex] += 1;
        this->partsRemaining[currentPlayerIndex] += ship->length;
        //cout<<"Ship placed successfully"<<endl;
        return ship;

    }




    int currentPlayerIndex;
    int otherPlayerIndex;
    int boardY;
    int boardX;
    char ** board;
    char * playerIDs;
    int * partsRemaining;
    Ship **** fleet;
    int * startPositionY;
    int * startPositionX;
    int * endPositionY;
    int * endPositionX;
    int ** shipCounter;
    int ** maxNumberOfShips;
    bool commandBlockOpen;
    bool gameEnd;
    char command[BUFFER_SIZE];


    //states
    static const int DEFAULT_BOARD_Y = 21;
    static const int DEFAULT_BOARD_X = 10;
    static const int DEFAULT_NUMBER_OF_PLAYERS = 2;
    static const int NUMBER_OF_TYPES_OF_SHIPS = 4;
};





int main(){
    Game * game = new Game();
    game->initMaxNumbersOfShips();
    game->inputCommands();
    return 0;
}