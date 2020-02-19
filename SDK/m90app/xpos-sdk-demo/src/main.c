#include "libapi_xpos/inc/libapi_system.h"
#include "sdk_xgui.h"
#include "libapi_xpos/inc/libapi_emv.h"


#define APP_VER "DEMO-V1.1.5"

static void add_capkByAid(unsigned char* cAID,unsigned char Index)
{
	CAPUBLICKEY ppkKey;
	char *szBuf = 0;
	char szHash[20]={0};

	int nLen = 0;
	int npacklen = 0;
	nLen = sizeof(CAPUBLICKEY)+30;
	szBuf = (char *)malloc(nLen);
	if(0 == szBuf) {
		return ;
	}
	memset(&ppkKey,0,sizeof(CAPUBLICKEY));
	memcpy(ppkKey.RID,cAID,5);
	ppkKey.CA_PKIndex=Index;
	memcpy(ppkKey.CAPKModulus,
		"\x81\xCC\xF2\xD6\xE5\xCD\x28\xE4\xE1\x21\x05\xB5\x05\xAA\x16\x1D\x98\x30\xDE\xFE\x2A"
		"\xBD\x8F\xFF\xA5\x00\x83\x9E\x34\x52\x76\xCD\x3C\xCD\xF6\x1B\x0F\xDE\x18\xAE\x48\xE1"
		"\xEA\x1A\x5C\xD7\xDA\x7A\x11\x9B\xEF\xAE\x31\x6C\x1F\x91\xD7\x4B\xB7\x7C\xD5\xC4\xE2"
		"\xEB\x91\xC3\xC3\x56\x05\x7D\x78\x56\x1D\x1C\x66\x13\x13\xD9\x54\x08\x37\xCC\xDF\x93"
		"\x69\xC1\x8E\x41\x7E\x96\x4C\x26\x8B\x7F\xE6\x0A\x38\x74\x64\xC3\x1A\x11\x35\x8F\x30"
		"\x3C\x18\xFB\x7C\x18\x2B\xB3\xBD\x04\x14\x8E\x09\x73\xA9\xFA\x8A\x12\x8D\xA7\xB8\xF4"
		"\xE4\x75\xC2\x9A\x5C\xC5\xF2\xA2\x89\x11\x4F\xE7\xA3\xB3\x4E\x1F\xEC\xDA\xBC\x8F\x85"
		"\x24\xA9\xC2\x23\x0C\x77\x80\x38\xB9\x16\x10\x6F\xF9\x1E\xB7\x7D\xBB\xF5\xAC\x97\x3F"
		"\x3F\x2A\x35\x07\x59\x0F\x5B\xF7\x7C\xF9\x4F\x39\xAF\x6F\x9D\x97\x1B\x92\x07\x51\x6A"
		"\x08\xF1\x09\xB1\x6D\xF1\xD1\xB4\xE6\x73\x90\x5E\xBC\x7B\x78\x56\x19\x02\xB2\xC4\xC3"
		"\x9C\xA8\x64\xF4\xF4\x22\xFA\xE9\xFE\x59\xCB\x11\x2F\x82\xFF\xAB\xBC\x9A\xCC\xB2\x46"
		"\xEC\x46\xF0\x02\x0B\xED\xBF\x98\xEE\x76\x8C\x20\x6A\x0F\x13\xB5\xB3",248);
	memcpy(ppkKey.CAPKExponent,"\x00\x00\x03",3);
	memcpy(ppkKey.CAPKExpDate,"\x20\x20\x12\x31",4);
	ppkKey.LengthOfCAPKModulus=0xF8;
	ppkKey.LengthOfCAPKExponent=3;

	memcpy(szBuf, cAID, 5);
	szBuf[5]=Index;
	//memcpy(szBuf+5, (char *)stCAPK->cCAPKIndex_b_9F22, 1);
	memcpy(szBuf+6, (char *)ppkKey.CAPKModulus,ppkKey.LengthOfCAPKModulus);
	nLen = ppkKey.LengthOfCAPKModulus + 6;
	if(ppkKey.CAPKExponent[2]==0x03) {
		memcpy(szBuf+nLen, (char *)&(ppkKey.CAPKExponent[2]), 1);
		nLen += 1;
	}
	else {
		memcpy(szBuf+nLen, (char *)ppkKey.CAPKExponent, 3);
		nLen += 3;
	}
	memset(szHash, 0, sizeof(szHash));
	mf_sha1((unsigned char *)szBuf, nLen, (unsigned char *)szHash);
	memcpy(ppkKey.ChecksumHash,szHash,20);
	EMV_PrmSetCAPK(&ppkKey);
	free(szBuf);
}

