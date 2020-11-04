// vGet0FCommandSet
// 0: Basic (wireless)
// 1: Base
// 2: Basic & Advance
// 3: Basic & Advance & Option

//---------------------------------------------------------------------------
#include "PTRAFFIC92TC.h"
#include "shirink_app.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include "DIGITALIO.h"
#include "CHexToInt.h"
#include "CSTC.h"
#include "CTools.h"
#include "shirink_app.h"
#include "SCREENLast92TCPlanSegmentUpdate.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <strings.h>
#include "SMEM.h"
#include "CTIMER.h"
#include "screenActuateBUSEdit.h"
#include "screenActuateArwenManyStatus2.h"
#include "screenActuateArwenManyStatus.h"

//unsigned char SubPhaseId_5F19 = 0;
//unsigned char ActuateType_5F19 = 0;
//unsigned char TimeExtend_5F19 = 0;
//unsigned char ActuateData1_5F19 = 0;
//unsigned char ActuateData2_5F19 = 0;

//---------------------------------------------------------------------------
PTRAFFIC92TC::PTRAFFIC92TC(void)
{
}
//---------------------------------------------------------------------------
PTRAFFIC92TC::~PTRAFFIC92TC(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::DoWorkViaPTraffic92(MESSAGEOK message)
{
    try
    {

        switch (message.packet[8])
        {
        case 0x10:
            vWriteControlStrategy5F10(message);
            break;

        case 0x40:
            vQurryControlStrategy5F40(message);
            break;

        case 0x11:
            vWriteReverseTimeData5F11(message);
            break;

        case 0x41:
            vQueryReverseTimeData5F41(message);
            break;

        case 0x12:
            vWriteReverseTimeData5F12(message);
            break;

        case 0x42:
            vQueryReverseTimeData5F42(message);
            break;

        case 0x14:
            vWritePlan5F14(message);
            break;

        case 0x44:
            vQueryPlan5F44(message);
            break;

        case 0x13:
            vWritePhase5F13(message);
            break;

        case 0x43:
            vQueryPhase5F43(message);
            break;

        case 0x15:
            vWritePlanDBMS5F15(message);
            break;

        case 0x45:
            vQueryPlanDBMS5F45(message);
            break;

        case 0x16:
            vWriteSegment5F16(message);
            break;

        case 0x46:
            vQuerySegment5F46(message);
            break;

        case 0x17:
            vWriteSpecialSegment5F17(message);
            break;

        case 0x47:
            vQueryHolidaySegment5F47(message);
            break;

        case 0x18:
            vWriteRunPlan5F18(message);
            break;

        case 0x48:
            vQueryRunPlan5F48(message);
            break;

//jantai 20140216 start. add actuated segment control
        case 0x19://set actuated parameters for actuated type(1~9)
            vSetActuatedType_5F19(message);
            break;

        case 0x49://query actuated parameters
            vQueryActuatedType_5F49(message);
            break;

        case 0x1A://set actuated segments
            vSetActuatedSegment_5F1A(message);
            break;

        case 0x4A://query actuated segments
            vQueryActuatedSegment_5F4A(message);
            break;

        case 0x1B://set actuated segments
            vSetActuatedHolidaySegment_5F1B(message);
            break;

        case 0x4B://query actuated segments
            vQueryActuatedHolidaySegment_5F4B(message);
            break;
//jantai 20140216 end.

        case 0x2F:
            vWritePhaseByStep5F2F(message);
            break;

//OT Debug 4010
        case 0x5F:
            vQueryPhase5F5F(message);
            break;


        case 0x3F:
            vTransmitCycleSetting_5F3F(message);
            break;

        case 0x6F:
            vTransmitCycleQuery_5F6F(message);
            break;

        case 0x1C:
            vGoToNextPhaseStepControl_5F1C(message);
            break;

        case 0x4C:
            vQueryPhaseStepControl_5F4C(message);
            break;
            /*CCJ20150723 mark start
                    case 0x19:
                      vTouchControlConfiguration_5F19(message);
                      break;

                    case 0x49:
                      vTouchControlConfiguration_5F49(message);
                      break;
            CCJ20150723 mark end*/

//¥D°Ê¦^³ø
        case 0x00:
        case 0x03:
        case 0x0C:
        case 0x08:
        case 0x0A:
        case 0x0B:
        case 0x0F:
            writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            //OT20140519
        case 0x2A:
            vSetDynSegParameter(message);
            break;
        case 0x5A:
            vQueryDynSegParameter(message);
            break;
        case 0x2B:
            vSetDynSegSwitch(message);
            break;

        default:
            vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0);
            break;

        }

        return true;

    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWriteControlStrategy5F10(MESSAGEOK DataMessageIn)
{
    try
    {

        unsigned short int usiAmegidsDevLCN;
        DATA_Bit _ControlStrategy;
        int EffectTime;
        int iTmp;
        unsigned short int usiTmp;

        unsigned char data[32];
        MESSAGEOK _MsgOK;
        printf("[get TC 5F10]\n");

        int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level

        if(iReturnCommandSet <= 1)
        {
            vReturnToCenterNACK(0x5F, 0x10, 0x80, 0x00);
            return false;
        } // not include level "A""
        if( DataMessageIn.packetLength < 14 )
        {
            vReturnToCenterNACK(0x5F, 0x10, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 14 )
        {
            vReturnToCenterNACK(0x5F, 0x10, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

        _ControlStrategy.DBit = DataMessageIn.packet[9];
        EffectTime = DataMessageIn.packet[10] * 60;

        if(iReturnCommandSet <= 1)
        {
            vReturnToCenterNACK(0x5F, 0x10, 0x80, 0x00);    // not include level "A""
            return false;
        }
        if( DataMessageIn.packetLength < 14 )
        {
            vReturnToCenterNACK(0x5F, 0x10, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 14 )
        {
            vReturnToCenterNACK(0x5F, 0x10, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

        if(1)    //not amegids mode.
        {

            printf("get TC 5F10, smem.vGetUCData(amegidsDynEnable) <= 0\n");
            int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level
            vReturnToCenterACK(0x5F, 0x10);

            _ControlStrategy.DBit = DataMessageIn.packet[9];
            EffectTime = DataMessageIn.packet[10] * 60;                                   //EffectTime save using Sec, but protocal using Min.

            data[0] = 0x5F;
            data[1] = 0x91;
            data[2] = DataMessageIn.packet[9];
            data[3] = DataMessageIn.packet[10];

            smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);
            smem.vSetINTData(TC92_iEffectTime, EffectTime);

            smem.vSet5F18EffectTime(EffectTime);

            smem.vSetINTData(TC92_iEffectTime, EffectTime);

            if(_ControlStrategy.switchBit.b1 == true &&
                    _ControlStrategy.switchBit.b5 == false &&
                    _ControlStrategy.switchBit.b6 == false    )
            {
//OT1000218        if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )
//OT1000218          _intervalTimer.vAllDynamicToTODCount(10);

                if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )   //OT20111020
                {
                    _intervalTimer.vAllDynamicToTODCount(10);
                }

                smem.vSet5F18EffectTime(2);
            }
            else if(_ControlStrategy.DBit == 0x30)
            {
                printf("Get Amegids 0F 10 30\n\n");
                if( stc.Lock_to_LoadControlStrategy() == STRATEGY_TOD )
                {
                    printf("Get Amegids, now is TOD\n\n");
//OT20111020
                    //OT20111107 stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC);
                    printf("Get Amegids, now changing to ALLDYNAMIC\n\n");

                    //OI20111107
                    _intervalTimer.vAllDynamicToTODCount(EffectTime);

                    /*
                              int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
                              if(usiCurrentSubphaseStep == 0) {
                                if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
                                  printf("Set Sec:%d, stepID:%d\n", EffectTime, usiCurrentSubphaseStep);
                                  _intervalTimer.vAllDynamicToTODCount(EffectTime);
                                }
                                else {
                                  usiTmp = stc.vGetUSIData(CSTC_exec_plan_green_time);
                                  printf("Set Sec:%d, StepID:%d\n", usiTmp, usiCurrentSubphaseStep);
                                  _intervalTimer.vAllDynamicToTODCount(usiTmp);
                                }
                              }
                              else if(usiCurrentSubphaseStep == 1) {
                                usiTmp = stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time);
                                printf("Set Sec:%d, StepID:%d\n", usiTmp, usiCurrentSubphaseStep);
                                _intervalTimer.vAllDynamicToTODCount(usiTmp);
                              }
                              else if(usiCurrentSubphaseStep == 2) {
                                if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
                                  printf("Set Sec:%d, StepID:%d, in Protocal5F1CStopStep\n", EffectTime, usiCurrentSubphaseStep);
                                  _intervalTimer.vAllDynamicToTODCount(EffectTime);
                                }
                                else {
                                  usiTmp = stc.vGetUSIData(CSTC_exec_plan_pedred_time);
                                  printf("Set Sec:%d, StepID:%d\n", usiTmp, usiCurrentSubphaseStep);
                                  _intervalTimer.vAllDynamicToTODCount(usiTmp);
                                }
                              }
                              else if(usiCurrentSubphaseStep == 3) {
                                usiTmp = stc.vGetUSIData(CSTC_exec_plan_yellow_time);
                                printf("Set Sec:%d, StepID:%d\n", usiTmp, usiCurrentSubphaseStep);
                                _intervalTimer.vAllDynamicToTODCount(usiTmp);
                              }
                              else if(usiCurrentSubphaseStep == 4) {
                                usiTmp = stc.vGetUSIData(CSTC_exec_plan_allred_time);
                                printf("Set Sec:%d, StepID:%d\n", usiTmp, usiCurrentSubphaseStep);
                                _intervalTimer.vAllDynamicToTODCount(usiTmp);
                              }
                    */

                }
            }



//OT1000218
            /* disable all

              //  _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);              //Read Data from share memory
            //  EffectTime = smem.vGetINTData(TC92_iEffectTime);
                  printf("get TC 5F10, smem.vGetUCData(amegidsDynEnable) <= 0\n");
                  int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level

                  vReturnToCenterACK(0x5F, 0x10);

                  _ControlStrategy.DBit = DataMessageIn.packet[9];
                  EffectTime = DataMessageIn.packet[10] * 60;                                   //EffectTime save using Sec, but protocal using Min.

                  data[0] = 0x5F;
                  data[1] = 0x91;
                  data[2] = DataMessageIn.packet[9];
                  data[3] = DataMessageIn.packet[10];

                  smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);
                  smem.vSetINTData(TC92_iEffectTime, EffectTime);
                //then save!?

            //For disable Dynamic

                  if(MACHINELOCATE == MACHINELOCATEATYULIN)                                     // For CCJ
                  {

                    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
                    _MsgOK.InnerOrOutWard = cOutWard;
                    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

                  } else {

                    if(_ControlStrategy.switchBit.b1 == true &&
                       _ControlStrategy.switchBit.b5 == false &&
                       _ControlStrategy.switchBit.b6 == false    )
                    {
                      if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )
                        _intervalTimer.vAllDynamicToTODCount(10);
                //      _intervalTimer.vAllDynamicToTODCount(EffectTime);

                    }

                    else if(_ControlStrategy.DBit == 0x30)
                    {
                      printf("Get Amegids 0F 10 30\n\n");
                      if( stc.Lock_to_LoadControlStrategy() == STRATEGY_TOD ) {
                        printf("Get Amegids, now is TOD\n\n");
                        stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC);
                        printf("Get Amegids, now changing to ALLDYNAMIC\n\n");

                        int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
                        if(usiCurrentSubphaseStep == 0) {
                          if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
                            _intervalTimer.vAllDynamicToTODCount(EffectTime);
                          }
                          else {
                            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_green_time));
                          }
                        }
                        else if(usiCurrentSubphaseStep == 1)
                          _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
                        else if(usiCurrentSubphaseStep == 2)
                          if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
                            _intervalTimer.vAllDynamicToTODCount(EffectTime);
                          }
                          else {
                            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedred_time));
                          }
                        else if(usiCurrentSubphaseStep == 3)
                          _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_yellow_time));
                        else if(usiCurrentSubphaseStep == 4)
                          _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_allred_time));

                      }
                    }
                  }
                  */


        }

        return true;

    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQurryControlStrategy5F40(MESSAGEOK DataMessageIn)
{
    try
    {
        DATA_Bit _ControlStrategy;
        int EffectTime;
        int iReturnCommandSet;

        _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);              //Read Data from share memory
        EffectTime = smem.vGetINTData(TC92_iEffectTime);
        iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level

        if(iReturnCommandSet <= 1)
        {
            vReturnToCenterNACK(0x5F, 0x40, 0x80, 0x00);    // not include level "A""
            return false;
        }
        if( DataMessageIn.packetLength < 12 )
        {
            vReturnToCenterNACK(0x5F, 0x40, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 12 )
        {
            vReturnToCenterNACK(0x5F, 0x40, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }
//Don't respond  vReturnToCenterACK(0x5F, 0x40);

        unsigned char data[6];
        data[0] = 0x5F;
        data[1] = 0xC0;
        data[2] = _ControlStrategy.DBit;
        data[3] = EffectTime / 60;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        return true;

    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWriteReverseTimeData5F11(MESSAGEOK DataMessageIn)
{
    try
    {

        int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level
        if(iReturnCommandSet <= 1)
        {
            vReturnToCenterNACK(0x5F, 0x11, 0x80, 0x00);    // not include level "A""
            return false;
        }

        // here have error, should check first.
        vReturnToCenterACK(0x5F, 0x11);



        int i=0;

//  stc.Lock_to_Reset_Segment_for_Center(DataMessageIn.packet[9],DataMessageIn.packet[10]);
//  stc.Lock_to_Load_WeekDaySegment_for_Center();

// need check first
        stc._for_center_reversetime.ucNonRevLight = 0;

        stc._for_center_reversetime.usiDirectIn    = DataMessageIn.packet[9];
        stc._for_center_reversetime.usiHourStartIn = DataMessageIn.packet[10];
        stc._for_center_reversetime.usiMinStartIn  = DataMessageIn.packet[11];
        stc._for_center_reversetime.usiHourEndIn   = DataMessageIn.packet[12];
        stc._for_center_reversetime.usiMinEndIn    = DataMessageIn.packet[13];

        stc._for_center_reversetime.usiDirectOut    = DataMessageIn.packet[14];
        stc._for_center_reversetime.usiHourStartOut = DataMessageIn.packet[15];
        stc._for_center_reversetime.usiMinStartOut  = DataMessageIn.packet[16];
        stc._for_center_reversetime.usiHourEndOut   = DataMessageIn.packet[17];
        stc._for_center_reversetime.usiMinEndOut    = DataMessageIn.packet[18];

        stc._for_center_reversetime.usiClearTime    = DataMessageIn.packet[19]*10;
        stc._for_center_reversetime.usiFlashGreen = DataMessageIn.packet[20]*10;
        stc._for_center_reversetime.usiGreenTime  = DataMessageIn.packet[21]*10;

        stc._for_center_reversetime._reverse_time_type  = DataMessageIn.packet[22];

        if(stc._for_center_reversetime.usiHourStartIn*60+stc._for_center_reversetime.usiMinStartIn != 0 || stc._for_center_reversetime.usiHourEndIn*60+stc._for_center_reversetime.usiMinEndIn != 0)
        {
            if(stc._for_center_reversetime.usiHourStartOut*60+stc._for_center_reversetime.usiMinStartOut == 0 && stc._for_center_reversetime.usiHourEndOut*60+stc._for_center_reversetime.usiMinEndOut == 0)
            {
                stc._for_center_reversetime.ucNonRevLight = 1;
            }
        }
        else if(stc._for_center_reversetime.usiHourStartOut*60+stc._for_center_reversetime.usiMinStartOut != 0 || stc._for_center_reversetime.usiHourEndOut*60+stc._for_center_reversetime.usiMinEndOut != 0)
        {
            if(stc._for_center_reversetime.usiHourStartIn*60+stc._for_center_reversetime.usiMinStartIn == 0 && stc._for_center_reversetime.usiHourEndIn*60+stc._for_center_reversetime.usiMinEndIn == 0)
            {
                stc._for_center_reversetime.ucNonRevLight = 2;
            }
        }
        else      //unknow type.
        {
            stc._for_center_reversetime.ucNonRevLight = 0;
        }


        stc.Lock_to_Save_ReverseTime_Step1(0);


//OTMARKPRINTF  printf("segment_type%d\n",stc._for_center_segment._segment_type);
//OTMARKPRINTF  printf("segment_Count%d\n",stc._for_center_segment._segment_count);


        int numWeekDay = DataMessageIn.packet[23];

//OTMARKPRINTF  printf("numWeekDay:%d\n",numWeekDay);

        for (int j=0; j<numWeekDay; j++)
        {

            //OTMARKPRINTF  printf("%3x--%d\n",DataMessageIn.packet[12+3*i+j],stc._for_center_segment._segment_type);

            switch (DataMessageIn.packet[24+j])
            {
            case 0x01:
                stc._for_center_weekdayrev[0]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x02:
                stc._for_center_weekdayrev[1]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x03:
                stc._for_center_weekdayrev[2]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x04:
                stc._for_center_weekdayrev[3]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x05:
                stc._for_center_weekdayrev[4]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x06:
                stc._for_center_weekdayrev[5]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x07:
                stc._for_center_weekdayrev[6]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x11:
                stc._for_center_weekdayrev[7]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x12:
                stc._for_center_weekdayrev[8]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x13:
                stc._for_center_weekdayrev[9]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x14:
                stc._for_center_weekdayrev[10]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x15:
                stc._for_center_weekdayrev[11]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x16:
                stc._for_center_weekdayrev[12]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            case 0x17:
                stc._for_center_weekdayrev[13]._reverse_time_type=stc._for_center_reversetime._reverse_time_type;
                break;
            default:
                break;
            }
        }

//  stc.Lock_to_Save_Segment_from_Center();
        stc.Lock_to_Save_WeekDayReversetime_from_Center();

        return true;

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryReverseTimeData5F41(MESSAGEOK DataMessageIn)
{
    try
    {
        unsigned char data[256];

        unsigned char ucQueryReverseTimeType = DataMessageIn.packet[9];
        unsigned char ucWeekDay = DataMessageIn.packet[10];

        if( DataMessageIn.packetLength < 14)
        {
            vReturnToCenterNACK(0x5F, 0x41, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 14)
        {
            vReturnToCenterNACK(0x5F, 0x41, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

        if(ucQueryReverseTimeType == 0xFF)
        {
            stc.Lock_to_Load_ReverseTime_Step1(2, ucWeekDay);
        }
        else
        {

            /*
            bSegStatus = smem.vGetTCPhasePlanSegTypeData(TC_SegType, iQuerySegmentType);
            if(bSegStatus == false) {
              vReturnToCenterNACK(0x5F, 0x46, 0x02, 0x0); return false;
            }
            */
            stc.Lock_to_Load_ReverseTime_Step1(0, ucQueryReverseTimeType);              //Zero for center
        }

        stc.Lock_to_Load_WeekDayReverseTime_for_Center();
        ;
        int iDataPtr;
        int iNumWeekDayPrt = 0;
        int iNumWeekDay = 0;
        data[0] = 0x5F;
        data[1] = 0xC1;
        data[2] = stc._for_center_reversetime.usiDirectIn;
        data[3] = stc._for_center_reversetime.usiHourStartIn;
        data[4] = stc._for_center_reversetime.usiMinStartIn;
        data[5] = stc._for_center_reversetime.usiHourEndIn;
        data[6] = stc._for_center_reversetime.usiMinEndIn;
        data[7] = stc._for_center_reversetime.usiDirectOut;
        data[8] = stc._for_center_reversetime.usiHourStartOut;
        data[9] = stc._for_center_reversetime.usiMinStartOut;
        data[10] = stc._for_center_reversetime.usiHourEndOut;
        data[11] = stc._for_center_reversetime.usiMinEndOut;

        data[12] = (stc._for_center_reversetime.usiClearTime / 10);
        data[13] = (stc._for_center_reversetime.usiFlashGreen / 10);
        data[14] = (stc._for_center_reversetime.usiGreenTime / 10);
        data[15] = stc._for_center_reversetime._reverse_time_type;

        iNumWeekDayPrt = 16;
        iDataPtr = 16;

        if(stc._for_center_weekdayrev[0]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x01;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[1]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x02;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[2]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x03;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[3]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x04;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[4]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x05;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[5]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x06;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[6]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x07;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[7]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x11;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[8]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x12;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[9]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x13;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[10]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x14;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[11]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x15;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[12]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x16;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayrev[13]._reverse_time_type == ucQueryReverseTimeType)
        {
            data[iDataPtr] = 0x17;
            iNumWeekDay++;
            iDataPtr++;
        }
        data[iNumWeekDayPrt] = iNumWeekDay;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;
    }
    catch (...) {}
}


//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWriteReverseTimeData5F12(MESSAGEOK DataMessageIn)
{
    try
    {

        int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level
        if(iReturnCommandSet <= 1)
        {
            vReturnToCenterNACK(0x5F, 0x12, 0x80, 0x00);    // not include level "A""
            return false;
        }

        // here have error, should check first.
        vReturnToCenterACK(0x5F, 0x12);


        int i=0;

//  stc.Lock_to_Reset_Segment_for_Center(DataMessageIn.packet[9],DataMessageIn.packet[10]);
//  stc.Lock_to_Load_WeekDaySegment_for_Center();

// need check first

        stc._for_center_reversetime.usiDirectIn    = DataMessageIn.packet[9];
        stc._for_center_reversetime.usiHourStartIn = DataMessageIn.packet[10];
        stc._for_center_reversetime.usiMinStartIn  = DataMessageIn.packet[11];
        stc._for_center_reversetime.usiHourEndIn   = DataMessageIn.packet[12];
        stc._for_center_reversetime.usiMinEndIn    = DataMessageIn.packet[13];

        stc._for_center_reversetime.usiDirectOut    = DataMessageIn.packet[14];
        stc._for_center_reversetime.usiHourStartOut = DataMessageIn.packet[15];
        stc._for_center_reversetime.usiMinStartOut  = DataMessageIn.packet[16];
        stc._for_center_reversetime.usiHourEndOut   = DataMessageIn.packet[17];
        stc._for_center_reversetime.usiMinEndOut    = DataMessageIn.packet[18];

        stc._for_center_reversetime.usiClearTime    = DataMessageIn.packet[19];
        stc._for_center_reversetime.usiFlashGreen = DataMessageIn.packet[20];
        stc._for_center_reversetime.usiGreenTime  = DataMessageIn.packet[21];

        stc._for_center_reversetime._reverse_time_type  = DataMessageIn.packet[22];

        stc.Lock_to_Save_ReverseTime_Step1(0);                                        // (0) means for center

        stc._for_center_holidayrev._reverse_time_type  = DataMessageIn.packet[22];
        stc._for_center_holidayrev._start_year  = DataMessageIn.packet[23] + 1911;
        stc._for_center_holidayrev._start_month = DataMessageIn.packet[24];
        stc._for_center_holidayrev._start_day   = DataMessageIn.packet[25];
        stc._for_center_holidayrev._end_year    = DataMessageIn.packet[26] + 1911;
        stc._for_center_holidayrev._end_month   = DataMessageIn.packet[27];
        stc._for_center_holidayrev._end_day     = DataMessageIn.packet[28];

        stc.Lock_to_Save_HoliDayReverseTime_Step1(0);

        return true;

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryReverseTimeData5F42(MESSAGEOK DataMessageIn)
{
    try
    {
        unsigned char data[256];

        unsigned char ucQueryReverseTimeType = DataMessageIn.packet[9];

        if( DataMessageIn.packetLength < 13)
        {
            vReturnToCenterNACK(0x5F, 0x42, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 13)
        {
            vReturnToCenterNACK(0x5F, 0x42, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

        stc.Lock_to_Load_HoliDayReverseTime_for_Center(ucQueryReverseTimeType);

        ;
        int iDataPtr;
        int iNumWeekDayPrt = 0;
        int iNumWeekDay = 0;
        data[0] = 0x5F;
        data[1] = 0xC2;
        data[2] = stc._for_center_reversetime.usiDirectIn;
        data[3] = stc._for_center_reversetime.usiHourStartIn;
        data[4] = stc._for_center_reversetime.usiMinStartIn;
        data[5] = stc._for_center_reversetime.usiHourEndIn;
        data[6] = stc._for_center_reversetime.usiMinEndIn;
        data[7] = stc._for_center_reversetime.usiDirectOut;
        data[8] = stc._for_center_reversetime.usiHourStartOut;
        data[9] = stc._for_center_reversetime.usiMinStartOut;
        data[10] = stc._for_center_reversetime.usiHourEndOut;
        data[11] = stc._for_center_reversetime.usiMinEndOut;

        data[12] = stc._for_center_reversetime.usiClearTime;
        data[13] = stc._for_center_reversetime.usiFlashGreen;
        data[14] = stc._for_center_reversetime.usiGreenTime;
        data[15] = stc._for_center_reversetime._reverse_time_type;

        data[16] = stc._for_center_holidayrev._start_year - 1911;
        data[17] = stc._for_center_holidayrev._start_month;
        data[18] = stc._for_center_holidayrev._start_day;
        data[19] = stc._for_center_holidayrev._end_year - 1911;
        data[20] = stc._for_center_holidayrev._end_month;
        data[21] = stc._for_center_holidayrev._end_day;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 22, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;
    }
    catch (...) {}
}



//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWritePhase5F13(MESSAGEOK DataMessageIn)
{
    try
    {
//OTMARKPRINTF  printf("[OT] Go to 5F13.\n");
        int i=0;
        bool bMollacOK=false;
        unsigned char ucStepSignal[8];
        unsigned short int usiTMP;
        unsigned char ucTMP;

        //OT Debug Dir
        int iLightOutNo;
        unsigned short int usiShift = 0;
        unsigned char ucLightConvert[8][8];
        for(int i = 0; i < 8; i++)                                                    //init
            for(int j = 0; j < 8; j++)
                ucLightConvert[i][j] = 0x0;

//OT Debug 950322

        int iPhaseOrder = DataMessageIn.packet[9];
        DATA_Bit _SignalMap;
        _SignalMap.DBit = DataMessageIn.packet[10];
        int iSignalCount = DataMessageIn.packet[11];
        int iSubPhaseCount = DataMessageIn.packet[12];

        if( DataMessageIn.packetLength < 16+(iSignalCount*iSubPhaseCount) )
        {
            vReturnToCenterNACK(0x5F, 0x13, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 16+(iSignalCount*iSubPhaseCount) )
        {
            vReturnToCenterNACK(0x5F, 0x13, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

        int iSignalMapCountSignal = 0;
        if(_SignalMap.switchBit.b1) iSignalMapCountSignal++;
        if(_SignalMap.switchBit.b2) iSignalMapCountSignal++;
        if(_SignalMap.switchBit.b3) iSignalMapCountSignal++;
        if(_SignalMap.switchBit.b4) iSignalMapCountSignal++;
        if(_SignalMap.switchBit.b5) iSignalMapCountSignal++;
        if(_SignalMap.switchBit.b6) iSignalMapCountSignal++;
        if(_SignalMap.switchBit.b7) iSignalMapCountSignal++;
        if(_SignalMap.switchBit.b8) iSignalMapCountSignal++;
        if( iSignalMapCountSignal != iSignalCount)
        {
            vReturnToCenterNACK(0x5F, 0x13, 0x04, 0x02);    //signalmap error
            return false;
        }

        vReturnToCenterACK(0x5F, 0x13);


        //OT Debug Dir  copy data to convert array
//======================
        for(int i = 0; i < iSubPhaseCount; i++)
            for(int j = 0; j < iSignalCount; j++)
                ucLightConvert[i][j] = DataMessageIn.packet[i*iSignalCount + j + 13];

        if(_SignalMap.switchBit.b1 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirN);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                    DataMessageIn.packet[i*iSignalCount + 13 + iLightOutNo] = ucLightConvert[i][usiShift];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b2 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirNE);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                    DataMessageIn.packet[i*iSignalCount + 13 + iLightOutNo] = ucLightConvert[i][usiShift];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b3 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirE);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                    DataMessageIn.packet[i*iSignalCount + 13 + iLightOutNo] = ucLightConvert[i][usiShift];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b4 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirES);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                    DataMessageIn.packet[i*iSignalCount + 13 + iLightOutNo] = ucLightConvert[i][usiShift];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b5 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirS);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                    DataMessageIn.packet[i*iSignalCount + 13 + iLightOutNo] = ucLightConvert[i][usiShift];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b6 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirSW);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                    DataMessageIn.packet[i*iSignalCount + 13 + iLightOutNo] = ucLightConvert[i][usiShift];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b7 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirW);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                    DataMessageIn.packet[i*iSignalCount + 13 + iLightOutNo] = ucLightConvert[i][usiShift];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b8 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirWN);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                    DataMessageIn.packet[i*iSignalCount + 13 + iLightOutNo] = ucLightConvert[i][usiShift];
                usiShift++;
            }
        }

//======================


        bMollacOK = stc.Lock_to_Reset_Phase_for_Center(iPhaseOrder, iSubPhaseCount, iSignalCount);

        if (bMollacOK)
        {

            stc._for_center_phase._phase_order=iPhaseOrder;
            stc._for_center_phase._signal_map=_SignalMap.DBit;
            stc._for_center_phase._signal_count=iSignalCount;
            stc._for_center_phase._subphase_count=iSubPhaseCount;
            stc._for_center_phase._total_step_count = iSubPhaseCount * 5;  //¤­­Ó¨B¶¥

            for(int j = 0; j < iSubPhaseCount; j++)
            {

                for(int qq=0; qq<iSignalCount; qq++)
                {
                    ucStepSignal[qq] = DataMessageIn.packet[iSignalCount*j+13+qq];   //¥ý§â¦P¤@®É¬Û¤§¿O¸¹©ñ¨ìarray
                }

                for( int k=0; k< stc._for_center_phase._ptr_subphase_step_count[j]; k++ )    //¥ý§â©Ò¦³¨B¶¥³£¶ñ¥þ¬õ
                {
                    for(int l=0; l<stc._for_center_phase._signal_count; l++)
                    {
                        stc._for_center_phase._ptr_subphase_step_signal_status[j][k][l] = 0x3003;
                    }
                }

                for( int k=0; k< stc._for_center_phase._ptr_subphase_step_count[j]; k++ )
                {
                    for(int l=0; l<stc._for_center_phase._signal_count; l++)
                    {
                        if(k==0)   //step 1
                        {
                            ucTMP = ucStepSignal[l];
                        }
                        else if(k==1)
                        {
                            ucTMP = oTools.vGuessLightStep(2, ucStepSignal[l]);
                        }
                        else if(k==2)
                        {
                            ucTMP = oTools.vGuessLightStep(2, ucStepSignal[l]);
                            ucTMP = oTools.vGuessLightStep(3, ucTMP);
                        }
                        else if(k==3)
                        {
                            ucTMP = oTools.vGuessLightStep(2, ucStepSignal[l]);
                            ucTMP = oTools.vGuessLightStep(3, ucTMP);
                            ucTMP = oTools.vGuessLightStep(4, ucTMP);
                        }
                        else if(k==4)
                        {
                            ucTMP = oTools.vGuessLightStep(2, ucStepSignal[l]);
                            ucTMP = oTools.vGuessLightStep(3, ucTMP);
                            ucTMP = oTools.vGuessLightStep(4, ucTMP);
                            ucTMP = oTools.vGuessLightStep(5, ucTMP);
                        }
                        //OTMARKPRINTF  printf("ucTMP = %X.\n",ucTMP);

                        usiTMP = oTools.vVer30LightToCCTLight(ucTMP);
                        stc._for_center_phase._ptr_subphase_step_signal_status[j][k][l] = usiTMP;

                    }
                }
            }
        }

        //OTMARKPRINTF  printf("[OT] Make OK.\n");

        smem.vWriteMsgToDOM("WritePhaseBy5F13");

        stc.Lock_to_Save_Phase_from_Center();             //Àx¦s¦^¥h
        smem.vSetINTData(TC92_iUpdatePhaseData, 1);
        smem.vSetTCPhasePlanSegTypeData(TC_Phase, iPhaseOrder, true);
        //OTMARKPRINTF  printf("[OT] Save Ok.\n");

//OTMARKPRINTF  printf("SAVE Phase:%d OK!!\n",iPhaseOrder);

        return true;

    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryPhase5F43(MESSAGEOK DataMessageIn)
{
    try
    {
        //CHeck

        //OT Debug Dir
        int iLightOutNo;
        unsigned short int usiShift = 0;
        unsigned char ucLightConvert[8][8];
        DATA_Bit _SignalMap;
        for(int i = 0; i < 8; i++)                                                    //init
            for(int j = 0; j < 8; j++)
                ucLightConvert[i][j] = 0x0;

//Don't respond  vReturnToCenterACK(0x5F, 0x43);

        int iQueryPhaseOrder = DataMessageIn.packet[9];

        bool bPhaseStatus;
        bPhaseStatus = smem.vGetTCPhasePlanSegTypeData(TC_Phase, iQueryPhaseOrder);

        if(bPhaseStatus == false)
        {
            vReturnToCenterNACK(0x5F, 0x43, 0x02, 0x0);
            return false;
        }

        stc.Lock_to_Load_Phase_for_Center(iQueryPhaseOrder);

        unsigned char data[256];
        int iDataPtr = 6; //Form 6 Start to send SignalStatus
        data[0] = 0x5F;
        data[1] = 0xC3;
        data[2] = stc._for_center_phase._phase_order;
        data[3] = stc._for_center_phase._signal_map;
        data[4] = stc._for_center_phase._signal_count;
        data[5] = stc._for_center_phase._subphase_count;

        int usiTMP;
        _SignalMap.DBit = data[3];
        int iSignalCount = data[4];
        int iSubPhaseCount = data[5];

        for(int i = 0; i < stc._for_center_phase._subphase_count; i++)
        {
            for(int j = 0; j < stc._for_center_phase._signal_count; j++)
            {
                usiTMP = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][j];
                data[iDataPtr] = oTools.vCCTLightToVer30Light(usiTMP);
                iDataPtr++;
            }
        }

        //OT Debug Dir  copy data to convert array
//======================
        for(int i = 0; i < iSubPhaseCount; i++)                                              //stc._for_center_phase._subphase_count
            for(int j = 0; j < iSignalCount; j++)                                            //stc._for_center_phase._signal_count
                ucLightConvert[i][j] = data[i*data[4] + j + 6];

        /*
          printf("printfMsg _SignalMap.switchBit.b1:%d\n", _SignalMap.switchBit.b1);
          printf("printfMsg _SignalMap.switchBit.b2:%d\n", _SignalMap.switchBit.b2);
          printf("printfMsg _SignalMap.switchBit.b3:%d\n", _SignalMap.switchBit.b3);
          printf("printfMsg _SignalMap.switchBit.b4:%d\n", _SignalMap.switchBit.b4);
          printf("printfMsg _SignalMap.switchBit.b5:%d\n", _SignalMap.switchBit.b5);
          printf("printfMsg _SignalMap.switchBit.b6:%d\n", _SignalMap.switchBit.b6);
          printf("printfMsg _SignalMap.switchBit.b7:%d\n", _SignalMap.switchBit.b7);
          printf("printfMsg _SignalMap.switchBit.b8:%d\n", _SignalMap.switchBit.b8);
          printf("printfMsg usiShift:%d\n", usiShift);
        */
        if(_SignalMap.switchBit.b1 == true && usiShift < data[4])
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirN);
            printf("printfMsg dirN iLightOutNo:%d usiShift:%d\n", iLightOutNo, usiShift);
            if(iLightOutNo < iSignalCount)
            {
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    data[i*iSignalCount + 6 + usiShift] = ucLightConvert[i][iLightOutNo];
                    printf("printfMsg dirN locate:%d\n", i*iSignalCount + 6 + usiShift);
                }
                usiShift++;
            }
            printf("printfMsg go re-defg.\n");
        }
        if(_SignalMap.switchBit.b2 == true && usiShift < data[4])
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirNE);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg dirNE iLightOutNo:%d usiShift:%d\n", iLightOutNo, usiShift);
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    data[i*iSignalCount + 6 + usiShift] = ucLightConvert[i][iLightOutNo];
                    printf("printfMsg dirNE locate:%d\n", i*iSignalCount + 6 + usiShift);
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b3 == true && usiShift < data[4])
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirE);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg dirE iLightOutNo:%d usiShift:%d\n", iLightOutNo, usiShift);
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    data[i*iSignalCount + 6 + usiShift] = ucLightConvert[i][iLightOutNo];
                    printf("printfMsg dirE locate:%d\n", i*iSignalCount + 6 + usiShift);
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b4 == true && usiShift < data[4])
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirES);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg dirES iLightOutNo:%d usiShift:%d\n", iLightOutNo, usiShift);
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    data[i*iSignalCount + 6 + usiShift] = ucLightConvert[i][iLightOutNo];
                    printf("printfMsg dirES locate:%d\n", i*iSignalCount + 6 + usiShift);
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b5 == true && usiShift < data[4])
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirS);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg dirS iLightOutNo:%d usiShift:%d\n", iLightOutNo, usiShift);
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    data[i*iSignalCount + 6 + usiShift] = ucLightConvert[i][iLightOutNo];
                    printf("printfMsg dirS locate:%d\n", i*iSignalCount + 6 + usiShift);
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b6 == true && usiShift < data[4])
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirSW);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg dirSW iLightOutNo:%d usiShift:%d\n", iLightOutNo, usiShift);
                for(int i = 0; i < iSubPhaseCount; i++)
                    data[i*iSignalCount + 6 + usiShift] = ucLightConvert[i][iLightOutNo];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b7 == true && usiShift < data[4])
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirW);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg dirW iLightOutNo:%d usiShift:%d\n", iLightOutNo, usiShift);
                for(int i = 0; i < iSubPhaseCount; i++)
                    data[i*iSignalCount + 6 + usiShift] = ucLightConvert[i][iLightOutNo];
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b8 == true && usiShift < data[4])
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirWN);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg dirWN iLightOutNo:%d usiShift:%d\n", iLightOutNo, usiShift);
                for(int i = 0; i < iSubPhaseCount; i++)
                    data[i*iSignalCount + 6 + usiShift] = ucLightConvert[i][iLightOutNo];
                usiShift++;
            }
        }


        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;

    }
    catch(...) {}
}


