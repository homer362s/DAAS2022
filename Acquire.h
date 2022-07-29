extern struct acquireStruct
{
    struct {
        int setup;      //  "data acquisition control" panel ID
        int datainfo;}  //  "data acquisition info" panel ID
        p;
}   acqG;      // global var

extern struct expStruct {
    void (*InitExp) (void);
    void (*UpdatePanel) (void);
    void (*DoExp) (void);
    double delay;
    acqstatusType acqstatus;
}   expG;                        // global var

extern void acquire_Init (void);
extern void acquire_Exit(void);
extern int  acquire_GetMenuBar (void);
extern void acquire_UpdatePanel(void);
extern void acquire_UpdatePanelExp (void);
extern void acquire_UpdateDataInfoPanel (void);

extern void acqTimerSetInterval(double delay);
extern void acqTimerReset( );

extern int exp_Begin(void);
