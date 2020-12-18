/*
 * CardReader.h
 *
 *  Created on: 2020年9月17日
 *      Author: zhiqiang
 */

#ifndef CARDREADER_H_
#define CARDREADER_H_

#include <Arduino.h>
#include <MFRC522.h>

class CardReader {


public:

	//隐藏回调函数的函数指针类型的复杂性
	typedef void (*CardListener)(const u32 uid);


	CardReader(const u8 rc522_sda_pin, const u8 rc522_rst_pin);
	virtual ~CardReader();

	//监听函数,需要放进循环的
	void Handle();

	//设置当感应到卡片时的回调函数
	void SetOnSensedCardListener(const CardListener callback);

	MFRC522* getMFRC522();
private:
	//RC522实例指针
	MFRC522 *rc522;

	//当感应到卡片的回调
	CardListener sensedCallback = NULL;

	//输出uid的工具函数
	u32 readUid();
};

#endif /* CARDREADER_H_ */
