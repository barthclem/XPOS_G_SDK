#include "upay_consum.h"
#include "upay_print.h"
#include "emvapi/inc/emv_api.h"
#include "libapi_xpos/inc/def.h"
#include "libapi_xpos/inc/libapi_emv.h"
#include "EntryPoint/lib_emvpub/inc/emv_interface.h"


#define APP_TRACE	printf
#define APP_TRACE_BUFF_LOG(...) 
//#define TEST_MAGTEK

static int first = 0;


int upay_consum( void );

int tt_upay_consum(void)
{	
	int nChoice;
	upay_consum();
	return 0;
}

static void _pack_8583()
{
	return;
}
//#define COUNTRYCODE "\x01\x56"//CNY
#define COUNTRYCODE "\x03\x56"//INR
//#define COUNTRYCODE "\x09\x78"//EUR
//#define COUNTRYCODE "\x05\x66"//NGN
//#define COUNTRYCODE "\x08\x40"//USD

void TestSetTermConfig(TERMCONFIG *termconfig)
{
	//char szBuf[100] = {0};
	if(termconfig==0)
		return;
	APP_TRACE( "TestSetTermConfig" );

	memset(termconfig,0x00,sizeof(TERMCONFIG));	
	memcpy( termconfig->TermCap, "\xE0\xF8\xC8", 3);	/*Terminal performance '9F33'*/
	memcpy( termconfig->AdditionalTermCap,"\xFF\x80\xF0\x00\x01", 5);/*Terminal additional performance*/	
	memcpy( termconfig->IFDSerialNum,"morefun1",8);		/*IFD serial number '9F1E'*/ 
	memcpy(termconfig->TermCountryCode, COUNTRYCODE, 2);	/*Terminal country code '9F1A'*/
	memcpy( termconfig->TermID, "12312312", 8);			/*Terminal identification '9F1C'*/
	termconfig->TermType = 0x22;						/*Terminal type '9F35'*/
	memcpy( termconfig->TransCurrCode,COUNTRYCODE, 2);	/*Transaction currency '5F2A'*/
	termconfig->TransCurrExp = 0x02;					/*Transaction currency index '5F36'*/

	termconfig->bPSE = YES;					/*Whether to support the choice PSE 1*/
	termconfig->bCardHolderConfirm = YES;	/*Whether to support cardholder confirmation 1*/
	termconfig->bPreferedOrder = YES;		/*Whether to support the preferred display 1*/
	termconfig->bPartialAID = YES;			/*Whether to support partial AID matching 1*/
	termconfig->bMultiLanguage  = YES;		/*Whether to support multiple languages 0*/
	termconfig->bCommonCharset = YES;		/*Whether to support public character sets 0*/	
	termconfig->bIPKCValidtionCheck = YES;	/*Whether to check the validity of the issuer's public key authentication 1*/
	termconfig->bContainDefaultDDOL = YES;	/*Whether to include the defaultDDOL 1*/
	termconfig->bCAPKFailOperAction = NO;	/*CAPK Is operator intervention required when reading an error? 1*/
	termconfig->bCAPKChecksum = YES;		/*Whether to perform CAPK check 1*/
	/**<Cardholder Verification Method*/
	termconfig->bBypassPIN = YES;			/*Whether to support skipping PIN input (requires change process, to be determined)*/
	termconfig->bGetDataForPINCounter =YES;	/*PIN Try to check if the counter is supportedGetData 1*/
	termconfig->bFailCVM = YES;				/*Whether to support the wrong CVM (must be Yes)*/
	termconfig->bAmountBeforeCVM = YES;		/*CVM Whether the amount is known before 1*/
	/**<Term Risk Management*/
	termconfig->bLimitFloorCheck = YES;		/*Whether to perform a minimum check 1*/
	termconfig->bRandomTransSelect = YES;	/*Whether to make random trading choices 1*/
	termconfig->bValocityCheck = YES;		/*Whether to check the frequency 1*/
	termconfig->bTransLog = YES;			/* Whether to record the transaction log 1*/
	termconfig->bExceptionFile = YES;		/*Whether to support card blacklist 1*/
	termconfig->bTRMBaseOnAIP = NO;			/*Whether terminal risk management is based on application interaction characteristics 1*/
	/**<Terminal Action Analysis*/
	termconfig->bTerminalActionCodes = YES;	/*Whether to support terminal behavior code 1*/
	termconfig->bDefActCodesBefore1stGenAC = NO;/*Is the default behavior code prior to FirstGenerateAC ?*/
	termconfig->bDefActCodesAfter1stGenAC = NO;/*Is the default behavior code after FirstGenerateAC ?*/
	/**<Completion Processing*/
	termconfig->bForceOnline= NO;			/*Whether to allow forced online 1*/
	termconfig->bForceAccept = NO;			/*Whether to allow forced acceptance of transactions 1*/
	termconfig->bAdvices = YES;				/*Whether to support notification 0*/
	termconfig->bIISVoiceReferal = YES;		/*Whether to support the voice reference initiated by the card issuer ?*/
	termconfig->bBatchDataCapture = YES;	/*Whether to support batch data collection*/
	termconfig->bDefaultTDOL = YES;			/*Is there a default? TDOL*/
	termconfig->bAccountSelect = YES;		/*Whether to support account selection*/
}


