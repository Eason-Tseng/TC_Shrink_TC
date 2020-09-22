#ifndef varH
#define varH
//---------------------------------------------------------------------------

#include <time.h>

// #define ShowInfoToSTD

#define INIPATHFILE "/cct/Data/SETTING/SET.ini"
#define SEGMENTFILE "/Data/SegmentData.ini"
#define VDDATAFILE "/cct/vddata/dev.cfg"

//XML define Arwen 1001006
#define PLANXMLFILE "/tmp/www/xml/planInfo.xml"
#define SEGMENTXMLFILE "/tmp/www/xml/segment.xml"
#define WEEKDAYXMLFILE "/tmp/www/xml/weekday.xml"
#define HOLIDAYXMLFILE "/tmp/www/xml/holiday.xml"
#define DEVIDXMLFILE "/tmp/www/xml/devid.xml"
#define PHASEIDXMLFILE "/tmp/www/xml/phaseid.xml"
#define ACTMODEPLANFILE "/tmp/www/xml/actModeplan.xml"

#define SignalConflictVersion 0                                                 //GreenConflictVersion
#define MaxOpenUDPPort 32

#define BYTE unsigned char                                                      //ANSI C ���{��BYTE�o�Ӧr

#define HI(w) ((BYTE) (((unsigned short int) (w) >> 8) & 0xFF))                 //�DHI byte
#define LO(w) ((BYTE) (w))                                                      //�DLO byte

//OT20111020
#define noRcvAckReSend  false                                                    //true:normal false: debug
// #define noRcvAckReSend  true                                                    //true:normal false: debug
//when no receive ack, after 5 sec, resend message again?
#define Protocal5F1CStopStep 0                                                  // only 0, 2 means in green or people Redd
//OT20111128, remove~ #define MACHINELOCATE 0                                                         /* Hsinchu:0 Tainan:1 Yulin:2 */
#define MACHINELOCATEATHSINCHU 0
#define MACHINELOCATEATTAINAN  1
#define MACHINELOCATEATYULIN   2

void SendRequestToKeypad(void);                                              //�߰ݥثekeypad���A
void BRT_Query8051Status(void);     // jacky20140108 Query 8051 Status
void BRT_QueryKeypadStatus(void);   // jacky20140114 Query Keypad Status

extern unsigned int CheckOnTimeCount;
extern unsigned short int CycleRunCount;
extern unsigned short int CountBF08;
extern unsigned short int TenCycle;

#define BUFFERSIZE          1024
#define MSGINSIZE             64
#define MSGWAITREPONSECOUNT  256
#define cWRITE                81                                                //�g�X
#define cREAD                 82                                                //Ū�J
#define cRS232                83                                                //RS232
#define cUDP                  84                                                //UDP

#define cInner                85                                                //�����ǿ�
#define cOutWard              86                                                //I/O

#define cComingFromScreen     87                                                //�����L�ӡA�P�ɦ�cInner���ʽ�
#define cTester               88                                                //�ⴣ��վ�\uFFFD

//OT Pass
#define cP1ToCenter           91
#define cP2ToCenter           92
#define cP3ToCenter           93
#define cP4ToCenter           94

#define cCenterToP1           101
#define cCenterToP2           102
#define cCenterToP3           103
#define cCenterToP4           104

#define cComingFromAmegidsCrossServer  105


#define INVAILDVALUE         999

#define cHEX                  31                                                //16�i��s�X
#define cBCD                  32                                                //BCD�s�X

//OTCombo
#define TC92_ucControlStrategy  1
#define TC92_iEffectTime        2
#define TC92_iUpdatePhaseData   3                                               //��Phase��?�s�ɡA��flag�Ϊ�
#define TC92_TransmitCycle5F0F_Change 4
#define TC92_TransmitCycle5F03_Change 5
#define TC92SignalLightStatus_5F0F_IntervalTime 6
#define TC92SignalStepStatus_5F03_IntervalTime 7
#define TC92_PlanOneTime5F18_Change 8
#define TC92_PlanOneTime5F18_PlanID 9
#define TC_DoorStatus_Change 10
#define TC_SignalConflictError 11
#define TC92_SubPhaseOfPhasePlanIncorrent 12
#define TC92_RedCountVer 13
#define Com2_TYPE 14

