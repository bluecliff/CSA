#include "csa.h"
#include <string.h>
#include <iostream>
using namespace std;
csa::csa()
{
    this->_size=0;
    this->level=0;
	this->sigma=0;
    this->sc=NULL;
    this->B=NULL;
    this->sa_l=NULL;
    this->sa_odd=NULL;
}
csa::csa(uchar *T , uint* sa , uint n , uint sigma)
{
    this->_size=n;
    // level=round(loglog(n)),k=0,1---l-1
	this->level=log2(log2(this->_size))+1;
    //this->level=3;
    this->sigma=sigma;
    //使数组的下标从1开始，且sa的值的范围也变为从1--n
    T--;
    sa--;
    for(uint i=1;i<=this->_size;i++)
    {
        sa[i]++;
    }
    //////////////////////////
    //////////////////////////////////////
    //初始化

	this->B=new rank_select*[this->level];
    //this->rt=new unaryCode*[this->level];
    //this->qt=new compactIntArray*[this->level];
    this->sc=new succintCode*[this->level];
	this->sa_l=new uint[(this->_size>>this->level)+1]; //the sa of lth level
    this->sa_odd=new uint[level];     //to store the biggest odd value of sa of 0-level-1 level
    memset(this->sa_l,0,((this->_size>>this->level)+1)*sizeof(uint));
    memset(this->sa_odd,255,this->level*sizeof(uint));

    uint* isa=new uint[this->_size+1]; // inverse of sa
    ulong* list=NULL;
    uint size_temp=this->_size;
    for(uint i=0;i<this->level;i++)
    {
        size_temp=this->_size>>i;
        B[i]=new rank_select(size_temp+1);
        list=new ulong[size_temp/2+1];
        memset(list,0,(size_temp/2+1)*sizeof(ulong));

        //to get the isa and the B ,rank of ith lever
        for(uint j=1;j<=size_temp;j++)
        {
            isa[sa[j]]=j;
            B[i]->set(j,(sa[j]+1)%2);
        }
        B[i]->init_table();
        ////////////////////////
        /////////////////////////
        //to get the psi array and make concatenated list L_i of ith level
        uint k=1;
        string str_old;
        string str_new;
        int id=-1;
        for(uint j=1;j<=size_temp;j++)
        {
            if((sa[j]%2))
            {
                if(sa[j]!=size_temp)
                {
                    str_old=str_new;
                    str_new.clear();
                    list[k]=isa[sa[j]+1];

                    /*
                    ul begin=(sa[j])*pow(2,i);
                    ul end=(sa[j]+1)*pow(2,i)-1;
                    */
                    uint begin = sa[j] * (1UL<<i);
                    uint end = (sa[j]+1) * (1UL<<i)-1;
                    while(begin<=end)
                    {
                        str_new.append(1,T[begin]);
                        //num+=T[begin]*pow(this->sigma,end-begin);
                        begin++;
                    }
                    if(str_new!=str_old)
                    {
                        id++;
                    }
                    ulong num=(ulong)id;
                    list[k]+=num<<(log2(size_temp)+1);

                    if(list[k]<list[k-1])
                    {
                        cout<<"wrong"<< list[k-1]<<" "<<list[k]<<endl;
                        cout<<i<<" "<<j<<endl;
                        cout<<num<<endl;
                    }
                    k++;
                }
                else
                {
                    this->sa_odd[i]=j;//implies that sa[j] is odd and is the biggest in the ith level
                }
            }
        }

        //////////////////
        //to get the sa of next level
        k=1;
        for(uint j=1;j<=size_temp;j++)
        {
            if(!(sa[j]%2))
            {
                sa[k]=sa[j]/2;
                k++;
            }
        }
        /////////////////////
        this->sc[i]=new succintCode(size_temp/2+1,list);
        //this->sc[i]=new succincode(size_temp/2+1,list[size_temp/2],list);
        if(list!=NULL)
        {
            delete[] list;
            list=NULL;
        }
    }
    for(uint j=1;j<=size_temp/2;j++)
    {
        this->sa_l[j]=sa[j];
    }
    delete[] isa;
}
uint csa::size()
{
    return _size;
}
csa::~csa()
{
    if(this->sc != NULL)
    {
        for(uint i=0;i<this->level;i++)
        {
            if(this->sc[i]!=NULL)
            {
                delete this->sc[i];
            }
        }
        delete[] this->sc;
        this->sc=NULL;
    }
    if(this->B!=NULL)
    {
        for(uint i=0;i<this->level;i++)
        {
            if(this->B[i]!=NULL)
            {
                delete this->B[i];
            }
        }
        delete[] this->B;
        this->B=NULL;
    }
    if(this->sa_l!=NULL)
    {
        delete[] this->sa_l;
        this->sa_l=NULL;
    }
    if(this->sa_odd!=NULL)
    {
        delete[] this->sa_odd;
        this->sa_odd=NULL;
    }
}
uint csa::lookup(int i)
{
    return rlookup(i,0);
}
uint csa::rlookup(uint i,uint k)
{
    if(k==this->level)
    {
        return this->sa_l[i];
    }
    else if(this->sa_odd[k]==i)
    {
        return this->_size>>k;
    }
    else
    {
        if(this->B[k]->get(i))
        {
            return 2*this->rlookup(this->B[k]->rank(i),k+1);
        }
        else
        {
            uint size_temp=(this->_size>>k);
            //for the odd sa_k there are two cases to consider
            if(i>this->sa_odd[k])
            {
                //ul temp=this->list[k][i-this->rank[k][i]-1] & (mypow(2,(log2(this->size>>k)+1))-1);
                //ul temp=this->sc[k]->get(i-this->rt[k]->rank(i)-1) & (mypow(2,(log2(this->size>>k)+1))-1);
                ulong temp=this->sc[k]->get(i-this->B[k]->rank(i)-1);
                temp=(temp << (ulong_word_size-(log2(size_temp)+1))) >> (ulong_word_size-(log2(size_temp)+1));
                //return 2*this->rlookup(this->rank[k][temp],k+1)-1;
                //return 2*this->rlookup(this->rt[k]->rank(temp),k+1)-1;
                return 2*this->rlookup(this->B[k]->rank((uint)temp),k+1)-1;
            }
            else
            {
                //ul temp=this->list[k][i-this->rank[k][i]] & (mypow(2,(log2(this->size>>k)+1))-1);
                //ul temp=this->sc[k]->get(i-this->rt[k]->rank(i)) & (mypow(2,(log2(this->size>>k)+1))-1);
                ulong temp=this->sc[k]->get(i-this->B[k]->rank(i));
                temp=(temp << (ulong_word_size-(log2(size_temp)+1))) >> (ulong_word_size-(log2(size_temp)+1));
                //return 2*this->rlookup(this->rank[k][temp],k+1)-1;
                //return 2*this->rlookup(this->rt[k]->rank(temp),k+1)-1;
                return 2*this->rlookup(this->B[k]->rank((uint)temp),k+1)-1;
            }
        }
    }
}
bool csa::write(ofstream& fout)
{
    if(fout)
    {
        fout.write((char*)&_size,sizeof(_size));
        fout.write((char*)&sigma,sizeof(sigma));
        fout.write((char*)sa_l,sizeof(*sa_l)*((_size >> level)+1));
        fout.write((char*)sa_odd,sizeof(*sa_odd)*(level));
        for(uint i=0;i<level;i++)
        {
            this->B[i]->write(fout);
        }
        for(uint i=0;i<level;i++)
        {
            this->sc[i]->write(fout);
        }
        return true;
    }
    else
    {
        return false;
    }
}
bool csa::read(ifstream& fin)
{
    if(fin)
    {
        fin.read((char*)&_size,sizeof(_size));
        fin.read((char*)&sigma,sizeof(sigma));
        this->level=log2(log2(this->_size))+1;
        this->sa_l=new uint[(_size >> level)+1];
        this->sa_odd=new uint[level];
        fin.read((char*)sa_l,sizeof(uint)*((_size >> level)+1));
        fin.read((char*)sa_odd,sizeof(uint)*(level));
        this->sc=new succintCode*[this->level];
        this->B=new rank_select*[this->level];
        for(uint i=0;i<level;i++)
        {
            this->B[i]=new rank_select();
            this->B[i]->read(fin);
        }
        for(uint i=0;i<level;i++)
        {
            this->sc[i]=new succintCode();
            this->sc[i]->read(fin);
        }
        return true;
    }
    else
    {
        return false;
    }
}
