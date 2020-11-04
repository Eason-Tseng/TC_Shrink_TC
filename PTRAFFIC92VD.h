//---------------------------------------------------------------------------

#ifndef PTRAFFIC92VDH
#define PTRAFFIC92VDH

#include "var.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
class PTRAFFIC92VD
{
    public:

        PTRAFFIC92VD(void);
        ~PTRAFFIC92VD(void);

        bool DoWorkViaPTraffic92(MESSAGEOK);                                    //�ھ�PTraffic92�Ӫ��T���@�Ʊ�

        bool ProcessZoneStatus6F05(MESSAGEOK);//arwen ++
        bool ProcessACTZoneAllStatus6F08(MESSAGEOK);//arwen ++

    private:


        int vReturnToCenterACK(unsigned char , unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);



};
//---------------------------------------------------------------------------
#endif
