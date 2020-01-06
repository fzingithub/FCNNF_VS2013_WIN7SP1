#include "content.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned long MAX_LEN = 1024;

static char PathBuffer[MAX_LEN];

static bool getFilePath(){
	memset(PathBuffer, 0, MAX_LEN);
	if (GetTempPathA(MAX_LEN - 24, PathBuffer) > MAX_LEN - 24)
		return false;
	return true;
}

FILE* getMFile(int choice){
	if (choice < 1 || choice > 3)
		return nullptr;

	if (!getFilePath())
		return nullptr;
	if (choice == 1)
		strcat(PathBuffer, "mc_need_1.m");
	else if (choice == 2)
		strcat(PathBuffer, "mc_need_2.m");
	else
		strcat(PathBuffer, "mc_need_3.m");
	
	FILE *mf = fopen(PathBuffer,"w+");
	if (choice == 1)
		fwrite(simulate,strlen(simulate),1,mf);
	else if (choice == 2)
		fwrite(model, strlen(model), 1, mf);
	else
		fwrite(async, strlen(async), 1, mf);
	
	fseek(mf,0L, 0);
	return mf;
}

char *simulate =
"/*��ÿ����֧Ҳ����ÿ��if...else���ǰ����changeState()���������current����len˵�������µĽڵ㣬���ӽڵ㣬���current��1������������һ����֧*/\n"
"function changeState()\n"
"{\n"
"	if(current=length) then\n"
"	{\n"
"		length:=length+1;\n"
"		flag[length-1]:=0\n"
"	}\n"
"	else\n"
"	{\n"
"		skip\n"
"	};\n"
"	current:=current+1\n"
"};\n"
"\n"
"pathNum<==1 /*and length <== 0 and current <== 0 and firstTime<==0 and flag1<==0 and flag2<==0*/ and skip;\n"
"while(flag2=0)\n"
"{\n"
"	flag1:=0;\n"
"	while(length>0 AND flag1=0)\n"
"	{\n"
"		if(flag[length-1]=1) then//ɾ���Ѿ����������ߵĽڵ�\n"
"		{\n"
"			length:=length-1\n"
"		}\n"
"		else //�������һ�����֧����һ����֧\n"
"		{\n"
"			flag[length-1]:=1;\n"
"			flag1:=1\n"
"		}\n"
"	};\n"
"	if((length=0 AND flag[0]=1) OR (current=0 AND firstTime=1)) then\n"
"	{\n"
"		flag2:=1\n"
"	}\n"
"	else\n"
"	{\n"
"		skip\n"
"	}\n"
"	//����������������\n"
"}";

char *model = 
"/*��ÿ����֧Ҳ����ÿ��if...else���ǰ����changeState()���������current����len˵�������µĽڵ㣬���ӽڵ㣬���current��1������������һ����֧*/\n"
"function changeState()\n"
"{\n"
"	if(current=length) then\n"
"	{\n"
"		length:=length+1;\n"
"		flag[length-1]:=0;\n"
"		SaveCurrentNode()\n"
"	}\n"
"	else\n"
"	{\n"
"		skip\n"
"	};\n"
"\n"
"	if(flag[current]=1)then\n"
"	{\n"
"		UpdateCurrentNode() and skip\n"
"	};\n"
"	current:=current+1\n"
"};\n"
"\n"
"pathNum<==1 /*and length <== 0 and current <== 0 and firstTime<==0 and flag1<==0 and flag2<==0*/ and skip;\n"
"while(flag2=0)\n"
"{\n"
"	flag1:=0;\n"
"	while(length>0 AND flag1=0)\n"
"	{\n"
"		if(flag[length-1]=1) then//ɾ���Ѿ����������ߵĽڵ�\n"
"		{\n"
"			length:=length-1\n"
"		}\n"
"		else //�������һ�����֧����һ����֧\n"
"		{\n"
"			flag[length-1]:=1;\n"
"			flag1:=1\n"
"		}\n"
"	};\n"
"	if((length=0 AND flag[0]=1) OR (current=0 AND firstTime=1)) then\n"
"	{\n"
"		flag2:=1\n"
"	}\n"
"	else\n"
"	{\n"
"		skip\n"
"	}\n"
"	//����������������\n"
"}";