#define TC92_5F1C_SPID 15
#define TC92_5F1C_SID 16
#define TC92_5F1C_ET 17

//OT 960425 CHAIN
#define TC92_5F31Manual          301
#define TC92_5F31TOD             302
#define TC92_5F31StartSubPhaseId 303
#define TC92_5F31EndSubPhaseId   304
#define TC92_5F32StartSubPhaseId 305
#define TC92_5F32EndSubPhaseId   306
#define TC_MotherChainStartStepId 307
#define TC_MotherChainEndStepId 308

#define TC_KeyPadP6Value         310

#define TC_ActuateVDID           311

#define TC_TrainChainNOW         312
#define TC_TrainChainLast        313
#define TC_TrainComingBanSubphase 314
#define TC_TrainComingForceJumpSubphase 315
#define TC_TrainChainEnable      316
#define TC_ReverseLane_Control_Mode      317
#define TC_ReverseLane_Manual_Enable_Status      318

//OT980420
#define TC_Redcount_Booting_Display_Enable  319
#define TC_Redcount_Display_Enable  320

#define TC_CSTC_ControlStrategy 321
#define TC_CSTC_FieldOperate    322

//OT980907
#define TC_CCT_ActuateType_By_TOD 323

//OT990419
#define DynShowRedCountForAct 324

#define TC_CCT_Send_ActuateArwen_Protocal 325


//OT990621
#define revSyncEnable 326
#define revSyncInterval 327
#define revSyncTimeDiffTolarence 328
#define revSyncCountineStatusAdj 329
#define revDefaultVehWay 330
#define revLogToFile 331
#define revSendProtocol 332
#define revSyncByteStatus   333

#define amegidsDynEnable  334
#define amegidsDynDevLCN  335

//ARWEN 100/01/21
#define PedestrianModeOpen 336
//ARWEN 100/09/22
#define fixedGreenModeOpen 337
//ARWEN 100/10/07
#define actuateSwitchChange_for_Arwen 338

#define TOD_PLAN_ID       339

//OT20110607
#define TC_CCT_SendCCTPhaseCycleProtocalForCenter 340
#define CSTC_SegmentNoRunning 341

//OT20110624
#define CSTC_RedcountRepeatCount 342

//OT20111128
#define TC_CCT_MachineLocation 343

//ARWEN++1001207
#define ArwenActRedCountSec 344

//Chou++2012/10/01
#define TC_Actuateautoswitch 345

//jacky20140115 BRT
#define BRT_Light_Display_Enable 346

//OT20111201
#define TC_TrainComingBanSubphase2 347
//OT20131225
#define CCJ_HeartBeatCount 348

//OT20140415
// #define TCDynSegAdjustType  349
#define TCDynSegAdjustKeepTime   350
#define TCDynSegRemainSec   351
#define TCDynSegAdjustHour   352
#define TCDynSegAdjustMin   353

//CCJ 20140526 ActType
#define ActType 354
#define ActPlan 355
#define ActComp 356

//OT Debug 0523
#define TC_CCT_In_LongTanu_ActuateType_Switch 18
#define TC_CCT_In_LongTanu_ActuateType_FunctionEnable 19

//OT Debug Signal 951116
#define TC_SIGNAL_NEXT_STEP_OK 20
#define TC_SIGNAL_DRIVER_UNIT  21

#define TC_GreenConflictDetFromCSTC 30
#define TC_GreenConflictDetFromDIO 31

#define TC_Actuate_By_TFD         25
#define TC_CCTActuate_TOD_Running 26
#define TC_CCT_TFD_ActuateType_ExtenCount 27
#define TC_CCT_TFD_ActuateType_Switch 28
#define TC_CCT_TFD_ActuateType_FunctionEnable 29

