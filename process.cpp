#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "process.h"
#include "utility.h"
int Process::idCounter = 0;

Process::Process(int at, int tc, int cb, int io,int sp){
    AT=at;
    TC=tc;
    CB=cb;
    IO=io;
    PID=idCounter++;
    static_priority=sp;
    dynamic_priority = static_priority-1;
}
Process::Process(std::string line,int sp){
    std::size_t lastFound = 0;
    int* parameters[]={&AT, &TC, &CB, &IO};
    //convert input string into parameters
    for(int i = 0; i<4; i++){
        std::size_t found = line.find(" ", lastFound);
        if (found!=std::string::npos){
            *(parameters[i]) = Util::s2i(line.substr(lastFound, found-lastFound));
            lastFound = found+1;
            while(lastFound < line.size()&&line.at(lastFound)==' ') lastFound++;
        }
        else if(i ==3 ){
            *(parameters[i]) = Util::s2i(line.substr(lastFound));
        }
        else{
            std::cerr<<"Error input"<<std::endl;
            exit(0);
        }
    }
       
    PID=idCounter++;
    static_priority=sp;
    dynamic_priority = static_priority-1;
}
void Process::print(){
    std::cout<<"PID:"<<PID
        <<" AT="<<AT
        <<" TC="<<TC
        <<" CB="<<CB
        <<" IO="<<IO
        <<" Status="<<state
        <<" static_prio:"<<static_priority
        <<" dymi_prio:"<<dynamic_priority
        <<std::endl;
}