static void add_capkByAid_sm(unsigned char* cAID,unsigned char Index)
{
	CAPUBLICKEY ppkKey;
	char *szBuf = 0;
	char szHash[20]={0};

	int nLen = 0;
	int npacklen = 0;
	nLen = sizeof(CAPUBLICKEY)+30;
	szBuf = (char *)malloc(nLen);
	if(0 == szBuf) {
		return ;
	}
	memset(&ppkKey,0,sizeof(CAPUBLICKEY));
	memcpy(ppkKey.RID,cAID,5);
	ppkKey.CA_PKIndex=Index;
	memcpy(ppkKey.CAPKModulus,
		"\x37\x26\x03\x0D\x0D\x74\xA5\x99\x8A\x12\xFA\x6B\x4B\x5C\x69\x96\xA8\x6F\x48\x57\xDD\x88\x05\x37\x84\xE0\xDD\x44\x70"
		"\xD5\x61\x6C\x7E\x22\xAF\x76\x60\x13\xDA\xD2\xED\xB7\xE8\x9C\x26\x42\x8F\xC8\x1E\xF0\x50\xFA\x2B\xCA\xA3\xD6\x31\x26\xDF\x41\x9D\xFA\x9E\x0A",64);
	memcpy(ppkKey.CAPKExponent,"\x00\x00\x03",3);
	memcpy(ppkKey.CAPKExpDate,"\x20\x20\x12\x31",4);
	ppkKey.LengthOfCAPKModulus=0x40;
	ppkKey.LengthOfCAPKExponent=3;

	memcpy(szBuf, cAID, 5);
	szBuf[5]=Index;
	//memcpy(szBuf+5, (char *)stCAPK->cCAPKIndex_b_9F22, 1);
	memcpy(szBuf+6, (char *)ppkKey.CAPKModulus,ppkKey.LengthOfCAPKModulus);
	nLen = ppkKey.LengthOfCAPKModulus + 6;
	if(ppkKey.CAPKExponent[2]==0x03) {
		memcpy(szBuf+nLen, (char *)&(ppkKey.CAPKExponent[2]), 1);
		nLen += 1;
	}
	else {
		memcpy(szBuf+nLen, (char *)ppkKey.CAPKExponent, 3);
		nLen += 3;
	}
	memset(szHash, 0, sizeof(szHash));
	mf_sha1((unsigned char *)szBuf, nLen, (unsigned char *)szHash);
	memcpy(ppkKey.ChecksumHash,szHash,20);
	EMV_PrmSetCAPK(&ppkKey);
	free(szBuf);

}
static void add_capk()
{
	add_capkByAid("\xA0\x00\x00\x00\x04",0x7A); //mastercard
	add_capkByAid("\xA0\x00\x00\x03\x24",0x7A); //dpas
	add_capkByAid("\xA0\x00\x00\x00\x03",0x7A); //visa
	add_capkByAid("\xA0\x00\x00\x01\x52",0x7A); //dpas
	add_capkByAid("\xA0\x00\x00\x00\x65",0x7A); //jcb
	add_capkByAid("\xA0\x00\x00\x00\x25",0x7A);//amex
	add_capkByAid("\xA0\x00\x00\x00\x29",0x7A);//amex
	add_capkByAid("\xA0\x00\x00\x03\x33",0x7A);//quics
	add_capkByAid_sm("\xA0\x00\x00\x03\x33",0x7B);//quics_sm
}
void app_main()
{
	xgui_default_msg_func_add((void*)sdk_power_proc_page);	// Default message processing
	Sys_Init(0,0,"test");// Application initialization
	Sys_SetAppVer(APP_VER);
	EMV_iKernelInit();//Init EMV
	//add_capk();
	sdk_main_page(); 
}
#ifndef WIN32
void main(){
	app_main();
}
#endif