#define CCJ_SendStepToCCJ         30

//OT070402
#define GPS_SYNC                  33

#define NextStepForceEnable       34

#define EnableUpdateRTC           35
#define IFGetResetTCSignal        36

#define TCSegmentTypeUpdate       37

//OT20140415
#define TCDynSegSwitch            38
#define TCDynSegStatus            39
#define TCDynSegNextPlanIdSendToCCJ   40
#define TC_CCT_PedSW_Check 41

#define TC_Phase 0
#define TC_Plan 1
#define TC_SegType 2

#define TC_RedCountNA 0
#define TC_RedCountVerHK 1
#define TC_RedCountVer94 2
#define TC_RedCountVer94v2 3
#define TC_RedCountVerCCT97  4
#define TC_RedCountVerV3 5 //Eason_Ver4.3

#define Com2IsGPSPort 0
#define Com2IsTesterPort 1
#define Com2IsPassingPort 3
#define Com2IsRedCount 4
#define Com2IsTainanPeopleLight 5
#define Com2IsPedestrianPushButton 6 //add arwen
#define Com2IsRedCountAndCom1IsPedPushButton 7 //add arwen
#define Com2IsHOLUXGPSPort 8

//CSTC VAR=================================================
#define CSTC_exec_phase_current_subphase      0
#define CSTC_exec_phase_current_subphase_step 1
#define CSTC_exec_segment_current_seg_no      2
#define CSTC_exec_plan_phase_order 3
#define CSTC_exec_plan_plan_ID 4
#define CSTC_exec_plan_green_time 5
#define CSTC_exec_plan_pedgreenflash_time 6
#define CSTC_exec_plan_pedred_time 7
#define CSTC_exec_plan_yellow_time 8
#define CSTC_exec_plan_allred_time 9
#define CSTC_exec_segment_type 10 //110802arwen++
//OT20140214
#define CSTC_exec_plan_maxgreen_time 11
#define CSTC_exec_plan_mingreen_time 12
#define CSTC_exec_plan_green_time_compesated 13
//jacky20140813
#define CSTC_exec_BF13plan_Extend_East 14
#define CSTC_exec_BF13plan_Extend_West 15

//OT20140415
#define CSTC_exec_plan_cycle 16

#define TC_CHAIN_SIGNAL_IN_START_SUB_PAHSE   true
#define TC_CHAIN_SIGNAL_IN_END_SUB_PAHSE     false

//---------------------------------------------------------------------------
//Copy From ParseTraffic
typedef struct BCD{
        BYTE b1:4;
        BYTE b2:4;
}BCD;
typedef union BCDSW{
        BCD bcdHiLo;
        BYTE bcdCode;
}BCDSW;

//-----------------------------------------------
typedef struct MESSAGEOK {
        int protocol;                                                           //�ϥΪ��q�T��w����,�аѦ�var.h�w�q�i�઺��w
        BYTE packet[BUFFERSIZE];                                                //��ڨ�w�ʥ]���e
        int packetLcn;                                                          //����w����f�s��,�u�A�Ω�PTRAFFIC(��q���q�T��w)�ΥõaCMS...��
        int packetLength;                                                       //����w���\uFFFD
        bool cksStatus;                                                         //�P�O���ʥ]��checkSum���T�P�_,�i�a��
        bool success;                                                           //�P�O���ʥ]����w��׵���L�ԭ����P�O,�i�a��
        int UDPorRS232;                                                         //���ʥ]�Ӧۦ�B
        int ReadorWrite;                                                        //�M�w���ʥ]�OŪ�J���μg�X���ʥ] read or write
        int InnerOrOutWard;                                                     //�����ǿ��٬O�z�LI/O mtyu
        int packetint[BUFFERSIZE];                                              //packect for int chou++ 2013/06/05
        bool bGetDLE;
        bool bGetSOH; //the same with STX
        bool bGetACK;
        bool bGetNAK;
        unsigned int uiPacketTEXTLEN;


} MESSAGEOK;

