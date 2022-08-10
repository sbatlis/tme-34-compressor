
#include "Clock_Ip.h"
#include "IntCtrl_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Lpuart_Lin_Ip.h"

static uint8_t sTxBuf[] = {0x01, 0x02, 0x03, 4, 5, 6};

Lpuart_Lin_Ip_PduType sLinPdu =
{
    .Pid = 0x12,
    .Cs = LPUART_LIN_IP_ENHANCED_CS,
    .SduPtr = sTxBuf,
    .Drc = LPUART_LIN_IP_FRAMERESPONSE_RX,
    .Dl = sizeof(sTxBuf)
};

    uint8_t DummyBuffer;
    Lpuart_Lin_Ip_TransferStatusType LpuartMasterStatus;

int main( void )
{
    Clock_Ip_Init( &Clock_Ip_aClockConfig[0] );
    Siul2_Port_Ip_Init( NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0 );
    IntCtrl_Ip_Init( &IntCtrlConfig_0 );
    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);
    while( LPUART_LIN_IP_STATUS_SUCCESS != Lpuart_Lin_Ip_Init( Lpuart_Lin_Ip_Sa_pHwConfigPB_0_BOARD_INITPERIPHERALS.Instance, &Lpuart_Lin_Ip_Sa_pHwConfigPB_0_BOARD_INITPERIPHERALS ) );

    IntCtrl_Ip_EnableIrq(LPUART9_IRQn);

    for (uint8_t i=0;i<(1<<5);i++)
    {
        sLinPdu.Pid = Lin_Ip_ProcessParity( i, LIN_IP_MAKE_PARITY );


        Lpuart_Lin_Ip_SendWakeupSignal( Lpuart_Lin_Ip_Sa_pHwConfigPB_0_BOARD_INITPERIPHERALS.Instance );
        Lpuart_Lin_Ip_SendFrame(Lpuart_Lin_Ip_Sa_pHwConfigPB_0_BOARD_INITPERIPHERALS.Instance, &sLinPdu );

        do
        {
            LpuartMasterStatus = Lpuart_Lin_Ip_GetStatus(Lpuart_Lin_Ip_Sa_pHwConfigPB_0_BOARD_INITPERIPHERALS.Instance, (uint8 **)&DummyBuffer);
        }
        while ((LPUART_LIN_IP_STATUS_TX_BUSY == LpuartMasterStatus) );
        if(  LPUART_LIN_IP_STATUS_RX_NO_RESPONSE != LpuartMasterStatus)
        {
            while( 1 )
            {
            }
        }
    }

    while( 1 );
}


