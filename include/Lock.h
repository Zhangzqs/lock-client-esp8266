/*
 * Lock.h
 *
 *  Created on: 2020年9月16日
 *      Author: zhiqiang
 */

#ifndef LOCK_H_
#define LOCK_H_

#include <Arduino.h>
#include <Servo.h>
class Lock {
public:
	//指定舵机的io
	Lock(u8 servoIO);
	virtual ~Lock();

	//开锁
	void Open();

	//关锁
    void Close();

	//开关锁
	void Unlock();
private:
	Servo servo;
};

#endif /* LOCK_H_ */
