#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <list>
#include "process.h"
enum SCHEDULE_MODE {FCFS,LCFS,SJF,RR,PRIO};

class Scheduler{
public:
    std::string name;
    virtual void add_to_qeueue(Process *p) = 0;
    virtual Process * get_next_process() = 0;
};

class Sched_FCFS: public Scheduler{
private:
    std::queue<Process*> procQ;
public:
    void add_to_qeueue(Process *p);
    Process * get_next_process();
    Sched_FCFS();
};

class Sched_LCFS: public Scheduler{
    private:
    std::stack<Process*> procStk;
public:
    void add_to_qeueue(Process *p);
    Process * get_next_process();
    Sched_LCFS();
};

class Sched_SJF: public Scheduler{
private:
    std::list<Process*> procLst;
public:
    void add_to_qeueue(Process *p);
    Process * get_next_process();
    Sched_SJF();
};

class Sched_RR: public Sched_FCFS{
public:
    Sched_RR();
};

class Sched_PRIO: public Scheduler{
private:
    std::queue<Process*>* currentQ = new std::queue<Process*>[NUM_OF_PRIO];
    std::queue<Process*>* expiredQ = new std::queue<Process*>[NUM_OF_PRIO];
public:
    static const int NUM_OF_PRIO = 4;
    void add_to_qeueue(Process *p);
    Process * get_next_process();
    Sched_PRIO();
};

#endif /* SCHEDULER_H */
