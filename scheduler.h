#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "process.h"
enum SCHEDULE_MODE {FCFS,LCFS,SJF,RR,PRIO};

class Scheduler{
public:
    virtual int add_to_qeueue(Process *p) = 0;
    virtual Process * get_next_process() = 0;

};

class Sched_FCFS: public Scheduler{
public:
    int add_to_qeueue(Process *p);
    Process * get_next_process();
};

class Sched_LCFS: public Scheduler{
public:
    int add_to_qeueue(Process *p);
    Process * get_next_process();
};

class Sched_SJF: public Scheduler{
public:
    int add_to_qeueue(Process *p);
    Process * get_next_process();
};

class Sched_RR: public Scheduler{
public:
    int add_to_qeueue(Process *p);
    Process * get_next_process();
};

class Sched_PRIO: public Scheduler{
public:
    int add_to_qeueue(Process *p);
    Process * get_next_process();
};

#endif /* SCHEDULER_H */
