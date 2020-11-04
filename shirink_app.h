#ifndef SHIRINK_APP_H
#define SHIRINK_APP_H
#include "var.h"
#include <string>
#include "CReverseTime.h"
#include "CSegmentInfo.h"
#include "CPhaseInfo.h"
#include "CPlanInfo.h"
#include "CIOCom.h"
#include "CPacketCluster.h"
#include "json.h"
#include "CRTMSDecoder.h"
#include "CRTMSInformation.h"
#include "CTrafficAnalyzer.h"
#include "CSTC.h"
class shirink_app {
 public:
  Json::Value string_to_app;
  Json::Value phaseinfo;
  Json::Value subplaninfo;
  Json::Value planinfo;
  Json::Value segmentinfo;
  Json::Value current_state;
  Json::Value Holidaysegtype;
  Json::Value WeekdaySegtype;
  Json::Value RealTime_info;
  Json::FastWriter faster_writer;
  Json::Reader reader;
  shirink_app();
  virtual ~shirink_app();

  typedef struct s_IP {
    int localIP1[4];
    int localIP1_port;
    int distIP0[4];
    int distIP0_port;
    int distIP1[4];
    int distIP1_port;
    int netmask[4];
    int gateway[4];

  };

  typedef struct s_Railchain_prama {
    BYTE sTC_TrainChainEnable;
    BYTE sTrainComingForceJumpSubphase;
    BYTE sTrainComingBanSubphase;

  };

  typedef struct s_TC_manual_setting {
    int dbOperStat;
    BYTE HWCycleCodeIN;
    unsigned short int WayMappingRedCountIN[8];
  };

  typedef struct s_Proxy_transfer//0f8e 0f8f
  {
    BYTE PassMode_0F8E8FIN;
    unsigned short int proxy_LCN;
  };

  typedef struct s_Signal_card_direcion {
    unsigned short int SignamMappingDirIN[8];
    unsigned short int SignamMappingLightBoardIN[6];
  };

  typedef struct s_TC_stratage_set {
    BYTE TC_ControlStrategyIN;
    int SignalStepStatus_5F03_IntervalTimeIN;
    int TC_SignalStepStatus_5F03_IntervalTimeIN;
    int RedCountVerSelectIN;
    bool TC_ActuateTypeFunctionEnableIN;
    unsigned short int TC_CCT_In_LongTanu_ActuateType_PlanID;
    BYTE ActuatePhaseExtendIN;
    bool ActuateautoswitchIN;
  };

  typedef struct s_Compensation_param {
    //  bool enable_switch;
    unsigned short int cycle_num;
  };

  typedef struct s_Learn_mode_group {
    //    bool enable_car_learn;
    unsigned short int CarLearn;
    //  bool enable_ped_learn;
    unsigned short int PedLearn;
    //bool enable_CarCountDown_learn;
    unsigned short int CarCountDownLearn;
    //bool enable_PedCount_learn;
    unsigned short int PedCountLearn;
  };

  typedef struct s_Ped_control_set {
    //  bool enable_SWCount;
    unsigned short int SWDevCount;

  };
//  typedef struct s_RedCountSetting {
//    unsigned short int WayMappingRedCount[8];
//    unsigned short int WayMappingGreenCount[8];
//
//  };

  typedef struct s_Chain_set_group {
    BYTE sTC_5F31Manual;
    BYTE sTC_5F31TOD;
    BYTE sTC_5F31StartSubPhaseId;
    BYTE sTC_5F31EndSubPhaseId;
    BYTE sTC_5F32StartSubPhaseId;
    BYTE sTC_5F32EndSubPhaseId;
    BYTE sTC_MotherChainStartStepId;
    BYTE sTC_MotherChainEndStepId;
    short int TC_5F33StartOffset[64];
    short int TC_5F33EndOffset[64];
  };

