#ifndef PROCESS_H
#define PROCESS_H
#include <string>

enum PROCESS_STATUS {CREATED,READY,RUNNING,BLOCKED};

class Process{
private:
    static int idCounter;
public:
    int AT, //Arrival Time
        TC, //Total CPU Time
        CB, //CPU Burst
        IO; //IO Burst
    int PID;
    int timeInPrevState = 0;
    int remainingCB=-1, remainingIO=-1;
    int static_priority, dynamic_priority;
    int state_ts=0;
    PROCESS_STATUS state = CREATED;
    Process(int at, int tc, int cb, int io,int sp);
    Process(std::string line, int sp );

    void print();
        
};
#endif /* PROCESS_H */
