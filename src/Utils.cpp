/*
 * Utils.cpp
 *
 *  Created on: 2020年10月15日
 *      Author: zhiqiang
 */

#include "Utils.h"

#include <ESP8266WiFi.h>

void connectWiFi(const char *ssid,const char *password) {
	Serial.print("\nConnecting to WiFI:");
	Serial.print(ssid);

	//设置WiFI工作模式为STA无线终端模式
	WiFi.mode(WIFI_STA);

	//开始连接WiFi
	WiFi.begin(ssid, password);

	//WiFi正在连接，显示正在连接的动画
	//若连接成功，则跳过该while循环
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.print(".");
	}

	Serial.println("\nWiFI connected.");
	Serial.print("IP address: ");
	IPAddress ip = WiFi.localIP();
	Serial.println(ip);
}

void u32_to_4u8(u32 uid,u8 *bytes){
	bytes[0] = uid>>24 & 0xff;
	bytes[1] = uid>>16 & 0xff;
	bytes[2] = uid>>8 & 0xff;
	bytes[3] = uid & 0xff;
}

u32 _4u8_to_u32(u8 *bytes){
	u32 uid = 0;
	uid |= bytes[0]<<24;
	uid |= bytes[1]<<16;
	uid |= bytes[2]<<8;
	uid |= bytes[3];
	return uid;
}

u16 _2u8_to_u16(u8 *bytes){
    u16 ret = 0;
    ret |= bytes[0]<<8;
    ret |= bytes[1];
    return ret;
}