typedef struct SwitchBIT {
        BYTE b1:1;
        BYTE b2:1;
        BYTE b3:1;
        BYTE b4:1;
        BYTE b5:1;
        BYTE b6:1;
        BYTE b7:1;
        BYTE b8:1;
} SwitchBIT;
typedef union  DATA_Bit{
        SwitchBIT switchBit;
        BYTE DBit;
} DATA_Bit;

typedef struct sChildChain{
        bool bHaveReasonableChildChainSignal;
        time_t oldStartTime;
        time_t newStartTime;
        time_t oldEndTime;
        time_t newEndTime;
        int iChainCycle;
        int iStartKeepTime;
        int iEndKeepTime;
} sChildChain;


typedef struct sRevSyncStatus{
  bool bYMD_OK;
  bool bTime_OK;
  bool bRevStep_OK;
  bool bRevSegSet_OK;
  bool bRevTime_OK;
} sRevSyncStatus;


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------   //�w�q�i��s�����]��
#define DEVICEUNKNOW          99                                                //������O���F�F
#define DEVICEMUTILPROTOCOL  100                                                //�i�঳�h�ب�w�q�o��port�i��
#define DEVICECENTER77       101                                                //77�~���汱����
#define DEVICECENTER87       102                                                //87�~���汱����
#define DEVICECENTER92       103                                                //92�~���汱����
#define DEVICECENTERNTCIP    104                                                //NTCIP�~���汱����
#define DEVICETESTER77       105                                                //77�~���{���\uFFFD���վ�
#define DEVICETESTER87       106                                                //87�~���{���\uFFFD���վ�
#define DEVICETESTER92       107                                                //92�~���{���\uFFFD���վ�
#define DEVICETESTERNTCIP    108                                                //NTCIP�~���{���\uFFFD���վ�
#define DEVICE77TC           109                                                //77�~�����x���
#define DEVICE87TC           110                                                //87�~�����x���
#define DEVICE87CMS          111                                                //87�~����T�i�ܱ��
#define DEVICE87VD           112                                                //87�~�������
#define DEVICE92TC           113                                                //92�~�����x���
#define DEVICE92CMS          114                                                //92�~����T�i�ܱ��
#define DEVICE92VD           115                                                //92�~�������
#define DEVICENTCIPTC        116                                                //NTCIP���x���
#define DEVICENTCIPCMS       117                                                //NTCIP��T�i�ܱ��
#define DEVICENTCIPVD        118                                                //NTCIP�����
#define DEVICEKEYPAD         119                                                //�F�ͭ��O������L
#define DEVICERTMS           120                                                //RTMS
#define DEVICESS             121                                                //SMART SENSOR
#define DEVICEOWNWAY         122                                                //������O�˼�
#define DEVICEEOI            123                                                //��Ŭ�O�˼�
#define DEVICELISCMS         124                                                //�x�_�õaCMS
/*OTCombo0713*/
#define DEVICETRAFFICLIGHT   125                                                //�ص{���O����O
#define DEVICEREDCOUNTVER94  126                                                //��O�˼Ƹ˸m (���\uFFFD
#define DEVICEREDCOUNTVERHK  127                                                //��O�˼Ƹ˸m (����)
//#define DEVICEGPS            128
#define DEVICETAINANPEOPLELIGHT 129

#define DEVICECCJDYNCTL      130
#define DEVICEREDCOUNTVERCCT97  131
#define DEVICEREVMANUALPAD   132

