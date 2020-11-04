#ifndef SCREENCtlNoH
#define SCREENCtlNoH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENCtlNo:public SCREEN
{
    public:

      SCREENCtlNo(void);
      ~SCREENCtlNo(void);

      void DisplayCtlNo(void);                                                  //��ܳ]�ƽs����
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

      void DisplayNum(void);

    private:

      DISP_WORD lcn[5];
      int address;                                                        //�������ťճB,�w�q�y��

      BYTE ctlNoBitmap[3840];                                                   //����
      //jacky20140311 BRT
      char LCNaddress[5];
      int   tmpaddress;
      unsigned int cPosition;

      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����
      void InitDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�

      //20140226 BRT
      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);
      void DoKey3Work(void);
      void DoKey4Work(void);
      void DoKey5Work(void);
      void DoKey6Work(void);
      void DoKey7Work(void);
      void DoKey8Work(void);
      void DoKey9Work(void);
      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);

      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern SCREENCtlNo screenCtlNo;
#endif

