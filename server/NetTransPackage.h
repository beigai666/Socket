#pragma once
#include"stdafx.h"
#include <vector>


typedef struct NetTransPackage {
	uint8_t Len[2]; /*传输包大小，不包括头部4字节*/
	uint8_t CRC[2];/*传输包CRC16bety*/
	uint8_t Data[0];/*数据区*/
};


#define GET_UINT16_BE(n,b)                              \
{                                                       \
    n = (unsigned char)(b)[0] << 8  | (unsigned char)(b)[1];        \
}           

#define PUT_UINT16_BE(n,b)                              \
{                                                       \
    (b)[0]=(unsigned char) ((n)>>8);                    \
    (b)[1] = (unsigned char)(n);                        \
}

#define MOV_UINT16_BE(in,out)                                           \
{                                                                       \
    ((unsigned char*)(out))[0]=((unsigned char*)(in))[0];                \
    ((unsigned char*)(out))[1] = ((unsigned char*)(in))[1];              \
}

#define CMP_UINT16(n,b)  (((unsigned char)(b)[0] == (unsigned char)((n) >> 8))&&((unsigned char)(b)[1] == (unsigned char)(n))) 


NetTransPackage* createNetTransPackage(uint16_t leng, const uint8_t* data);



int verification(NetTransPackage* pack);

void deletePackage(NetTransPackage*& package);

NetTransPackage* getPackage(NetTransPackage& temp);