#define DEVICEREVSYNC   133
#define DEVICEAMEGIDS   134
#define DEVICEPEDESTRIAN 135//add Arwen
#define DEVICEARWENVDINFO 136//add Arwen
#define DEVICEWEBSET 137//add Arwen
#define DEVICEGPS    138 //add chou 2013/03/22           //NEWcom5 GPS
#define DEVICENSPREDCOUNT    139 //add chou 2013/03/22      //NEWcom6 REDCOUNT
#define DEVICEGREENMANCOUNT 140//add chou 2013/05/23     //NEWcom7 GREENMAN
#define DEVICEBRT           141 //OT20140224
#define DEVICEJSON          142 //jacky20140327
#define DEVICECOUNTDOWN     143  //jacky20140426
#define DEVICEREDCOUNTVERV3 144 //Eason_Ver4.3
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------   //�w�q�i�઺��w
#define PROTOCOLUNKNOW       200                                                //���M������w
#define PROTOCOL77           201                                                //��q��77�~���q�T��w
#define PROTOCOL87           202                                                //��q��87�~���q�T��w
#define PROTOCOL92           203                                                //��q��92�~���q�T��w
#define PROTOCOLNTCIP        204                                                //��q��NTCIP�q�T��w
#define PROTOCOLKEYPAD       205                                                //�F�ͭ��O������L�q�T��w
#define PROTOCOLRTMS         206                                                //RTMS�q�T��w
#define PROTOCOLSS           207                                                //SMART SENSOR�q�T��w
#define PROTOCOLOWNWAY       208                                                //������O�˼Ƴq�T��w
#define PROTOCOLEOI          209                                                //��Ŭ�O�˼Ƴq�T��w
#define PROTOCOLLISCMS       210                                                //�x�_�õaCMS�q�T��w
#define PROTOCOLTRAFFICLIGHT 211
#define PROTOCOLREDCOUNTVER94 212                                               //94�~��O�˼�
#define PROTOCOLREDCOUNTVERHK 213                                               //93�~��O�˼� (����)

#define PROTOCOLCCJ        214
#define PROTOCOLREDCOUNTVERCCT97  215
#define PROTOCOLREVMANUALPAD  216

#define PROTOCOLREVSYNC      217
#define PROTOCOLAMEGIDS      218
#define PROTOCOLPEDESTRIAN      219//add Arwen
#define PROTOCOLARWENVD      220//add Arwen
#define PROTOCOLWEBSET      221//add Arwen
#define PROTOCOLNSPREDCOUNT 222//add chou 2013/05/23 �n����O�˼ƥ�
#define PROTOCOLGREENMANCOUNT 223//add chou 2013/05/23�n���H�˼ƥ�
//OT20140224
#define PROTOCOLBRT 224
//jacky20140426
#define PROTOCOLCOUNTDOWN 225

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------   //�w�q�i�઺LCD�e��
#define cNONE                300                                                //�ťխ���(���O�@)
#define cLOGO                301                                                //����LOGO
#define cLOGIN               302                                                //�n�J�e��
#define cLOGINERR            303                                                //�n�J��~�e��
#define cMAIN                304                                                //�D���\uFFFD
#define cCTLSETUP            305                                                //����]�w�D���\uFFFD
#define cCTLNO               306                                                //�]�ƽs��
#define cLASTDOWN            307                                                //�W���_�q�ɶ�
#define cMODIFYDT            308                                                //��nɶ��޲z
#define cFWVERDT             309                                                //�b�骩�����\uFFFD
#define cHWCYCLE             310                                                //�w��^��g�s]�w
#define cMODIFYPWD           311                                                //���K�X
#define cDBSETUP             312                                                //��s�t�αK�X
#define cOPERSTAT            313                                                //�B�@���p
#define cOPERMODE            314                                                //�ާ@�Ҧ�
#define cCOMMSTAT            315                                                //�q�T���A

#define cCARLENGTH           316                                                //����]�w
#define cSENDCYCLE           317                                                //�ǿ�g�s]�w
#define cRECORDCYCLE         318                                                //�O��g�s]�w
#define cUPDATEDB            319                                                //��s��?w
#define cLANESET             320                                                //���D��Ӫ�\uFFFD
#define cREALSTAT            321                                                //���y�Y�ɸ�T
#define cREALEVENT           322                                                //�Y�ɨƥ�

