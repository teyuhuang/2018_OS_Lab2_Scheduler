#include <iostream>
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
    printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
            PID,
            AT, TC, CB, IO, static_priority,
            state_ts, // last time stamp
            state_ts - AT,
            IT,
            CW);
}