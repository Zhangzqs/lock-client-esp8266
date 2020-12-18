/*
 * Utils.h
 *
 *  Created on: 2020年10月15日
 *      Author: zhiqiang
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <Arduino.h>

/*连接至WiFi*/
void connectWiFi(const char *ssid,const char *password);

/*32位整数分解为4个字节*/
void u32_to_4u8(u32 uid,u8 *bytes);

/*4个字节合并为32位整数*/
u32 _4u8_to_u32(u8 *bytes);

/*2个字节合并为16位整数*/
u16 _2u8_to_u16(u8 *bytes);

#endif /* UTILS_H_ */