#define cONLINE              323                                                //���ߦ��s�u���n�J�e��
#define cOFFLINE             324                                                //���ߨS���s�u���n�J�e��
#define cLOCKDB              325                                                //���O��Ū���e��
#define cTOTALRUN            326                                                //�`�}��ɶ�\uFFFD

#define cABOERROR            327                                                //�T���]�w�v������
#define cCMDSET              328                                                //�T���]�w
#define cHWRESET             329                                                //�w�魫�m
#define cLCNENCODE           330                                                //�]�ƽs���s�X

//OTADD
#define cSEGTYPE             331                                                //�ɬq���A
#define cSEGTYPEMENU         332
#define cWEEKDAY             333
#define cSPECIALDAY          334
#define cSEGTYPESEL          335
#define cPLANMENU            336
#define cPLANSEL             337
#define cPLAN                338
#define cRUNTABLE            339
#define cSTRATEGY            340
#define cRTMSInfo            341
#define cCURRENTLIGHTSTATUS  342
#define cCURRENTCOMMPACKET   343
#define cPLANEditStep1       344
#define cPLANEditStep2       345
#define cPLANEditStep3       346
#define cPLANEditStep4       347
#define cREDCOUNTMENU        348
#define cREDCOUNTHWCHECKSEL  349
#define cREDCOUNTHWCHECK     350
#define cREDCOUNTHWVERMENU   351
#define cREDCOUNTMAPPING     352
#define cCOMSETMENU          353
#define cPEDGREENCOUNTMAPPING 354
#define cSegTypeEditStep1    360
#define cSegTypeEditStep2    361
#define cSegTypeEditStep3    362

#define cREVERSETIMTMENU     363
#define cREVWEEKDAY          364
#define cREVSPECIALDAY       365
#define cREVERSE             366

#define cPLAN0523            370
#define cPLANF2              371
#define cSEGTYPEEDITF2       372
#define cWEEKDAYEDITF2       373
#define cSPECIALDAYVIEW      374
#define cSPECIALDAYEDITF2    375
#define cSEGTYPEPREVIEW      376
#define cPLANPREVIEW         377
#define cPHASEPREVIEW        378

#define cActuateEdit         380

//OT Pass
#define cPassModeSet         381
//OT Debug Direct
#define cDIRMAPPING          382
#define cLIGHTBOARDMAPPING   383
#define cDIRLIGHTBOARDMAPPINGMENU   384

#define cPHASESEL            385

#define cLAST92TCPLANSEGMENTUPDATE 386

//OT CHAIN
#define cCHAINMENU           387
#define cMOTHERCHAINSET      388
#define cCHILDCHAINSET       389
#define cCHAINSTATUS         390

//Light Edit
#define cLIGHTPREVIEW        391
#define cLIGHTEDIT           392
#define cLIGHTEMULATOR       393

//IP Editor
#define cNETWORKSET          394
#define cNETWORKSETCONFIRM   395

#define cMBSELECT            396
#define cUART2SELECT         397
#define cWARNING             398

#define cTFDSTATUS           399

#define cREDCOUNTHWCHECKDEVID     400
#define cREDCOUNTHWCHECKCYCLENUM  401
#define cREDCOUNTHWCHECKNUM88     402

#define cRAILWAYCHAINSET     403

#define cACTUATEARWENSTATUS 410




//arwen++
#define cSYSTEMINFO 411
#define cV3RELATED 412
#define cCTRLSTRATEGY 413
#define cACTUATEARWENMANYSTATUS 414
#define cVDPHASEMAP 415
#define cVDPHASEMAPEDIT 416
#define cACTSTRATEGY 417
#define cVDActPhaseEdit 418
#define cFLASHMODE 419
#define cACTSTRATEGY_CLOSE 420
#define cFLASHMODE_SETTING 421
#define cVDActPhaseGreen 422
#define cNETWORKSET_MAIN 423
#define cACTARWENMENU 424
#define cACTUATEARWENFLASHMANYSTATUS 425
#define cPedSignalMenu 426
#define cPedSignalHW 427
#define cPEDSWITCH 428
#define cACTDetector 429
#define cACTUATEPLAYSWITCH 430

