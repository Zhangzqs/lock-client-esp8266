/*
 * Beep.cpp
 *
 *  Created on: 2020年10月16日
 *      Author: zhiqiang
 */

#include "Beep.h"

Beep::Beep(u8 pin) : pin(pin) {
    pinMode(pin, OUTPUT);
}

void Beep::beep(u16 hz, u16 ms) {
    tone(pin, hz, ms);
}

Beep::~Beep() {

}

void Beep::playSuccessful() {
    u16 hzs[] = {523, 587, 659, 698, 784, 880, 998, 1046, 1175, 1318, 1397, 1568, 1760, 1976};
//    u16 mus[][2] = {
//            {7, 500},
//            {6, 500},
//            {5, 500},
//            {6, 500},
//            {7, 500},
//            {9, 1000},
//            {6, 500},
//            {7, 500},
//            {6, 500},
//            {2, 500},
//            {5, 1500},
//    };
    u16 mus[][2] = {
            {7, 500},
            {6, 500},
            {5, 500},
            {6, 500},
            {7, 500},
            {9, 1000},
            {6, 500},
            {7, 500},
            {6, 500},
            {2, 500},
            {5, 1500},
    };
    for(auto & mu : mus){
        beep(hzs[mu[0]-1],mu[1]);
        delay(mu[1]);
    }
}


void Beep::playFailed() {

}

void Beep::playSensedCard() {
    beep(600, 500);
}