//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWritePlan5F14(MESSAGEOK DataMessageIn)
{
    try
    {

        if( DataMessageIn.packetLength < 14+(7*DataMessageIn.packet[10]) )
        {
            vReturnToCenterNACK(0x5F, 0x14, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 14+(7*DataMessageIn.packet[10]) )
        {
            vReturnToCenterNACK(0x5F, 0x14, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

        if(DataMessageIn.InnerOrOutWard != cComingFromScreen)
        {
            vReturnToCenterACK(0x5F, 0x14);
        }
        screenLast92TCPlanSegmentUpdate.DisplayPlanUpdate();
      shrinkAPP.Init_DBupdateInfo();
        /*
        PlanID(®É¨î­p¹º):[9]
        SubPhaseCount(¤À¬Û¼Æ):[10]

        MinGreen(µuºñ):[11]
        MaxGreen(ªøºñ):[12],[13]
        Yellow(¶À):[14]
        AllRed(¥þ¬õ):[15]
        PedGreenFlash(¦æ¤Hºñ°{):[16]
        PedRed(¦æ¤H¬õ):[17]
        */

        int i=0;
        bool bMollacOK=false;

//OT Debug 0523
        if (DataMessageIn.packet[10]==0)                    //°{¥ú®É¬Û
            bMollacOK=stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9],1);
        else  bMollacOK=stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9],DataMessageIn.packet[10]);

        if (bMollacOK)
        {

            stc._for_center_plan._planid=DataMessageIn.packet[9];
            stc._for_center_plan._subphase_count=DataMessageIn.packet[10];

            //OTMARKPRINTF  printf("Plan:%d, PhaseCount:%d\n",stc._for_center_plan._planid,stc._for_center_plan._subphase_count);

            if (stc._for_center_plan._subphase_count==0)     //°{¥ú®É¬Û
            {
                stc._for_center_plan._subphase_count=1;      //µêÀÀ¤@­ÓPhase

                stc._for_center_plan._ptr_subplaninfo[0]._min_green=0;
                stc._for_center_plan._ptr_subplaninfo[0]._max_green=0;
                stc._for_center_plan._ptr_subplaninfo[0]._yellow=0;
                stc._for_center_plan._ptr_subplaninfo[0]._allred=0;
                stc._for_center_plan._ptr_subplaninfo[0]._pedgreen_flash=0;
                stc._for_center_plan._ptr_subplaninfo[0]._pedred=0;

            }
            else
            {

                for (i=0; i<stc._for_center_plan._subphase_count; i++)
                {

                    //OTMARKPRINTF  printf("Phase:%d\n",i+1);

                    stc._for_center_plan._ptr_subplaninfo[i]._min_green=DataMessageIn.packet[11+7*i];
                    stc._for_center_plan._ptr_subplaninfo[i]._max_green=DataMessageIn.packet[12+7*i]*256+DataMessageIn.packet[13+7*i];
                    stc._for_center_plan._ptr_subplaninfo[i]._yellow=DataMessageIn.packet[14+7*i];
                    stc._for_center_plan._ptr_subplaninfo[i]._allred=DataMessageIn.packet[15+7*i];
                    stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash=DataMessageIn.packet[16+7*i];
                    stc._for_center_plan._ptr_subplaninfo[i]._pedred=DataMessageIn.packet[17+7*i];

                    //OTMARKPRINTF  printf("minGreen:%d,maxGreen:%d,yellow:%d,allred:%d,pedGFlash:%d,pedRed%d\n",
//OTMARKPRINTF             stc._for_center_plan._ptr_subplaninfo[i]._min_green,stc._for_center_plan._ptr_subplaninfo[i]._max_green,stc._for_center_plan._ptr_subplaninfo[i]._yellow,
//OTMARKPRINTF             stc._for_center_plan._ptr_subplaninfo[i]._allred,stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash,stc._for_center_plan._ptr_subplaninfo[i]._pedred);

                }
            }
            printf("-------------------------5F14--------------------------\n");
            stc.Lock_to_Save_Plan_from_Center();             //Àx¦s¦^¥h

            //OTMARKPRINTF  printf("SAVE Plan:%d OK!!\n",DataMessageIn.packet[9]);


            return true;
        }
    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryPlan5F44(MESSAGEOK DataMessageIn)
{
    try
    {

        int iQueryPlan = DataMessageIn.packet[9];

        bool bPlanStatus;
        bPlanStatus = smem.vGetTCPhasePlanSegTypeData(TC_Plan, iQueryPlan);
        if(bPlanStatus == false)
        {
            vReturnToCenterNACK(0x5F, 0x44, 0x02, 0x0);
            return false;
        }
        if( DataMessageIn.packetLength < 13)
        {
            vReturnToCenterNACK(0x5F, 0x44, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 13)
        {
            vReturnToCenterNACK(0x5F, 0x44, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }


        stc.Lock_to_Load_Plan_for_Center(iQueryPlan);

        unsigned char data[256];
        int iDataPtr = 4; //Form 6 Start to send SignalStatus
        data[0] = 0x5F;
        data[1] = 0xC4;
        data[2] = stc._for_center_plan._planid;
        data[3] = stc._for_center_plan._subphase_count;
        for(int i = 0; i < stc._for_center_plan._subphase_count; i++)
        {
            data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._min_green;
            iDataPtr++;
//OT Debug 950410  -2 line
            data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._max_green / 256;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._max_green % 256;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._yellow;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._allred;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._pedred;
            iDataPtr++;
        }

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWritePlanDBMS5F15(MESSAGEOK DataMessageIn)
{
    try
    {

        int iPhaseOrder = DataMessageIn.packet[11];
        int iSubphaseCount = DataMessageIn.packet[12];

        if(smem.vGetTCPhasePlanSegTypeData(TC_Phase, iPhaseOrder) == false)
        {
            vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x03);
            return false;
        }
        stc.Lock_to_Load_Phase_for_Center(iPhaseOrder);

        printf("5F15: stc._for_center_phase._subphase_count:%d\n", stc._for_center_phase._subphase_count);
        printf("5F15: iSubphaseCount:%d\n", iSubphaseCount);

        if( stc._for_center_phase._subphase_count != iSubphaseCount && iSubphaseCount != 0)
        {
            vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x04);
            return false;
        }

        if( DataMessageIn.packetLength < 20+(2*DataMessageIn.packet[12]))
        {
            vReturnToCenterNACK(0x5F, 0x15, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 20+(2*DataMessageIn.packet[12]))
        {
            vReturnToCenterNACK(0x5F, 0x15, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

//OT Debug 0523
//  vReturnToCenterACK(0x5F, 0x15);

        /*
        PlanID(®É¨î­p¹º):[9]
        Direct(°ò·Ç¤è¦V):[10]
        PhaseOrder(®É¬Û½s¸¹):[11]
        SubPhaseCount(¤À¬Û¼Æ):[12]

        Green(ºñ¿O®É¶¡):[13][14],[15][16],[17][18]
        CycleTime(¶g´Á):[19][20]
        Offset(®É®t):[21][22]
        */

        int i=0;
        bool bMollacOK=false;

        //OT Debug 0523
        bool bRet;

        if (iSubphaseCount == 0)      //°{¥ú®É¬Û
        {
            bMollacOK = stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9],1);
        }
        else
        {
            bMollacOK = stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9], iSubphaseCount);
        }

        if (bMollacOK)
        {

            stc._for_center_plan._planid=DataMessageIn.packet[9];
            stc._for_center_plan._dir=DataMessageIn.packet[10];
            printf("Plan:%d,Dir:%d\n",stc._for_center_plan._planid,stc._for_center_plan._dir);

            /*
                  if (iPhaseOrder == 0x30) iPhaseOrder = 0x20;
                  else if (iPhaseOrder==0xB0) iPhaseOrder = 0x80;
            */

            stc._for_center_plan._phase_order = iPhaseOrder;
            stc._for_center_plan._subphase_count= iSubphaseCount;
            printf("Phase_Order:%d,Phase_Count:%d\n",stc._for_center_plan._phase_order,stc._for_center_plan._subphase_count);

            for (i=0; i<stc._for_center_plan._subphase_count; i++)
            {
                printf("Phase:%d\n",i+1);
                stc._for_center_plan._ptr_subplaninfo[i]._green=DataMessageIn.packet[13+2*i]*256+DataMessageIn.packet[14+2*i];
                printf("Green-A:%d\n",stc._for_center_plan._ptr_subplaninfo[i]._green);
                if(stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash > 0 && stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash < stc._for_center_plan._ptr_subplaninfo[i]._green)
                {
                    stc._for_center_plan._ptr_subplaninfo[i]._green = stc._for_center_plan._ptr_subplaninfo[i]._green - stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
                    printf("Special A!\n");
                }
                if(stc._for_center_plan._ptr_subplaninfo[i]._pedred > 0 && stc._for_center_plan._ptr_subplaninfo[i]._pedred < stc._for_center_plan._ptr_subplaninfo[i]._green)
                {
                    stc._for_center_plan._ptr_subplaninfo[i]._green = stc._for_center_plan._ptr_subplaninfo[i]._green - stc._for_center_plan._ptr_subplaninfo[i]._pedred;
                    printf("Special B!\n");
                }
                printf("Green-B:%d\n",stc._for_center_plan._ptr_subplaninfo[i]._green);
            }

            stc._for_center_plan._cycle_time=DataMessageIn.packet[13+2*i]*256+DataMessageIn.packet[14+2*i];
            printf("cycle_time:%d\n",stc._for_center_plan._cycle_time);
            stc._for_center_plan._offset=DataMessageIn.packet[15+2*i]*256+DataMessageIn.packet[16+2*i];
            printf("offset:%d\n",stc._for_center_plan._offset);

            if (stc._for_center_plan._subphase_count==0)              //°{¥ú®É¬Û
            {
                stc._for_center_plan._subphase_count=1;               //µêÀÀ¤@­ÓPhase

                //OT Debug 0523
                stc._for_center_plan._ptr_subplaninfo[i]._green=10;
            }

//OT Debug 0523
            bRet = stc.Lock_to_Save_Plan_from_Center();                      //Àx¦s¦^¥h
            if(bRet) smem.vSetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[9], true);
            else
            {
                vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x05);
                return false;
            }
            vReturnToCenterACK(0x5F, 0x15);
//      smem.vSaveLast92TC_5F15Time();
            screenLast92TCPlanSegmentUpdate.DisplayPlanUpdate();
          shrinkAPP.Init_DBupdateInfo();
            printf("SAVE Plan:%d OK!\n",DataMessageIn.packet[9]);

        }
    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryPlanDBMS5F45(MESSAGEOK DataMessageIn)
{
    try
    {
        int iQueryPlan = DataMessageIn.packet[9];
        unsigned short int usiGreenTMP;

        bool bPlanStatus;
        bPlanStatus = smem.vGetTCPhasePlanSegTypeData(TC_Plan, iQueryPlan);
        if(bPlanStatus == false)
        {
            vReturnToCenterNACK(0x5F, 0x45, 0x02, 0x0);
            return false;
        }
        if( DataMessageIn.packetLength < 13)
        {
            vReturnToCenterNACK(0x5F, 0x45, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 13)
        {
            vReturnToCenterNACK(0x5F, 0x45, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }


        stc.Lock_to_Load_Plan_for_Center(iQueryPlan);

        unsigned char data[256];
        int iDataPtr = 6;
        data[0] = 0x5F;
        data[1] = 0xC5;
        data[2] = stc._for_center_plan._planid;
        data[3] = stc._for_center_plan._dir;
        data[4] = stc._for_center_plan._phase_order;
        data[5] = stc._for_center_plan._subphase_count;

        for(int i = 0; i < data[5]; i++)
        {
            usiGreenTMP = 0;

            //OT Debug 0523 -3line
            if(stc._for_center_plan._phase_order != 0xB0)                               //Flash PhaseOrder In HsinChu
            {
                usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._green;
            }
            else { }
            usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
            usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._pedred;

            data[iDataPtr] = HI(usiGreenTMP);
            iDataPtr++;
            data[iDataPtr] = LO(usiGreenTMP);
            iDataPtr++;
        }

        //OT Debug 0523 -3line
        if(stc._for_center_plan._phase_order != 0xB0)                               //Flash PhaseOrder In HsinChu
        {
            data[iDataPtr] = stc._for_center_plan._cycle_time / 256;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_plan._cycle_time % 256;
            iDataPtr++;
        }
        else
        {
            data[iDataPtr] = 0;
            data[iDataPtr+1] = 0;
            iDataPtr+=2;
        }

        data[iDataPtr] = stc._for_center_plan._offset / 256;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_plan._offset % 256;
        iDataPtr++;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWriteSegment5F16(MESSAGEOK DataMessageIn)
{
    try
    {

        /*
        SegmentType(®É¬q«¬ºA):[9]
        SegmentCount(®É¬q¤À¬Û¼Æ):[10]

        Hour(°_©l®É):[11][14]
        Min(°_©l¤À):[12][15]
        PlanID(®É¨î­p¹º):[13][16]

        NumWeekDay(¦³´X¤Ñ­n·Ó³oSegtype):[]
        WeekDay(­þ´X¤Ñ?):[][][][]
        */

        int i=0;
        /********arwen ++ Start *************/
        unsigned short int centerActMode[32]= {0};
        if (stc.Lock_to_Load_Segment_for_Center(DataMessageIn.packet[9]))
        {
            for (int i=0; i<stc._for_center_segment._segment_count; i++)    //¦A­«·s¼g
            {
                centerActMode[i]=stc._for_center_segment._ptr_seg_exec_time[i]._actMode;//arwen ++ 1000930
            }
        }
        /***************Arwen ++ End****************/



        stc.Lock_to_Reset_Segment_for_Center(DataMessageIn.packet[9],DataMessageIn.packet[10]);
        stc.Lock_to_Load_WeekDaySegment_for_Center();

        stc._for_center_segment._segment_type=DataMessageIn.packet[9];

//OTMARKPRINTF  printf("segment_type%d\n",stc._for_center_segment._segment_type);

        stc._for_center_segment._segment_count=DataMessageIn.packet[10];

//OTMARKPRINTF  printf("segment_Count%d\n",stc._for_center_segment._segment_count);

        printf("PTRAFFIC92TC::vWriteSegment5F16 ARWEN ADD _segment_count:%d \n",stc._for_center_segment._segment_count);
        for (i=0; i<stc._for_center_segment._segment_count; i++)
        {
            stc._for_center_segment._ptr_seg_exec_time[i]._hour=DataMessageIn.packet[11+3*i];
            stc._for_center_segment._ptr_seg_exec_time[i]._minute=DataMessageIn.packet[12+3*i];
            stc._for_center_segment._ptr_seg_exec_time[i]._planid=DataMessageIn.packet[13+3*i];
            stc._for_center_segment._ptr_seg_exec_time[i]._actMode=centerActMode[i];//arwen ++ 1000930;

            if(smem.vGetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[13+3*i]) == false)
            {
                vReturnToCenterNACK(0x5F, 0x16, 0x04, 3*i + 5);
                return false;
            }
            printf("hour: %d minute: %d planid:%d actMode:%d",stc._for_center_segment._ptr_seg_exec_time[i]._hour,stc._for_center_segment._ptr_seg_exec_time[i]._minute,stc._for_center_segment._ptr_seg_exec_time[i]._planid,stc._for_center_segment._ptr_seg_exec_time[i]._actMode);


            //OTMARKPRINTF  printf("%d:%d--%d\n",stc._for_center_segment._ptr_seg_exec_time[i]._hour,stc._for_center_segment._ptr_seg_exec_time[i]._minute,stc._for_center_segment._ptr_seg_exec_time[i]._planid);

        }
        printf("\n");

        int numWeekDay=DataMessageIn.packet[11+3*i];

//OTMARKPRINTF  printf("numWeekDay:%d\n",numWeekDay);

        if( DataMessageIn.packetLength < 15+(3*DataMessageIn.packet[10])+numWeekDay)
        {
            vReturnToCenterNACK(0x5F, 0x16, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 15+(3*DataMessageIn.packet[10])+numWeekDay)
        {
            vReturnToCenterNACK(0x5F, 0x16, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }
        vReturnToCenterACK(0x5F, 0x16);


        for (int j=0; j<numWeekDay; j++)
        {

            //OTMARKPRINTF  printf("%3x--%d\n",DataMessageIn.packet[12+3*i+j],stc._for_center_segment._segment_type);

            switch (DataMessageIn.packet[12+3*i+j])
            {
            case 0x01:
                stc._for_center_weekdayseg[0]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x02:
                stc._for_center_weekdayseg[1]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x03:
                stc._for_center_weekdayseg[2]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x04:
                stc._for_center_weekdayseg[3]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x05:
                stc._for_center_weekdayseg[4]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x06:
                stc._for_center_weekdayseg[5]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x07:
                stc._for_center_weekdayseg[6]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x11:
                stc._for_center_weekdayseg[7]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x12:
                stc._for_center_weekdayseg[8]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x13:
                stc._for_center_weekdayseg[9]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x14:
                stc._for_center_weekdayseg[10]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x15:
                stc._for_center_weekdayseg[11]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x16:
                stc._for_center_weekdayseg[12]._segment_type=stc._for_center_segment._segment_type;
                break;
            case 0x17:
                stc._for_center_weekdayseg[13]._segment_type=stc._for_center_segment._segment_type;
                break;
            default:
                break;
            }
        }

        stc.Lock_to_Save_Segment_from_Center();
        stc.Lock_to_Save_WeekDaySegment_from_Center();
        smem.vSetTCPhasePlanSegTypeData(TC_SegType, DataMessageIn.packet[9], true);
        screenLast92TCPlanSegmentUpdate.DisplaySegmentUpdate();
      shrinkAPP.Init_DBupdateInfo();
        return true;

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQuerySegment5F46(MESSAGEOK DataMessageIn)
{
    try
    {
        bool bSegStatus;
        int iQuerySegmentType = DataMessageIn.packet[9];
        int iWeekDay = DataMessageIn.packet[10];

        if( DataMessageIn.packetLength < 14)
        {
            vReturnToCenterNACK(0x5F, 0x46, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 14)
        {
            vReturnToCenterNACK(0x5F, 0x46, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

        if(iQuerySegmentType == 0xFF)
        {
            stc.Lock_to_Load_Segment_for_Center_inWeekDay(iWeekDay);
        }
        else
        {
            bSegStatus = smem.vGetTCPhasePlanSegTypeData(TC_SegType, iQuerySegmentType);
            if(bSegStatus == false)
            {
                vReturnToCenterNACK(0x5F, 0x46, 0x02, 0x0);
                return false;
            }

            stc.Lock_to_Load_Segment_for_Center(iQuerySegmentType);
        }

        stc.Lock_to_Load_WeekDaySegment_for_Center();

        unsigned char data[256];
        int iDataPtr = 4; //Form 6 Start to send SignalStatus
        int iNumWeekDayPrt = 0;
        int iNumWeekDay = 0;
        data[0] = 0x5F;
        data[1] = 0xC6;
        data[2] = stc._for_center_segment._segment_type;
        data[3] = stc._for_center_segment._segment_count;

        for (int i=0; i<stc._for_center_segment._segment_count; i++)
        {
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._hour;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._minute;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._planid;
            iDataPtr++;
        }
        iNumWeekDayPrt = iDataPtr;
        iDataPtr++;

        if(iQuerySegmentType != 0xFF)
        {
            if(stc._for_center_weekdayseg[0]._segment_type == data[2])    // data[2] = stc._for_center_segment._segment_type
            {
                data[iDataPtr] = 0x01;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[1]._segment_type == data[2])
            {
                data[iDataPtr] = 0x02;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[2]._segment_type == data[2])
            {
                data[iDataPtr] = 0x03;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[3]._segment_type == data[2])
            {
                data[iDataPtr] = 0x04;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[4]._segment_type == data[2])
            {
                data[iDataPtr] = 0x05;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[5]._segment_type == data[2])
            {
                data[iDataPtr] = 0x06;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[6]._segment_type == data[2])
            {
                data[iDataPtr] = 0x07;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[7]._segment_type == data[2])
            {
                data[iDataPtr] = 0x11;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[8]._segment_type == data[2])
            {
                data[iDataPtr] = 0x12;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[9]._segment_type == data[2])
            {
                data[iDataPtr] = 0x13;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[10]._segment_type == data[2])
            {
                data[iDataPtr] = 0x14;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[11]._segment_type == data[2])
            {
                data[iDataPtr] = 0x15;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[12]._segment_type == data[2])
            {
                data[iDataPtr] = 0x16;
                iNumWeekDay++;
                iDataPtr++;
            }
            if(stc._for_center_weekdayseg[13]._segment_type == data[2])
            {
                data[iDataPtr] = 0x17;
                iNumWeekDay++;
                iDataPtr++;
            }
        }
        else
        {
            data[iDataPtr] = iWeekDay;
            iNumWeekDay++;
            iDataPtr++;
        }

        data[iNumWeekDayPrt] = iNumWeekDay;                                           //³Ì«á¸É¤WNumWeekDay¤§­È

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;
    }
    catch (...) {}
}


//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWriteSpecialSegment5F17(MESSAGEOK DataMessageIn)
{
    try
    {

        if( DataMessageIn.packetLength < 20+(3*DataMessageIn.packet[10]))
        {
            vReturnToCenterNACK(0x5F, 0x17, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 20+(3*DataMessageIn.packet[10]))
        {
            vReturnToCenterNACK(0x5F, 0x17, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }
        vReturnToCenterACK(0x5F, 0x17);

        /*
        SegmentType(®É¬q«¬ºA):[9]
        SegmentCount(®É¬q¤À¬Û¼Æ):[10]

        Hour(°_©l®É):[11]
        Min(°_©l¤À):[12]
        PlanID(®É¨î­p¹º):[13]

        Year():[][]
        Month():[][]
        Day():[][]
        */

        int i=0;

        stc.Lock_to_Reset_Segment_for_Center(DataMessageIn.packet[9],DataMessageIn.packet[10]);
        stc.Lock_to_Load_HoliDaySegment_for_Center(DataMessageIn.packet[9]);

        stc._for_center_segment._segment_type=DataMessageIn.packet[9];

//OTMARKPRINTF  printf("segment_type%d\n",stc._for_center_segment._segment_type);

        stc._for_center_segment._segment_count=DataMessageIn.packet[10];

//OTMARKPRINTF  printf("segment_Count%d\n",stc._for_center_segment._segment_count);


        for (i=0; i<stc._for_center_segment._segment_count; i++)
        {
            stc._for_center_segment._ptr_seg_exec_time[i]._hour=DataMessageIn.packet[11+3*i];
            stc._for_center_segment._ptr_seg_exec_time[i]._minute=DataMessageIn.packet[12+3*i];
            stc._for_center_segment._ptr_seg_exec_time[i]._planid=DataMessageIn.packet[13+3*i];

            if(smem.vGetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[13+3*i]) == false)
            {
                vReturnToCenterNACK(0x5F, 0x17, 0x04, 3*i + 5);
                return false;
            }


            //OTMARKPRINTF  printf("%d:%d--%d\n",stc._for_center_segment._ptr_seg_exec_time[i]._hour,stc._for_center_segment._ptr_seg_exec_time[i]._minute,stc._for_center_segment._ptr_seg_exec_time[i]._planid);

        }

        stc._for_center_holidayseg._segment_type=DataMessageIn.packet[9];
        stc._for_center_holidayseg._start_year=DataMessageIn.packet[11+3*i]+1911;
        stc._for_center_holidayseg._start_month=DataMessageIn.packet[12+3*i];
        stc._for_center_holidayseg._start_day=DataMessageIn.packet[13+3*i];
        stc._for_center_holidayseg._end_year=DataMessageIn.packet[14+3*i]+1911;
        stc._for_center_holidayseg._end_month=DataMessageIn.packet[15+3*i];
        stc._for_center_holidayseg._end_day=DataMessageIn.packet[16+3*i];


//OTMARKPRINTF  printf("%d-%d-%d  %d-%d-%d ::%d\n",stc._for_center_holidayseg._start_year,stc._for_center_holidayseg._start_month,stc._for_center_holidayseg._start_day,
//OTMARKPRINTF  stc._for_center_holidayseg._end_year,stc._for_center_holidayseg._end_month,stc._for_center_holidayseg._end_day,stc._for_center_holidayseg._segment_type);


        stc.Lock_to_Save_Segment_from_Center();              //save
        stc.Lock_to_Save_HoliDaySegment_from_Center();       //save
        smem.vSetTCPhasePlanSegTypeData(TC_SegType, DataMessageIn.packet[9], true);
        screenLast92TCPlanSegmentUpdate.DisplaySegmentUpdate();
      shrinkAPP.Init_DBupdateInfo();
        return true;

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryHolidaySegment5F47(MESSAGEOK DataMessageIn)
{
    try
    {
        int iQueryHolidaySegmentType = DataMessageIn.packet[9];

        bool bSegStatus;
        bSegStatus = smem.vGetTCPhasePlanSegTypeData(TC_SegType, iQueryHolidaySegmentType);
        if(bSegStatus == false)
        {
            vReturnToCenterNACK(0x5F, 0x47, 0x02, 0x0);
            return false;
        }
        if( DataMessageIn.packetLength < 13)
        {
            vReturnToCenterNACK(0x5F, 0x47, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 13)
        {
            vReturnToCenterNACK(0x5F, 0x47, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }


        if( stc.Lock_to_Load_HoliDaySegment_for_Center(iQueryHolidaySegmentType) )
        {
        }
        else
        {
            //NACK
            return false;
        }


        unsigned char data[256];
        int iDataPtr = 4; //Form 6 Start to send SignalStatus

        data[0] = 0x5F;
        data[1] = 0xC7;
        data[2] = stc._for_center_holidayseg._segment_type;
        data[3] = stc._for_center_segment._segment_count;

        for (int i=0; i<stc._for_center_segment._segment_count; i++)
        {
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._hour;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._minute;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._planid;
            iDataPtr++;
        }

        data[iDataPtr] = stc._for_center_holidayseg._start_year - 1911;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._start_month;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._start_day;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._end_year - 1911;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._end_month;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._end_day;
        iDataPtr++;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;
    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWriteRunPlan5F18(MESSAGEOK DataMessageIn)
{
    try
    {

        DATA_Bit _ControlStrategy;
        _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
        printf("[OTDebug] in 5F18, _ControlStrategy.DBit:%X\n", _ControlStrategy.DBit);


        unsigned int uiTimeOutPlan;
        int iSetPlanID = DataMessageIn.packet[9];

        if( DataMessageIn.packetLength < 13)
        {
            vReturnToCenterNACK(0x5F, 0x18, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 13)
        {
            vReturnToCenterNACK(0x5F, 0x18, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }
        vReturnToCenterACK(0x5F, 0x18);

        //OT1000310
        //OT20110729   if(_ControlStrategy.DBit == 0x02) {
//OT20111107 if((_ControlStrategy.DBit & 0x02) > 0 ) {  //OT20110729
        if(1)    //OT20111107
        {
            printf("iSetPlanID = %d \n",iSetPlanID);
            smem.vSetINTData(TC92_PlanOneTime5F18_PlanID, iSetPlanID);

            //OT1000218
            uiTimeOutPlan = stc.vDetermineTimeToNextPlan();
            smem.vSet5F18EffectTime(uiTimeOutPlan);

            printf("uiTimeOutPlan:%d\n", uiTimeOutPlan);
            printf("iSetPlanID:%d\n", iSetPlanID);

        }

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryRunPlan5F48(MESSAGEOK DataMessageIn)                   //then return 5FC8
{
    try
    {
        unsigned char data[256];
        int iDataPtr = 6; //Form 6 Start to send SignalStatus

        unsigned short int usiGreen;

        unsigned short int usiRunningPlanID = stc.vGetUSIData(CSTC_exec_plan_plan_ID);

        printf("usiRunningPlanID:%d\n", usiRunningPlanID);

        stc.Lock_to_Load_Plan_for_Center(usiRunningPlanID);
//  unsigned short int iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
//  stc.Lock_to_Load_Phase_for_Center(iCurrentPhaseID);

        /*
          DATA_Bit _SignalMap, _GSignalMap;
          _SignalMap.DBit = stc._for_center_phase._signal_map;
          int iSignalMapWayCount;
          unsigned short int usiSignalStatus;
          int iGSignalMap[8];

          for(int i = 0; i < stc._for_center_phase._subphase_count; i++) {
            _GSignalMap.DBit = 0;
            iSignalMapWayCount = 0;
            for(int j = 0; j < 8; j++)                                                  //Init
              iGSignalMap[j] = 0;

            if(_SignalMap.switchBit.b1 == true) {                                         //±qSignalMap¹ïÀ³CCTªº¿OÂI
              usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
              iGSignalMap[0] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
              iSignalMapWayCount++;
            }
            if(_SignalMap.switchBit.b2 == true) {                                         //±qSignalMap¹ïÀ³CCTªº¿OÂI
              usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
              iGSignalMap[1] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
              iSignalMapWayCount++;
            }
            if(_SignalMap.switchBit.b3 == true) {                                         //±qSignalMap¹ïÀ³CCTªº¿OÂI
              usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
              iGSignalMap[2] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
              iSignalMapWayCount++;
            }
            if(_SignalMap.switchBit.b4 == true) {                                         //±qSignalMap¹ïÀ³CCTªº¿OÂI
              usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
              iGSignalMap[3] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
              iSignalMapWayCount++;
            }
            if(_SignalMap.switchBit.b5 == true) {                                         //±qSignalMap¹ïÀ³CCTªº¿OÂI
              usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
              iGSignalMap[4] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
              iSignalMapWayCount++;
            }
            if(_SignalMap.switchBit.b6 == true) {                                         //±qSignalMap¹ïÀ³CCTªº¿OÂI
              usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
              iGSignalMap[5] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
              iSignalMapWayCount++;
            }
            if(_SignalMap.switchBit.b7 == true) {                                         //±qSignalMap¹ïÀ³CCTªº¿OÂI
              usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
              iGSignalMap[6] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
              iSignalMapWayCount++;
            }
            if(_SignalMap.switchBit.b8 == true) {                                         //±qSignalMap¹ïÀ³CCTªº¿OÂI
              usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
              iGSignalMap[7] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
              iSignalMapWayCount++;
            }

            for(int j = 0; j < 8; j++) {
              if(iGSignalMap[j] == 1) {
                if(j == 0) _GSignalMap.switchBit.b1 = true;
                if(j == 1) _GSignalMap.switchBit.b2 = true;
                if(j == 2) _GSignalMap.switchBit.b3 = true;
                if(j == 3) _GSignalMap.switchBit.b4 = true;
                if(j == 4) _GSignalMap.switchBit.b5 = true;
                if(j == 5) _GSignalMap.switchBit.b6 = true;
                if(j == 6) _GSignalMap.switchBit.b7 = true;
                if(j == 7) _GSignalMap.switchBit.b8 = true;
              }
            }
            data[iDataPtr] = _GSignalMap.DBit;
            iDataPtr++;
          }
        */

//  int iDataPtr = 6; //Form 6 Start to send SignalStatus
        data[0] = 0x5F;
        data[1] = 0xC8;
        data[2] = usiRunningPlanID;
        data[3] = stc._for_center_plan._dir;
        data[4] = stc._for_center_plan._phase_order;
        data[5] = stc._for_center_plan._subphase_count;

        /*
        data[6] = have beed place before
        */


//OT Debug CECI
        for(int i = 0; i < data[5]; i++)                                              //data[5] = subphasecount
        {
            usiGreen = 0;
            if(stc._for_center_plan._phase_order != 0xB0)                               //Flash PhaseOrder In HsinChu
            {
                usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._green;
            }
            else { }
            usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
            usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._pedred;

            data[iDataPtr] = HI(usiGreen);
            iDataPtr++;
            data[iDataPtr] = LO(usiGreen);
            iDataPtr++;
        }

        if(stc._for_center_plan._phase_order != 0xB0)                                 //Flash PhaseOrder In HsinChu
        {
            data[iDataPtr] = HI(stc._for_center_plan._cycle_time);
            iDataPtr++;
            data[iDataPtr] = LO(stc._for_center_plan._cycle_time);
            iDataPtr++;
        }
        else
        {
            data[iDataPtr] = 0;
            data[iDataPtr+1] = 0;
            iDataPtr+=2;
        }

        data[iDataPtr] = HI(stc._for_center_plan._offset);
        iDataPtr++;
        data[iDataPtr] = LO(stc._for_center_plan._offset);
        iDataPtr++;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vSetActuatedType_5F19(MESSAGEOK DataMessageIn)
{
    try
    {
        //int tmpPhaseId = 0;
        DATA_Bit tmpPhaseId;
        int tmpActParameterPlan = 0;//觸動
        int tmpActType = 0;//觸動類型
        int tmpActPlan = 0;
        int tmpActionSec = 0;
        int tmpCompensateMode = 0;
        int valueSwitch[8] = {0};
        int valueExtendSec[8] = {0};
        int tmp1 = 0;
        int tmp2 = 0;

        tmpPhaseId.DBit = DataMessageIn.packet[9];// not use
        tmpActParameterPlan = DataMessageIn.packet[10];// V3 ActuateType 觸動參數之計畫編號
        tmpActionSec = DataMessageIn.packet[11];// V3 TimeExtendSec
        tmpActPlan   = DataMessageIn.packet[12];// V3 ActuateData1 閃光或長綠觸動發生所要執行之計畫編號
        /*
         * b1:
         * b2-b7:觸動類型
         */
        DATA_Bit modeAndCompensateMode;
        modeAndCompensateMode.DBit = DataMessageIn.packet[13];// V3 ActuateData2 是否連鎖與觸動類型
        if(modeAndCompensateMode.switchBit.b1 == 1) //是否連鎖
        {
            tmpCompensateMode = 1;
        }
        if(modeAndCompensateMode.switchBit.b2 == 1)
        {
            tmpActType = 1;//觸動
        }
        else if(modeAndCompensateMode.switchBit.b3 == 1)
        {
            tmpActType = 2;//固定
        }
        else if(modeAndCompensateMode.switchBit.b4 == 1)
        {
            tmpActType = 3;//閃光
        }
        else if(modeAndCompensateMode.switchBit.b5 == 1)
        {
            tmpActType = 4;//長綠
        }
        else
        {
            tmpActType = 0;//觸動關閉
        }
        //b6~b8尚未使用
        unsigned char bTmp = 0x01;
        for(int i = 0 ; i < 8 ; i++)
        {
            valueExtendSec[i] = DataMessageIn.packet[14+i];
            valueSwitch[i] = (tmpPhaseId.DBit&bTmp)?1:0;
            bTmp=bTmp<<1;
        }
        tmp1 = DataMessageIn.packet[14+8];
        tmp2 = DataMessageIn.packet[14+9];

        bool res=false;
        if(tmpActType>5)
        {
            printf("tmpActType %d is error\n",tmpActType);
            res=true;
        }
        for(int i = 0 ; i < 8 ; i++)
        {
            if(valueSwitch[i]>1)
            {
                printf("valueSwitch[%d] %d is error\n",i,valueSwitch[i]);
                res=true;
                break;
            }
            if(valueExtendSec[i]>9)
            {
                printf("valueExtendSec[%d] %d is error\n",i,valueExtendSec[i]);
                res=true;
                break;
            }
        }
        //unsigned int tmpActPlan=ucTMP_ActuatePlan[0]*10+ucTMP_ActuatePlan[1];
        if(tmpActPlan>48) //
        {
            printf("tmpActPlan %d is error\n",tmpActPlan);
            res=true;
        }
        //unsigned int tmpActionSec=valueActionSec[0]*10+valueActionSec[1];
        if( tmpActionSec>20 && tmpActionSec != 0)
        {
            printf("tmpActionSec %d is error\n",tmpActionSec);
            res=true;
        }
        //unsigned int tmpCompensateMode = valueCompensateMode;
        if(tmpCompensateMode > 1)
        {
            printf("tmpCompensateMode %d is error\n",tmpCompensateMode);
            res=true;
        }
        if(res)
        {
            printf("parameter is error, could not write\n");
            vReturnToCenterNACK(0x5F,0x19,0x00,0x00);
            return false;
        }
        else
        {
            smem.vSetUCData(actuateSwitchChange_for_Arwen,1);//arwen actChange 1000707
            //screenSegtype.DisplaySegtype(stc._panel_segment._segment_type, INVAILDVALUE);
            smem.saveSegmentActData(tmpActParameterPlan,tmpActType,valueSwitch,valueExtendSec,tmpActPlan,tmpActionSec,tmpCompensateMode);
            vReturnToCenterACK(0x5F,0x19);
        }
        return true;
    }
    catch(...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryActuatedType_5F49(MESSAGEOK DataMessageIn)
{
    try
    {
        int tmpActParameterPlan = DataMessageIn.packet[9];
        if(tmpActParameterPlan>9 || tmpActParameterPlan<0)
        {
            vReturnToCenterNACK(0x5F,0x49,0x00,0x00);
            return false;
        }
        BYTE data[32] = { 0x00 };
        unsigned short int data_length = 0;
        data[0] = 0x5F;
        data_length++;
        data[1] = 0xC9;
        data_length++;

        unsigned short int usiActType        = 0;
        unsigned short int tmpActionSec      = 0;
        unsigned short int usiActPlan        = 0;
        unsigned short int usiCompensateMode = 0;
        if(tmpActParameterPlan>0 && tmpActParameterPlan <10)
        {
            //do nothing
        }
        else
        {
            tmpActParameterPlan = smem.getRunSegmentActMode();//取的目前執行的計畫
        }
        usiActType   = smem.getSegmentActType(tmpActParameterPlan);
        tmpActionSec = smem.getSegmentZoneTimes(tmpActParameterPlan);
        usiActPlan   = smem.getSegmentActPlan(tmpActParameterPlan);
        usiCompensateMode = smem.getSegmentCompensateMode(tmpActParameterPlan);
        DATA_Bit phase_trigger;
        phase_trigger.DBit = 0x00;
        BYTE ucOnBit = 0x01;
        for(int i = 0; i < 8; i++)
        {
            if(smem.getSegmentActVDPhaseTriggerSwitch(tmpActParameterPlan,i)==1)
            {
                phase_trigger.DBit=(phase_trigger.DBit|ucOnBit);
            }
            ucOnBit=ucOnBit<<1;
        }
        data[2] = phase_trigger.DBit;
        data_length++;
        data[3] = tmpActParameterPlan;
        data_length++;
        data[4] = tmpActionSec;
        data_length++;
        data[5] = usiActPlan;
        data_length++;
        DATA_Bit ModeAndCompensateMode;
        ModeAndCompensateMode.DBit = 0x00;
        switch(usiActType)
        {
        case(1):
            ModeAndCompensateMode.switchBit.b2 = 1;
            break;
        case(2):
            ModeAndCompensateMode.switchBit.b3 = 1;
            break;
        case(3):
            ModeAndCompensateMode.switchBit.b4 = 1;
            break;
        case(4):
            ModeAndCompensateMode.switchBit.b5 = 1;
            break;
        }
        if(1 == usiCompensateMode)
        {
            ModeAndCompensateMode.switchBit.b1 = 1;
        }
        data[6] = ModeAndCompensateMode.DBit;
        data_length++;
        for(int i = 0; i < 8; i++)
        {
            data[7+i] = smem.getSegmentActExtendSec(tmpActParameterPlan,i);
            data_length++;
        }
        data[7+8] = 0x00;
        data_length++;
        data[7+9] = 0x00;
        data_length++;

        vReturnToCenterACK(0x5F,0x49);
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        return true;
    }
    catch(...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vSetActuatedSegment_5F1A(MESSAGEOK DataMessageIn)
{
    try
    {
        int i=0;
        /********arwen ++ Start *************/
        unsigned short int plan[32]= {0};
        if (stc.Lock_to_Load_Segment_for_Center(DataMessageIn.packet[9]))
        {
            for (int i=0; i<stc._for_center_segment._segment_count; i++)    //¦A­«·s¼g
            {
                plan[i]=stc._for_center_segment._ptr_seg_exec_time[i]._planid;//arwen ++ 1000930
            }
        }
        /***************Arwen ++ End****************/

        //stc.Lock_to_Reset_Segment_for_Center(DataMessageIn.packet[9],DataMessageIn.packet[10]);//jantai 重建struct
        stc.Lock_to_Load_WeekDaySegment_for_Center();

        if(stc._for_center_segment._segment_type != DataMessageIn.packet[9])
        {
            printf("\n");
            return false;
        }
        //stc._for_center_segment._segment_type=DataMessageIn.packet[9]; //jantai

        //OTMARKPRINTF  printf("segment_type%d\n",stc._for_center_segment._segment_type);
        if(stc._for_center_segment._segment_count != DataMessageIn.packet[10])
        {
            return false;
        }
        //stc._for_center_segment._segment_count=DataMessageIn.packet[10]; //jantai

        //OTMARKPRINTF  printf("segment_Count%d\n",stc._for_center_segment._segment_count);

        printf("PTRAFFIC92TC::vWriteSegment5F1A ARWEN ADD _segment_count:%d \n",stc._for_center_segment._segment_count);
        for (i=0; i<stc._for_center_segment._segment_count; i++)
        {
            if(stc._for_center_segment._ptr_seg_exec_time[i]._hour!=DataMessageIn.packet[11+3*i])
            {
                printf("hour is error\n");
                return false;
            }
            //stc._for_center_segment._ptr_seg_exec_time[i]._hour=DataMessageIn.packet[11+3*i]; //jantai
            if(stc._for_center_segment._ptr_seg_exec_time[i]._minute!=DataMessageIn.packet[12+3*i])
            {
                printf("min is error\n");
                return false;
            }
            //stc._for_center_segment._ptr_seg_exec_time[i]._minute=DataMessageIn.packet[12+3*i]; //jantai
            //stc._for_center_segment._ptr_seg_exec_time[i]._planid=plan[i];
            stc._for_center_segment._ptr_seg_exec_time[i]._actMode=DataMessageIn.packet[13+3*i];//arwen ++ 1000930;

            if(smem.vGetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[13+3*i]) == false)  //要改成檢查actMode是否存在
            {
                vReturnToCenterNACK(0x5F, 0x1A, 0x04, 3*i + 5);
                return false;
            }
            printf("hour: %d minute: %d planid:%d actMode:%d",stc._for_center_segment._ptr_seg_exec_time[i]._hour,stc._for_center_segment._ptr_seg_exec_time[i]._minute,stc._for_center_segment._ptr_seg_exec_time[i]._planid,stc._for_center_segment._ptr_seg_exec_time[i]._actMode);


            //OTMARKPRINTF  printf("%d:%d--%d\n",stc._for_center_segment._ptr_seg_exec_time[i]._hour,stc._for_center_segment._ptr_seg_exec_time[i]._minute,stc._for_center_segment._ptr_seg_exec_time[i]._planid);

        }
        printf("\n");

        int numWeekDay=DataMessageIn.packet[11+3*i];

        //OTMARKPRINTF  printf("numWeekDay:%d\n",numWeekDay);

        if( DataMessageIn.packetLength < 15+(3*DataMessageIn.packet[10])+numWeekDay)
        {
            vReturnToCenterNACK(0x5F, 0x1A, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 15+(3*DataMessageIn.packet[10])+numWeekDay)
        {
            vReturnToCenterNACK(0x5F, 0x1A, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }
        vReturnToCenterACK(0x5F, 0x1A);

        /*
        	  for (int j=0;j<numWeekDay;j++) {

        	   //OTMARKPRINTF  printf("%3x--%d\n",DataMessageIn.packet[12+3*i+j],stc._for_center_segment._segment_type);

        	     switch (DataMessageIn.packet[12+3*i+j]) {
        	             case 0x01:
        	             stc._for_center_weekdayseg[0]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x02:
        	             stc._for_center_weekdayseg[1]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x03:
        	             stc._for_center_weekdayseg[2]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x04:
        	             stc._for_center_weekdayseg[3]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x05:
        	             stc._for_center_weekdayseg[4]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x06:
        	             stc._for_center_weekdayseg[5]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x07:
        	             stc._for_center_weekdayseg[6]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x11:
        	             stc._for_center_weekdayseg[7]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x12:
        	             stc._for_center_weekdayseg[8]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x13:
        	             stc._for_center_weekdayseg[9]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x14:
        	             stc._for_center_weekdayseg[10]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x15:
        	             stc._for_center_weekdayseg[11]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x16:
        	             stc._for_center_weekdayseg[12]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             case 0x17:
        	             stc._for_center_weekdayseg[13]._segment_type=stc._for_center_segment._segment_type;
        	             break;
        	             default:
        	             break;
        	     }
        	  }
        */
        stc.Lock_to_Save_Segment_from_Center();
//	  stc.Lock_to_Save_WeekDaySegment_from_Center(); //jantai
        smem.vSetTCPhasePlanSegTypeData(TC_SegType, DataMessageIn.packet[9], true);
        screenLast92TCPlanSegmentUpdate.DisplaySegmentUpdate();
      shrinkAPP.Init_DBupdateInfo();
        return true;
    }
    catch(...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryActuatedSegment_5F4A(MESSAGEOK DataMessageIn)
{
    try
    {
        bool bSegStatus;
        int iQuerySegmentType = DataMessageIn.packet[9];
        int iWeekDay = DataMessageIn.packet[10];

        if( DataMessageIn.packetLength < 14)
        {
            vReturnToCenterNACK(0x5F, 0x4A, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 14)
        {
            vReturnToCenterNACK(0x5F, 0x4A, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }

        if(iQuerySegmentType == 0xFF)
        {
            stc.Lock_to_Load_Segment_for_Center_inWeekDay(iWeekDay);
        }
        else
        {
            bSegStatus = smem.vGetTCPhasePlanSegTypeData(TC_SegType, iQuerySegmentType);
            if(bSegStatus == false)
            {
                vReturnToCenterNACK(0x5F, 0x4A, 0x02, 0x0);
                return false;
            }

            stc.Lock_to_Load_Segment_for_Center(iQuerySegmentType);
        }

        stc.Lock_to_Load_WeekDaySegment_for_Center();

        unsigned char data[256];
        int iDataPtr = 4; //Form 6 Start to send SignalStatus
        int iNumWeekDayPrt = 0;
        int iNumWeekDay = 0;
        data[0] = 0x5F;
        data[1] = 0xCA;
        data[2] = stc._for_center_segment._segment_type;
        data[3] = stc._for_center_segment._segment_count;

        for (int i=0; i<stc._for_center_segment._segment_count; i++)
        {
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._hour;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._minute;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._actMode;
            iDataPtr++;
        }
        iNumWeekDayPrt = iDataPtr;
        iDataPtr++;

        if(stc._for_center_weekdayseg[0]._segment_type == data[2])    // data[2] = stc._for_center_segment._segment_type
        {
            data[iDataPtr] = 0x01;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[1]._segment_type == data[2])
        {
            data[iDataPtr] = 0x02;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[2]._segment_type == data[2])
        {
            data[iDataPtr] = 0x03;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[3]._segment_type == data[2])
        {
            data[iDataPtr] = 0x04;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[4]._segment_type == data[2])
        {
            data[iDataPtr] = 0x05;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[5]._segment_type == data[2])
        {
            data[iDataPtr] = 0x06;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[6]._segment_type == data[2])
        {
            data[iDataPtr] = 0x07;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[7]._segment_type == data[2])
        {
            data[iDataPtr] = 0x11;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[8]._segment_type == data[2])
        {
            data[iDataPtr] = 0x12;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[9]._segment_type == data[2])
        {
            data[iDataPtr] = 0x13;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[10]._segment_type == data[2])
        {
            data[iDataPtr] = 0x14;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[11]._segment_type == data[2])
        {
            data[iDataPtr] = 0x15;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[12]._segment_type == data[2])
        {
            data[iDataPtr] = 0x16;
            iNumWeekDay++;
            iDataPtr++;
        }
        if(stc._for_center_weekdayseg[13]._segment_type == data[2])
        {
            data[iDataPtr] = 0x17;
            iNumWeekDay++;
            iDataPtr++;
        }

        data[iNumWeekDayPrt] = iNumWeekDay;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;
    }
    catch(...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vSetActuatedHolidaySegment_5F1B(MESSAGEOK DataMessageIn)
{
    try
    {

        if( DataMessageIn.packetLength < 20+(3*DataMessageIn.packet[10]))
        {
            vReturnToCenterNACK(0x5F, 0x1B, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 20+(3*DataMessageIn.packet[10]))
        {
            vReturnToCenterNACK(0x5F, 0x1B, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }
        //vReturnToCenterACK(0x5F, 0x1B);//jantai

        /*
        SegmentType(®É¬q«¬ºA):[9]
        SegmentCount(®É¬q¤À¬Û¼Æ):[10]

        Hour(°_©l®É):[11]
        Min(°_©l¤À):[12]
        PlanID(®É¨î­p¹º):[13]

        Year():[][]
        Month():[][]
        Day():[][]
        */

        int i=0;

        //stc.Lock_to_Reset_Segment_for_Center(DataMessageIn.packet[9],DataMessageIn.packet[10]);//jantai
        stc.Lock_to_Load_HoliDaySegment_for_Center(DataMessageIn.packet[9]);

        //stc._for_center_segment._segment_type=DataMessageIn.packet[9];//jantai
        if(stc._for_center_segment._segment_type != DataMessageIn.packet[9])
        {
            vReturnToCenterNACK(0x5F, 0x1B, 0x08, 0x00);
            return false;
        }

        //OTMARKPRINTF  printf("segment_type%d\n",stc._for_center_segment._segment_type);

        //stc._for_center_segment._segment_count=DataMessageIn.packet[10];//jantai
        if(stc._for_center_segment._segment_count != DataMessageIn.packet[10])
        {
            vReturnToCenterNACK(0x5F, 0x1B, 0x08, 0x00);
            return false;
        }

        //OTMARKPRINTF  printf("segment_Count%d\n",stc._for_center_segment._segment_count);


        for (i=0; i<stc._for_center_segment._segment_count; i++)
        {
            //stc._for_center_segment._ptr_seg_exec_time[i]._hour=DataMessageIn.packet[11+3*i];//jantai
            if(stc._for_center_segment._ptr_seg_exec_time[i]._hour!=DataMessageIn.packet[11+3*i])
            {
                vReturnToCenterNACK(0x5F, 0x1B, 0x08, 0x00);
                return false;
            }
            //stc._for_center_segment._ptr_seg_exec_time[i]._minute=DataMessageIn.packet[12+3*i];//jantai
            if(stc._for_center_segment._ptr_seg_exec_time[i]._minute!=DataMessageIn.packet[12+3*i])
            {
                vReturnToCenterNACK(0x5F, 0x1B, 0x08, 0x00);
                return false;
            }
            //stc._for_center_segment._ptr_seg_exec_time[i]._planid=DataMessageIn.packet[13+3*i];//jantai
            stc._for_center_segment._ptr_seg_exec_time[i]._actMode=DataMessageIn.packet[13+3*i];

            if(smem.vGetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[13+3*i]) == false)
            {
                vReturnToCenterNACK(0x5F, 0x1B, 0x04, 3*i + 5);
                return false;
            }


            //OTMARKPRINTF  printf("%d:%d--%d\n",stc._for_center_segment._ptr_seg_exec_time[i]._hour,stc._for_center_segment._ptr_seg_exec_time[i]._minute,stc._for_center_segment._ptr_seg_exec_time[i]._planid);

        }
        vReturnToCenterACK(0x5F, 0x1B);//jantai

        //stc._for_center_holidayseg._segment_type=DataMessageIn.packet[9];//jantai
        //stc._for_center_holidayseg._start_year=DataMessageIn.packet[11+3*i]+1911;//jantai
        //stc._for_center_holidayseg._start_month=DataMessageIn.packet[12+3*i];//jantai
        //stc._for_center_holidayseg._start_day=DataMessageIn.packet[13+3*i];//jantai
        //stc._for_center_holidayseg._end_year=DataMessageIn.packet[14+3*i]+1911;//jantai
        //stc._for_center_holidayseg._end_month=DataMessageIn.packet[15+3*i];//jantai
        //stc._for_center_holidayseg._end_day=DataMessageIn.packet[16+3*i];//jantai


        //OTMARKPRINTF  printf("%d-%d-%d  %d-%d-%d ::%d\n",stc._for_center_holidayseg._start_year,stc._for_center_holidayseg._start_month,stc._for_center_holidayseg._start_day,
        //OTMARKPRINTF  stc._for_center_holidayseg._end_year,stc._for_center_holidayseg._end_month,stc._for_center_holidayseg._end_day,stc._for_center_holidayseg._segment_type);


        stc.Lock_to_Save_Segment_from_Center();              //save
        //stc.Lock_to_Save_HoliDaySegment_from_Center();       //save //jantai
        smem.vSetTCPhasePlanSegTypeData(TC_SegType, DataMessageIn.packet[9], true);
        screenLast92TCPlanSegmentUpdate.DisplaySegmentUpdate();
      shrinkAPP.Init_DBupdateInfo();
        return true;
    }
    catch(...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryActuatedHolidaySegment_5F4B(MESSAGEOK DataMessageIn)
{
    try
    {
        int iQueryHolidaySegmentType = DataMessageIn.packet[9];

        bool bSegStatus;
        bSegStatus = smem.vGetTCPhasePlanSegTypeData(TC_SegType, iQueryHolidaySegmentType);
        if(bSegStatus == false)
        {
            vReturnToCenterNACK(0x5F, 0x4B, 0x02, 0x0);
            return false;
        }
        if( DataMessageIn.packetLength < 13)
        {
            vReturnToCenterNACK(0x5F, 0x4B, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 13)
        {
            vReturnToCenterNACK(0x5F, 0x4B, 0x08, DataMessageIn.packetLength - 12);
            return false;
        }


        if( stc.Lock_to_Load_HoliDaySegment_for_Center(iQueryHolidaySegmentType) )
        {
        }
        else
        {
            //NACK
            return false;
        }


        unsigned char data[256];
        int iDataPtr = 4; //Form 6 Start to send SignalStatus

        data[0] = 0x5F;
        data[1] = 0xCB;
        data[2] = stc._for_center_holidayseg._segment_type;
        data[3] = stc._for_center_segment._segment_count;

        for (int i=0; i<stc._for_center_segment._segment_count; i++)
        {
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._hour;
            iDataPtr++;
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._minute;
            iDataPtr++;
            //data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._planid; iDataPtr++;//jantai
            data[iDataPtr] = stc._for_center_segment._ptr_seg_exec_time[i]._actMode;
            iDataPtr++;
        }

        data[iDataPtr] = stc._for_center_holidayseg._start_year - 1911;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._start_month;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._start_day;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._end_year - 1911;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._end_month;
        iDataPtr++;
        data[iDataPtr] = stc._for_center_holidayseg._end_day;
        iDataPtr++;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        //  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        return true;
    }
    catch(...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vWritePhaseByStep5F2F(MESSAGEOK DataMessageIn)
{
    try
    {
//OTMARKPRINTF  printf("[OT] Go to 5F2F.\n");
        int i=0;
        bool bMollacOK=false;
        unsigned char ucStepSignal[8];
        unsigned short int usiTMP;
        unsigned char ucTMP;
        int iDataPtr = 13;  //From DataMessageIn.packet[13] start read
        int iSubPhaseStepCount;

        //OT Debug Dir
        int iLightOutNo;
        unsigned short int usiShift = 0;
        unsigned char ucLightConvert[8][5][8];
        int iConvertPtr = 13;
        for(int i = 0; i < 8; i++)                                                    //init
            for(int j = 0; j < 5; j++)
                for(int k = 0; k < 8; k++)
                    ucLightConvert[i][j][k] = 0x0;

        vReturnToCenterACK(0x5F, 0x2F);

        int iPhaseOrder = DataMessageIn.packet[9];
        DATA_Bit _SignalMap;
        _SignalMap.DBit = DataMessageIn.packet[10];
        int iSignalCount = DataMessageIn.packet[11];
        int iSubPhaseCount = DataMessageIn.packet[12];

        printf("Write 5F2F phase, iPhaseOrder:%2X iSignalCount:%d iSubPhaseCount:%d", iPhaseOrder, iSignalCount, iSubPhaseCount);

//Copy data to convert array
        for(int i = 0; i < iSubPhaseCount; i++)
        {
            iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
            iConvertPtr++;
            if(i < 8)
            {
                for(int j = 0; j < iSubPhaseStepCount; j++)
                {
                    if(j < 5)
                    {
                        for(int k = 0; k < iSignalCount; k++)
                        {
                            if(k < 8)
                            {
                                ucLightConvert[i][j][k] = DataMessageIn.packet[iConvertPtr];
                            }
                            iConvertPtr++;
                        }
                    }
                }
            }
        }

        if(_SignalMap.switchBit.b1 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirN);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 13;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        DataMessageIn.packet[iConvertPtr+1 + j*iSignalCount + iLightOutNo] = ucLightConvert[i][j][usiShift];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b2 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirNE);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 13;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        DataMessageIn.packet[iConvertPtr+1 + j*iSignalCount + iLightOutNo] = ucLightConvert[i][j][usiShift];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b3 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirE);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 13;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        DataMessageIn.packet[iConvertPtr+1 + j*iSignalCount + iLightOutNo] = ucLightConvert[i][j][usiShift];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b4 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirES);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 13;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        DataMessageIn.packet[iConvertPtr+1 + j*iSignalCount + iLightOutNo] = ucLightConvert[i][j][usiShift];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b5 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirS);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 13;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        DataMessageIn.packet[iConvertPtr+1 + j*iSignalCount + iLightOutNo] = ucLightConvert[i][j][usiShift];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b6 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirSW);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 13;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        DataMessageIn.packet[iConvertPtr+1 + j*iSignalCount + iLightOutNo] = ucLightConvert[i][j][usiShift];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b7 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirW);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 13;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        DataMessageIn.packet[iConvertPtr+1 + j*iSignalCount + iLightOutNo] = ucLightConvert[i][j][usiShift];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        if(_SignalMap.switchBit.b8 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirWN);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 13;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = DataMessageIn.packet[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        DataMessageIn.packet[iConvertPtr+1 + j*iSignalCount + iLightOutNo] = ucLightConvert[i][j][usiShift];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }


        bMollacOK = stc.Lock_to_Reset_Phase_for_Center(iPhaseOrder, iSubPhaseCount, iSignalCount);

        if (bMollacOK)
        {

            stc._for_center_phase._phase_order=iPhaseOrder;
            stc._for_center_phase._signal_map=_SignalMap.DBit;
            stc._for_center_phase._signal_count=iSignalCount;
            stc._for_center_phase._subphase_count=iSubPhaseCount;
            stc._for_center_phase._total_step_count = iSubPhaseCount * 5;  //¤­­Ó¨B¶¥

            /*
                for(int j = 0; j < iSubPhaseCount; j++) {
                  for( int k=0; k< stc._for_center_phase._ptr_subphase_step_count[j]; k++ ) {  //¥ý§â©Ò¦³¨B¶¥³£¶ñ¥þ¬õ
                    for(int l=0; l<stc._for_center_phase._signal_count; l++) {
                      stc._for_center_phase._ptr_subphase_step_signal_status[j][k][l] = 0x3003;
                    //OTMARKPRINTF  printf("[OT] stc._for_center_phase._ptr_subphase_step_signal_status[%d][%d][%d] = %x\n",j,k,l, stc._for_center_phase._ptr_subphase_step_signal_status[j][k][l]);
                    }
                  }
                }
            */

            for(int j = 0; j < iSubPhaseCount; j++)
            {

                iSubPhaseStepCount = DataMessageIn.packet[iDataPtr];
                iDataPtr++;

                for( int k=0; k< iSubPhaseStepCount; k++ )
                {

                    for(int qq=0; qq<iSignalCount; qq++)
                    {
                        ucStepSignal[qq] = DataMessageIn.packet[iDataPtr];   //¥ý§â¦P¤@®É¬Û¤§¿O¸¹©ñ¨ìarray
                        iDataPtr++;
                    }

                    for(int l=0; l<stc._for_center_phase._signal_count; l++)
                    {
                        ucTMP = ucStepSignal[l];

                        //OTMARKPRINTF  printf("ucTMP = %X.\n",ucTMP);

                        usiTMP = oTools.vVer30LightToCCTLight(ucTMP);

                        for(int iForTmp = k; iForTmp < stc._for_center_phase._ptr_subphase_step_count[j]; iForTmp++)
                        {
                            stc._for_center_phase._ptr_subphase_step_signal_status[j][iForTmp][l] = usiTMP;
                            //OTMARKPRINTF  printf("[OT] stc._for_center_phase._ptr_subphase_step_signal_status[%d][%d][%d] = %x\n", j,iForTmp,l,stc._for_center_phase._ptr_subphase_step_signal_status[j][iForTmp][l]);
                        }

                    }
                }
            }

        }

//OTMARKPRINTF  printf("[OT] 5F2F Make OK.\n");

        smem.vWriteMsgToDOM("WritePhaseBy5F2F");

        stc.Lock_to_Save_Phase_from_Center();             //Àx¦s¦^¥h
        smem.vSetINTData(TC92_iUpdatePhaseData, 1);
        smem.vSetTCPhasePlanSegTypeData(TC_Phase, iPhaseOrder, true);


//OTMARKPRINTF  printf("SAVE Phase:%d OK!!\n",iPhaseOrder);

        return true;

    }
    catch(...) {}
}

//OT Debug 0410  add 5F5F and 5FDF
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryPhase5F5F(MESSAGEOK DataMessageIn)
{
    try
    {

        unsigned short int usiTmpCmp0[8];
        unsigned short int usiTmpCmp1[8];
        bool bCmpResult;

        //OT Debug Dir
        int iLightOutNo;
        unsigned short int usiShift = 0;
        unsigned char ucLightConvert[8][5][8];
        int iConvertPtr = 6;
        int iSubPhaseStepCount;
        for(int i = 0; i < 8; i++)                                                    //init
            for(int j = 0; j < 5; j++)
                for(int k = 0; k < 8; k++)
                    ucLightConvert[i][j][k] = 0x0;


        //CHeck

        int iQueryPhaseOrder = DataMessageIn.packet[9];

//OT Debug 950629
        if(smem.vGetTCPhasePlanSegTypeData(TC_Phase, iQueryPhaseOrder) == false)
        {
            vReturnToCenterNACK(0x5F, 0x5F, 0x02, 1);
            return false;
        }


        stc.Lock_to_Load_Phase_for_Center(iQueryPhaseOrder);

        unsigned char data[256];
        int iDataPtr = 6; //Form 6 Start to send SignalStatus
        int iStepCountPtr;
        data[0] = 0x5F;
        data[1] = 0xDF;
        data[2] = stc._for_center_phase._phase_order;
        data[3] = stc._for_center_phase._signal_map;
        data[4] = stc._for_center_phase._signal_count;
        data[5] = stc._for_center_phase._subphase_count;

        int usiTMP;


        for(int i = 0; i < stc._for_center_phase._subphase_count; i++)
        {

            iStepCountPtr = iDataPtr;
            data[iStepCountPtr] = 0;
            iDataPtr++;

            //clear usiTmpCmp
            for(int ii = 0; ii < 8; ii++)
            {
                usiTmpCmp0[ii] = 0;
                usiTmpCmp1[ii] = 0;
            }

            for(int j = 0; j < stc._for_center_phase._ptr_subphase_step_count[i]; j++)
            {

                for(int k = 0; k < stc._for_center_phase._signal_count; k++)
                {
                    usiTmpCmp1[k] = stc._for_center_phase._ptr_subphase_step_signal_status[i][j][k];
                }

//compare old and new signal light
                bCmpResult = false;
                for(int k = 0; k < 8; k++)
                {
                    if(usiTmpCmp0[k] != usiTmpCmp1[k])
                        bCmpResult = true;
                }

//      if(bCmpResult == true) {
                if(1)   //don't care.
                {
                    data[iStepCountPtr]++;
                    for(int k = 0; k < stc._for_center_phase._signal_count; k++)
                    {
                        usiTMP = stc._for_center_phase._ptr_subphase_step_signal_status[i][j][k];
                        data[iDataPtr] = oTools.vCCTLightToVer30Light(usiTMP);
                        iDataPtr++;
                    }
                }

                for(int k =0; k < 8; k++)
                {
                    usiTmpCmp0[k] = usiTmpCmp1[k];
                }


            }
        }

        printf("printfMsg 1iDataPtr:%d\n" ,iDataPtr);


        DATA_Bit _SignalMap;
        _SignalMap.DBit = data[3];
        int iSignalCount = data[4];
        int iSubPhaseCount = data[5];

        printf("printfMsg 2iDataPtr:%d\n" ,iDataPtr);

//Copy data to convert array
        for(int i = 0; i < iSubPhaseCount; i++)
        {
            iSubPhaseStepCount = data[iConvertPtr];
            printf("printfMsg iSubPhaseStepCount data[iConvertPtr]X:%x\n", data[iConvertPtr]);
            iConvertPtr++;
            for(int j = 0; j < iSubPhaseStepCount; j++)
            {
                for(int k = 0; k < iSignalCount; k++)
                {
                    ucLightConvert[i][j][k] = data[iConvertPtr];
                    iConvertPtr++;
                }
            }
        }
        printf("printfMsg 3iDataPtr:%d\n" ,iDataPtr);

        if(_SignalMap.switchBit.b1 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirN);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 6;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = data[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        printf("printfMsg iSubPhaseStepCount:%d at:%d\n", iSubPhaseStepCount, iConvertPtr);
                        data[iConvertPtr+1 + j*iSignalCount + usiShift] = ucLightConvert[i][j][iLightOutNo];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                    printf("iSignalCount:%d iSubPhaseStepCount:%d\n", iSignalCount, iSubPhaseStepCount);
                }
                usiShift++;
            }
        }
        printf("printfMsg 4iDataPtr:%d\n" ,iDataPtr);
        if(_SignalMap.switchBit.b2 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirNE);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 6;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = data[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        data[iConvertPtr+1 + j*iSignalCount + usiShift] = ucLightConvert[i][j][iLightOutNo];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        printf("printfMsg 5iDataPtr:%d\n" ,iDataPtr);
        if(_SignalMap.switchBit.b3 == true && usiShift < iSignalCount)
        {
            printf("printfMsg 5a_iDataPtr:%d\n" ,iDataPtr);
            iLightOutNo = smem.vGetSignamMapMappingDir(dirE);
            printf("printfMsg 5b_iDataPtr:%d\n" ,iDataPtr);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg 5c_iDataPtr:%d\n" ,iDataPtr);
                iConvertPtr = 6;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    printf("printfMsg 5d_iDataPtr:%d\n" ,iDataPtr);
                    iSubPhaseStepCount = data[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        printf("printfMsg 5e_iDataPtr:%d\n" ,iDataPtr);
                        data[iConvertPtr+1 + j*iSignalCount + usiShift] = ucLightConvert[i][j][iLightOutNo];
                    }
                    printf("printfMsg 5f_iDataPtr:%d\n" ,iDataPtr);
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                printf("printfMsg 5g_iDataPtr:%d\n" ,iDataPtr);
                usiShift++;
            }
        }
        printf("printfMsg 6iDataPtr:%d\n" ,iDataPtr);
        if(_SignalMap.switchBit.b4 == true && usiShift < iSignalCount)
        {
            printf("printfMsg 6a_iDataPtr:%d\n" ,iDataPtr);
            iLightOutNo = smem.vGetSignamMapMappingDir(dirES);
            if(iLightOutNo < iSignalCount)
            {
                printf("printfMsg 6b_iDataPtr:%d\n" ,iDataPtr);
                iConvertPtr = 6;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    printf("printfMsg 6c_iDataPtr:%d\n" ,iDataPtr);
                    iSubPhaseStepCount = data[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        printf("printfMsg 6d_iDataPtr:%d\n" ,iDataPtr);
                        data[iConvertPtr+1 + j*iSignalCount + usiShift] = ucLightConvert[i][j][iLightOutNo];
                    }
                    printf("printfMsg 6e_iDataPtr:%d\n" ,iDataPtr);
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        printf("printfMsg 7iDataPtr:%d\n" ,iDataPtr);
        if(_SignalMap.switchBit.b5 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirS);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 6;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = data[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        data[iConvertPtr+1 + j*iSignalCount + usiShift] = ucLightConvert[i][j][iLightOutNo];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        printf("printfMsg 8iDataPtr:%d\n" ,iDataPtr);
        if(_SignalMap.switchBit.b6 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirSW);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 6;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = data[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        data[iConvertPtr+1 + j*iSignalCount + usiShift] = ucLightConvert[i][j][iLightOutNo];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        printf("printfMsg 9iDataPtr:%d\n" ,iDataPtr);
        if(_SignalMap.switchBit.b7 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirW);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 6;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = data[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        data[iConvertPtr+1 + j*iSignalCount + usiShift] = ucLightConvert[i][j][iLightOutNo];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }
        printf("printfMsg 10iDataPtr:%d\n" ,iDataPtr);
        if(_SignalMap.switchBit.b8 == true && usiShift < iSignalCount)
        {
            iLightOutNo = smem.vGetSignamMapMappingDir(dirWN);
            if(iLightOutNo < iSignalCount)
            {
                iConvertPtr = 6;                                                         // init
                for(int i = 0; i < iSubPhaseCount; i++)
                {
                    iSubPhaseStepCount = data[iConvertPtr];
                    for(int j = 0; j < iSubPhaseStepCount; j++)
                    {
                        data[iConvertPtr+1 + j*iSignalCount + usiShift] = ucLightConvert[i][j][iLightOutNo];
                    }
                    iConvertPtr += iSignalCount*iSubPhaseStepCount+1;
                }
                usiShift++;
            }
        }


        /*
          for(int i = 0; i < stc._for_center_phase._subphase_count; i++) {
            for(int j = 0; i < stc._for_center_phase._subphase_count
            for(int j = 0; j < stc._for_center_phase._signal_count; j++) {
              usiTMP = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][j];
              data[iDataPtr] = oTools.vCCTLightToVer30Light(usiTMP);
              iDataPtr++;
            }
          }
        */
        printf("printfMsg 11iDataPtr:%d\n" ,iDataPtr);
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;

    }
    catch(...) {}
}


//-------------------------------------------------------------------
bool PTRAFFIC92TC::vTransmitCycleSetting_5F3F(MESSAGEOK DataMessageIn)
{
    try
    {

        printf("printfMsg go to 5F3F.\n");

        short int TT, TC;
        TT = DataMessageIn.packet[9];
        TC = DataMessageIn.packet[10];

        int iReturnCommandSet = smem.vGet0FCommandSet();
        if (iReturnCommandSet == 0 )   //wireless
            if ( TT==2 ||  (TT == 1 && ((TC>=1 && TC<=4) || (TC >= 6 && TC < 0xFF))) )
            {
                vReturnToCenterNACK(0x5F, 0x3F, 0x80, 0x00);    // in wireless
                return false;
            }

        if ( DataMessageIn.packetLength < 14 )
        {
            vReturnToCenterNACK(0x5F, 0x3F, 0x08, 0x00);
            return false;
        }
        else if ( DataMessageIn.packetLength > 14 )
        {
            vReturnToCenterNACK(0x5F, 0x3F, 0x08, DataMessageIn.packetLength - 14);
            return false;
        }

        if(TT >= 3 || TT == 0)
        {
            vReturnToCenterNACK(0x5F, 0x3F, 0x04, 0x01);
            return false;
        }

        //OTMARKPRINTF  printf("Go to Function\n\n");

        if(TC == 3)
            TC = 5;  //Sec
        else if(TC == 4)
            TC = 60;
        else if(TC == 5)
            TC = 300;
        else if(TC == 0)        // send change
            TC = 0;
        else if(TC == 1)
            TC = 1;
        else if(TC == 2)
            TC = 2;
        else if(TC == 0xFF)     // stop send
            TC = 0xFF;

        else
        {
            vReturnToCenterNACK(0x5F, 0x3F, 0x04, 0x02);
            return false;
        }

        if(DataMessageIn.InnerOrOutWard != cComingFromScreen)
        {
            printf("printfMsg go to ACK.\n");
            vReturnToCenterACK(0x5F, 0x3F);
        }
        printf("printfMsg after ACK.\n");

        if(TT == 1)
        {
            smem.vSetINTData(TC92SignalLightStatus_5F0F_IntervalTime, TC);                            //±N¬ö¿ý¶¡¹j³]¨ìsmem¸Ì
        }
        if(TT == 2)
        {
            smem.vSetINTData(TC92SignalStepStatus_5F03_IntervalTime, TC);                            //±N¬ö¿ý¶¡¹j³]¨ìsmem¸Ì
        }

        return true;
    }
    catch (...) {}
}
void PTRAFFIC92TC::shrinkAPP_vTransmitCycleSetting_5F3F(int select,int select_sec)
{
    try
    {

        printf("printfMsg go to 5F3F.\n");

        short int TT, TC;
TT=select;//only 1 or2
TC=select_sec;//

        int iReturnCommandSet = smem.vGet0FCommandSet();


        //OTMARKPRINTF  printf("Go to Function\n\n");

        if(TC == 3)
            TC = 5;  //Sec
        else if(TC == 4)
            TC = 60;
        else if(TC == 5)
            TC = 300;
        else if(TC == 0)        // send change
            TC = 0;
        else if(TC == 1)
            TC = 1;
        else if(TC == 2)
            TC = 2;
        else if(TC == 6)     // stop send
            TC = 0xFF;




        if(TT == 1)
        {
            smem.vSetINTData(TC92SignalLightStatus_5F0F_IntervalTime, TC);                            //±N¬ö¿ý¶¡¹j³]¨ìsmem¸Ì
        }
        if(TT == 2)
        {
            smem.vSetINTData(TC92SignalStepStatus_5F03_IntervalTime, TC);                            //±N¬ö¿ý¶¡¹j³]¨ìsmem¸Ì
        }


    }
    catch (...) {}
}

//-------------------------------------------------------------------
bool PTRAFFIC92TC::vTransmitCycleQuery_5F6F(MESSAGEOK DataMessageIn)
{
    try
    {

        short int TT, TC;
        TT = DataMessageIn.packet[9];

        if(TT == 0 || TT > 2)
        {
            vReturnToCenterNACK(0x5F, 0x6F, 0x04, 0x01);
            return false;
        }
        if( DataMessageIn.packetLength < 13 )
        {
            vReturnToCenterNACK(0x5F, 0x6F, 0x08, 0x00);
            return false;
        }
        else if( DataMessageIn.packetLength > 13 )
        {
            vReturnToCenterNACK(0x5F, 0x6F, 0x08, DataMessageIn.packetLength - 13);
            return false;
        }

        if(TT == 1)
            TC = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
        if(TT == 2)
            TC = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);

        if(TC == 0)         //¨t²Î¬°¶Ç°eÅÜ¤Æ
            TC = 0;
        else if(TC == 1)
            TC = 1;
        else if(TC == 2)
            TC = 2;
        else if(TC == 5)
            TC = 3;
        else if(TC == 60)
            TC = 4;
        else if(TC == 300)
            TC = 5;
        else if(TC == 0xFF)    //¨t²Îªº¶Ç°e¶g´Á¬° 0 (¤£¶Ç°e)
            TC = 0xFF;

        unsigned char data[6];
        data[0] = 0x5F;
        data[1] = 0xEF;
        data[2] = TT;
        data[3] = TC;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        return true;

    }
    catch(...) {}
}

//OT20111020

/*OT20111107
//-------------------------------------------------------------------
bool PTRAFFIC92TC::vGoToNextPhaseStepControl_5F1C(MESSAGEOK DataMessageIn)
{
try {

    short int siSubPhaseID, siStepID, siEffectTime;
    siSubPhaseID = DataMessageIn.packet[9];
    siStepID = DataMessageIn.packet[10];
    siEffectTime = DataMessageIn.packet[11];

    char msg[256];

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if (iReturnCommandSet <= 2 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00); return false; }

    if ( DataMessageIn.packetLength < 15 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x08, 0x00); return false; }
    else if ( DataMessageIn.packetLength > 15 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x08, DataMessageIn.packetLength - 15); return false; }

    if(siSubPhaseID > 8) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01); return false; }
    if(siStepID > 8) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02); return false; }

    DATA_Bit uc5F10_ControlStrategy;
    int i5F10_EffectTime;

    uc5F10_ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
    i5F10_EffectTime = smem.vGetINTData(TC92_iEffectTime);

    memset(msg,0,sizeof(msg));
    sprintf(msg,"[Debug] 5F1C: siSubPhaseID:%d, siStepID:%d, siEffectTime:%d, uc5F10_ControlStrategy:%x, ", siSubPhaseID, siStepID, siEffectTime, uc5F10_ControlStrategy.DBit);
    smem.vWriteMsgToDOM(msg);


    if(uc5F10_ControlStrategy.switchBit.b5 == false && uc5F10_ControlStrategy.switchBit.b6 == false) {
      memset(msg,0,sizeof(msg));
      sprintf(msg,"[Debug] 5F1C: bit 5/6 all false");
      smem.vWriteMsgToDOM(msg);

      vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00); return false;
    }

    if(siSubPhaseID == 0 && siStepID == 0 && siEffectTime == 0) {  //Go To Next Step
//      stc.Lock_to_Set_Control_Strategy(STRATEGY_MANUAL);
//      stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);

      if (!stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC)) {
        stc.Lock_to_Set_Next_Step();
      }
      usleep(50);
      int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);

      printf("IN 5F1C, usiCurrentSubphaseStep:%d, Protocal5F1CStopStep:%d\n", usiCurrentSubphaseStep, Protocal5F1CStopStep);

    if(usiCurrentSubphaseStep == 0) {
      if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
        _intervalTimer.vAllDynamicToTODCount(i5F10_EffectTime);
      }
      else {
        _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_green_time));
      }
    }
    else if(usiCurrentSubphaseStep == 1)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
    else if(usiCurrentSubphaseStep == 2) {
      if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
        _intervalTimer.vAllDynamicToTODCount(i5F10_EffectTime);
      }
      else {
        _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedred_time));
      }
    }
    else if(usiCurrentSubphaseStep == 3)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_yellow_time));
    else if(usiCurrentSubphaseStep == 4)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_allred_time));



//      stc.vLockToSetControlStrategyToManualNotBySignal();
//      usleep(50);
//      stc.vLockToSetControlStrategyToTODNotBySignal();


//OT20111104
      smem.vSet5F18EffectTime(i5F10_EffectTime);

      return true;
    }

    if(siSubPhaseID == 0) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01); return false; }  //¿ù»~§PÂ_
    if(siStepID == 0) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02); return false; }

    unsigned short int usiCSTC_RunningPhaseCurrentPhase = stc.vGetUSIData(CSTC_exec_plan_phase_order);  //Should +1
    unsigned short int usiCSTC_RunningPhaseCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);  //Should +1
    unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    stc.Lock_to_Load_Phase_for_Center(usiCSTC_RunningPhaseCurrentPhase);

    return false;

} catch(...) {}
}
*/

//OT20111107
//-------------------------------------------------------------------
bool PTRAFFIC92TC::vGoToNextPhaseStepControl_5F1C(MESSAGEOK DataMessageIn)
{
    try
    {
        int iTmp;
        short int siSubPhaseID, siStepID, siEffectTime;
        bool bFlag1, bFlag2;
        siSubPhaseID = DataMessageIn.packet[9];
        siStepID = DataMessageIn.packet[10];
        siEffectTime = DataMessageIn.packet[11];
        unsigned short int usiCSTC_RunningPhaseCurrentPhase = stc.vGetUSIData(CSTC_exec_plan_phase_order);  //Should +1
        unsigned short int usiCSTC_RunningPhaseCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);  //Should +1
        unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
        unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStepGreenTime = stc.vGetUSIData(CSTC_exec_plan_green_time_compesated);

        //OT20140214
        unsigned short int usiCSTC_RunningPlanMaxGreen = stc.vGetUSIData(CSTC_exec_plan_maxgreen_time);
        unsigned short int usiCSTC_RunningPlanMinGreen = stc.vGetUSIData(CSTC_exec_plan_mingreen_time);
        unsigned short int usiCSTC_RunningStepTime = smem.vGetStepRunCounter();

        char msg[256];
        char cTMP[128];
        int newTime = -1;
        int iReturnCommandSet = smem.vGet0FCommandSet();


        printf("\n\n\n\nBUS actuate START!\n\n\n\n");

        if(siEffectTime == 255)    //car leave
        {
            smem.vSet5F1CEffectTime(siSubPhaseID-1, 0);
            if(usiCSTC_RunningPhaseCurrentSubphaseStep == 0)
            {
                if(usiCSTC_RunningPhaseCurrentSubphaseStepGreenTime > usiCSTC_RunningStepTime)
                {
                    iTmp = usiCSTC_RunningPhaseCurrentSubphaseStepGreenTime - usiCSTC_RunningStepTime;
                    stc.vSetTODCurrentTime(iTmp);
                    newTime = iTmp;
                }
                else
                {
                    stc.vSetTODCurrentTime(1);
                    newTime = 1;
                }
            }
            smem.vSetCurrentSubphaseRunning5F1C(usiCSTC_RunningPhaseCurrentSubphase, false);

        }
        else
        {

            bFlag1 = false;
            bFlag2 = false;
            if((usiCSTC_RunningPhaseCurrentSubphase+1) == siSubPhaseID)
            {
                bFlag1 = true;
            }
            if(usiCSTC_RunningPhaseCurrentSubphaseStep == 0 && siStepID == 1)
            {
                bFlag2 = true;
            }
            if(bFlag1 == true && bFlag2 == true)
            {
                /*
                        printf("siEffectTime:%d\n", siEffectTime);
                        printf("usiCSTC_RunningPlanMinGreen:%d\n", usiCSTC_RunningPlanMinGreen);
                        printf("usiCSTC_RunningStepTime:%d\n", usiCSTC_RunningStepTime);
                        printf("usiCSTC_RunningPlanMaxGreen:%d\n", usiCSTC_RunningPlanMaxGreen);
                */
                // in current step
                smem.vSetCurrentSubphaseRunning5F1C(usiCSTC_RunningPhaseCurrentSubphase, true);
                //should set effect time.
                if( (siEffectTime+usiCSTC_RunningStepTime) <= usiCSTC_RunningPlanMaxGreen)
                {
                    if((siEffectTime+usiCSTC_RunningStepTime) >= usiCSTC_RunningPlanMinGreen)
                    {
                        stc.vSetTODCurrentTime(siEffectTime);
                        newTime = siEffectTime;
                    }
                    else
                    {
                        iTmp = usiCSTC_RunningPlanMinGreen - usiCSTC_RunningStepTime;  //run min green
                        if(iTmp > 0)
                        {
                            stc.vSetTODCurrentTime(iTmp);
                            newTime = iTmp;
                        }
                    }
                }
                else
                {
                    iTmp = usiCSTC_RunningPlanMaxGreen - usiCSTC_RunningStepTime;  //run max green
                    if(iTmp > 0)
                    {
                        stc.vSetTODCurrentTime(iTmp);
                        newTime = iTmp;
                    }
                    else
                    {
                        stc.vSetTODCurrentTime(1);
                        newTime = 1;
                    }
                }
            }
            else      // diff phase
            {
                // 1. set this subphase to min green
                if(smem.vGetCurrentSubphaseRunning5F1C(usiCSTC_RunningPhaseCurrentSubphase) == false)
                {
                    if(usiCSTC_RunningPhaseCurrentSubphaseStep == 0)
                    {
                        printf("in diff 5F1C subphase\n");
                        if(usiCSTC_RunningStepTime > usiCSTC_RunningPlanMinGreen)
                        {
                            //force to next step.
                            //stc.Lock_to_Set_Next_Step();  //not effect
                            stc.vSetTODCurrentTime(1);
                            newTime = 1;
                        }
                        else
                        {
                            printf("usiCSTC_RunningPlanMinGreen:%d\n", usiCSTC_RunningPlanMinGreen);
                            printf("usiCSTC_RunningStepTime:%d\n", usiCSTC_RunningStepTime);
                            iTmp = usiCSTC_RunningPlanMinGreen - usiCSTC_RunningStepTime;  //run min green
                            if(iTmp > 0)
                            {
                                stc.vSetTODCurrentTime(iTmp);
                                newTime = iTmp;
                            }
                        }
                    }
                }
                // 2. set other subphase to min green
                smem.vSet5F1CEffectTime(siSubPhaseID-1, siEffectTime);
            }

        }

        //OT20140304
        bzero(cTMP, 128);
        sprintf(cTMP, "get 5F1C, now sp:%d, st:%d, rt:%d, protocol sp:%d, st:%d, et:%d, new t:%d, ",
                usiCSTC_RunningPhaseCurrentSubphase+1, usiCSTC_RunningPhaseCurrentSubphaseStep+1,
                usiCSTC_RunningStepTime,
                siSubPhaseID, siStepID, siEffectTime, newTime );
        smem.vWriteMsgToTmp(cTMP);

//----- another
        if (smem.Restartcheck5F1C == false)
        {
            smem.Restartcheck5F1C = true ;
            if (smem.vGetBOOLData(TC_Actuateautoswitch) == true)
            {
                if (smem.GetcFace() != cLOGIN && smem.GetcFace() != cLOGO)
                {
                    printf("\n\n\n\nBUS actuate START!\n\n\n\n");
                    screenActuateArwenManyStatus.DisplayActuateArwenManyStatus();//BUS觸動
                }
            }
            else
            {
            }

            /*if (siSubPhaseID == 0xFF && siStepID == 0xFF){
                 stc.Fixgreen = true;
             }
             else{
                 stc.Fixgreen = false;
             }

             if (stc.Fixgreen == true){
                 if (usiCSTC_RunningPhaseCurrentSubphase == 0 && usiCSTC_RunningPhaseCurrentSubphaseStep == 0){
                     unsigned short int usiStepSec = stc.vGetStepTime();
                     stc.vExtendGreenTime(siEffectTime,stc.Fixgreen);
                     stc.Fixgreen = false;
                     vReturnToCenterACK(0x5F, 0x1C);
                 }
                 if (usiCSTC_RunningPhaseCurrentSubphase == 1 && usiCSTC_RunningPhaseCurrentSubphaseStep == 0){
                     unsigned short int Nowsteptime = smem.vGetActRunningGreenTime();
                     short int Mingreen = stc.vGetAdjCurrentMinGreen() ;
                     printf("\nNowsteptime= %d \n Mingreen = %d \n",Nowsteptime,Mingreen);

                     if(Mingreen > Nowsteptime)
                     {
                         printf("\nNowsteptime= %d < Mingreen = %d \n",Nowsteptime,Mingreen);
                         stc.vExtendGreenTime((Mingreen - Nowsteptime),stc.Fixgreen);
                     }
                     else{
                         smem.vSetBOOLData(NextStepForceEnable, true);
                         stc.vSetNextStepNotUseSignal();
                     }
                     stc.Fixgreen = false ;
                     vReturnToCenterACK(0x5F, 0x1C); return false;
                 }
                 if (usiCSTC_RunningPhaseCurrentSubphase == 0 && usiCSTC_RunningPhaseCurrentSubphaseStep != 0){
                     //printf("\n\n\n\n 3333 \n 3333 \n\n\n\n\n");
                     vReturnToCenterACK(0x5F, 0x1C); return false;
                 }
                 if (usiCSTC_RunningPhaseCurrentSubphase != 0 && usiCSTC_RunningPhaseCurrentSubphaseStep != 0){
                     //printf("\n\n\n\n 4444 \n 4444 \n\n\n\n\n");
                     vReturnToCenterNACK(0x5F, 0x1C, 0x02, 0x00); return false;
                 }
             }*/
            if (iReturnCommandSet <= 2 )
            {
                vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00);
                return false;
            }

            if ( DataMessageIn.packetLength < 15 )
            {
                vReturnToCenterNACK(0x5F, 0x1C, 0x08, 0x00);
                return false;
            }
            else if ( DataMessageIn.packetLength > 15 )
            {
                vReturnToCenterNACK(0x5F, 0x1C, 0x08, DataMessageIn.packetLength - 15);
                return false;
            }

            if(siSubPhaseID > 8)
            {
                vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01);
                return false;
            }
            if(siStepID > 8)
            {
                vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02);
                return false;
            }

            DATA_Bit uc5F10_ControlStrategy;
            int i5F10_EffectTime;

            uc5F10_ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
            i5F10_EffectTime = smem.vGetINTData(TC92_iEffectTime);

            memset(msg,0,sizeof(msg));
            sprintf(msg,"[Debug] 5F1C: siSubPhaseID:%d, siStepID:%d, siEffectTime:%d, uc5F10_ControlStrategy:%x, ", siSubPhaseID, siStepID, siEffectTime, uc5F10_ControlStrategy.DBit);
            smem.vWriteMsgToDOM(msg);


            if(uc5F10_ControlStrategy.switchBit.b5 == false && uc5F10_ControlStrategy.switchBit.b6 == false)
            {
                memset(msg,0,sizeof(msg));
                sprintf(msg,"[Debug] 5F1C: bit 5/6 all false");
                smem.vWriteMsgToDOM(msg);

                vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00);
                return false;
            }
            if(smem.vGet5F18EffectTime() <= 0)
            {
                memset(msg,0,sizeof(msg));
                sprintf(msg,"[Debug] 5F18: Effect time Error");
                smem.vWriteMsgToDOM(msg);
                vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00);
                return false;
            }

            if(siSubPhaseID == 0 && siStepID == 0 && siEffectTime == 0)    //Go To Next Step
            {
//      stc.Lock_to_Set_Control_Strategy(STRATEGY_MANUAL);
//      stc.Lock_to_Set_Control`_Strategy(STRATEGY_TOD);

//OT20111107      if (!stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC)) {
                if(1)
                {
                    int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
                    if(usiCurrentSubphaseStep <= 2)
                    {
                        stc.Lock_to_Set_Next_Step();
                        usleep(50);
                        printf("IN 5F1C, usiCurrentSubphaseStep:%d, Protocal5F1CStopStep:%d\n", usiCurrentSubphaseStep, Protocal5F1CStopStep);
                    }
                }

                return true;
            }

            if(siSubPhaseID == 0)
            {
                vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01);    //¿ù»~§PÂ_
                return false;
            }
            if(siStepID == 0)
            {
                vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02);
                return false;
            }

            unsigned short int usiCSTC_RunningPhaseCurrentPhase = stc.vGetUSIData(CSTC_exec_plan_phase_order);  //Should +1
            unsigned short int usiCSTC_RunningPhaseCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);  //Should +1
            unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
            stc.Lock_to_Load_Phase_for_Center(usiCSTC_RunningPhaseCurrentPhase);

        }
        else
        {
            printf("\n\n\n\n you can't call 5F1C again !!!\n\n\n\n");
            // printf("\n\n\n\n you can't call 5F1C again !!!\n\n\n\n");
        }

        return false;

    }
    catch(...) {}
}


//OT1000218 Version
//-------------------------------------------------------------------
/*
bool PTRAFFIC92TC::vGoToNextPhaseStepControl_5F1C(MESSAGEOK DataMessageIn)
{
try {

    short int siSubPhaseID, siStepID, siEffectTime;
    siSubPhaseID = DataMessageIn.packet[9];
    siStepID = DataMessageIn.packet[10];
    siEffectTime = DataMessageIn.packet[11];

    char msg[256];

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if (iReturnCommandSet <= 2 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00); return false; }

    if ( DataMessageIn.packetLength < 15 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x08, 0x00); return false; }
    else if ( DataMessageIn.packetLength > 15 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x08, DataMessageIn.packetLength - 15); return false; }

    if(siSubPhaseID > 8) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01); return false; }
    if(siStepID > 8) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02); return false; }

    DATA_Bit uc5F10_ControlStrategy;
    int i5F10_EffectTime;

    uc5F10_ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
    i5F10_EffectTime = smem.vGetINTData(TC92_iEffectTime);

    memset(msg,0,sizeof(msg));
    sprintf(msg,"[Debug] 5F1C: siSubPhaseID:%d, siStepID:%d, siEffectTime:%d, uc5F10_ControlStrategy:%x, ", siSubPhaseID, siStepID, siEffectTime, uc5F10_ControlStrategy.DBit);
    smem.vWriteMsgToDOM(msg);


    if(uc5F10_ControlStrategy.switchBit.b5 == false && uc5F10_ControlStrategy.switchBit.b6 == false) {
      memset(msg,0,sizeof(msg));
      sprintf(msg,"[Debug] 5F1C: bit 5/6 all false");
      smem.vWriteMsgToDOM(msg);

      vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00); return false;
    }

    if(siSubPhaseID == 0 && siStepID == 0 && siEffectTime == 0) {  //Go To Next Step
//      stc.Lock_to_Set_Control_Strategy(STRATEGY_MANUAL);
//      stc.Lock_to_Set_Control`_Strategy(STRATEGY_TOD);


//OT1000218
    int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    if(usiCurrentSubphaseStep <= 2) {
      stc.Lock_to_Set_Next_Step();
    }

    usleep(50);
    usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);

    memset(msg,0,sizeof(msg));
    sprintf(msg,"[Debug] 5F1C: new usiCurrentSubphaseStep:%d", usiCurrentSubphaseStep);
    smem.vWriteMsgToDOM(msg);

    //OT1000218

      return true;
    }

    if(siSubPhaseID == 0) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01); return false; }  //¿ù»~§PÂ_
    if(siStepID == 0) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02); return false; }

    unsigned short int usiCSTC_RunningPhaseCurrentPhase = stc.vGetUSIData(CSTC_exec_plan_phase_order);  //Should +1
    unsigned short int usiCSTC_RunningPhaseCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);  //Should +1
    unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    stc.Lock_to_Load_Phase_for_Center(usiCSTC_RunningPhaseCurrentPhase);

    return false;

} catch(...) {}
}
*/


//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryPhaseStepControl_5F4C(MESSAGEOK DataMessageIn)
{
    try
    {

//For Amegids of NCKU, not Ver3 Protocal
//  unsigned short int usiEffectTime = _intervalTimer.vGetEffectTime();
        unsigned short int usiStepSec = stc.vGetStepTime();

        unsigned char data[6];
        data[0] = 0x5F;
        data[1] = 0xCC;
        data[2] = smem.vGetUCData(TC92_ucControlStrategy);
        data[3] = stc.vGetUSIData(CSTC_exec_phase_current_subphase) + 1;
        data[4] = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step)  + 1;

//  stc.vGetStepTime();

//  if( stc.Lock_to_LoadControlStrategy == STRATEGY_ALLDYNAMIC )

//For Amegids of NCKU, not Ver3 Protocal
//    data[5] = (usiEffectTime>>8) & 0x00ff;
//    data[6] = usiEffectTime & 0x00ff;
        data[5] = (usiStepSec>>8) & 0x00ff;
        data[6] = usiStepSec & 0x00ff;

//  else
//    data[4] = 0;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 7, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        return true;

    }
    catch(...) {}
}

//OT20140415
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vSetDynSegParameter(MESSAGEOK DataMessageIn)
{
    try
    {
        unsigned char ucAdjHour, ucAdjMin;
        unsigned short int usiAdjSec;
        ucAdjHour = DataMessageIn.packet[9];
        ucAdjMin = DataMessageIn.packet[10];
//  ucAdjType = DataMessageIn.packet[10];
        usiAdjSec = DataMessageIn.packet[11]*256 + DataMessageIn.packet[12];
        smem.vSetUCData(TCDynSegAdjustHour, ucAdjHour);
        smem.vSetUCData(TCDynSegAdjustMin, ucAdjMin);
        smem.vSetUSIData(TCDynSegAdjustKeepTime, usiAdjSec);
        smem.vSetUSIData(TCDynSegRemainSec, usiAdjSec);

        vReturnToCenterACK(0x5F, 0x2A);
        return true;

    }
    catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vQueryDynSegParameter(MESSAGEOK DataMessageIn)
{
    try
    {

        unsigned char ucAdjHour = smem.vGetUCData(TCDynSegAdjustHour);
        unsigned char ucAdjMin = smem.vGetUCData(TCDynSegAdjustMin);
        unsigned short int usiAdjKeepTime = smem.vGetUSIData(TCDynSegAdjustKeepTime);
        unsigned short int usiRemainSec = smem.vGetUSIData(TCDynSegRemainSec);
        bool bSW = smem.vGetBOOLData(TCDynSegSwitch);
        bool bStatus = smem.vGetBOOLData(TCDynSegStatus);
        unsigned short int usiPlanCycle = stc.vGetUSIData(CSTC_exec_plan_cycle);
        time_t now = smem.vGetSystemTime();

        unsigned char data[17];
        data[0] = 0x5F;
        data[1] = 0xDA;
        data[2] = ucAdjHour;
        data[3] = ucAdjMin;
        data[4] = usiAdjKeepTime / 256;
        data[5] = usiAdjKeepTime % 256;
        data[6] = bSW;
        data[7] = bStatus;
        data[8] = usiRemainSec / 256;
        data[9] = usiRemainSec % 256;

        data[10] = usiPlanCycle / 256;
        data[11] = usiPlanCycle % 256;

        data[12] = ( now >> 24) & 0xff;
        data[13] = ( now >> 16) & 0xff;
        data[14] = ( now >> 8)  & 0xff;
        data[15] = ( now ) & 0xff;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 16, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        //  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        return true;

    }
    catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92TC::vSetDynSegSwitch(MESSAGEOK DataMessageIn)
{
    try
    {
        unsigned char ucSW;
        ucSW = DataMessageIn.packet[9];
        if(ucSW > 0)
        {
            smem.vSetBOOLData(TCDynSegSwitch, true);
            smem.vSetBOOLData(TCDynSegNextPlanIdSendToCCJ, true);
            //stc.Lock_to_Determine_SegmentPlanPhase_DynSeg();
            //stc.vCCJTOMSendTODNextPlanID();
        }
        else
        {
            smem.vSetBOOLData(TCDynSegSwitch, false);

//OT20140520
            smem.vSetUCData(TCDynSegAdjustHour, 0);
            smem.vSetUCData(TCDynSegAdjustMin, 0);
            smem.vSetUSIData(TCDynSegAdjustKeepTime, 0);

        }
        vReturnToCenterACK(0x5F, 0x2B);
        return true;
    }
    catch(...) {}
}



//ACK------------------------------------------------------------------------
int PTRAFFIC92TC::vReturnToCenterACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode)
{
    try
    {
        unsigned char data[4];

        data[0] = 0x0F;
        data[1] = 0x80;
        data[2] = ucDevCode;
        data[3] = ucControlCode;

        MESSAGEOK _MsgOK;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
        _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        //jacky20140415 to jsonv3
        //writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICEJSON);

        return 0;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
int PTRAFFIC92TC::vReturnToCenterNACK(unsigned char ucDevCode,
                                      unsigned char ucControlCode,
                                      unsigned char ErrorCode,
                                      unsigned char ParameterNumber)
{
    try
    {
        unsigned char data[6];

        data[0] = 0x0F;
        data[1] = 0x81;
        data[2] = ucDevCode;
        data[3] = ucControlCode;
        data[4] = ErrorCode;
        data[5] = ParameterNumber;

        MESSAGEOK _MsgOK;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
        _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        //jacky20140415 to jsonv3
        // writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICEJSON);

        return 0;
    }
    catch (...) {}
}
////----------------jacky20150324--------------------------------------------------
//bool PTRAFFIC92TC::vTouchControlConfiguration_5F19(MESSAGEOK DataMessageIn)
//{
//try{
//    if( DataMessageIn.packetLength < 17 ) {
//        vReturnToCenterNACK(0x5F, 0x19, 0x08, 0x00);
//        return false;
//    }
//    else if( DataMessageIn.packetLength > 17 ) {
//        vReturnToCenterNACK(0x5F, 0x19, 0x08, DataMessageIn.packetLength - 12);
//        return false;
//    }
//    else if(DataMessageIn.packet[9] < 1 && DataMessageIn.packet[9] > 8){
//        vReturnToCenterNACK(0x5F, 0x19, 0x04, 0x01);
//        return false;
//    }
//    else if(DataMessageIn.packet[10] != 0 && DataMessageIn.packet[10] != 1 && DataMessageIn.packet[10] != 2 &&
//            DataMessageIn.packet[10] != 4 && DataMessageIn.packet[10] != 8 && DataMessageIn.packet[10] != 16 &&
//            DataMessageIn.packet[10] != 32 && DataMessageIn.packet[10] != 64){
//        vReturnToCenterNACK(0x5F, 0x19, 0x04, 0x02);
//        return false;
//    }
//
//    if(DataMessageIn.packet[10] == 0){
//        SubPhaseId_5F19 = 0;
//        ActuateType_5F19 = 0;
//        TimeExtend_5F19 = 0;
//        ActuateData1_5F19 = 0;
//        ActuateData2_5F19 = 0;
//    }
//    else {
//        SubPhaseId_5F19 = DataMessageIn.packet[9];
//        ActuateType_5F19 = DataMessageIn.packet[10];
//        TimeExtend_5F19 = DataMessageIn.packet[11];
//        ActuateData1_5F19 = DataMessageIn.packet[12];
//        ActuateData2_5F19 = DataMessageIn.packet[13];
//    }
//
//    vReturnToCenterACK(0x5F, 0x19);
//
// } catch(...){}
//}
////---------------------------------------------------------------------------
//bool PTRAFFIC92TC::vTouchControlConfiguration_5F49(MESSAGEOK DataMessageIn)
//{
//try{
//    vReturnToCenterACK(0x5F, 0x49);
//
//    if( DataMessageIn.packetLength < 13 ) {
//        vReturnToCenterNACK(0x5F, 0x49, 0x08, 0x00);
//        return false;
//    }
//    else if( DataMessageIn.packetLength > 13 ) {
//        vReturnToCenterNACK(0x5F, 0x49, 0x08, DataMessageIn.packetLength - 12);
//        return false;
//    }
//    else if(DataMessageIn.packet[9] != 0 && DataMessageIn.packet[9] != 1 && DataMessageIn.packet[9] != 2 &&
//            DataMessageIn.packet[9] != 4 && DataMessageIn.packet[9] != 8 && DataMessageIn.packet[9] != 16 &&
//            DataMessageIn.packet[9] != 32 && DataMessageIn.packet[9] != 64){
//        vReturnToCenterNACK(0x5F, 0x49, 0x04, 0x01);
//        return false;
//    }
//
//    unsigned char data[7];
//
//    data[0] = 0x5F;
//    data[1] = 0xC9;
//    data[2] = SubPhaseId_5F19;
//    data[3] = ActuateType_5F19;
//    data[4] = TimeExtend_5F19;
//    data[5] = ActuateData1_5F19;
//    data[6] = ActuateData2_5F19;
//
//    MESSAGEOK _MsgOK;
//
//    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 7,true);
//    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
//
// } catch(...){}
//}
