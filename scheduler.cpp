#include <iostream>
#include "scheduler.h"

using namespace std;

/*
* Implementation of FCFS
* Override add_to_qeueue() and get_next_process()
*/
Sched_FCFS::Sched_FCFS(){
	name = "FCFS";
}
void Sched_FCFS::add_to_qeueue(Process *p){
	procQ.push(p);
}
Process * Sched_FCFS::get_next_process(){
	if(procQ.empty()) return nullptr;
	Process * tmp =procQ.front();
	procQ.pop();
	return tmp;
}

/*
* Implementation of LCFS
* Override add_to_qeueue() and get_next_process()
*/
Sched_LCFS::Sched_LCFS(){
	name = "LCFS";
}
void Sched_LCFS::add_to_qeueue(Process *p){
	procStk.push(p);
}
Process * Sched_LCFS::get_next_process(){
	if(procStk.empty()) return nullptr;
	Process * tmp =procStk.top();
	procStk.pop();
	return tmp;
}


/*
* Implementation of SJF
* Override add_to_qeueue() and get_next_process()
*/
Sched_SJF::Sched_SJF(){
	name = "SJF";
}
void Sched_SJF::add_to_qeueue(Process *p){
	list<Process*>::iterator it;
    for(it = procLst.begin();it!= procLst.end();it++){
        if( ((*it)->TC-(*it)->CT) > (p->TC-p->CT)){
            procLst.insert(it,p);
            return;
        }
    }
    procLst.push_back(p);
}
Process * Sched_SJF::get_next_process(){
	if(procLst.empty()) return nullptr;
    Process* tmp = procLst.front();
    procLst.pop_front();
    return tmp;
}


/*
* Implementation of RR
* Override add_to_qeueue() and get_next_process()
*/
Sched_RR::Sched_RR(){
	name = "RR";
}
void Sched_RR::add_to_qeueue(Process *p){

}
Process * Sched_RR::get_next_process(){
	return new Process(4, 4, 4, 4, 4);
}

/*
* Implementation of PRIO
* Override add_to_qeueue() and get_next_process()
*/
Sched_PRIO::Sched_PRIO(){
	name = "PRIO";
}
void Sched_PRIO::add_to_qeueue(Process *p){
}
Process * Sched_PRIO::get_next_process(){
	return new Process(5, 5, 5, 5, 5);
}



