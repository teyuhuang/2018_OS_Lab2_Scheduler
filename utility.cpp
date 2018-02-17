#include <iostream>
#include <fstream>
#include <sstream>
#include "utility.h"
using namespace std;
int Util::s2i(string s){
    return stoi(s,nullptr,10);
    }

int myRand::myrandom(int burst) {
    if(isFileLoaded){
         int rand = 1 + (randvals[ofs++] % burst);
        if(ofs>=MAX_ofs) ofs = 0;
        return rand;
    }
    else{
        cerr << "Read random file first\n";
        exit(0);
    }
}
int myRand::open(std::string filename){
    ifstream inFile;
    string inputStr;
    int idx = 0;
    ofs = 0;

    try {
        inFile.open(filename);
        if(inFile.is_open()){
            while(getline(inFile, inputStr)){
                if(MAX_ofs<0){      // first line
                    MAX_ofs = Util::s2i(inputStr);
                    randvals = new int[MAX_ofs];
                }
                else{
                    randvals[idx++] = Util::s2i(inputStr);
                }
            }
            inFile.close();
            if(idx != MAX_ofs){
                cerr << "[Random File] number of entry doesn't meet\n";
                MAX_ofs = idx;
            }
        }
    }
    catch (ifstream::failure e) {
        cerr << "Exception opening/reading/closing file\n";
        return 1;
    }
    isFileLoaded = true;
    return 0;
}
myRand::myRand(std::string filename){
    this->open(filename);
}
myRand::myRand(){}