/*
 * Beep.h
 *
 *  Created on: 2020年10月16日
 *      Author: zhiqiang
 */

#ifndef BEEP_H_
#define BEEP_H_

#include <Arduino.h>

class Beep {
public:
	Beep(u8 pin);
	virtual ~Beep();
	void beep(u16 hz,u16 ms);
	void playSuccessful();
	void playFailed();
	void playSensedCard();
private:
	u8 pin;
};

#endif /* BEEP_H_ */