void TestDownloadAID(TERMINALAPPLIST *TerminalApps)
{
	int i = 0;
	int count = UMAX_TERMINAL_APPL;

	if(TerminalApps==0)
		return;

	APP_TRACE( "TestDownloadAID" );
	memset(TerminalApps,0x00,sizeof(TERMINALAPPLIST));	
	TerminalApps->bTermAppCount = UMAX_TERMINAL_APPL;//The maximum number of AID
	memcpy(TerminalApps->TermApp[0].AID, "\xA0\x00\x00\x00\x01\x10\x10", 7);//AID
	TerminalApps->TermApp[0].AID_Length = 7;
	memcpy(TerminalApps->TermApp[1].AID, "\xA0\x00\x00\x00\x03\x10\x10", 7);
	TerminalApps->TermApp[1].AID_Length = 7;
	memcpy(TerminalApps->TermApp[2].AID, "\xA0\x00\x00\x00\x03\x20\x10", 7);
	TerminalApps->TermApp[2].AID_Length = 7;
	memcpy(TerminalApps->TermApp[3].AID, "\xA0\x00\x00\x00\x03\x20\x20", 7);
	TerminalApps->TermApp[3].AID_Length = 7;
	memcpy(TerminalApps->TermApp[4].AID, "\xA0\x00\x00\x00\x03\x80\x10", 7);
	TerminalApps->TermApp[4].AID_Length = 7;
	memcpy(TerminalApps->TermApp[5].AID, "\xA0\x00\x00\x00\x04\x10\x10", 7);
	TerminalApps->TermApp[5].AID_Length = 7;
	memcpy(TerminalApps->TermApp[6].AID, "\xA0\x00\x00\x00\x04\x99\x99", 7);
	TerminalApps->TermApp[6].AID_Length = 7;
	memcpy(TerminalApps->TermApp[7].AID, "\xA0\x00\x00\x00\x04\x30\x60", 7);
	TerminalApps->TermApp[7].AID_Length = 7;
	memcpy(TerminalApps->TermApp[8].AID, "\xA0\x00\x00\x00\x04\x60\x00", 7);
	TerminalApps->TermApp[8].AID_Length = 7;
	memcpy(TerminalApps->TermApp[9].AID, "\xA0\x00\x00\x00\x05\x00\x01", 7);
	TerminalApps->TermApp[9].AID_Length = 7;
	memcpy(TerminalApps->TermApp[10].AID, "\xA0\x00\x00\x00\x25\x01", 6);
	TerminalApps->TermApp[10].AID_Length = 6;
	memcpy(TerminalApps->TermApp[11].AID, "\xA0\x00\x00\x00\x29\x10\x10", 7);
	TerminalApps->TermApp[11].AID_Length = 7;
	memcpy(TerminalApps->TermApp[12].AID, "\xA0\x00\x00\x00\x42\x10\x10", 7);
	TerminalApps->TermApp[12].AID_Length = 7;
	memcpy(TerminalApps->TermApp[13].AID, "\xA0\x00\x00\x00\x42\x20\x10", 7);
	TerminalApps->TermApp[13].AID_Length = 7;
	memcpy(TerminalApps->TermApp[14].AID, "\xA0\x00\x00\x00\x65\x10\x10",7);
	TerminalApps->TermApp[14].AID_Length = 7;
	memcpy(TerminalApps->TermApp[15].AID, "\xA0\x00\x00\x01\x21\x10\x10", 7);
	TerminalApps->TermApp[15].AID_Length = 7;
	memcpy(TerminalApps->TermApp[16].AID, "\xA0\x00\x00\x01\x21\x47\x11", 7);
	TerminalApps->TermApp[16].AID_Length = 7;
	memcpy(TerminalApps->TermApp[17].AID, "\xA0\x00\x00\x01\x41\x00\x01", 7);
	TerminalApps->TermApp[17].AID_Length = 7;
	memcpy(TerminalApps->TermApp[18].AID, "\xA0\x00\x00\x01\x52\x30\x10", 7);
	TerminalApps->TermApp[18].AID_Length = 7;
	memcpy(TerminalApps->TermApp[19].AID, "\xA0\x00\x00\x03\x33\x01\x01", 7);
	TerminalApps->TermApp[19].AID_Length = 7;
	memcpy(TerminalApps->TermApp[20].AID, "\xA0\x00\x00\x05\x24\x10\x10", 7);
	TerminalApps->TermApp[20].AID_Length = 7;	
	memcpy(TerminalApps->TermApp[21].AID, "\xA0\x00\x00\x05\x24\x10\x11", 7);
	TerminalApps->TermApp[21].AID_Length = 7;	
	for(i=0; i<UMAX_TERMINAL_APPL; i++)
	{
		TerminalApps->TermApp[i].bTerminalPriority = 0x03;	//Terminal priority
		TerminalApps->TermApp[i].bTargetPercentageInt = 0x00;/*Randomly selected target percentage DF17*/
		memcpy(TerminalApps->TermApp[i].TAC_Default, "\x00\x00\x00\x00\x00", 5);/* TAC Default data format (n5) DF11*/ 
		memcpy(TerminalApps->TermApp[i].TAC_Denial, "\x00\x00\x00\x00\x00", 5);/* TAC Refuse: data format (n5) DF12*/
		memcpy(TerminalApps->TermApp[i].TAC_Online, "\x00\x00\x00\x00\x00", 5);/* TAC Online: data format (n5) DF13*/
		memcpy(TerminalApps->TermApp[i].abTrnCurrencyCode, COUNTRYCODE, 2);/* Currency code tag: 5F2A */
		memcpy(TerminalApps->TermApp[i].abTerminalCountryCode, COUNTRYCODE, 2);/* Country code terminal tag: 9F1A */
		TerminalApps->TermApp[i].abTrnCurrencyExp = 0x02;/* tag: 5F36 */
		memcpy(TerminalApps->TermApp[i].abRFCVMLimit, "\x00\x00\x00\x10\x00", 6);/*Terminal performs CVM quota: DF21*/
		memcpy(TerminalApps->TermApp[i].abDDOL, "\x9F\x37\x04", 3);/* TDOL DF14*/
		TerminalApps->TermApp[i].DDOL_Length = 0x03;/* TDOL Length */
		TerminalApps->TermApp[i].TerminalType = 0x22;/* Terminal type: data format (n 3) */
		memcpy(TerminalApps->TermApp[i].TerminalCap, "\xE0\xF8\xC8", 3);/* Terminal capability: data format (n 3) */		
	}

	for(i=0; i<20; i++)
	{
		TerminalApps->TermApp[i].bMaxTargetPercentageInt = 0x00;/*Offset randomly selected maximum target percentage DF16*/
		memcpy(TerminalApps->TermApp[i].abTFL_International, "\x00\x00\x3A\x98", 4);/* Terminal minimum 9F1B//*/
		memcpy(TerminalApps->TermApp[i].abThresholdValueInt, "\x00\x00\x13\x88", 4);/*Offset randomly selected threshold DF15*/
		memcpy(TerminalApps->TermApp[i].abTerminalApplVersion, "\x00\x96", 2);/* Terminal application version 9F09 9F08 */
		memcpy(TerminalApps->TermApp[i].abEC_TFL, "\x00\x00\x00\x20\x00", 6);/* Terminal electronic cash transaction limit tag: 9F7B n12*/
		memcpy(TerminalApps->TermApp[i].abRFOfflineLimit, "\x00\x00\x00\x20\x00", 6);/*Contactless offline minimum :DF19*/
		memcpy(TerminalApps->TermApp[i].abRFTransLimit, "\x00\x00\x02\x00\x00", 6);/*Contactless transaction limit:DF20*/
		TerminalApps->TermApp[i].cOnlinePinCap = 0x01;/* Terminal online pin capability DF18*/
	}

	for(i=20; i<UMAX_TERMINAL_APPL; i++)
	{
		memcpy(TerminalApps->TermApp[i].abTerminalApplVersion, "\x00\x02", 2);/* Terminal application version 9F09 9F08 */
		memcpy(TerminalApps->TermApp[i].abTFL_International, "\x00\x01\x86\xA0", 4);/* Terminal minimum 9F1B//*/
		memcpy(TerminalApps->TermApp[i].abThresholdValueInt, "\x00\x00\x00\x00", 4);/*Offset randomly selected threshold DF15*/
		TerminalApps->TermApp[i].bMaxTargetPercentageInt = 0x05;/*Offset randomly selected maximum target percentage DF16*/
		TerminalApps->TermApp[i].cOnlinePinCap = 0x31;/* Terminal online pin capability DF18*/
		memcpy(TerminalApps->TermApp[i].abEC_TFL, "\x00\x00\x01\x00\x00", 6);/* Terminal electronic cash transaction limit tag: 9F7B n12*/
		memcpy(TerminalApps->TermApp[i].abRFOfflineLimit, "\x00\x00\x01\x00\x00", 6);/*Contactless offline minimum :DF19*/
		memcpy(TerminalApps->TermApp[i].abRFTransLimit, "\x00\x00\x05\x00\x00", 6);/*Contactless transaction limit:DF20*/
	}
}

