/*********************************************
// Created by m1556 on 2024/3/28.
*********************************************/
#include "def.h"


Prototype::~Prototype(){
    for (int i = 0; i < this->Constants.size(); ++i) {
        delete this->Constants[i];
    }

    for (int j = 0; j < this->Protos.size(); ++j) {
        delete this->Protos[j];
    }

    for (int k = 0; k < this->Locvars.size(); ++k) {
        delete this->Locvars[k];
    }
}