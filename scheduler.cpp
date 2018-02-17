#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "scheduler.h"

using namespace std;

/*
* Implementation of FCFS
* Override add_to_qeueue() and get_next_process()
*/
int Sched_FCFS::add_to_qeueue(Process *p){
	p->print();
	return 1;
}
Process * Sched_FCFS::get_next_process(){
	return new Process(1, 1, 1, 1, 1);
}

/*
* Implementation of LCFS
* Override add_to_qeueue() and get_next_process()
*/
int Sched_LCFS::add_to_qeueue(Process *p){
	return 1;
}
Process * Sched_LCFS::get_next_process(){
	return new Process(2, 2, 2, 2, 2);
}


/*
* Implementation of SJF
* Override add_to_qeueue() and get_next_process()
*/
int Sched_SJF::add_to_qeueue(Process *p){
	return 1;
}
Process * Sched_SJF::get_next_process(){
	return new Process(3, 3, 3, 3, 3);
}


/*
* Implementation of RR
* Override add_to_qeueue() and get_next_process()
*/
int Sched_RR::add_to_qeueue(Process *p){
	return 1;
}
Process * Sched_RR::get_next_process(){
	return new Process(4, 4, 4, 4, 4);
}

/*
* Implementation of PRIO
* Override add_to_qeueue() and get_next_process()
*/
int Sched_PRIO::add_to_qeueue(Process *p){
	return 1;
}
Process * Sched_PRIO::get_next_process(){
	return new Process(5, 5, 5, 5, 5);
}



