#ifndef PROCESS_H
#define PROCESS_H
#include <string>

enum PROCESS_STATUS {CREATED,READY,RUNNING,BLOCKED};

class Process{
private:
    static int idCounter;
public:
    int AT=0, //Arrival Time
        TC=0, //Total CPU Time
        CB=0, //CPU Burst
        IO=0, //IO Burst
        FT=0, //Finishing Time
        TT=0, //Turnaround time ( finishing time - AT )
        IT=0, //I/O Time ( time in blocked state)
        CW=0, //CPU Waiting time ( time in Ready state )
        CT=0, //CPU Time(time in Running state)
        REM=0, //Remaining execution time
        WhenInReady = 0;
    int PID;
    int timeInPrevState = 0;
    int remainingCB=0, remainingIO=0;
    int static_priority, dynamic_priority;
    int state_ts=0;
    PROCESS_STATUS state = CREATED;
    Process(int at, int tc, int cb, int io,int sp);
    Process(std::string line, int sp );

    void print();
        
};
#endif /* PROCESS_H */