void m_DispOffPin(int Count)	
{
	if (Count == 0)
	{
		gui_messagebox_show("", "PIN OK" , "" , "ok" , 2000);
	}
    else if (Count == 1 || Count == 2)
	{
		gui_messagebox_show("", "WRONG PIN!" , "" , "ok" , 4000);
	}
	else 
	{
		gui_messagebox_show("", "INCORRECT PIN!" , "" , "ok" ,4000);
	}
}

void m_ReadingCardDisp(int iTranMode)
{
	if(iTranMode == 1)
	{
		//contect
		gui_messagebox_show("", "Reading card,pls don't pull the card!" , "" , "" ,10);
	}
	else if(iTranMode == 2)
	{
		//uncontect
		gui_messagebox_show("", "Reading card,pls don't take the card!" , "" , "" ,10);
	}
}

int upay_consum( void )
{
	char *title = "SALE";
	int ret;
	long long namt = 0;
	long long other_amt = 0;
	st_read_card_in *card_in = 0;
	st_read_card_out *card_out = 0;
	st_card_info card_info={0};
 	TERMCONFIG termconfig;
 	TERMINALAPPLIST *TerminalApps=0;
// 	CAPUBLICKEY pkKey={0};

	APP_TRACE( "upay_consum" );
	card_in=(st_read_card_in *)malloc(sizeof(st_read_card_in));
	memset(card_in,0x00,sizeof(st_read_card_in));
	//Set card_in
	memset(&termconfig,0,sizeof(TERMCONFIG));
	//EMV_SetInputPin( m_InputPin );//Set offline PIN verification interface
	//EMV_SetDispOffPin( m_DispOffPin );//Set offline PIN prompt interface
	EMV_SetReadingCardDisp(m_ReadingCardDisp);
	card_in->trans_type=EMV_SALE;//EMV_CASHBACK;
	card_in->pin_input=1;
	card_in->pin_max_len=12;
	card_in->key_pid = 2;//1 KF_MKSK 2 KF_DUKPT
	card_in->pin_mksk_gid=-1;//The key index of MKSK; -1 is not encrypt
	card_in->pin_dukpt_gid=0;//The key index of DUKPT PIN KEY
	card_in->data_dukpt_gid=1;//The key index of DUPKT Track data KEY
	card_in->pin_timeover=60000;
	strcpy(card_in->title, title);
	strcpy(card_in->card_page_msg, "Please insert/swipe");//Swipe interface prompt information, a line of 20 characters, up to two lines, automatic branch.

	//ret = input_num_page(card_in.amt, "input the amount", 1, 9, 0, 0, 1);		// Enter the amount
	//if(ret != INPUT_NUM_RET_OK) return -1;
	namt = inputamount_page(title, 9, 90000);
	if(namt <= 0)
    {
		free(card_in);
        return -1;
    }
        
    sprintf(card_in->amt, "%lld" , namt);

	if(card_in->trans_type == EMV_CASHBACK)
	{
		namt = inputamount_page("CASHBACK", 9, 90000);
		if(namt <= 0)
		{
			free(card_in);
			return -1;
		}
		sprintf(card_in->other_amt, "%lld" , namt);
	}

	card_in->forceIC=1;
	card_in->show_PAN=0;    
	card_in->card_mode = READ_CARD_MODE_MAG | READ_CARD_MODE_IC | READ_CARD_MODE_RF;	// Card reading method
	card_in->card_timeover = 60000;	// Card reading timeout ms
	
	if (first == 0)
	{
		first = 1;
		TestSetTermConfig(&termconfig);
 		EMV_TermConfigInit(&termconfig);//Init EMV terminal parameter
		TerminalApps=(TERMINALAPPLIST*)malloc(sizeof(TERMINALAPPLIST));
		if(TerminalApps)
		{
			memset(TerminalApps,0,sizeof(TERMINALAPPLIST));
			TestDownloadAID(TerminalApps);
			EMV_PrmClearAIDPrmFile();
			EMV_PrmSetAIDPrm(TerminalApps);//Set AID
			free(TerminalApps);
		}

	// 	EMV_PrmSetCAPK(&pkKey);//Set CAPK	
	}

	APP_TRACE( "emv_read_card" );
	card_out= (st_read_card_out *)malloc(sizeof(st_read_card_out));
    if(card_out==0)
	{
		free(card_in);
		return -1;
	}
loop_card:
	memset(card_out, 0, sizeof(st_read_card_out));
	ret = emv_read_card(card_in, card_out);

	APP_TRACE( "-----------------------upay_consum Ret:%d-------------------------",ret);
	//if(ret == READ_CARD_RET_MAGNETIC){					// Magnetic stripe cards
	//	sdk_log_out("trackb:%s\r\n", card_out.track2);
	//	sdk_log_out("trackc:%s\r\n", card_out.track3);
	//	sdk_log_out("pan:%s\r\n", card_out.pan);
	//	sdk_log_out("expdate:%s\r\n", card_out.exp_data);

	//	
	//}
	//else if(ret == INPUTCARD_RET_ICC){					// 
	//	
	//}
	//else if(ret == INPUTCARD_RET_RFID){
	//card_rf_emv_proc(0, 1, m_tmf_param,_pack_8583);
	//}
	//else{
	//	return ret;
	//}
	if(EMVAPI_RET_FALLBACk==ret){
		card_in->card_mode = READ_CARD_MODE_MAG;
		card_in->forceIC=0;
		memset(card_in->card_page_msg,0x00,sizeof(card_in->card_page_msg));
		strcpy(card_in->card_page_msg,"please try to swipe");
			goto loop_card;
	}
	else if(EMVAPI_RET_FORCEIC==ret){
		if(card_in->card_mode == READ_CARD_MODE_MAG)
			ret =EMVAPI_RET_ARQC;
		else{
			card_in->card_mode = READ_CARD_MODE_IC | READ_CARD_MODE_RF;
			memset(card_in->card_page_msg,0x00,sizeof(card_in->card_page_msg));
			strcpy(card_in->card_page_msg,"don't swipe,please tap/insert the card");
			goto loop_card;
		}
	}
	else if(EMVAPI_RET_OTHER==ret){
		card_in->card_mode = READ_CARD_MODE_IC;
		memset(card_in->card_page_msg,0x00,sizeof(card_in->card_page_msg));
		strcpy(card_in->card_page_msg,"please insert card");
			goto loop_card;
	}

	if(EMVAPI_RET_ARQC == ret)
	{
		gui_messagebox_show("", "Online Request" , "" , "ok" , 0);
	}
	else if(EMVAPI_RET_TC == ret)
	{
		gui_messagebox_show("", "Approved" , "" , "ok" , 0);
	}
	else if(EMVAPI_RET_AAC == ret)
	{
		gui_messagebox_show("", "Declined" , "" , "ok" , 0);
		free(card_in);
		free(card_out);
		return 0;
	}
	else if(EMVAPI_RET_AAR == ret)
	{
		gui_messagebox_show("", "Terminate" , "" , "ok" , 0);
		free(card_in);
		free(card_out);
		return 0;
	}
	else
	{
		gui_messagebox_show("", "Cancel" , "" , "ok" , 0);
		free(card_in);
		free(card_out);
		return 0;
	}

	APP_TRACE( "-----------------------card_info---------------------------" );
	APP_TRACE( "card_type:%d\r\n", card_out->card_type );
	APP_TRACE("trackb:%s\r\n", card_out->track2);
	APP_TRACE("trackc:%s\r\n", card_out->track3);
	APP_TRACE("pan:%s\r\n", card_out->pan);
	APP_TRACE("expdate:%s\r\n", card_out->exp_data);
	APP_TRACE_BUFF_LOG(card_out->pin_block, sizeof(card_out->pin_block), "pin_block:");
	APP_TRACE("vChName:%s\r\n", card_out->vChName);
	APP_TRACE("ic_data_len:%d\r\n", card_out->ic_data_len);
	APP_TRACE_BUFF_LOG(card_out->ic_data, card_out->ic_data_len, "ic_data:");

	gui_messagebox_show("track2", card_out->track2 , "" , "ok" , 0);
	gui_messagebox_show("pan", card_out->pan , "" , "ok" , 0);
	gui_messagebox_show("expdate", card_out->exp_data , "" , "ok" , 0);

	strcpy(card_info.amt, card_in->amt);
	strcpy(card_info.title, card_in->title);
	strcpy(card_info.pan, card_out->pan);
	strcpy(card_info.expdate, card_out->exp_data);
	EMV_online_cardemv_free();
	free(card_in);
	free(card_out);

	upay_print_proc(&card_info);			// Printout

	return 0;

}
