/*
 * Remote.h
 *
 *  Created on: 2020年9月18日
 *      Author: zhiqiang
 */

#ifndef REMOTE_H_
#define REMOTE_H_

#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/*
 * 均为UDP通信
 * 当收到卡信息后，发送0x00+4byte的uid到服务器
 * 当收到0x01+4byte uid信息后，表示该卡成功开锁
 * 当收到0x02时，表示该卡信息验证失败，开锁失败
 * 当按钮被按下时，发送一条udp消息到服务器
 * */

class Remote {
public:
	typedef void (*RemoteUnLockCallback)(uint32_t uid);
    typedef void (*ValidFailedCallback)();
    typedef void (*OtherCallback)(int size,byte *buffer);

	Remote(const String &remoteHost,const u16 port);
	virtual ~Remote();

	//循环处理，事件分发
	void Handle();


	//发送Uid信息
	void SendUid (const u32 &uid);

	//发送按键开锁消息（直接在下位机就处理按键事件，顺便发送到服务器一条消息）
	void SendButtonMsg();

	//设置远程开锁的响应回调函数
	void SetRemoteUnlockCallback(RemoteUnLockCallback callback);

	//设置卡信息不合法时的回调
	void SetValidFailedCallback(ValidFailedCallback failedCallback);

	//设置其他UDP指令的回调函数
	void SetOtherCallback(OtherCallback otherCallback);
private:

	//服务器主机名
	String remoteHost;

	//服务器ip（在构造函数中初始化）
	IPAddress remoteIp;

	//服务器的端口号
	u16 port;

	//远程开锁的回调函数
	RemoteUnLockCallback unlockCallback;

	//远程验证失败的回调函数
    ValidFailedCallback failedCallback;

    //其他远程指令的回调函数
    OtherCallback otherCallback;

	//udp对象
	WiFiUDP udp;

	//udp接收数据报文的缓冲区
	uint8_t buffer[255];

	//发送udp数据报文
	void Send(const u8 *pkg,int size);

	//当接收udp数据报文时调用
	void Received(const int packetSize);
};

#endif /* REMOTE_H_ */
