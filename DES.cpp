#include <iostream>
#include <fstream>
#include <sstream>
#include "DES.h"
EVENT::EVENT(TRANS_TYPE typ, Process *p, int t):transition(typ),evtProcess(p),evtTimeStamp(t){}
std::string EVENT::toString(){
    string s = "";
    s+= to_string(evtTimeStamp)+' ';
    s+= to_string(evtProcess->PID)+' ';
    if(transition == TRANS_TO_DONE){
        s+= to_string(evtProcess->timeInPrevState)+": Done";
        return s;
    }
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
            s+="  cb="+to_string(evtProcess->remainingCB);
            s+=" rem="+to_string(evtProcess->REM);
            s+=" prio="+to_string(evtProcess->dynamic_priority);
            break;
        case TRANS_TO_BLOCK:
            s+= "BLOCK";
            s+="  ib="+to_string(evtProcess->remainingIO);
            s+=" rem="+to_string(evtProcess->REM);
            break;
        case TRANS_TO_PREEMPT:
            s+= "READY";
            break;
    }
    return s;
}
DES::DES(string inputFile, string rfile,SCHEDULE_MODE mode,int quantum, bool v){
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
    this->printVerbose = v;
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
        if(printVerbose)  cout<<evt->toString()<<endl;
        switch (evt->transition)
        { // which state to transition to?
        case TRANS_TO_DONE:
            evt->evtProcess->FT =CURRENT_TIME;
            evt->evtProcess->state_ts = CURRENT_TIME;
            CURRENT_RUNNING_PROCESS = nullptr;
            CALL_SCHEDULER = true;
            break;
        case TRANS_TO_READY:
            // must come from BLOCKED or from PREEMPTION
            // must add to run queue
            evt->evtProcess->state = READY;
            evt->evtProcess->state_ts = CURRENT_TIME;
            THE_SCHEDULER ->add_to_qeueue(evt->evtProcess);
            CALL_SCHEDULER = true; // conditional on whether something is run
            break;
        case TRANS_TO_RUN:
            // create event for either preemption or blocking
            evt->evtProcess->state = RUNNING;
            evt->evtProcess->state_ts = CURRENT_TIME;
            if(evt->evtProcess->remainingCB<=quantum){
                evt->evtProcess->REM-=evt->evtProcess->remainingCB;
                evt->evtProcess->CT+=evt->evtProcess->remainingCB;
                if(evt->evtProcess->REM == 0){  
                    //done
                    add_event(TRANS_TO_DONE,evt->evtProcess,CURRENT_TIME+evt->evtProcess->remainingCB);    
                    break;
                }
                else{
                    evt->evtProcess->remainingIO = this->rnd.myrandom(evt->evtProcess->IO);
                    add_event(TRANS_TO_BLOCK,evt->evtProcess,CURRENT_TIME+evt->evtProcess->remainingCB);
                }
                evt->evtProcess->remainingCB = 0;
            }
            else{
                evt->evtProcess->remainingCB -= quantum;
                // evt->evtProcess->timeInPrevState = quantum;
                add_event(TRANS_TO_PREEMPT,evt->evtProcess,CURRENT_TIME+quantum);
                evt->evtProcess->REM-=quantum;
                evt->evtProcess->CT+=quantum;
            }
            break;
        case TRANS_TO_BLOCK:
            //create an event for when process becomes READY again
            CURRENT_RUNNING_PROCESS = nullptr;
            evt->evtProcess->state = BLOCKED;
            evt->evtProcess->state_ts = CURRENT_TIME;
            evt->evtProcess->timeInPrevState = evt->evtProcess->remainingIO;
            evt->evtProcess->IT+=evt->evtProcess->remainingIO;
            evt->evtProcess->dynamic_priority = evt->evtProcess->static_priority-1;
            this->add_event(TRANS_TO_READY,evt->evtProcess,CURRENT_TIME+evt->evtProcess->remainingIO);
            IOCounter.addTimeSpan(CURRENT_TIME,CURRENT_TIME+evt->evtProcess->remainingIO);
            CALL_SCHEDULER = true;
            break;
        case TRANS_TO_PREEMPT:
            // add to runqueue (no event is generated)
            CURRENT_RUNNING_PROCESS = nullptr;
            evt->evtProcess->dynamic_priority--;
            evt->evtProcess->state = READY;
            evt->evtProcess->state_ts = CURRENT_TIME;
            THE_SCHEDULER->add_to_qeueue(evt->evtProcess);
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
                else{
                    if(CURRENT_RUNNING_PROCESS->remainingCB<=0){
                        int burst =this->rnd.myrandom(CURRENT_RUNNING_PROCESS->CB);
                        CURRENT_RUNNING_PROCESS->remainingCB = (CURRENT_RUNNING_PROCESS->REM<burst)?CURRENT_RUNNING_PROCESS->REM:burst;
                    }
                    CURRENT_RUNNING_PROCESS->CW += CURRENT_TIME-CURRENT_RUNNING_PROCESS->state_ts;
                    this->add_event(TRANS_TO_RUN,CURRENT_RUNNING_PROCESS,CURRENT_TIME);
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

    // Replace in the future!!
    int    maxfintime = -1;
	double cpu_util = 0;
	double io_util  = IOCounter.getTotalTime();
	double avg_turnaround = 0.0;
	double avg_waittime = 0.0;
	double throughput = 0.0;

    for(auto proc: processList){
        proc->print();
        if(proc->FT>maxfintime) 
            maxfintime=proc->FT;
        avg_turnaround += proc->state_ts - proc->AT;
        avg_waittime += proc->CW;
        cpu_util += proc->TC;
    }
    if(processList.size()>0&&maxfintime>0){
        cpu_util = cpu_util/maxfintime*100.0;
        avg_turnaround = avg_turnaround/processList.size();
        avg_waittime = avg_waittime/processList.size();
        throughput = processList.size()*100.0/maxfintime;
        io_util = io_util*100.0/maxfintime;
    }
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
	       maxfintime,
	       cpu_util,
	       io_util,
	       avg_turnaround,
	       avg_waittime, 
	       throughput);
}