#define cNETMASKSET         431
#define cACTUATEARWENMANYSTATUS2         432

//--------------Chou ++ ------------------------------
#define cActuateBUSEdit 433
//-----------------------------------------------------------
//jacky20140324    BRT priority
#define cBRTPRIORITY 434
#define cCompensation 435
#define cBRTExtendsec 436
#define cCommunication 437
#define cPrioritymenu 438
#define cCarLearn 439
#define cCountdownProperty 440
#define cCarCountdownProperty 441
#define cPedCountdownProperty 442
#define cPedLearn 443

//smem.vSetACK_W2W / smem.vGetACK_W2W
#define cTCToCenter          0
#define cVD01ToCenter        1
#define cVD02ToCenter        2
#define cVD03ToCenter        3
#define cVD04ToCenter        4
#define cVD05ToCenter        5
#define cVD06ToCenter        6
#define cVD07ToCenter        7
#define cVD08ToCenter        8
#define cVD09ToCenter        9
#define cVD10ToCenter        10
#define cVD11ToCenter        11
#define cVD12ToCenter        12
#define cVD13ToCenter        13
#define cVD14ToCenter        14
#define cVD15ToCenter        15
#define cVD16ToCenter        16
#define cVD17ToCenter        17
#define cVD18ToCenter        18
#define cVD19ToCenter        19
#define cVD20ToCenter        20
#define cVD21ToCenter        21
#define cVD22ToCenter        22
#define cVD23ToCenter        23
#define cVD24ToCenter        24
#define cVD25ToCenter        25
#define cVD26ToCenter        26
#define cVD27ToCenter        27
#define cVD28ToCenter        28
#define cVD29ToCenter        29
#define cVD30ToCenter        30
#define cVD31ToCenter        31
#define cVD32ToCenter        32
#define cVD33ToCenter        33
#define cVD34ToCenter        34
#define cVD35ToCenter        35
#define cVD36ToCenter        36
#define cVD37ToCenter        37
#define cVD38ToCenter        38
#define cVD39ToCenter        39
#define cVD40ToCenter        40

#define cCenterToTC          100
#define cCenterToVD01        101
#define cCenterToVD02        102
#define cCenterToVD03        103
#define cCenterToVD04        104
#define cCenterToVD05        105
#define cCenterToVD06        106
#define cCenterToVD07        107
#define cCenterToVD08        108
#define cCenterToVD09        109
#define cCenterToVD10        110
#define cCenterToVD11        111
#define cCenterToVD12        112
#define cCenterToVD13        113
#define cCenterToVD14        114
#define cCenterToVD15        115
#define cCenterToVD16        116
#define cCenterToVD17        117
#define cCenterToVD18        118
#define cCenterToVD19        119
#define cCenterToVD20        120
#define cCenterToVD21        121
#define cCenterToVD22        122
#define cCenterToVD23        123
#define cCenterToVD24        124
#define cCenterToVD25        125
#define cCenterToVD26        126
#define cCenterToVD27        127
#define cCenterToVD28        128
#define cCenterToVD29        129
#define cCenterToVD30        130
#define cCenterToVD31        131
#define cCenterToVD32        132
#define cCenterToVD33        133
#define cCenterToVD34        134
#define cCenterToVD35        135
#define cCenterToVD36        136
#define cCenterToVD37        137
#define cCenterToVD38        138
#define cCenterToVD39        139
#define cCenterToVD40        140
//OT Pass uPassServer
#define cNoPassMode          0
#define cPassServerMode      1
#define cTermPassMode        2

//OT Debug Direct
#define dirN                 0
#define dirNE                1
#define dirE                 2
#define dirES                3
#define dirS                 4
#define dirSW                5
#define dirW                 6
#define dirWN                7
#define lightOut1            0
#define lightOut2            1
#define lightOut3            2
#define lightOut4            3
#define lightOut5            4
#define lightOut6            5
#define lightOut7            6
#define lightOut8            7

