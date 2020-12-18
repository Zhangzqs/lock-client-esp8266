/*
 * Button.h
 *
 *  Created on: 2020年9月18日
 *      Author: zhiqiang
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include <Arduino.h>

class Button {
public:

	typedef void (*KeyEvent)();

	Button(const u8 &pin);
	virtual ~Button();

	//事件循环
	void Handle();

	//设置按下回调
	void SetOnKeyDownCallback(KeyEvent callback);

	//设置抬起回调
	void SetOnKeyUpCallback(KeyEvent callback);


private:
	u8 pin;
	KeyEvent keyDownCallback = NULL;
	KeyEvent keyUpCallback = NULL;

	//当前的按键状态(true表示按下，false表示未按下)
	bool currentStatus = false;
	//上一刻的按键状态
	bool lastStatus = false;
};

#endif /* BUTTON_H_ */