char *async =
"//Ԫ��0��value��aux�ֱ������󳤶Ⱥ͵�ǰ����\n"
"struct Chn {\n"
"int value and\n"
"int aux\n"
"};\n"
"\n"
"int $$x and skip;\n"
"\n"
"function SendInt(Chn *channel,int x)\n"
"{\n"
"	await(channel[0].value>channel[0].aux+1);\n"
"	channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=x\n"
"	\n"
"};\n"
"\n"
"//ͨ������λ����,����ֵΪ��Ԫ��\n"
"function $$myShift(Chn *channel, int RValue)\n"
"{\n"
"frame(i,x,y) and\n"
"(\n"
"	int i,x and skip;\n"
"	x:=channel[1].value;\n"
"	i:=1;\n"
"	while(i<=channel[0].aux)\n"
"	{\n"
"		channel[i].value:=channel[i+1].value;\n"
"		i:=i+1\n"
"	};\n"
"	channel[channel[0].aux+1].value:=0;\n"
"	RValue:=x\n"
")\n"
"};\n"
"\n"
"function ReceiveInt(Chn *channel,int *y)\n"
"{\n"
"	await(channel[0].aux!=0);\n"
"	channel[0].aux:=channel[0].aux-1 and  *y:= (extern  $$myShift(channel))\n"
"	\n"
"};\n"
"\n"
"function PutInt(Chn *channel,int x)\n"
"{\n"
"   if(channel[0].value>channel[0].aux+1) then \n"
"	{\n"
"		channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=x\n"
"	}else\n"
"	{skip}\n"
"};\n"
"\n"
"function GetInt(Chn *channel,int *y)\n"
"{\n"
"	if(channel[0].aux!=0) then\n"
"	{\n"
"		channel[0].aux:=channel[0].aux-1 and  *y:= (extern  $$myShift(channel))\n"
"	}else\n"
"	{skip}\n"
"};\n"
"\n"
"function SendChar(Chn *channel,char x)\n"
"{\n"
"	await(channel[0].value>channel[0].aux+1);\n"
"	channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x\n"
"	\n"
"};\n"
"\n"
"function ReceiveChar(Chn *channel,char *y)\n"
"{\n"
"	await(channel[0].aux!=0);\n"
"	channel[0].aux:=channel[0].aux-1 and  *y:= (char)(extern  $$myShift(channel))\n"
"	\n"
"};\n"
"\n"
"function PutChar(Chn *channel,char x)\n"
"{\n"
"	if(channel[0].value>channel[0].aux+1) then\n"
"	{\n"
"		channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x\n"
"	}else\n"
"	{skip}\n"
"};\n"
"\n"
"function GetChar(Chn *channel,char *y)\n"
"{\n"
"	if(channel[0].aux!=0) then\n"
"	{\n"
"		channel[0].aux:=channel[0].aux-1 and  *y:= (char)(extern  $$myShift(channel))\n"
"	}else\n"
"	{skip}\n"
"};\n"
"\n"
"function SendFloat(Chn *channel,float x)\n"
"{\n"
"	await(channel[0].value>channel[0].aux+1);\n"
"	channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x\n"
"	\n"
"};\n"
"\n"
"function ReceiveFloat(Chn *channel,float *y)\n"
"{\n"
"	await(channel[0].aux!=0);\n"
"	channel[0].aux:=channel[0].aux-1 and  *y:= (float)(extern  $$myShift(channel))\n"
"	\n"
"};\n"
"\n"
"function PutFloat(Chn *channel,float x)\n"
"{\n"
"	if(channel[0].value>channel[0].aux+1) then\n"
"	{\n"
"		channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x\n"
"	}else\n"
"	{skip}\n"
"};\n"
"\n"
"function GetFloat(Chn *channel,float *y)\n"
"{\n"
"	if(channel[0].aux!=0) then\n"
"	{\n"
"		channel[0].aux:=channel[0].aux-1 and  *y:= (float)(extern  $$myShift(channel))\n"
"	}else\n"
"	{skip}\n"
"};\n"
"\n"
"function $$Mymemcpy(char *dst,char *src)\n"
"{\n"
"while(*src!=0)\n"
"{\n"
"	*dst := *src;\n"
"	src:=src+1 and dst:=dst+1\n"
"};\n"
"*dst := 0\n"
"};\n"
"\n"
"function SendStr(Chn *channel,char *x)\n"
"{\n"
"	await(channel[0].value>channel[0].aux+1);\n"
"	channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x\n"
"	\n"
"};\n"
"\n"
"function ReceiveStr(Chn *channel,char *y)\n"
"{\n"
"	await(channel[0].aux!=0);\n"
"	channel[0].aux:=channel[0].aux-1 and  extern $$Mymemcpy(y, (char*)(extern  $$myShift(channel)) )\n"
"	\n"
"};\n"
"\n"
"function PutStr(Chn *channel,char *x)\n"
"{\n"
"	if(channel[0].value>channel[0].aux+1) then\n"
"	{\n"
"		channel[0].aux:=channel[0].aux+1 and channel[channel[0].aux].value:=(int)x\n"
"	}else\n"
"	{skip}\n"
"};\n"
"\n"
"function GetStr(Chn *channel,char *y)\n"
"{\n"
"	if(channel[0].aux!=0) then\n"
"	{\n"
"		channel[0].aux:=channel[0].aux-1 and  extern $$Mymemcpy(y, (char*)(extern  $$myShift(channel)) )\n"
"	}else\n"
"	{skip}\n"
"}";