/*
 * Lock.cpp
 *
 *  Created on: 2020年9月16日
 *      Author: zhiqiang
 */

#include "Lock.h"
#include <Servo.h>

Lock::Lock(u8 servoIO){
	servo.attach(servoIO);
	servo.write(20);
}

//开锁
void Lock::Open(){
    Serial.println("准备开锁");
    servo.write(0);
    Serial.println("锁已打开");
}

//关锁
void Lock::Close(){
    servo.write(20);
    Serial.println("锁已关闭！");
}

void Lock::Unlock(){
    Open();
	delay(1000);
	Close();
}

Lock::~Lock() {
	servo.detach();
}

