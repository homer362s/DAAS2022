
// **************GPIB+USB+PCI+RS232 version************* 
#include <cvintwrk.h>
#include <userint.h>
//password for instacal univ lib .exe is: mcc5_72
#include "util.h"
#include "list.h"               
#include "channel.h"
#include "changen.h"
#include "chanfnc.h"
#include "chanops.h"
#include "acqchan.h"
#include "curve.h"
#include "acqcrv.h"
#include "graph.h"
#include "curveop.h"
#include "acquire.h"
#include "acquireu.h"
#include "gpibio.h"
#include "source.h"
#include "rs232util.h"
#include "MCCdevices.h"
 // insert headers for instruments below
#include "sr830.h"
#include "sr844.h"
#include "k2400.h"
#include "BK9201.h" 
#include "k2000.h"
//#include "k213.h"
#include "k236.h"  
#include "HP33120A.h"
#include "HP3245A.h"   
#include "LS335.h"
#include "LS340.h"
#include "itc4.h"
#include "HP4156.h"
#include "AFG3252.h"
#include "AFG3252init.h"
#include "DAS-6036.h"
#include "das-1602.h"
#include "DEMO-BOARD.h"
#include "USB-234.h" 
#include "USB-1808.h"  
#include "dda08.h"
#include "USB-PMD1208LS.h"

#define DFLT_IDLE_EVENT_RATE 30// this is actually a period in ms 60ms = 16Hz

int AcquireData (int panel, int control, int event, void *callbackData, int event1, int event2);

main (void)
{
    // we display initial panel choosing daq or analysis-only option
    utilG_Init (gpibio_Exit);
    // TODO: init what is necessary in anal-only and skip others 
    // if in analysis-only mode if(utilG.acq.status == ACQ_NONE)
    listG_Init ();
    channelG_Init();
    changen_Init();
    chanfunc_Init();
    chanops_Init();
    acqchan_Init();
    graphG_Init();
    curveop_Init();
    acquire_Init();
    gpibio_Init();
   // rs232_Init();
    source_Init();
    
/*Don't change the order of these because the load function will not work properly*/

	BK9201_Init();
    k2400_Init();
    k2000_Init();
    //k213_Init();
	k236_Init(); 
    sr844_Init();
    sr830_Init();
    hp33120a_Init();
	hp3245a_Init(); 
    LS335_Init();
    LS340_Init();
    hp4156_Init();
	//hp8657b_Init();		// Isnt ready for master yet
	//afg3252_Init();		// I believe some needed files arent in the git repo for this to work
/*rs232 instruments init*/

//    itc4_Init();

/*PCI slot and USB Instacal compatible devices*/
    das6036_Init();
    das1602_Init();
    dda08_Init();
    usb1208ls_Init();
	USB234_Init();
	USB1808_Init();
    demo_board_Init();
    init_MCCdevices();
/***********************************************************************************/    
    SetIdleEventRate (DFLT_IDLE_EVENT_RATE);  // 40ms time, 25Hz rate
	if (utilG.acq.status != ACQ_NONE) util_printfLog("Set idle event rate to %d ms\n", DFLT_IDLE_EVENT_RATE);  //if acquisition is enabled the idle rate is shown 
	else  util_printfLog("Acquisition disabled. Ready to analyze data...", DFLT_IDLE_EVENT_RATE); // analysis only  message
    InstallMainCallback (AcquireData, 0, 1);
    
    utilG.err = 0;
    util_RemoveInitMessage();
    RunUserInterface();
}

int AcquireData (int panel, int control, int event, void *callbackData, int event1, int event2)
{
    if ((event == EVENT_IDLE) && (utilG.acq.status != ACQ_NONE)) {

        devPanel_UpdateReadings();

        switch (utilG.acq.status) {
            case ACQ_BEGIN:
                if (!exp_Begin()) utilG.acq.status = ACQ_STOPPED;
                acquire_UpdatePanel();
                break;
            case ACQ_TERMINATE:
                acqchanlist_CloseFile();
                graphlist_RemoveReadings();
                graphlist_PlotCurves();
                graphlist_AutoSave();
                acqchanlist_CopytoChannelList();

                if (utilG.acq.pt == utilG.acq.nPts) {
                    utilG.acq.status = ACQ_DONE;
                    util_WriteLogLine("Experiment Done");
                }
                else {
                    utilG.acq.status = ACQ_STOPPED;
                    util_WriteLogLine("Experiment Stopped");
                }                    
                acquire_UpdatePanel();
                utilG.acq.pt = 0;
                break;
        }
    }
    if(event == EVENT_KEYPRESS && event1 == VAL_F1_VKEY)
    {
//        InetLaunchDefaultWebBrowser ("help.html");    // no help yet
    }
    return 0;
    
}

