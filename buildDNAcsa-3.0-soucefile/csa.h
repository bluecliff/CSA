/**
 * @file csa.h
 * @brief	CSA的实现，采用vitter的方法，逐层压缩
 * @author li shuangjiang
 * @version 3.0.0
 * @date 2012-11-28
 */
#ifndef CSA_H_
#define CSA_H_
#include "def.h"
#include "./rank_select/rank_select.h"
#include "./succintCode/succintCode.h"
#include <fstream>
using namespace std;

class csa
{
    public:
        csa();
        csa(uchar* T , uint* sa, uint n ,uint sigma);
        //void makecsa(int *T , int* sa);
        //void output();
        uint lookup(int i);
        bool write(ofstream& fout);
        bool read(ifstream& fin);
        uint size();
        ~csa();
    private:
        uint _size;
        uint level;
        uint sigma;
        rank_select** B;
        succintCode** sc;
        uint* sa_l;
        uint* sa_odd;
        ///////
        uint rlookup(uint i,uint k);
};
#endif
