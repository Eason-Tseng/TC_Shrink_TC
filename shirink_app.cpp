#include "shirink_app.h"
#include "SMEM.h"
#include "CTIMER.h"
#include "screenLightEdit.h"
#include "SCREENLast92TCPlanSegmentUpdate.h"
#include "PTRAFFIC92COMM.h"
#include "PTRAFFIC92TC.h"
#include "CSTC.h"
#include "screenRedCountHWCheckNum88.h"
#include "screenRedCountHWCheckDevID.h"
#include "screenSegtype.h"
shirink_app shrinkAPP;
shirink_app::shirink_app() {
  //ctor
}
shirink_app::~shirink_app() {
  //dtor
}
void shirink_app::refresh_railchain_parama() {

  try {
    s_railchain_parama.sTC_TrainChainEnable =
        smem.vGetUCData(TC_TrainChainEnable);
    s_railchain_parama.sTrainComingBanSubphase =
        smem.vGetUCData(TC_TrainComingBanSubphase);
    s_railchain_parama.sTrainComingForceJumpSubphase =
        smem.vGetUCData(TC_TrainComingForceJumpSubphase);

  }
  catch (...) {

  }

}
void shirink_app::refresh_manual_setting() {
  try {
    /*      s_tc_manual_setting.dbOperStat=smem.GetDbOperStat();
        //  s_tc_manual_setting.HWCycleCodeIN
          s_tc_manual_setting.WayMappingRedCountIN=smem.vGetWayMappingRedCount();*/
  }
  catch (...) {}

}
void shirink_app::refresh_proxy_transfer() {


  try {

    s_proxy_transfer.PassMode_0F8E8FIN = smem.vGetPassMode();
    s_proxy_transfer.proxy_LCN = smem.vGetPassServerLCN();

  }
  catch (...) {}

}
void shirink_app::refresh_signal_card_direction() {
  try {

    for (int i = 0; i < 8; i++)
      s_signal_card_direction.SignamMappingDirIN[i] =
          smem.vGetSignamMapMappingDir(i);
    for (int i = 0; i < 6; i++)
      s_signal_card_direction.SignamMappingLightBoardIN[i] =
          smem.vGetSignamMapMappingLightBoard(i);

  }
  catch (...) {}

}
void shirink_app::refresh_tc_stratage_set() {
  try {
    s_tc_stratage_set.TC_ControlStrategyIN =
        smem.vGetUCData(TC92_ucControlStrategy);
    s_tc_stratage_set.SignalStepStatus_5F03_IntervalTimeIN =
        smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
    s_tc_stratage_set.TC_SignalStepStatus_5F03_IntervalTimeIN =
        smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
    s_tc_stratage_set.RedCountVerSelectIN = smem.vGetINTData(TC92_RedCountVer);
    s_tc_stratage_set.TC_ActuateTypeFunctionEnableIN =
        smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable);
    s_tc_stratage_set.TC_CCT_In_LongTanu_ActuateType_PlanID =
        smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
    s_tc_stratage_set.ActuatePhaseExtendIN = smem.vGetActuatePhaseExtend();
    s_tc_stratage_set.ActuateautoswitchIN =
        smem.vGetBOOLData(TC_Actuateautoswitch);

  }
  catch (...) {}

}
void shirink_app::refresh_compensation() {
  try {
    //  s_compensation.enable_switch=smem.
    s_compensation.cycle_num = smem.GetCompensationcycle();

  }
  catch (...) {}

}
void shirink_app::refresh_learn_mode_group() {
  try {
    // s_learn_mode_group.enable_car_learn;
    s_learn_mode_group.CarLearn = smem.GetCarLearnSwitch();
    //   s_learn_mode_group.enable_ped_learn;
    s_learn_mode_group.PedLearn = smem.GetPedLearnSwitch();
    // s_learn_mode_group.enable_CarCountDown_learn;
    s_learn_mode_group.CarCountDownLearn = smem.GetCarCountdownProperty();
    // s_learn_mode_group.enable_PedCount_learn;
    s_learn_mode_group.PedCountLearn = smem.GetPedCountdownProperty();

  }
  catch (...) {}

}
void shirink_app::refresh_ped_control_set() {
  try {
    //s_ped_control_set.enable_SWCount=smem.;
    s_ped_control_set.SWDevCount = smem.cPedPushButton.GetDevCount();

  }
  catch (...) {}
}
void shirink_app::refresh_chain_set_group() {
  try {

    s_chain_set_group.sTC_5F31Manual = smem.vGetUCData(TC92_5F31Manual);
    s_chain_set_group.sTC_5F31TOD = smem.vGetUCData(TC92_5F31TOD);
    s_chain_set_group.sTC_5F31StartSubPhaseId =
        smem.vGetUCData(TC92_5F31StartSubPhaseId);
    s_chain_set_group.sTC_5F32EndSubPhaseId =
        smem.vGetUCData(TC92_5F31EndSubPhaseId);
    s_chain_set_group.sTC_5F32StartSubPhaseId =
        smem.vGetUCData(TC92_5F32StartSubPhaseId);
    s_chain_set_group.sTC_5F32EndSubPhaseId =
        smem.vGetUCData(TC92_5F32EndSubPhaseId);
    s_chain_set_group.sTC_MotherChainStartStepId =
        smem.vGetUCData(TC_MotherChainStartStepId);
    s_chain_set_group.sTC_MotherChainEndStepId =
        smem.vGetUCData(TC_MotherChainEndStepId);
    for (int i = 0; i < 64; i++) {
      s_chain_set_group.TC_5F33StartOffset[i] = smem.vGetChainOffset(1, i);
      s_chain_set_group.TC_5F33EndOffset[i] = smem.vGetChainOffset(2, i);
    }

  }
  catch (...) {}

}
void shirink_app::refresh_tc_project_data() {
  try {
    for (int phaseNum = 0; phaseNum < 256; phaseNum++) {

      s_tc_project_data.phase[phaseNum] = stc.Get_stc_phase(phaseNum);//256
      s_tc_project_data.plan[phaseNum] = stc.Get_stc_plan(phaseNum);//256
      if (phaseNum < 21)
        s_tc_project_data.segment[phaseNum] = stc.Get_stc_segment(phaseNum);//21
      if (phaseNum < 17)
        s_tc_project_data.reversetime[phaseNum] = stc.Get_stc_reversetime(phaseNum);//17
      if (phaseNum < 14) {
        s_tc_project_data.weekdayseg[phaseNum] =
            stc.Get_stc_weekdayseg(phaseNum);//14 //{0-6,7-13} according to {1-7,11-17}
        s_tc_project_data.weekdayrev[phaseNum] = stc.Get_stc_weekdayrev(phaseNum);
      }//14 //{0-6,7-13} according to {1-7,11-17}
      if (phaseNum < 13) {
        s_tc_project_data.holidayrev[phaseNum] =
            stc.Get_stc_holidayrev(phaseNum);//13 //{0-12} according to {4-16}
        s_tc_project_data.holidayseg[phaseNum] =
            stc.Get_stc_holidayseg(phaseNum);//13 //{0-12} according to {8-20}
      }
    }

  }
  catch (...) {}

}
void shirink_app::refresh_execute_data() {
  execute_data._exec_phase_current_subphase =
      stc.vGetUSIData(CSTC_exec_phase_current_subphase);;  //start from 0
  execute_data._exec_phase_current_subphase_step =
      stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
  execute_data._exec_segment_current_seg_no =
      stc.vGetUSIData(CSTC_exec_segment_current_seg_no);
  execute_data._exec_reversetime_current_rev_step =
      stc.Get_exec_reversetime_current_rev_step();  //start from 0
  execute_data._old_strategy = stc.Get_current_stratage();
  execute_data._current_strategy = stc.Get_old_stratage();
}
/***-------SET SERIRES----------------------****/
void shirink_app::set_ip(Json::Value object) {
  try {
//Json::Value object;
//
//printf("set IP\n");
//
//object=object1["IP_Group"];
    printf("obj\n");
    printf("%s\n", object.toStyledString().c_str());

//       printf("obj1\n");
    //printf("%s\n",object1.toStyledString().c_str());
    char cRunString[256];
    int localIp1_[6];//localIp[5]=localport
    int distIp0_[6];//distIp0_[5]=distIp0_port
    int distIp1_[6];//distIp1_[5]=distIp1_port
    int netmask[5];
    int gateway[5];
    for (int i = 1; i < 6; i++) {
      if (i < 5) {
        localIp1_[i] = object["HostIP"][i - 1].asInt();
        distIp0_[i] = object["DestIP0"][i - 1].asInt();
        distIp1_[i] = object["DestIP1"][i - 1].asInt();
      }
      if (i == 5) {
        localIp1_[i] = object["HostPort"].asInt();
        distIp0_[i] = object["Dest0_Port"].asInt();
        distIp1_[i] = object["Dest1_Port"].asInt();
      }
      smem.SetLocalIP1(i, localIp1_[i]);
      smem.SetdistIp0(i, distIp0_[i]);
      smem.SetDistIP(i, distIp1_[i]);
    }
    for (int i = 1; i < 5; i++) {
      netmask[i] = object["Netmask"][i - 1].asInt();
      gateway[i] = object["Gateway"][i - 1].asInt();
      smem.SetNetmask(i, netmask[i]);
      smem.SetGateway(i, gateway[i]);
    }
    smem.WriteLCNby_shrinkAPP(object["LCN"].asInt());
    printf("****receive network setting is =***\n");
    for (int i = 1; i < 6; i++)
      printf("%d ", localIp1_[i]);
    printf("\n");
    for (int i = 1; i < 6; i++)
      printf("%d ", distIp0_[i]);

    printf("\n");
    for (int i = 1; i < 6; i++)
      printf("%d ", distIp0_[i]);
    printf("\n");
    for (int i = 1; i < 5; i++)
      printf("%d ", netmask[i]);
    printf("\n");
    for (int i = 1; i < 5; i++)
      printf("%d ", gateway[i]);
    printf("\n");
    printf("LCN=%d\n", object["LCN"].asInt());

    system("rm -rf /bin/quickNetwork2");
    system("rm -rf /cct/quickNetwork2");
    bzero(cRunString, sizeof(cRunString));
    sprintf(cRunString, "cp /bin/spaceSH /bin/quickNetwork2");
    sprintf(cRunString, "cp /bin/spaceSH /cct/quickNetwork2");
    system(cRunString);
    bzero(cRunString, sizeof(cRunString));
    sprintf(cRunString,
            "echo /sbin/ifconfig eth0:3 %d.%d.%d.%d netmask %d.%d.%d.%d >> /bin/quickNetwork2",
            localIp1_[1],
            localIp1_[2],
            localIp1_[3],
            localIp1_[4],
            netmask[1],
            netmask[2],
            netmask[3],
            netmask[4]);
    system(cRunString);
    bzero(cRunString, sizeof(cRunString));
    sprintf(cRunString,
            "echo /sbin/route add default gw %d.%d.%d.%d >> /bin/quickNetwork2",
            gateway[1],
            gateway[2],
            gateway[3],
            gateway[4]);
    system(cRunString);
    bzero(cRunString, sizeof(cRunString));
    sprintf(cRunString, "chmod +x /bin/quickNetwork2");
    system(cRunString);


    system("sync");
    system("sync");
//          system("reboot");





  }
  catch (...) {}
}
void shirink_app::set_railchain_parama() {
  try {

    /*       smem.vSetUCData(TC_TrainChainEnable);
             smem.vSetUCData(TC_TrainComingBanSubphase);
               smem.vSetUCData(TC_TrainComingForceJumpSubphase);*/

  }
  catch (...) {}
}
void shirink_app::set_proxy_transfer() {
  try {
    /*
    smem.vSetPassMode()
    smem.vSetPassServerLCN()

    */
  }
  catch (...) {}
}
void shirink_app::set_signal_card_direction() {
  try {
    /*smem.vSetSignamMapMappingDir(i);
    smem.vSetSignamMapMappingLightBoard(i);*/
  }
  catch (...) {}
}
void shirink_app::set_tc_stratage_set() {
  try {
    /*
      smem.vSetUCData(TC92_ucControlStrategy);
          smem.vSetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
          smem.vSetINTData(TC92SignalStepStatus_5F03_IntervalTime);
          smem.vSetINTData(TC92_RedCountVer);
          smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable);
          smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
          smem.vSetActuatePhaseExtend();
          smem.vSetBOOLData(TC_Actuateautoswitch);

      */

  }
  catch (...) {}
}
void shirink_app::set_compensation() {
  try {
    /*
    smem.SetCompensationcycle();
    */

  }
  catch (...) {}
}
void shirink_app::set_learn_mode_group() {
  try {
    /*
           smem.SetCarLearnSwitch();

           smem.SetPedLearnSwitch();

           smem.SetCarCountdownProperty();

           smem.SetPedCountdownProperty();

       */

  }
  catch (...) {}
}
void shirink_app::set_ped_control_set() {
  try {
//    refresh_ped_control_set();

  }
  catch (...) {}
}
void shirink_app::set_chain_set_group() {
  try {
    /*
       smem.vSetUCData(TC92_5F31Manual);
          smem.vSetUCData(TC92_5F31TOD);
          smem.vSetUCData(TC92_5F31StartSubPhaseId);
          smem.vSetUCData(TC92_5F31EndSubPhaseId);
          smem.vSetUCData(TC92_5F32StartSubPhaseId);
          smem.vSetUCData(TC92_5F32EndSubPhaseId);
          smem.vSetUCData(TC_MotherChainStartStepId);
          smem.vSetUCData(TC_MotherChainEndStepId);
          for(int i=0; i<64; i++)
          {
              smem.vSetChainOffset(1,i);
              smem.vSetChainOffset(2,i);
          }
      */
  }
  catch (...) {}
}
void shirink_app::set_tc_project_data() {
  try {
    /*   stc.Lock_to_Save_Phase();//phase
       stc.Lock_to_Save_Plan();//plan
       stc.Lock_to_Save_Segment();//segment
       stc.Lock_to_Save_Segment_from_Panel()//weekday
       stc.Lock_to_Save_HoliDaySegment();
       stc.Lock_to_Save_WeekDayReversetime_from_Center();
       stc.Lock_to_Save_HoliDaySegment()
       stc.Lock_to_Save_ReverseTime_Step2();
       */

  }
  catch (...) {}
}
bool shirink_app::setAcutateSetting(ActuateSetStruct actuateParameter) {
  try {
    int usiActType = actuateParameter.usiActType;
    int *valueSwitch = actuateParameter.valueSwitch;
    int *valueExtendSec = actuateParameter.valueExtendSec;
    unsigned int tmpActPlan = actuateParameter.usiActuatePlan;
    unsigned int tmpActionSec = actuateParameter.ActionSec;
    unsigned int tmpCompensateMode = actuateParameter.valueCompensateMode;
    unsigned int usActplayNo = actuateParameter.usActplayNo;
    bool res = false;
    if (usiActType > 6) {
      res = true;
    }
    for (int i = 0; i < 8; i++) {
      if (valueSwitch[i] > 1) {
        smem.vWriteMsgToDOM("ActuateSetting Fail 1\n");
        res = true;
        break;
      }
      if (valueExtendSec[i] > 9) {
        res = true;
        char temp[200]="";
        char tempBuff[16],buff[128]="";

        for (int i = 0; i < 8; i++) {
          sprintf(tempBuff, "Extend_%d=%d",i,valueExtendSec[i]);
          strcat(buff, tempBuff);
        }
        sprintf(temp,buff);
        smem.vWriteMsgToDOM(buff);
        smem.vWriteMsgToDOM("ActuateSetting Fail 2\n");

        break;
      }
    }
    if (tmpActPlan > 48) {
      res = true;
      smem.vWriteMsgToDOM("ActuateSetting Fail 3\n");

    }
    if (tmpActionSec > 20 && tmpActionSec != 0) {
      res = true;
      smem.vWriteMsgToDOM("ActuateSetting Fail 4\n");

    }
    if (tmpCompensateMode > 1) {
      res = true;
      smem.vWriteMsgToDOM("ActuateSetting Fail 5\n");

    }
    if (res) {//有參數超出範圍或寫錯
      return false;
    } else {
      smem.vSetUCData(actuateSwitchChange_for_Arwen,
                      1);//arwen actChange 1000707
      screenSegtype.DisplaySegtype(stc._panel_segment._segment_type,
                                   INVAILDVALUE);
      smem.saveSegmentActData(usActplayNo,
                              usiActType,
                              valueSwitch,
                              valueExtendSec,
                              tmpActPlan,
                              tmpActionSec,
                              tmpCompensateMode);
    }
    return true;
  } catch (...) {}

}
void shirink_app::Init_tc_project_data() {

  try {
    refresh_tc_project_data();
    Packed_Tod_info();
    Packed_Special_day_info();
    Packed_segmentinfo();
    Packed_plancontext_info();
    Packed_step_info();
    Packed_RedCountHWSetting_info();
    Packed_ActuateParameter();
    // printf("======================%s\n",string_to_app.toStyledString().c_str());

    //   printf("%s\n",string_to_app.toStyledString().c_str());
  }
  catch (...) {

  }
}
void shirink_app::Packed_Tod_info() {
  try {
    stc.Lock_to_Load_WeekDaySegment_for_Panel();
    for (int i = 0; i < 14; i++) {
      string_to_app["weekdaysegment"][i] =
          stc._panel_weekdayseg[i]._segment_type;
    }
  }
  catch (...) {}
}
void shirink_app::Packed_Special_day_info() {
  try {

    Json::Value specialdaycontext;
    for (int i = 8; i < 21; i++) {
      stc.Lock_to_Load_HoliDaySegment_for_Panel(i);
      specialdaycontext["segmenttype"] = stc._panel_holidayseg._segment_type;
      specialdaycontext["start_year"] = stc._panel_holidayseg._start_year;
      specialdaycontext["start_month"] = stc._panel_holidayseg._start_month;
      specialdaycontext["start_day"] = stc._panel_holidayseg._start_day;
      specialdaycontext["end_year"] = stc._panel_holidayseg._end_year;
      specialdaycontext["end_month"] = stc._panel_holidayseg._end_month;
      specialdaycontext["end_day"] = stc._panel_holidayseg._end_day;
      string_to_app["specialdaycontext"][i - 8] = specialdaycontext;
    }

  }
  catch (...) {}
}
void shirink_app::Packed_RedCountHWSetting_info() {
  Json::Value RedCountHWSetting;

  RedCountHWSetting["RedCountHWVersion"] = smem.vGetINTData(TC92_RedCountVer);
  for (int i = 0; i < 8; i++)
    RedCountHWSetting["RedCountLightBoard"][i] = smem.vGetWayMappingRedCount(i);
  string_to_app["RedCountHWSetting"] = RedCountHWSetting;

}
void shirink_app::Packed_segmentinfo() {
  try {
    Json::Value segcontext;
    for (int i = 0; i < 21; i++) {
      stc.Lock_to_Load_Segment_for_Panel(i);


      // segmentinfo["segment_type"][i]=stc._panel_segment._segment_type;
      segcontext["segment_type"] = stc._panel_segment._segment_type;
      for (int j = 0; j < 32; j++) //32
      {
        segcontext["hour"][j] = 0;
        segcontext["minute"][j] = 0;
        segcontext["plan"][j] = 0;
        segcontext["actMode"][j] = 0;
        if (j < stc._panel_segment._segment_count) {
          segcontext["hour"][j] =
              stc._panel_segment._ptr_seg_exec_time[j]._hour;
          segcontext["minute"][j] =
              stc._panel_segment._ptr_seg_exec_time[j]._minute;
          segcontext["plan"][j] =
              stc._panel_segment._ptr_seg_exec_time[j]._planid;
          segcontext["actMode"][j] =
              stc._panel_segment._ptr_seg_exec_time[j]._actMode;
        }
      }
      segcontext["segment_count"] = stc._panel_segment._segment_count;
      segmentinfo["segcontext"][i] = segcontext;
      //segmentinfo["segcontext"]=segcontext;
    }

    string_to_app["segmentinfo"] = segmentinfo;
  }
  catch (...) {}
}
void shirink_app::Packed_plancontext_info() {
  try {
    Json::Value plancontext;
    for (int j = 0; j < 49; j++) {
      stc.Lock_to_Load_Plan_for_Panel(j);

      plancontext["plan_id"] = stc._panel_plan._planid;
      plancontext["dir"] = stc._panel_plan._dir;

      plancontext["phase_order"] = stc._panel_plan._phase_order;
      plancontext["subphase_count"] = stc._panel_plan._subphase_count;
      plancontext["cycle_time"] = stc._panel_plan._cycle_time;
      plancontext["offset"] = stc._panel_plan._offset;
      for (int i = 0; i < 8; i++)       //8
      {
        plancontext["subphase_green"][i] =
            stc._panel_plan._ptr_subplaninfo[i]._green;
        plancontext["subphase_min_green"][i] =
            stc._panel_plan._ptr_subplaninfo[i]._min_green;
        plancontext["subphase_max_green"][i] =
            stc._panel_plan._ptr_subplaninfo[i]._max_green;
        plancontext["subphase_yellow"][i] =
            stc._panel_plan._ptr_subplaninfo[i]._yellow;
        plancontext["subphase_allred"][i] =
            stc._panel_plan._ptr_subplaninfo[i]._allred;
        plancontext["subphase_pedgreen_flash"][i] =
            stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash;
        plancontext["subphase_pedred"][i] =
            stc._panel_plan._ptr_subplaninfo[i]._pedred;

      }
      string_to_app["plancontext"][j] = plancontext;
    }
  }
  catch (...) {}
}
void shirink_app::Packed_step_info() {
  try {
    Json::Value step;

    for (int i = 0; i < 256; i++) {
      Json::Value
          subphase;//it can't be put out side the for() to avoid unexpected info
      Json::Value
          light;//it can't be put out side the for() to avoid unexpected info
      Json::Value
          stepdetail;//it can't be put out side the for() to avoid unexpected info

      stc.Lock_to_Load_Phase_for_Panel(i);
      step["phase_ID"] = i;
      step["phase_order"] = stc._panel_phase._phase_order;
      step["signal_map"] = stc._panel_phase._signal_map;
      step["signal_count"] = stc._panel_phase._signal_count;
      step["subphase_count"] = stc._panel_phase._subphase_count;
      for (int j = 0; j < 8; j++)
        step["sub_stepcount"][j] = stc._panel_phase._ptr_subphase_step_count[j];

      for (int subphaseIndex = 0;
           subphaseIndex < stc._panel_phase._subphase_count; subphaseIndex++) {
        for (int stepIndex = 0; stepIndex
            < stc._panel_phase._ptr_subphase_step_count[subphaseIndex];
             stepIndex++) {
          for (int signalIndex = 0;
               signalIndex < stc._panel_phase._signal_count; signalIndex++) {
            light["light"][signalIndex] =
                stc._panel_phase._ptr_subphase_step_signal_status[subphaseIndex][stepIndex][signalIndex];
          }
          stepdetail["stepdetail"][stepIndex] = light;

        }
        subphase["subphase"][subphaseIndex] = stepdetail;
      }
      step["stepcontext"] = subphase;
      string_to_app["step"][i] = step;
    }
  }
  catch (...) {}
}
void shirink_app::Packed_ActuateParameter() {
  try {
    Json::Value ActuateDTOs;
    for (int ActNum = 0; ActNum < 10; ActNum++) {
      Json::Value ActuateDTO;
      Json::Value PhaseTriggerSwitch;
      Json::Value PhaseExtendSec;
      for (int PedTriggerIndex = 0; PedTriggerIndex < 8; PedTriggerIndex++) {
        PhaseTriggerSwitch[PedTriggerIndex] =
            smem.getSegmentActVDPhaseTriggerSwitch(ActNum, PedTriggerIndex);
        PhaseExtendSec[PedTriggerIndex] =
            smem.getSegmentActExtendSec(ActNum, PedTriggerIndex);
      }
      ActuateDTO["ActNum"]=ActNum;
      ActuateDTO["PhaseTriggerSwitch"] = PhaseTriggerSwitch;
      ActuateDTO["PhaseExtendSec"] = PhaseExtendSec;
      ActuateDTO["ActType"] = smem.getSegmentActType(ActNum);
      ActuateDTO["ActPlan"] = smem.getSegmentActPlan(ActNum);
      ActuateDTO["ActSec"] = smem.getSegmentZoneTimes(ActNum);
      ActuateDTO["CompensateMode"] = smem.getSegmentCompensateMode(ActNum);
      ActuateDTOs["ActuateDTO"][ActNum] = ActuateDTO;
    }
    string_to_app["ActuateDTOs"] = ActuateDTOs;
  }
  catch (...) {}
}
void shirink_app::send_execute_data() {
  try {

    unsigned short int
        iCurrentPhaseID = stc.vGetUSIData(CSTC_exec_plan_phase_order);
    unsigned short int iCurrentPlanID = stc.vGetUSIData(CSTC_exec_plan_plan_ID);
    current_state["current_phaseID"] = iCurrentPhaseID;
    current_state["current_planID"] = iCurrentPlanID;
    /*        stc.Lock_to_Load_Plan_for_Panel(iCurrentPhaseID);
            stc.Lock_to_Load_Phase_for_Panel(iCurrentPlanID);
    */
    current_state["current_second"] = stc.vGetStepTime();
    int iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);

    int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;
    current_state["current_subphase"] = iCurrentSubphase + 1;
    current_state["current_total_subphase"] = iCurrentSubphaseTotal;
    int iCurrentStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    int iCurrentStepTotal = stc._panel_phase._total_step_count;
    printf("current_total_subphase=%d,current_total_step=%d\n",
           iCurrentSubphaseTotal,
           iCurrentStepTotal);
    current_state["current_step"] = iCurrentStep + ((iCurrentSubphase) * 5) + 1;
    current_state["current_total_step"] = iCurrentStepTotal;


    printf("%s\n", current_state.toStyledString().c_str());

  }
  catch (...) {}
}
void shirink_app::Init_ip() {

  try {

    Json::Value IP_Group;
    for (int Column = 1; Column < 5; Column++) {
      IP_Group["HostIP"][Column - 1] = s_ip.localIP1[Column - 1] = smem.GetLocalIP1(Column);
      IP_Group["DestIP0"][Column - 1] = smem.GetdistIp0(Column);
      IP_Group["DestIP1"][Column - 1] = smem.GetDistIP(Column);
      IP_Group["Netmask"][Column - 1] = smem.GetNetmask(Column);
      IP_Group["Gateway"][Column - 1] = smem.GetGateway(Column);
    }
    IP_Group["HostPort"] = smem.GetLocalIP1(5);
    IP_Group["Dest0_Port"] = smem.GetdistIp0(5);
    IP_Group["Dest1_Port"] = smem.GetDistIP(5);
    IP_Group["LCN"] = smem.GetAddress();
    string_to_app["IP_Group"] = IP_Group;
    printf("%s\n", IP_Group.toStyledString().c_str());
  }
  catch (...) {

  }

}
void shirink_app::set_manual_setting(Json::Value object1) {
  try {
    Json::Value object;
    object = object1["manual_setting"];
    if (object.isMember("DbOperStat")) {
      smem.SetDbOperStat(object["DbOperStat"].asInt());
      printf("change db control");
    }

  }
  catch (...) {}
}
void shirink_app::Init_manual_setting() {
  try {
    Json::Value manual_setting;
    manual_setting["DbOperStat"] = smem.GetDbOperStat();

    string_to_app["manual_setting"] = manual_setting;
  }
  catch (...) {}
}
void shirink_app::Init_RedCountHWSetting() {
  try {
    Json::Value RedCountHWSetting;
    RedCountHWSetting["RedCountHWVersion"] = smem.vGetINTData(TC92_RedCountVer);
    for (int i = 0; i < 8; i++)
      RedCountHWSetting["RedCountLightBoard"][i] =
          smem.vGetWayMappingRedCount(i);
    string_to_app["RedCountHWSetting"] = RedCountHWSetting;
  }
  catch (...) {}
}
void shirink_app::send_railchain_parama() {
  try {
    Json::Value railchaininfo;
    railchaininfo["TC_TrainChainEnable"] = smem.vGetUCData(TC_TrainChainEnable);
    railchaininfo["TrainComingBanSubphase"] =
        smem.vGetUCData(TC_TrainComingBanSubphase);
    railchaininfo["TrainComingForceJumpSubphase"] =
        smem.vGetUCData(TC_TrainComingForceJumpSubphase);

    string_to_app["railchaininfo"] = railchaininfo;
  }
  catch (...) {}
}
void shirink_app::send_proxy_transfer() {
  try {
    Json::Value proxy_transfer;
    proxy_transfer["proxy_PassMode"] = smem.vGetPassMode();
    proxy_transfer["proxy_LCN"] = smem.vGetPassServerLCN();

    string_to_app["proxy_transfer"] = proxy_transfer;
  }
  catch (...) {}
}
void shirink_app::send_signal_card_direction() {
  try {

    Json::Value signal_card_dir;
    for (int i = 0; i < 8; i++) {
      signal_card_dir["signalMapMappingDir"][i] =
          smem.vGetSignamMapMappingDir(i);
      if (i < 6)
        signal_card_dir["signalMappingLightBoard"][i] =
            smem.vGetSignamMapMappingLightBoard(i);

    }

    string_to_app["signal_car_dir"] = signal_card_dir;
  }
  catch (...) {}
}
void shirink_app::send_tc_stratage_send() {
  try {
    Json::Value tc_stratage;
    tc_stratage["ControlStratage"] = smem.vGetUCData(TC92_ucControlStrategy);
    tc_stratage["LightStatus_5F05_report_period"] =
        smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
    tc_stratage["StepStatus_report_period"] =
        smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
    tc_stratage["Redcount_version"] = smem.vGetINTData(TC92_RedCountVer);
    tc_stratage["ActuateTypeFunction_enable_switch"] =
        smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable);
    tc_stratage["In_LongTanu_ActuateType_PlanID"] =
        smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
    tc_stratage["ActuatePhaseExtend"] = smem.vGetActuatePhaseExtend();
    tc_stratage["Actuateautoswitch"] = smem.vGetBOOLData(TC_Actuateautoswitch);
    string_to_app["tc_stratage"] = tc_stratage;

  }
  catch (...) {}
}
void shirink_app::send_compensation() {
  try {
    Json::Value compensation;
    compensation["compensation_cycle"] = smem.GetCompensationcycle();
    string_to_app["compensation"] = compensation;
  }
  catch (...) {}
}
void shirink_app::send_learn_mode_group() {
  try {

    Json::Value learn_mode;
    learn_mode["CarLearn"] = smem.GetCarLearnSwitch();
    learn_mode["PedLearn"] = smem.GetPedLearnSwitch();
    learn_mode["CarCountDownLearn"] = smem.GetCarCountdownProperty();
    learn_mode["PedCountLearn"] = smem.GetPedCountdownProperty();
    string_to_app["learn_mode"] = learn_mode;
  }
  catch (...) {}
}
void shirink_app::send_ped_control_send() {
  try {
    Json::Value ped_control;
    ped_control["SWDevCount"] = smem.cPedPushButton.GetDevCount();


    string_to_app["ped_control"] = ped_control;
  }
  catch (...) {}
}
void shirink_app::set_step_info(Json::Value object) {


  try {


    if (object.isMember("step")) {


      unsigned short int usiLight[8][5][8];
      unsigned char ucPhaseID;
      //string ucPhaseID;
      unsigned char ucSignalMap;
      unsigned char ucSignalCount;
      unsigned char ucSubphaseCount;
      unsigned char ucStepCount[8];

      printf("set_step_info***********\n");
      printf("set_step_info***********\n");
      printf("set_step_info***********\n");
      printf("set_step_info***********\n");
      printf("set_step_info***********\n");
      printf("%s\n", object.toStyledString().c_str());
      ucPhaseID = object["step"]["phase_ID"].asInt();
      ucSignalCount = object["step"]["signal_count"].asInt();
      ucSubphaseCount = object["step"]["subphase_count"].asInt();
      printf("phase_ID by setting =%x  sigmancount=%d subphasecount=%d\n",
             ucPhaseID,
             ucSignalCount,
             ucSubphaseCount);


      bool bMollacOK;
      int iTMP;
      unsigned short int usiTmp;
      bMollacOK = stc.Lock_to_Reset_Phase_for_Panel(ucPhaseID,
                                                    ucSubphaseCount,
                                                    ucSignalCount);
      if (bMollacOK) {
        printf("mollacOK ucPhaseID:%X ucSubphaseCount:%d ucSignalCount:%d\n",
               ucPhaseID,
               ucSubphaseCount,
               ucSignalCount);

        stc._panel_phase._phase_order = ucPhaseID;
        if (ucSignalCount == 2) {
          // printf("ucSignalCount=%d\n",ucSignalCount);
          ucSignalMap = 0x11;
        }
        if (ucSignalCount == 3) {
          ucSignalMap = 0x51;
        }
        if (ucSignalCount == 4) {
          ucSignalMap = 0x55;
        }
        if (ucSignalCount == 5) {
          ucSignalMap = 0x5D;
        }
        if (ucSignalCount == 6) {
          ucSignalMap = 0x5F;
        }
        stc._panel_phase._signal_map = ucSignalMap;
        stc._panel_phase._signal_count = ucSignalCount;
        stc._panel_phase._subphase_count = ucSubphaseCount;
        stc._panel_phase._total_step_count = ucSubphaseCount * 5;  //¤­­Ó¨B¶¥



        printf("json subphase context=%s\n",
               object["step"]["stepcontext"]["subphase"][0]["stepdetail"][0].toStyledString().c_str());

        for (int i = 0; i < ucSubphaseCount; i++) {
          for (int j = 0; j < 5; j++) {
            for (int k = 0; k < ucSignalCount; k++) {
              printf("Subphasecount:%d StepCount:%d LightboardCount:%d\n",
                     i,
                     j,
                     k);
              usiLight[i][j][k] =
                  object["step"]["stepcontext"]["subphase"][i]["stepdetail"][j]["light"][k].asInt();

              printf("test steplight=%x %d subphase=%d step=%d board=%d\n",
                     usiLight[i][j][k],
                     usiLight[i][j][k],
                     i,
                     j,
                     k);
              //OT20110429
              usiTmp = usiLight[i][j][k];
              usiTmp = usiTmp & 0x3000;  //find red? or yellow
              if (usiTmp == 0x2000) {
                //change light!!
                usiLight[i][j][k] = usiLight[i][j][k] & 0xCFFF;  //clear old
                usiLight[i][j][k] = usiLight[i][j][k] | 0x1000;
              }

              usiTmp = usiLight[i][j][k];
              usiTmp = usiTmp & 0x000C;  //find red? or yellow
              if (usiTmp == 0x0008) {
                //change light!!
                usiLight[i][j][k] = usiLight[i][j][k] & 0xFFF3;  //clear old
                usiLight[i][j][k] = usiLight[i][j][k] | 0x0004;
              }


              if (usiLight[i][j][k] == 0x1004) {
                usiLight[i][j][k] = 0x2008;
              }
//OT20110429          if(usiLight[i][j][k] ==  0x2008) { usiLight[i][j][k] = 0x1004; }
              stc._panel_phase._ptr_subphase_step_signal_status[i][j][k] =
                  usiLight[i][j][k];
            }
          }
        }

        smem.vWriteMsgToDOM("WritePhaseByApp");

        stc.Lock_to_Save_Phase_from_Panel();             //Àx¦s¦^¥h
        smem.vSetINTData(TC92_iUpdatePhaseData, 1);
        smem.vSetTCPhasePlanSegTypeData(TC_Phase, ucPhaseID, true);

      }

    } else printf("step setting no step_object by json\n");
  }
  catch (...) {}

}
void shirink_app::set_plancontext_info(Json::Value object) {


  unsigned short int phase_order = object["plancontext"]["phase_order"].asInt();
  unsigned short int plan_id = object["plancontext"]["plan_id"].asInt();
  unsigned short int
      subphase_count = object["plancontext"]["subphase_count"].asInt();
  unsigned short int dir = object["plancontext"]["dir"].asInt();
  unsigned short int offset = object["plancontext"]["offset"].asInt();
  unsigned short int cycle_time = object["plancontext"]["cycle_time"].asInt();
  //Read phase firest, i want subphasecount of phase
  printf("printfMsg phase_order %x\n", phase_order);
  printf("printfMsg _planid:%d\n", plan_id);
  printf("printfMsg _subphase_count:%d\n", subphase_count);

  if (stc.Lock_to_Load_Phase_for_Panel(phase_order)) {
    if (stc._panel_phase._subphase_count == subphase_count &&
        stc._panel_phase._phase_order == phase_order) {
      printf("Data Right, writing\n");
      stc.Lock_to_Reset_Plan_for_Panel(plan_id, subphase_count);
      stc._panel_plan._planid = plan_id;
      stc._panel_plan._dir = dir;
      stc._panel_plan._phase_order = phase_order;
      stc._panel_plan._subphase_count = subphase_count;
      stc._panel_plan._cycle_time = cycle_time;
      stc._panel_plan._offset = offset;
      printf("savePlan _offset: %d\n", offset);

      for (int i = 0; i < subphase_count; i++) {
        stc._panel_plan._ptr_subplaninfo[i]._green =
            object["plancontext"]["subphase_green"][i].asInt();
        stc._panel_plan._ptr_subplaninfo[i]._min_green =
            object["plancontext"]["subphase_min_green"][i].asInt();
        stc._panel_plan._ptr_subplaninfo[i]._max_green =
            object["plancontext"]["subphase_max_green"][i].asInt();
        stc._panel_plan._ptr_subplaninfo[i]._yellow =
            object["plancontext"]["subphase_yellow"][i].asInt();
        stc._panel_plan._ptr_subplaninfo[i]._allred =
            object["plancontext"]["subphase_allred"][i].asInt();
        stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash =
            object["plancontext"]["subphase_pedgreen_flash"][i].asInt();
        stc._panel_plan._ptr_subplaninfo[i]._pedred =
            object["plancontext"]["subphase_pedred"][i].asInt();
      }
      stc.Lock_to_Save_Plan_from_Panel();
    } else if (stc._panel_phase._phase_order == phase_order &&
        phase_order == 0xB0 &&
        smem.vGetUCData(TC_CCT_MachineLocation) == MACHINELOCATEATHSINCHU) {
      printf("Phase 0xB0, writing\n");
      stc.Lock_to_Reset_Plan_for_Panel(plan_id, 1);
      stc._panel_plan._planid = plan_id;
      stc._panel_plan._dir = dir;
      stc._panel_plan._phase_order = phase_order;

      stc._panel_plan._subphase_count = 1;

      if (object["plancontext"]["subphase_green"][0].asInt() > 10
          || object["plancontext"]["subphase_green"][0].asInt() == 0) {
        stc._panel_plan._ptr_subplaninfo[0]._green = 10;
      }
      cycle_time = object["plancontext"]["subphase_green"][0].asInt();
      stc._panel_plan._cycle_time = cycle_time;
      stc._panel_plan._offset = offset;
      stc._panel_plan._ptr_subplaninfo[0]._min_green = 0;
      stc._panel_plan._ptr_subplaninfo[0]._max_green = 255;
      stc._panel_plan._ptr_subplaninfo[0]._yellow = 0;
      stc._panel_plan._ptr_subplaninfo[0]._allred = 0;
      stc._panel_plan._ptr_subplaninfo[0]._pedgreen_flash = 0;
      stc._panel_plan._ptr_subplaninfo[0]._pedred = 0;
      stc.Lock_to_Save_Plan_from_Panel();

    }
  }

}
void shirink_app::set_segment_info(Json::Value object) {
  printf("printfMsg goto vSaveSeg!\n");

  unsigned short int
      _segment_type = object["segmentinfo"]["segment_type"].asInt();
  unsigned short int
      _segment_count = object["segmentinfo"]["segment_count"].asInt();
  smem.vWriteMsgToDOM("set_segment_info start\n");

  stc.Lock_to_Reset_Segment_for_Panel(_segment_type, _segment_count);

  stc._panel_segment._segment_type = _segment_type;
  stc._panel_segment._segment_count = _segment_count;

  for (int i = 0; i < _segment_count; i++) {
    stc._panel_segment._ptr_seg_exec_time[i]._hour =
        object["segmentinfo"]["hour"][i].asInt();
    stc._panel_segment._ptr_seg_exec_time[i]._minute =
        object["segmentinfo"]["minute"][i].asInt();
    stc._panel_segment._ptr_seg_exec_time[i]._planid =
        object["segmentinfo"]["plan"][i].asInt();
    stc._panel_segment._ptr_seg_exec_time[i]._actMode =
        object["segmentinfo"]["actMode"][i].asInt();
    char buff[100]={};
    sprintf(buff,"printfMsg hour:%d min:%d planid:%d actMode:%d\n",
           stc._panel_segment._ptr_seg_exec_time[i]._hour,
           stc._panel_segment._ptr_seg_exec_time[i]._minute,
           stc._panel_segment._ptr_seg_exec_time[i]._planid,
           stc._panel_segment._ptr_seg_exec_time[i]._actMode);
    smem.vWriteMsgToDOM(buff);
  }

  stc.Lock_to_Save_Segment_from_Panel();

}
void shirink_app::set_weekdaysegment(Json::Value object) {
  try {
    bool bCheckOK;
    int tmp_segtype = 0;
    bCheckOK = true;

    for (int i = 0; i < 14; i++) {

      if ((object["weekdaysegment"][i].asInt() <= 0)
          || object["weekdaysegment"][i].asInt() > 7) {
        bCheckOK = false;
        printf("printfMsg tmp_segtype error: %d\n",
               object["weekdaysegment"][i].asInt());
      }
    }

    if (bCheckOK) {
      for (int i = 0; i < 14; i++) {
        stc._panel_weekdayseg[i]._segment_type =
            object["weekdaysegment"][i].asInt();
        if (i < 7)stc._panel_weekdayseg[i]._weekday = i + 1;
        if (i >= 7)stc._panel_weekdayseg[i]._weekday = i + 4;
        printf("weekday=%d segment=%d\n",
               stc._panel_weekdayseg[i]._weekday,
               stc._panel_weekdayseg[i]._segment_type);
      }
      stc.Lock_to_Save_WeekDaySegment_from_Panel();

    }

  }
  catch (...) {}
}
void shirink_app::set_specialdaycontext(Json::Value object) {
  try {
    for (int i = 8; i < 21; i++) {
      stc._panel_holidayseg._segment_type = i;
      stc._panel_holidayseg._start_year =
          object["specialdaycontext"][i - 8]["start_year"].asInt();
      stc._panel_holidayseg._end_year =
          object["specialdaycontext"][i - 8]["end_year"].asInt();
      stc._panel_holidayseg._start_month =
          object["specialdaycontext"][i - 8]["start_month"].asInt();
      stc._panel_holidayseg._end_month =
          object["specialdaycontext"][i - 8]["end_month"].asInt();
      stc._panel_holidayseg._start_day =
          object["specialdaycontext"][i - 8]["start_day"].asInt();
      stc._panel_holidayseg._end_day =
          object["specialdaycontext"][i - 8]["end_day"].asInt();

      printf("segment=%d start %d/%d/%d end %d/%d/%d\n ",
             stc._panel_holidayseg._segment_type,
             stc._panel_holidayseg._start_year,
             stc._panel_holidayseg._start_month,
             stc._panel_holidayseg._start_day,
             stc._panel_holidayseg._end_year,
             stc._panel_holidayseg._end_month,
             stc._panel_holidayseg._end_day);
      stc.Lock_to_Save_HoliDaySegment_from_Panel();
    }
  }
  catch (...) {}

}
void shirink_app::Init_hardwareVersion() {
  try {
    Json::Value HardwareGroup;
    HardwareGroup["Version"][0] = smem.vGetFirmwareInfo_0FC3(4);
    HardwareGroup["Version"][1] = smem.vGetFirmwareInfo_0FC3(5);
    HardwareGroup["modifyYear"] = smem.vGetFirmwareInfo_0FC3(0);
    HardwareGroup["modifyMonth"] = smem.vGetFirmwareInfo_0FC3(1);
    HardwareGroup["modifyDay"] = smem.vGetFirmwareInfo_0FC3(2);
    //(3)is default date
    string_to_app["HardwareGroup"] = HardwareGroup;
  }
  catch (...) {}
}
void shirink_app::Init_LastShutDownTime() {

  try {
    YMDHMS temp = smem.vGetLastResetTime();
    Json::Value LastShutDownTime;
    LastShutDownTime["year"] = temp.Year;
    LastShutDownTime["month"] = temp.Month;
    LastShutDownTime["day"] = temp.Day;
    LastShutDownTime["hour"] = temp.Hour;
    LastShutDownTime["min"] = temp.Min;
    LastShutDownTime["sec"] = temp.Sec;
    string_to_app["LastShutDownTime"] = LastShutDownTime;
  }
  catch (...) {}

}
void shirink_app::Init_TC_RealTime_info() {
  try {

//enum ControlStrategy
//{
//    STRATEGY_TOD       =10,
//    STRATEGY_AUTO_CADC =40,  //STRATEGY_AUTO_CADC is triggered from TOD
//    STRATEGY_CADC      =50,  //STRATEGY_CADC is triggered from outter
//    STRATEGY_MANUAL    =70,
//    STRATEGY_FLASH     =80,
//    STRATEGY_ALLRED    =90,
//    STRATEGY_ALLDYNAMIC =95,
//    STRATEGY_TODDYN    =96
//};
    RealTime_info["ControlStratege"] = stc.Lock_to_LoadControlStrategy();


    if (smem.vLoadCenterConnectStatus())
      RealTime_info["LinkStateWithCenter"] = 1;
    else RealTime_info["LinkStateWithCenter"] = 2;

    Json::Value SystemTime;
    time_t currentTime = time(NULL);
    struct tm *now = localtime(&currentTime);

    SystemTime["year"] = now->tm_year + 1900;
    SystemTime["month"] = now->tm_mon + 1;
    SystemTime["day"] = now->tm_mday;
    SystemTime["hour"] = now->tm_hour;
    SystemTime["min"] = now->tm_min;
    SystemTime["sec"] = now->tm_sec;
    RealTime_info["SystemTime"] = SystemTime;


    unsigned short int
        iCurrentPhaseID = stc.vGetUSIData(CSTC_exec_plan_phase_order);
    unsigned short int iCurrentPlanID = stc.vGetUSIData(CSTC_exec_plan_plan_ID);

    stc.Lock_to_Load_Plan_for_Panel(iCurrentPhaseID);
    stc.Lock_to_Load_Phase_for_Panel(iCurrentPlanID);
    current_state["current_phaseID"] = iCurrentPhaseID;
    current_state["current_planID"] = iCurrentPlanID;


    current_state["current_remaining_sec"] = stc.vGetStepTime();
    int iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);

    int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;
    current_state["current_subphase"] = iCurrentSubphase + 1;
    current_state["current_total_subphase"] = iCurrentSubphaseTotal;
    int iCurrentStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    int iCurrentStepTotal = stc._panel_phase._total_step_count;
    printf("current_total_subphase=%d,current_total_step=%d\n",
           iCurrentSubphaseTotal,
           iCurrentStepTotal);
    current_state["current_step"] =
        iCurrentStep + 1;// + ((iCurrentSubphase)*5)+1;
    current_state["current_total_step"] = iCurrentStepTotal;
    current_state["current_segmenttype"] =
        stc.vGetUSIData(CSTC_exec_segment_type);
    //current_state["current_subphasecount"]=;



    RealTime_info["current_state"] = current_state;

    string_to_app["RealTime_info"] = RealTime_info;

    printf("string=%s\n", RealTime_info.toStyledString().c_str());

  }
  catch (...) {}
}
void shirink_app::Init_TC_RealTime_info_udp() {
  try {

//enum ControlStrategy
//{
//    STRATEGY_TOD       =10,
//    STRATEGY_AUTO_CADC =40,  //STRATEGY_AUTO_CADC is triggered from TOD
//    STRATEGY_CADC      =50,  //STRATEGY_CADC is triggered from outter
//    STRATEGY_MANUAL    =70,
//    STRATEGY_FLASH     =80,
//    STRATEGY_ALLRED    =90,
//    STRATEGY_ALLDYNAMIC =95,
//    STRATEGY_TODDYN    =96
//};

    Init_TC_RealTime_info();
    BYTE RealTimeInfo[50];
    RealTimeInfo[0] = 0xaa;
    RealTimeInfo[1] = 0xee;
    RealTimeInfo[2] = 0x00;

    RealTimeInfo[3] =
        stc.Lock_to_LoadControlStrategy();//=RealTime_info["ControlStratege"]

    if (smem.vLoadCenterConnectStatus())
      RealTimeInfo[4] = 1;//RealTime_info["LinkStateWithCenter"]=link
    else RealTimeInfo[4] = 2;//RealTime_info["LinkStateWithCenter"]=disconnect

    Json::Value SystemTime;
    time_t currentTime = time(NULL);
    struct tm *now = localtime(&currentTime);

    RealTimeInfo[5] = now->tm_year;//=SystemTime["year"]
    RealTimeInfo[6] = now->tm_mon + 1;//SystemTime["month"]
    RealTimeInfo[7] = now->tm_mday;//=SystemTime["day"]
    RealTimeInfo[8] = now->tm_hour;//=SystemTime["hour"]
    RealTimeInfo[9] = now->tm_min;//=SystemTime["min"]
    RealTimeInfo[10] = now->tm_sec;//=SystemTime["sec"]
    //RealTime_info["SystemTime"]=SystemTime;



    unsigned short int
        iCurrentPhaseID = stc.vGetUSIData(CSTC_exec_plan_phase_order);
    unsigned short int iCurrentPlanID = stc.vGetUSIData(CSTC_exec_plan_plan_ID);

    stc.Lock_to_Load_Plan_for_Panel(iCurrentPhaseID);
    int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;
    stc.Lock_to_Load_Phase_for_Panel(iCurrentPlanID);
    RealTimeInfo[11] = iCurrentPhaseID;//=current_state["current_phaseID"]
    RealTimeInfo[12] = iCurrentPlanID;//=current_state["current_planID"]


    RealTimeInfo[13] =
        stc.vGetStepTime();//=current_state["current_remaining_sec"]
    int iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);


    RealTimeInfo[14] = iCurrentSubphase + 1;//=current_state["current_subphase"]

    RealTimeInfo[15] =
        iCurrentSubphaseTotal;//=current_state["current_total_subphase"]

    int iCurrentStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    int iCurrentStepTotal = stc._panel_phase._total_step_count;
    printf("current_total_subphase=%d,current_total_step=%d\n",
           iCurrentSubphaseTotal,
           iCurrentStepTotal);
    RealTimeInfo[16] = iCurrentStep + 1;// =current_state["current_step"]
    RealTimeInfo[17] = iCurrentStepTotal;//=current_state["current_total_step"]
    RealTimeInfo[18] =
        stc.vGetUSIData(CSTC_exec_segment_type);//=current_state["current_segmenttype"]
    //current_state["current_subphasecount"]=;

//printf("TESTTESTTEST tempera=%d humidity=%d\n",smem.getTemperature(),smem.getHumidity());




    RealTimeInfo[19] = (smem.getTemperature() / 10);
    RealTimeInfo[20] = (smem.getTemperature() % 10);
    RealTimeInfo[21] = smem.getHumidity();
    RealTimeInfo[22] = smem.getCom3GPS_state();
    RealTimeInfo[23] = smem.getT_H_state();

    RealTimeInfo[24] = 0xaa;
    RealTimeInfo[25] = 0xcc;
    RealTimeInfo[26] = 0;
    for (int i = 0; i < 26; i++)
      RealTimeInfo[26] ^= RealTimeInfo[i];

    //   Init_TC_RealTime_info();

    //RealTime_info["current_state"]=current_state;

    //string_to_app["RealTime_info"]=RealTime_info;

    //printf("string=%s\n",RealTime_info.toStyledString().c_str());
    writeJob.WritePhysicalOut(RealTimeInfo, 27, revAPP);

  }
  catch (...) {}
}
void shirink_app::send_PasswordCheck() {
  try {
    smem.vWriteMsgToDOM("login_check_by_shrink_APP");
    char cTmpPassword[6];
    unsigned char data[8];
    int i;
    Json::Value Password;
    char *_TmpPasswdPtr = smem.GetPassword();
    for (i = 0; i < 6; i++) {
      cTmpPassword[i] = _TmpPasswdPtr[i];
      Password["Password"][i] = cTmpPassword[i];
    }
    string_to_app["Passwd"] = Password;
//smem.SetPassword(666666)
  }
  catch (...) {}

}
void shirink_app::set_password(Json::Value object) {
  try {
    char passwd[6];
    for (int i = 0; i < 6; i++) {
      passwd[i] = object["Password"][i].asInt();
    }
    smem.SetPassword(passwd[0],
                     passwd[1],
                     passwd[2],
                     passwd[3],
                     passwd[4],
                     passwd[5]);
    smem.vWriteMsgToDOM("change_login_password_by_shrink_APP");
  }
  catch (...) {}
}
void shirink_app::Init_reportcycle() {
  try {
    Json::Value ReportCycle;
    ReportCycle["LightReportCycle"] =
        smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
    ReportCycle["StepReportCycle"] =
        smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);//StepChangeTranferCycle
    ReportCycle["HWReportCycle"] = smem.vGetHWCycleCodeFor_0F14_0FC4();

//                if (cSelect==0)  smem.vWriteMsgToDOM("Set Hardware Cycle To Stop Hardware Cycle");
//                else if (cSelect==1)  smem.vWriteMsgToDOM("Set Hardware Cycle To 1 Second");
//                else if (cSelect==2)  smem.vWriteMsgToDOM("Set Hardware Cycle To 2 Seconds");
//                else if (cSelect==3)  smem.vWriteMsgToDOM("Set Hardware Cycle To 5 Seconds");
//                else if (cSelect==4)  smem.vWriteMsgToDOM("Set Hardware Cycle To 1 Minute");
//                else if (cSelect==5)  smem.vWriteMsgToDOM("Set Hardware Cycle To 5 Minutes");
//
//            }
//

    string_to_app["ReportCycle"] = ReportCycle;
  }
  catch (...) {}
}
void shirink_app::setReportCycle(Json::Value object1) {
  try {
    PTRAFFIC92COMM ptf92;
    PTRAFFIC92TC ptf92tc;
    Json::Value object;
    object = object1["ReportCycle"];

    if (object.isMember("LightReportCycle")) {
      smem.vSetINTData(TC92SignalStepStatus_5F03_IntervalTime,
                       object["LightReportCycle"].asInt());
      printf("change LightReportCycle=%d\n",
             object["LightReportCycle"].asInt());
      ptf92tc.shrinkAPP_vTransmitCycleSetting_5F3F(2,
                                                   object["LightReportCycle"].asInt());

      smem.vWriteMsgToDOM("lightReportCycle by shrinkAPP");
    }
    if (object.isMember("StepReportCycle")) {
      smem.vSetINTData(TC92SignalLightStatus_5F0F_IntervalTime,
                       object["StepReportCycle"].asInt());
      printf("change StepReportCycle=%d\n", object["StepReportCycle"].asInt());
      ptf92tc.shrinkAPP_vTransmitCycleSetting_5F3F(1,
                                                   object["LightReportCycle"].asInt());
      smem.vWriteMsgToDOM("StepReportCycle by shrinkAPP");
    }
    if (object.isMember("HWReportCycle")) {
      smem.vSetHWCycleCodeFor_0F14_0FC4(object["HWReportCycle"].asInt());
      printf("change HWReportCycle=%d\n", object["HWReportCycle"].asInt());
      ptf92.shrinkAPP_setHWReportCycle(object["HWReportCycle"].asInt());
      smem.vWriteMsgToDOM("HWReportCycle by shrinkAPP");
    }

  }
  catch (...) {}
}
void shirink_app::RebootTC() {

  try {

    smem.vSetBOOLData(IFGetResetTCSignal, true);
    smem.vWriteMsgToDOM("Reset TC Signal By shrink_app!");
    system("sync");
    system("sync");
    system("reboot");
  }
  catch (...) {}

}
void shirink_app::UpdateDB(Json::Value object) {
  try {
    unsigned char data[5];

    data[0] = 0x5F;
    data[1] = 0x0B;
    data[2] = 0x0F;   //­n¨D¥þ³¡

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3, true);
    _MsgOK.InnerOrOutWard = cComingFromScreen;

    writeJob.WritePhysicalOut(_MsgOK.packet,
                              _MsgOK.packetLength,
                              DEVICECENTER92);

    smem.vWriteMsgToDOM(
        "Request Traffic Center To Update My Database by shrinkAPP");


    screenLast92TCPlanSegmentUpdate.DisplayLastUpdate();

  }
  catch (...) {}

}
void shirink_app::Init_lastUpdateDBdate() {
  try {
    YMDHMS lastUpDateDBtime = smem.vGetLast92TC_5F15Time();
    Json::Value UpdateDB;
    UpdateDB["lastUpdateYear"] = lastUpDateDBtime.Year;
    UpdateDB["lastUpdateMonth"] = lastUpDateDBtime.Month;
    UpdateDB["lastUpdateDay"] = lastUpDateDBtime.Day;
    UpdateDB["lastUpdateHour"] = lastUpDateDBtime.Hour;
    UpdateDB["lastUpdateMin"] = lastUpDateDBtime.Min;
    UpdateDB["lastUpdateSec"] = lastUpDateDBtime.Sec;
    string_to_app["LastUpdateDBTime"] = UpdateDB;
  }
  catch (...) {}

}
bool shirink_app::checkPassword(Json::Value Object) {
  try {
    char tempPasswd[6];
    printf("shirink_app::checkPassword(Json::Value Object)\n");
    strcpy(tempPasswd, Object["Password"].toStyledString().c_str());
    char *realpassword;
    realpassword = smem.GetPassword();
    printf("pass=%s temp=%s\n", realpassword, tempPasswd);
    bool passwdcheck = true;
    for (int i = 0; i < 6; i++) {
      if (tempPasswd[i + 1] != realpassword[i])passwdcheck = false;
      printf("i=%d 1 %x, 2 %x bool=%d\n",
             i,
             tempPasswd[i],
             realpassword[i],
             passwdcheck);

    }


    return passwdcheck;
  }
  catch (...) {}
}
void shirink_app::Init_DBupdateInfo() {
  try {
    YMDHMS lastUpDateDBtime = smem.vGetLast92TC_5F15Time();
    BYTE UpdateINFO[50];
    UpdateINFO[0] = 0xaa;
    UpdateINFO[1] = 0xee;
    UpdateINFO[2] = 0x01;
    UpdateINFO[3] = lastUpDateDBtime.Year;
    UpdateINFO[4] = lastUpDateDBtime.Month;
    UpdateINFO[5] = lastUpDateDBtime.Day;
    UpdateINFO[6] = lastUpDateDBtime.Hour;
    UpdateINFO[7] = lastUpDateDBtime.Min;
    UpdateINFO[8] = lastUpDateDBtime.Sec;
    UpdateINFO[9] = screenLast92TCPlanSegmentUpdate.GetUpdateSegmentQ();
    UpdateINFO[10] = screenLast92TCPlanSegmentUpdate.GetUpdatePlanQ();


    UpdateINFO[11] = 0xaa;
    UpdateINFO[12] = 0xcc;
    UpdateINFO[13] = 0;
    for (int i = 0; i < 13; i++)
      UpdateINFO[13] ^= UpdateINFO[i];

    writeJob.WritePhysicalOut(UpdateINFO, 14, revAPP);
  }
  catch (...) {}

}
//---------------------------------------------------------------------------
void shirink_app::ModifyDate(Json::Value object1) {
  try {
    Json::Value object;
    object = object1["date"];
    PTRAFFIC92COMM ptf92;
    if (smem.GetDbOperStat() == 0 || smem.vLoadCenterConnectStatus() == false) {
      unsigned int year = object["year"].asInt(),
          month = object["month"].asInt(), day = object["day"].asInt();
      unsigned int hour = object["hour"].asInt(), min = object["min"].asInt(),
          sec = object["sec"].asInt();

      if (year >= 1900 && year <= 2200 && month >= 1 && month <= 12 && day >= 1
          && day <= 31 && hour >= 0 && hour <= 24 && min >= 0 && min <= 60
          && sec >= 0 && sec <= 60) {

        stc.TimersRead_BeforeResetCMOSTime();  //OTBUG =1
        _intervalTimer.TimersRead_BeforeResetCMOSTime();
        smem.vSetTimerMutexRESET(1);
        ptf92.SendTimeisChangetoArwenActuate();//to arwen
        while (smem.vGetTimerMutexCTIMER() == 0
            || smem.vGetTimerMutexCSTC() == 0) {
          usleep(100);
        }

        smem.vSetSystemClockTime(year, month, day, hour, min, sec);


        char message[200] = {0};
        sprintf(message,
                "Change DateTime for %#04d-%#02d-%#02d %#02d:%#02d:%#02d",
                year,
                month,
                day,
                hour,
                min,
                sec);
        smem.vWriteMsgToDOM(message);

        _intervalTimer.TimersReset_AfterResetCMOSTime();
        stc.TimersReset_AfterResetCMOSTime();  //OTBUG =1

        smem.vSetTimerMutexRESET(0);

        unsigned char ucSendData[6] = {0};
        ucSendData[0] = 0x0F;
        ucSendData[1] = 0x02;

        MESSAGEOK _MSG;

        _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(ucSendData, 2, true);
        _MSG.InnerOrOutWard = cOutWard;
        writeJob.WriteWorkByMESSAGEOUT(_MSG);
        writeJob.WritePhysicalOut(_MSG.packet,
                                  _MSG.packetLength,
                                  DEVICECENTER92);

        smem.vSendTimerUpdateToCCJ_5F9E();

        system("hwclock -w");

      }
    }
  } catch (...) {}
}
void shirink_app::set_RedCountHWSetting(Json::Value object1) {
  try {
    Json::Value object = object1["RedCountHWSetting"];

    if (object.isMember("RedCountHWVersion")) {
      char logBuff[64] = "";
      smem.vSetINTData(TC92_RedCountVer,
                       object["RedCountHWVersion"].asInt());
      sprintf(logBuff, "change TC92_RedCountVer=%d\n",
              object["RedCountHWVersion"].asInt());
      printf(logBuff);
      smem.vWriteMsgToDOM(logBuff);
    }
    if (object.isMember("RedCountLightBoard")) {
      char logBuff[64] = "";

      for (int boardNumber = 0; boardNumber < 8; boardNumber++)
        smem.vSetWayMappingRedCount(boardNumber,
                                    object["RedCountLightBoard"][boardNumber].asInt());
      sprintf(logBuff,
              "change RedCountLightBoard 0.%d 1.%d 2.%d 3.%d 4.%d 5.%d 6.%d 7.%d\n",
              object["RedCountLightBoard"][0].asInt(),
              object["RedCountLightBoard"][1].asInt(),
              object["RedCountLightBoard"][2].asInt(),
              object["RedCountLightBoard"][3].asInt(),
              object["RedCountLightBoard"][4].asInt(),
              object["RedCountLightBoard"][5].asInt(),
              object["RedCountLightBoard"][6].asInt(),
              object["RedCountLightBoard"][7].asInt()
      );
      smem.vWriteMsgToDOM(logBuff);
    }

  }
  catch (...) {}

}
void shirink_app::DisplayRedCountCycle(int ID) {
  try {

//  int x=0;
    unsigned char ucData[6];
    MESSAGEOK _MsgOK;

//  if (x == 0x0B) { x = 65535; }

    ucData[0] = 0xEA;
    ucData[1] = 0x11;
    ucData[2] = 0x64;      //force define 100sec cycle

    _MsgOK =
        oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucData, 3, 0x36, ID);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet,
                              _MsgOK.packetLength,
                              DEVICEREDCOUNTVERCCT97);
  } catch (exception igonore) {};

}
void shirink_app::DisplayRedCount88(int ID) {
  try {
    int x = 0x0;
    unsigned char ucData[6];
    MESSAGEOK _MsgOK;

//  if (x == 0x0B) { x = 65535; }

    ucData[0] = 0xEA;
    ucData[1] = 0x13;

    _MsgOK =
        oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucData, 2, 0x36, ID);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet,
                              _MsgOK.packetLength,
                              DEVICEREDCOUNTVERCCT97);
  } catch (exception igonore) {};
}
void shirink_app::DisplayRedCountID(int ID) {
  try {
    MESSAGEOK _MsgOK;

    int x = ID;
    unsigned char ucData[6];

    if (x == 0x0B) { x = 65535; }

    ucData[0] = 0xEA;
    ucData[1] = 0x12;

    _MsgOK =
        oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucData, 2, 0x36, x);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet,
                              _MsgOK.packetLength,
                              DEVICEREDCOUNTVERCCT97);

  } catch (...) {}
}
//---------------------------------------------------------------------------
