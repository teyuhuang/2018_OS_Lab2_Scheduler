#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "utility.h"
#include "scheduler.h"
#include "process.h"
#include "DES.h"
#define DEFAULT_QUANTUM 100000

using namespace std;

int main(int argc, char **argv){
    SCHEDULE_MODE sm;
    bool IsVerbose = false;
    int quantum = DEFAULT_QUANTUM;
    int index;
    int c;

    opterr = 0;

    // Reading Argument
    while ((c = getopt(argc, argv, "vs:")) != -1){
        switch (c){
        case 'v':
            IsVerbose = true;
            break;
        case 's':
            switch (optarg[0]){
            case 'F':
                sm = FCFS;
                break;
            case 'L':
                sm = LCFS;
                break;
            case 'S':
                sm = SJF;
                break;
            case 'R':
                sm = RR;
                quantum = Util::s2i(optarg + 1);
                break;
            case 'P':
                sm = PRIO;
                quantum = Util::s2i(optarg + 1);
                break;
            default:
                fprintf(stderr,
                        "Unknown option character %s.\n",
                        optarg);
                return 1;
            }
            break;
        case '?':
            fprintf(stderr,
                    "Unknown option character `\\x%x'.\n",
                    optopt);
            return 1;
        default:
            abort();
        }
    }


    // cout<<sm<<":"<<quantum<<" v:"<<vlag<<endl;

    string inputName = argv[argc - 2];
    string randFile = argv[argc - 1];
    DES des(inputName,randFile,sm, quantum);
    des.print_queue();

    return 0;
}