//---------------------------------------------------------------------------
#define ViewScreen    0
#define PreviewScreen 1
#define EditScreen    2

//---------------------------------------------------------------------------
#define MB_ARBOR      0
#define MB_ICOP6047   1
#define MB_ICOP6115   2
//---------------------------------------------------------------------------
typedef struct Humidity
{
    BYTE percent;
    BYTE point_percent;
}Humidity;

typedef struct temperature
{
    BYTE celsius;
    BYTE point_celsius;
}temperature;

typedef struct YMDHMS {
        BYTE Year;
        BYTE Month;
        BYTE Day;
        BYTE Hour;
        BYTE Min;
        BYTE Sec;
} YMDHMS;


typedef struct st92VDLaneContent {
        short int BigVolume;                                                    //�j���y�q
        short int BigSpeed;                                                     //�j���t��
        short int CarVolume;                                                    //�@�먮�y�q
        short int CarSpeed;                                                     //�@�먮�t��
        short int MotorVolume;                                                  //��y�q
        short int MotorSpeed;
        short int AvgSpeed;                                                     //�j�p�������t��
        short int LaneOccupy;                                                   //���D��v
        short int AvgInt;                                                       //�����樮���Z 1/10��  �YAvgInt>250 -> AvgInt = 250

        short int TotalOccupyTime;
} st92VDLaneContent;

typedef struct SeqNoContent {
        st92VDLaneContent DataLane[8];  //�����Ҧ������D��
} SeqNoContent;

typedef struct SmartSensorINFO {
        unsigned char cSerialNumber[18];
        unsigned char cModemIP[17];
        unsigned char cServerIP[17];
        unsigned char cServerIPPort[4];
        unsigned char cDataScript[10];
        unsigned char cConfigScript[8];
        unsigned char cRtmsID[6];
        unsigned char cMultiDropID[6];
        unsigned char cVarName[8];
        unsigned char cOrientation[4];
        unsigned char cLocation[34];
        unsigned char cDescription[32];
        unsigned char cTimeInterval[10];
        unsigned char cBufferFlag[3];
        unsigned char cBCRStr[6];
        unsigned char cDACRStr[6];
        unsigned char cDirection[10];
} SmartSensorINFO;

typedef struct SSXDLaneContent {
       short int Volume;
       float AverageSpeed;
       float Occupancy;
       float SmallClass;
       float MediumClass;
       float LargeClass;
}SSXDLaneContent;

typedef struct tsUDPMappingLCN {
        char cLocalIP[15];
        int iListenPort;
        char cRemoteIP[15];
        int iSendPort;
        int iMachineLCN;
}tsUDPMappingLCN;

/*OT990618*/
typedef struct tsUDPInfo {
        char *cLocalIP;
        int iListenPort;
        char *cRemoteIP;
        int iSendPort;
        int iMachineLCN;
} tsUDPInfo;

#define messageInBuf 20

#define dCmToFt 0.03281
#define mphToKmhr 1.6093                                                        //mph�নkm/hr
#define TwoKBaseSec 946656000                                                   //UNIX TIME in year2000 00:00:00

#define RTSIGNAL_Timer 52                                                       //InterVal Timer�Ϊ�RT signal
#define RTSIGNAL_RECODE_SMEM 53                                                 //�����O���T�Ϊ�

#define simulationNum 720                                                       //VD���\uFFFD�ưO���\uFFFD
#define realRecordNum 1000
#define revAPP 1001
#define cSTOPSENDTIME 14400

// #define RTSIGNAL_WDT_PLAN 51
#define RTSIGNAL_WDT_PLAN 56


const char *const ColorRed = "\033[0;40;31m";
const char *const ColorGreen = "\033[0;40;32m";
const char *const ColorBlue = "\033[0;40;34m";
const char *const ColorWhiteBlue = "\033[0;40;36m";
const char *const ColorNormal = "\033[0m";

#endif
