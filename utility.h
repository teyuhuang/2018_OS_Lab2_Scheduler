#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>


typedef int stime_t;

struct Util{
    static void report();
    static void round_check();
    static int s2i(std::string s);
};

struct Event{

};
struct myRand{
    bool isFileLoaded = false;
    int MAX_ofs = -1;
    int ofs;
    int* randvals;
    int myrandom(int burst);
    int open(std::string filename);
    myRand();
    myRand(std::string filename);

};

// int myrandom(int burst);

#endif /* UTILITY_H */
