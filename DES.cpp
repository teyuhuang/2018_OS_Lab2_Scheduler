#include <iostream>
#include <fstream>
#include <sstream>
#include "DES.h"
EVENT::EVENT(TRANS_TYPE typ, Process *p, int t):transition(typ),evtProcess(p),evtTimeStamp(t){}
std::string EVENT::toString(){
    string s = "";
    s+= to_string(evtTimeStamp)+' ';
    s+= to_string(evtProcess->PID)+' ';
    s+= to_string(evtProcess->timeInPrevState)+": ";
    switch(evtProcess->state){
        case CREATED:
            s+= "CREATED";
            break;
        case READY:
            s+= "READY";
            break;
        case RUNNING:
            s+= "RUNNG";
            break;
        case BLOCKED:
            s+= "BLOCK";
            break;
    }
    s+= " -> ";
    switch(transition){
        case TRANS_TO_READY:
            s+= "READY";
            break;
        case TRANS_TO_RUN:
            s+= "RUNNG";
            break;
        case TRANS_TO_BLOCK:
            s+= "BLOCK";
            s+="  ib="+to_string(evtProcess->remainingIO);
            s+=" rem="+to_string(evtProcess->TC-evtProcess->CT);
            break;
        case TRANS_TO_PREEMPT:
            s+= "READY";
            s+="  cb="+to_string(evtProcess->remainingCB);
            s+=" rem="+to_string(evtProcess->TC-evtProcess->CT);
            s+=" prio="+to_string(evtProcess->dynamic_priority);
            break;
    }
    return s;
}
DES::DES(string inputFile, string rfile,SCHEDULE_MODE mode,int quantum){
    if(rnd.open(rfile)){
        return;
    }
    switch(mode){
        case FCFS:
            THE_SCHEDULER = new Sched_FCFS();
            break;
        case LCFS:
            THE_SCHEDULER = new Sched_LCFS();
            break;
        case SJF:
            THE_SCHEDULER = new Sched_SJF();
            break;
        case RR:
            printQuantum = true;
            THE_SCHEDULER = new Sched_RR();
            break;
        case PRIO:
            printQuantum = true;
            THE_SCHEDULER = new Sched_PRIO();
            break;
        default:
            cerr<<"Wrong scheduler type!"<<endl;
            return;
    }
    //Start Loading process into scheduler
    ifstream inFile;
    string line;
    Process* proc;
    try {
        inFile.open(inputFile);
        if(inFile.is_open()){
            while(getline(inFile, line)){
                proc = new Process(line, rnd.myrandom(4));
                processList.push_back(proc);
                simple_add_event(new EVENT(TRANS_TO_READY,proc,proc->AT));
            }
            inFile.close();
        }
    }
    catch (ifstream::failure e) {
        cerr << "Exception opening/reading/closing file\n";
    }
    this->quantum = quantum;
}
EVENT* DES::get_event(){
    if(eventQ.empty()) return nullptr;
    EVENT* tmp = eventQ.front();
    eventQ.pop_front();
    return tmp;
}
void DES::simple_add_event(EVENT* evt){
    eventQ.push_back(evt);
}
void DES::add_event(EVENT* evt){
    list<EVENT*>::iterator it;
    for(it = eventQ.begin();it!= eventQ.end();it++){
        if((*it)->evtTimeStamp > evt->evtTimeStamp){
            eventQ.insert(it,evt);
            return;
        }
    }
    eventQ.push_back(evt);
}
void DES::add_event(TRANS_TYPE type, Process *evtProcess, int evtTimeStamp){
    this->add_event(new EVENT(type,evtProcess,evtTimeStamp));
}
int DES::get_next_event_time(){
    if(eventQ.empty()) return -1;
    else return eventQ.front()->evtTimeStamp;
}
void DES::Simulation(){
    EVENT *evt;
    while ((evt = get_event()))
    {
        Process *proc = evt->evtProcess; // this is the process the event works on
        CURRENT_TIME = evt->evtTimeStamp;
        evt->evtProcess->timeInPrevState = CURRENT_TIME - proc->state_ts;
        switch (evt->transition)
        { // which state to transition to?
        case TRANS_TO_READY:
            // must come from BLOCKED or from PREEMPTION
            // must add to run queue
            CALL_SCHEDULER = true; // conditional on whether something is run
            break;
        case TRANS_TO_RUN:
            // create event for either preemption or blocking
            break;
        case TRANS_TO_BLOCK:
            //create an event for when process becomes READY again
            CALL_SCHEDULER = true;
            break;
        case TRANS_TO_PREEMPT:
            // add to runqueue (no event is generated)
            CALL_SCHEDULER = true;
            break;
        }
        //remove current event object from Memory
        delete evt;
        evt = nullptr;
        if (CALL_SCHEDULER)
        {
            if (get_next_event_time() == CURRENT_TIME)
            {
                continue; //process next event from Event queue
            }
            CALL_SCHEDULER = false;
            if (CURRENT_RUNNING_PROCESS == nullptr)
            {
                CURRENT_RUNNING_PROCESS = THE_SCHEDULER->get_next_process();
                if (CURRENT_RUNNING_PROCESS == nullptr)
                {
                    continue;
                }
            }
            // create event to make process runnable for same time.
        }
    }
}
void DES::print_queue(){
    for(auto evt:eventQ){
        cout<<evt->toString()<<endl;
    }
}
void DES::report(){
    if(printQuantum)
        cout<<THE_SCHEDULER->name<<" "<<quantum<<endl;
    else
        cout<<THE_SCHEDULER->name<<endl;
    for(auto proc: processList){
        proc->print();
    }

    // Replace in the future!!
    int    maxfintime = 10;
	double cpu_util = 255.9887;
	double io_util  = 3.1415927;
	double avg_turnaround = 128.367;
	double avg_waittime = 0.0;
	double throughput = 8.9567;
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
	       maxfintime,
	       cpu_util,
	       io_util,
	       avg_turnaround,
	       avg_waittime, 
	       throughput);
}