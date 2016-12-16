#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#include <cstring>

typedef struct SendSession
{
  char UpdateFlag;
  unsigned char msg[250];
}SendSession;


typedef struct SDKFilter
{
  int recvIndex;
  unsigned char recvBuf[1024];
}SDKFilter;

typedef struct Header
{
  unsigned char head1;      //0xFE
  unsigned char head2;      //'#'
  unsigned char length;     //数据包总长度
  unsigned char systemID;

  unsigned char msgID;
  unsigned char reserve1;
  unsigned char reserve2;
  unsigned char reserve3;
}Header;




#endif