  typedef struct s_TC_project_data {
    CPhaseInfo phase[AMOUNT_PHASEORDER];
    CPlanInfo plan[AMOUNT_PLANID];
    CSegmentInfo segment[AMOUNT_SEGMENT];
    CWeekDaySegType
        weekdayseg[AMOUNT_WEEKDAY_SEG]; //{0-6,7-13} according to {1-7,11-17}
    CHoliDaySegType holidayseg[AMOUNT_HOLIDAY_SEG]; //{0-12} according to {8-20}
    CReverseTimeInfo reversetime[AMOUNT_REVERSETIME];
    CWeekDayRevType
        weekdayrev[AMOUNT_WEEKDAY_REV]; //{0-6,7-13} according to {1-7,11-17}
    CHoliDayRevType holidayrev[AMOUNT_HOLIDAY_REV]; //{0-12} according to {4-16}
  };
  typedef struct Execute_data {
    unsigned short int _exec_phase_current_subphase;  //start from 0
    unsigned short int _exec_phase_current_subphase_step;  //start from 0
    unsigned short int _exec_segment_current_seg_no;  //start from 0
    unsigned short int _exec_reversetime_current_rev_no;  //start from 0
    unsigned short int _exec_reversetime_current_rev_step;  //start from 0
    ControlStrategy _current_strategy;
    ControlStrategy _old_strategy;
  };
  /***    refresh_series catch tc data to shrink_app class       ****/
  typedef struct ActuateSetStruct{
    int usActplayNo;// 螢幕上顯示的ID
    int usiActType;   //觸動編號  有 0關閉 1 2 3閃光 4長綠
    int valueSwitch[8]; //應該要用bool 但一開始的設計用int  是每個分向開關是否檢視的確認值
    int valueExtendSec[8];//就延長秒數
    int usiActionSec;//觸動後多少秒啟動
    unsigned short int usiActuatePlan;//要跑哪個plan
    int ActionSec;//開關觸動後幾秒開始執行
    int valueCompensateMode;//實際上是補償  螢幕顯示鐵路連鎖
  };
  bool setAcutateSetting(ActuateSetStruct actuateParameter);
  void refresh_railchain_parama();
  void refresh_manual_setting();
  void refresh_proxy_transfer();
  void refresh_signal_card_direction();
  void refresh_tc_stratage_set();
  void refresh_compensation();
  void refresh_learn_mode_group();
  void refresh_ped_control_set();
  void refresh_chain_set_group();
  void refresh_tc_project_data();
  void refresh_execute_data();
  /***     "set series" receive shrink_app_pad'data than set in tc, then use refresh_series update shrink_app's object    ***/
  void set_ip(Json::Value object);
  void set_railchain_parama();
  void set_manual_setting(Json::Value object);
  void set_proxy_transfer();
  void set_signal_card_direction();
  void set_tc_stratage_set();
  void set_compensation();
  void set_learn_mode_group();
  void set_ped_control_set();
  void set_chain_set_group();
  void set_tc_project_data();
  void set_execute_data();
  void set_RedCountHWSetting(Json::Value object);
  void set_step_info(Json::Value object);
  void set_plancontext_info(Json::Value object);
  void set_segment_info(Json::Value object);
  void set_weekdaysegment(Json::Value object);
  void set_specialdaycontext(Json::Value object);
  void set_password(Json::Value object);
  void RebootTC();
  void setReportCycle(Json::Value object1);
  void UpdateDB(Json::Value object);
  void DisplayRedCountID(int ID);

  void DisplayRedCount88(int ID);
  void DisplayRedCountCycle(int ID);
  void ModifyDate(Json::Value object);
  /***   "send series" send the tc's data with json form to shrink_app_pad  ***/
  void Init_ip();//v
  void Init_manual_setting();//v
  void Init_RedCountHWSetting();//v
  void send_railchain_parama();//V
  void send_proxy_transfer();//V
  void send_signal_card_direction();//V
  void send_tc_stratage_send();//v
  void send_compensation();//v
  void send_learn_mode_group();//V
  void send_ped_control_send();//V
  void send_chain_send_group();//V
  void Init_tc_project_data();// v
  void Packed_step_info();
  void Packed_plancontext_info();
  void Packed_segmentinfo();
  void Packed_Tod_info();//tod = time of day,spd =special day
  void Packed_Special_day_info();
  void Packed_RedCountHWSetting_info();
  void Packed_ActuateParameter();
  void send_execute_data();//v
  void Init_hardwareVersion();//v
  void Init_LastShutDownTime();//v
  void Init_TC_RealTime_info();
  void Init_TC_RealTime_info_udp();
  void send_PasswordCheck();
  void Init_reportcycle();
  void Init_DBupdateInfo();//DBupdate  5F0B udp
  void Init_lastUpdateDBdate();
  bool checkPassword(Json::Value Object);
//protected:
 private:
  s_IP s_ip;
  s_Railchain_prama s_railchain_parama;
  s_TC_manual_setting s_tc_manual_setting;
  s_Proxy_transfer s_proxy_transfer;
  s_Signal_card_direcion s_signal_card_direction;
  s_TC_stratage_set s_tc_stratage_set;
  s_Compensation_param s_compensation;
  s_Learn_mode_group s_learn_mode_group;
  s_Ped_control_set s_ped_control_set;
//  s_RedCountSetting s_RedCountSetting_set;
  s_Chain_set_group s_chain_set_group;
  s_TC_project_data s_tc_project_data;
  Execute_data execute_data;

  long lastRestSect;
  unsigned int LCN;
  time_t last_get_5F15time;
  int Com2Type;
  BYTE MBType;

};
extern shirink_app shrinkAPP;
#endif // SHIRINK_APP_H
