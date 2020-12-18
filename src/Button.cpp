/*
 * Button.cpp
 *
 *  Created on: 2020年9月18日
 *      Author: zhiqiang
 */

#include "Button.h"

Button::Button(const u8 &pin) :
		pin(pin) {
	pinMode(pin, INPUT);
}

Button::~Button() {
}

void Button::SetOnKeyDownCallback(const Button::KeyEvent callback) {
	this->keyDownCallback = callback;
}

void Button::SetOnKeyUpCallback(const Button::KeyEvent callback) {
	this->keyUpCallback = callback;
}

void Button::Handle() {
	currentStatus = digitalRead(pin) == LOW;
	if (currentStatus != lastStatus) {
		if (currentStatus) {
			if(keyDownCallback != NULL){
				keyDownCallback();
			}
		} else {
			if(keyUpCallback != NULL){
				keyUpCallback();
			}
		}
	}
	delay(20);
	//保存为上一个按键状态
	lastStatus = currentStatus;
}
