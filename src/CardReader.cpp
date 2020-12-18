/*
 * CardReader.cpp
 *
 *  Created on: 2020年9月17日
 *      Author: zhiqiang
 */

#include "CardReader.h"
#include <SPI.h>
#include "Utils.h"

CardReader::CardReader(const u8 rc522_sda_pin, const u8 rc522_rst_pin) {
	rc522 = new MFRC522(rc522_sda_pin, rc522_rst_pin);

	//启动硬件SPI通讯,用于与RC522模块连接
	SPI.begin();

	//初始化rc522模块
	rc522->PCD_Init();

}

CardReader::~CardReader() {
}

u32 CardReader::readUid() {
	MFRC522::Uid uid = rc522->uid;

	if (uid.size == 4) {
		return _4u8_to_u32(uid.uidByte);
	}
	return 0;
}

void CardReader::Handle() {

	//搜索卡片
	if (!rc522->PICC_IsNewCardPresent()) {
		return;
	}

	//验证卡的可读性
	if(!rc522->PICC_ReadCardSerial()){
		return;
	}
	u32 uid = readUid();

	sensedCallback(uid);

	delay(500);
}

//设置当感应到卡片时的回调函数
void CardReader::SetOnSensedCardListener(
		const CardReader::CardListener callback) {
	this->sensedCallback = callback;
}

MFRC522 *CardReader::getMFRC522() {
    return rc522;
}
