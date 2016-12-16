#include "MsgCode.h"
#include "MyDataProcess.h"


//InitShake message is sent from uav to gs
int encode_InitShake(GPS &Tmp, unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

	a[length] = 0x00;  length++;
	a[length] = 0x00; length++;								//Ground Station ID

	a[length] = msgID_InitShake; length++;		//message ID
	//////数据拆分解码////////////
	EncodeDoubleData(&Tmp.latitude, a+length);length+=8;
	EncodeDoubleData(&Tmp.longitude, a+length);length+=8;
	EncodeDoubleData(&Tmp.altitude, a+length);length+=8;
	////计算校验和//////
	a[length]=CountSum(a,length);length++;

	/**补上data length**/
	a[1] = (unsigned char)length-4;

	return length;
}

//LocalFrame message is sent from gs to uav
//send local frame to specific uav.if uavID is 0xff,that means broadcasting local frame to all uav.
//In general, we use 0xff when configurate
int encode_LocalFrame(GPS &Tmp,unsigned char uavID,unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

	a[length] = 0x00;  length++;
	a[length] = uavID; length++;

	a[length] = msgID_LocalFrame; length++;		//message ID
	//////数据拆分解码////////////
	EncodeDoubleData(&Tmp.latitude, a+length);length+=8;
	EncodeDoubleData(&Tmp.longitude, a+length);length+=8;
	EncodeDoubleData(&Tmp.altitude, a+length);length+=8;
	////计算校验和//////
	a[length]=CountSum(a,length);length++;

	/**补上data length**/
	a[1] = (unsigned char)length-4;

	return length;
}

//ShapeConfig message is sent from gs/UAV to uav
//send ShapeConfig to specific uav.if uavID is 0xff,that means broadcasting ShapeConfig to all uav.
//In general, we use 0xff when configurate
int encode_ShapeConfig(ShapeConfig tmp,unsigned char uavID,unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

	a[length] = 0x00;  length++;
	a[length] =  uavID; length++;

	a[length] = msgID_ShapeConfig; length++;		//message ID
	a[length] = tmp.i;  length++;   a[length] = tmp.j;   length++;
	EncodeFloatData(&tmp.x, a+length);length+=4;
	EncodeFloatData(&tmp.y, a+length);length+=4;
	EncodeFloatData(&tmp.z, a+length);length+=4;
	EncodeFloatData(&tmp.fi, a+length);length+=4;
	a[length] = tmp.totol_uav_num;  length++;

	a[length]=CountSum(a,length);length++;
	/**补上data length**/
	a[1] = (unsigned char)length-4;

	return length;
}

int encode_CmdAck(unsigned char msgID,unsigned char targetID,unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

	if(targetID!=0xff)
	{
		a[length] = 0x00;  length++;
		a[length] = targetID; length++;
	}
	else
	{
		a[length] = 0xff;  length++;
		a[length] = 0xff; length++;
	}

	a[length] = msgID; length++;		//message ID
	//////数据////////////
  a[length] = 'a';length++;
  a[length] = 'c';length++;
  a[length] = 'k';length++;
	////计算校验和//////
	a[length]=CountSum(a,length);length++;

	/**补上data length**/
	a[1] = (unsigned char)length-4;

	return length;
}

void decode_InitShake(GPS &Tmp,unsigned char *a)
{
  int length = 5;
	u16 temp;

  DecodeDoubleData(&Tmp.latitude,a+length);length+=8;
  DecodeDoubleData(&Tmp.longitude,a+length);length+=8;
  DecodeDoubleData(&Tmp.altitude,a+length);length+=8;
  DecodeU16Data(&temp,a+length);
	Tmp.ID = temp;
}


void decode_LocalFrame(GPS &Tmp,unsigned char *a)
{
  int length = 5;

  DecodeDoubleData(&Tmp.latitude, a+length);length+=8;
  DecodeDoubleData(&Tmp.longitude, a+length);length+=8;
  DecodeDoubleData(&Tmp.altitude, a+length);length+=8;

}

void decode_ShapeConfig(ShapeConfig tmp,unsigned char *a)
{
	int length = 5;
	tmp.i = a[length];length++;
	tmp.j = a[length];length++;

	DecodeFloatData(&tmp.x,a+length);length+=4;
	DecodeFloatData(&tmp.y,a+length);length+=4;
	DecodeFloatData(&tmp.z,a+length);length+=4;
	DecodeFloatData(&tmp.fi,a+length);length+=4;

	tmp.totol_uav_num = a[length];length++;
}

void decode_TakeOff(Posi tmp,unsigned char *a)
{
	int length = 5;

	DecodeFloatData(&tmp.x,a+length);length+=4;
	DecodeFloatData(&tmp.y,a+length);length+=4;
	DecodeFloatData(&tmp.z,a+length);length+=4;
	DecodeFloatData(&tmp.fi,a+length);length+=4;
}

unsigned char CountSum(unsigned char *a,int length)
{
	unsigned char sum = 0x00;
	for(int i=0;i<length;i++)
	{
		sum+=a[i];
	}
	return sum;
}
