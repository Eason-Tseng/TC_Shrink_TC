//---------------------------------------------------------------------------
#include "PDEVICE.h"
#include <stdio.h>

//---------------------------------------------------------------------------
PDEVICE::PDEVICE(void)
{
}
//---------------------------------------------------------------------------
PDEVICE::~PDEVICE(void)
{
}
//---------------------------------------------------------------------------
bool PDEVICE::DoWorkViaProtocol(MESSAGEOK message)
{
    try
    {
        switch (message.protocol)
        {
        case PROTOCOLUNKNOW:                                                //不清楚的協定
        case PROTOCOL77:                                                    //交通部77年版通訊協定
        case PROTOCOL87:                                                    //交通部87年版通訊協定
        case PROTOCOL92:                                                    //交通部92年版通訊協定
        case PROTOCOLNTCIP:                                                 //交通部NTCIP通訊協定
            break;
            //第一種分類:週邊設備:東生面板控制鍵盤
        case PROTOCOLKEYPAD:                                                //東生面板控制鍵盤通訊協定
            pDeviceKeypad.DoWorkViaPDevice(message);
            break;
            //第二種分類:週邊設備:RTMS
        case PROTOCOLRTMS:                                                  //RTMS通訊協定
            pDeviceRTMS.DoWorkViaPDevice(message);
            break;
            //第三種分類:週邊設備:SMART SENSOR
        case PROTOCOLSS:                                                    //SMART SENSOR通訊協定
            pDeviceSS.DoWorkViaPDevice(message);
            break;
            //第四種分類:週邊設備:宏楷紅燈倒數
        case PROTOCOLOWNWAY:                                                //宏楷紅燈倒數通訊協定
            break;
            //第五種分類:週邊設備:恆嘉紅燈倒數
        case PROTOCOLEOI:                                                   //恆嘉紅燈倒數通訊協定
            break;
            //第六種分類:週邊設備:台北永琦CMS
        case PROTOCOLLISCMS:                                                //台北永琦CMS通訊協定
            break;
            //第七種分類:週邊設備:紅綠燈控制板
        case PROTOCOLTRAFFICLIGHT:                                                //東生紅綠燈控制通訊協定
            pDeviceTrafficLight.DoWorkViaPDevice(message);
            break;

        case PROTOCOLREVMANUALPAD:
            pDeviceRevManualPad.DoWorkViaPDevice(message);
            break;
        case PROTOCOLPEDESTRIAN:
            pDevicePedPushButton.DoWorkViaPDevice(message);
            break;



        default:
            break;
        }

        return true;

    }
    catch (...) {}
}
//---------------------------------------------------------------------------
