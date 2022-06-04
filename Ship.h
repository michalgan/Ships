//
// Created by michal on 04.06.22.
//

#ifndef SHIPS_SHIP_H
#define SHIPS_SHIP_H


#include <string.h>

struct Ship{
    Ship(int headY, int headX, char direction, char * type){
        this->headY = headY;
        this->headX = headX;
        this->direction = direction;
        this->type = type;
        if(!strcmp(type, "CAR")){
            this->length = 5;

        }
        else if(!strcmp(type, "BAT")){
            this->length = 4;

        }
        else if(!strcmp(type, "CRU")){
            this->length = 3;

        }
        else if(!strcmp(type, "DES")){
            this->length = 2;

        }
        this->coords = new int *[this->length];
        for (int i = 0; i < this->length; i++) {
            coords[i] = new int[2];

        }

    }

    void setStates(char * partStates){
        this->partStates = partStates;

    }



    int headY;
    int headX;
    char direction;
    char * type;
    int length;
    int ** coords;
    char * partStates;

};


#endif //SHIPS_SHIP_H
