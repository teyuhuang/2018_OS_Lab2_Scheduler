#ifndef UTILITY_H
#define UTILITY_H
#include <string>


typedef int stime_t;

struct Util{
    static int s2i(std::string s);
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


#endif /* UTILITY_H */
