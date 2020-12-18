#include <ArduinoOTA.h>
#include <Ticker.h>
#include <Esp.h>

#include "CardReader.h"
#include "Lock.h"
#include "Button.h"
#include "Remote.h"
#include "Utils.h"
#include "Beep.h"

//舵机引脚
#define SERVO_PIN D0

//RC522芯片引脚,SPI通信引脚直接使用硬件SPI
#define RC522_RST_PIN D1
#define RC522_SDA_PIN D2
//#define RC522_SCK_PIN D5
//#define RC522_MOSI_PIN D7
//#define RC522_MISO_PIN D6

//按键开锁引脚
#define BUTTON_PIN D3

//峰鸣器引脚
#define BEEP_PIN D8

//定义读卡器
CardReader reader(RC522_SDA_PIN, RC522_RST_PIN);

//定义锁
Lock lock(SERVO_PIN);

//定义按键
Button button(BUTTON_PIN);

//定义峰鸣器
Beep beep(BEEP_PIN);

//定义WiFi账户与密码
const char *ssid = "Robot2";
const char *password = "fightforfuture";

//定义远程服务器地址与端口
Remote remote("10.1.160.247",10292);

//建立
void onSensedCard(const u32 uid){
    Serial.println("检测到卡片：");
    beep.playSensedCard();
    Serial.printf("发送uid数据包 %u 到服务器...\n",uid);
    remote.SendUid(uid);
    reader.getMFRC522()->PCD_Init();

}

void onKeyDown(){
    Serial.println("按键请求开锁");
    remote.SendButtonMsg();
    lock.Open();
    //reader.getMFRC522()->PCD_Init();
}

void onKeyUp(){
    lock.Close();
}

void onRemoteUnlockRequest(uint32_t uid){
    Serial.printf("%d 请求开锁 \n",uid);
    lock.Unlock();
    beep.playSuccessful();
    reader.getMFRC522()->PCD_Init();
}

//当卡信息不合法
void onCardNotValid(){
    Serial.printf("不合法的卡 \n");
    beep.playFailed();
    reader.getMFRC522()->PCD_Init();
}

//音乐播放的缓冲区(默认为255个音符)
#include "Queue.h"
Queue<u32> musicQueue(255);

//设置其他指令的UDP远程回调
void onOtherReceived(int size,byte *buffer){
    if(size == 5 && buffer[0] == 0x04){
        //0x04作包头时，两个字节声音频率+两个字节的声音长度
        u16 hz = _2u8_to_u16(&buffer[1]);

        u16 ms = _2u8_to_u16(&buffer[3]);

        beep.beep(hz,ms);
    }
    if(buffer[0] == 0x05){
        //0x05为清空音符缓冲区队列
        musicQueue.clear();
    }
    if(buffer[0] == 0x06){
        //0x06作包头时，下一个字节为音的数量n，向后读取n个32位整数，并入队列
        byte n = buffer[1];
        for(int i=0;i<n;i++){
            byte bs[4] = {
                    buffer[2+4*i],
                    buffer[3+4*i],
                    buffer[4+4*i],
                    buffer[5+4*i]
            };
            u32 mInfo = _4u8_to_u32(bs);    //一个32位音符信息
            musicQueue.enQueue(mInfo);
        }
    }
    if(buffer[0] == 0x07){
        //0x07作包头时，读取缓冲区下所有的音符并播放
        u32 mInfo;
        while(!musicQueue.isEmpty()){
            musicQueue.deQueue(mInfo);
            u16 hz = (mInfo >> 16) & 0xffff;
            u16 ms = mInfo & 0xffff;
            beep.beep(hz,ms);
            delay(ms);
        }
    }
    //0x08时，初始化一次芯片
    if(buffer[0] == 0x08){
        reader.getMFRC522()->PCD_Init();
    }
}

void setup() {
    //初始化串口调试
    Serial.begin(115200);
    Serial.println("\n 开机！！！");

    //注册读卡器事件监听函数
    reader.SetOnSensedCardListener(onSensedCard);

    //注册按键松手的事件监听函数
    button.SetOnKeyUpCallback(onKeyUp);

    //注册按键按下事件回调函数
    button.SetOnKeyDownCallback(onKeyDown);

    //注册远程开锁的回调监听函数
    remote.SetRemoteUnlockCallback(onRemoteUnlockRequest);

    //注册当卡不合法时的回调函数
    remote.SetValidFailedCallback(onCardNotValid);

    //设置其他的UDP指令回调
    remote.SetOtherCallback(onOtherReceived);

    //链接至WiFi
    connectWiFi(ssid, password);

    //启动OTA升级服务
    ArduinoOTA.begin();
    ArduinoOTA.setHostname("SITRobotLock");
    ArduinoOTA.setPassword("123456");

    //启用看门狗
    ESP.wdtEnable(3000);

    //ticker.attach(10,[](){
    //    reader.getMFRC522()->PCD_Init();
    //    beep.playSuccessful();
    //});
}

int count = 0;

void loop() {
    //Serial.println("开始消息循环！");
    count++;
    if(count > 150){
        count = 0;
        reader.getMFRC522()->PCD_Init();
    }
    reader.Handle();
    button.Handle();
    remote.Handle();
    ArduinoOTA.handle();
    //喂狗
    ESP.wdtFeed();
}