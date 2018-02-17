#ifndef DES_H
#define DES_H
#include <string>
#include <vector>
#include <list>
#include "utility.h"
#include "scheduler.h"
#include "process.h"

using namespace std;
enum TRANS_TYPE {TRANS_TO_READY, TRANS_TO_RUN, TRANS_TO_BLOCK, TRANS_TO_PREEMPT};
struct EVENT{
    TRANS_TYPE transition;
    Process *evtProcess;
    int evtTimeStamp = 0;
    EVENT(TRANS_TYPE typ, Process *p, int t);
    string toString();
};
class DES{
private:
    int quantum;
    myRand rnd;
    int CURRENT_TIME;
    bool CALL_SCHEDULER = false;
    Process* CURRENT_RUNNING_PROCESS = nullptr;
    Scheduler* THE_SCHEDULER = nullptr;
    list<EVENT*> eventQ;
    vector<Process*> processList;
public:
    DES(string inputFile, string rfile,SCHEDULE_MODE mode,int quantum);
    EVENT* get_event();
    int get_next_event_time();
    void add_event(EVENT* evt);
    void simple_add_event(EVENT* evt);
    void add_event(TRANS_TYPE type, Process *evtProcess, int evtTimeStamp);
    void Simulation();
    void print_queue();
};

#endif /* DES_H */
