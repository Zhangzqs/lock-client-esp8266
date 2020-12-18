/*
 * Remote.cpp
 *
 *  Created on: 2020年9月18日
 *      Author: zhiqiang
 */

#include "Remote.h"
#include "Utils.h"
#include <ESP8266WiFi.h>

Remote::Remote(const String &remoteHost, const u16 port) :
		remoteHost(remoteHost), port(port) {
    unlockCallback = nullptr;
    failedCallback = nullptr;

	//初始化加载远端ip地址
	remoteIp.fromString(remoteHost);

	//开启udp数据包监测
	udp.begin(port);
	const char *ip_addr = WiFi.localIP().toString().c_str();
	Serial.printf("本机IP %s, UDP端口为： %d，开始侦测udp数据包...\n", ip_addr, port);
}

Remote::~Remote() {
	udp.stopAll();
}

void Remote::Send(const u8 *pkg, int size) {
	//开始一个udp数据报文
	udp.beginPacket(remoteIp, port);
	udp.write(pkg, size);
	udp.endPacket();
}

void Remote::SendUid(const u32 &uid) {
	u8 pkg[5];
	pkg[0] = 0x00; //0x00表示传送了一个4字节的uid

	//大端字节序，发送uid信息
	u32_to_4u8(uid, &pkg[1]);

	Send(pkg, 5);
}

void Remote::SendButtonMsg() {
	const u8 head = 0x03;
	Send(&head, 1);
}

void Remote::Received(const int packetSize) {
	Serial.printf("接收到长度 %d 的字节数据，来自ip： %s, 端口： %d\n", packetSize,
					udp.remoteIP().toString().c_str(), udp.remotePort());
	int len = udp.read(buffer, 255);

	if (len == 1 && buffer[0] == 0x02) {
        if(failedCallback != nullptr){
            failedCallback();
        }
	}

	if (len == 5 && buffer[0] == 0x01) {
		u32 uid = _4u8_to_u32(&buffer[1]);
		if(unlockCallback != nullptr){
            unlockCallback(uid);
		}
	}

	otherCallback(len,buffer);

}

void Remote::Handle() {
	int packetSize = udp.parsePacket();
	if (packetSize) {
		Received(packetSize);
	}
}

void Remote::SetRemoteUnlockCallback(RemoteUnLockCallback callback) {
	this->unlockCallback = callback;
}

void Remote::SetValidFailedCallback(Remote::ValidFailedCallback failedCallback) {
    this->failedCallback = failedCallback;
}

void Remote::SetOtherCallback(Remote::OtherCallback otherCallback) {
    this->otherCallback = otherCallback;
}
