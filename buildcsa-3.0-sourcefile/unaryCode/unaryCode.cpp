/**
 * @file unaryCode.cpp
 * @brief	unaryCode类实现
 * @author li shuangjiang
 * @version 3.0.0
 * @date 2012-11-22
 */
#include <iostream>
#include "unaryCode.h"

unaryCode::unaryCode()
{
	head=0;
	list=NULL;
	listsize=0;
	n=0;

	f_level=NULL;
	f_level_size=0;

	s_level=NULL;
	s_level_size=NULL;

	f_width=0;
	s_width=0;
}

unaryCode::unaryCode(uint n,uint* table)
{
	this->n=n;
	this->head=table[0];
	//list保存的是01串，总长度为bits_num
	this->bits_num=table[n-1]+n-1-table[0];
	this->listsize=bits_num/ulong_word_size+1;
	this->list = new ulong[listsize];
	memset(this->list,0,listsize*sizeof(ulong));
	f_width=widthOfSB(bits_num);
	s_width=widthOfBlock(bits_num);

	this->f_level_size=bits_num/f_width+1;
	this->s_level_size=bits_num/s_width+1;
	this->f_level=new compactIntArray(f_level_size,table[n-1]+1);
	this->s_level=new compactIntArray(s_level_size,log2(bits_num)*log2(bits_num)+1);

    uint top=0;
	for(uint i=1;i<n;i++)
	{
		uint temp=table[i]-table[i-1];
		setfild(temp,top);
		top=top+temp+1;
	}
	settable(f_width,s_width);
}
unaryCode::~unaryCode()
{
	if(list!=NULL)
	{
		delete[] list;
		list=NULL;
	}
	if(this->f_level!=NULL)
	{
		delete f_level;
		f_level=NULL;
	}
	if(this->s_level!=NULL)
	{
		delete s_level;
		s_level=NULL;
	}
}
uint unaryCode::size()
{
    return this->n;
}
bool unaryCode::write(ofstream& fout)
{
    if(fout)
    {
        fout.write((char*)&head,sizeof(head));
        fout.write((char*)&n,sizeof(n));
        fout.write((char*)&listsize,sizeof(listsize));
        fout.write((char*)&bits_num,sizeof(bits_num));
        fout.write((char*)&f_level_size,sizeof(f_level_size));
        fout.write((char*)&s_level_size,sizeof(s_level_size));
        fout.write((char*)&f_width,sizeof(f_width));
        fout.write((char*)&s_width,sizeof(s_width));
        fout.write((char*)this->list,sizeof(ulong)*listsize);
        f_level->write(fout);
        s_level->write(fout);
        return true;
    }
    else
    {
        return false;
    }
}
bool unaryCode::read(ifstream& fin)
{
    if(fin)
    {
        if(this->list!=NULL)
        {
            delete[] this->list;
        }
        if(f_level!=NULL)
        {
            delete f_level;
        }
        if(s_level!=NULL)
        {
            delete s_level;
        }
        fin.read((char*)&head,sizeof(head));
        fin.read((char*)&n,sizeof(n));
        fin.read((char*)&listsize,sizeof(listsize));
        fin.read((char*)&bits_num,sizeof(bits_num));
        fin.read((char*)&f_level_size,sizeof(f_level_size));
        fin.read((char*)&s_level_size,sizeof(s_level_size));
        fin.read((char*)&f_width,sizeof(f_width));
        fin.read((char*)&s_width,sizeof(s_width));
        this->list=new ulong[listsize];
        memset(this->list,0,listsize*sizeof(ulong));
        fin.read((char*)this->list,sizeof(ulong)*listsize);
        f_level=new compactIntArray();
        s_level=new compactIntArray();
        f_level->read(fin);
        s_level->read(fin);
        return true;
    }
    else
    {
        return false;
    }
}
void unaryCode::settable(uint f_width,uint s_width)
{
	(*f_level)[0]=0;
	(*s_level)[0]=0;
	uint k=f_width/s_width;
	for(uint i=1;i<s_level_size;i++)
	{
		uint num_ones=popcount((i-1)*s_width,s_width);
		if(i%k == 0)
		{
			(*s_level)[i]=0;
			(*f_level)[i/k]=(*f_level)[i/k-1]+(*s_level)[i-1]+num_ones;
		}
		else
		{
		    uint temp_1=(*s_level)[i-1];
			(*s_level)[i]=(temp_1+num_ones);
		}
	}
//	for(uint i=0;i<s_level_size;i++)
//	{
//	    std::cout<<(*s_level)[i]<<endl;
//	}
}
uint unaryCode::popcount(uint beg,uint k)
{
	uint i=beg/ulong_word_size;
	uint j=beg%ulong_word_size;
	uint hole=ulong_word_size-j;
	if(hole>=k)
	{
		uint res=0;
		for(uint count=0;count<k;count++)
		{
			if((1UL<<(j+count) & list[i]))
			{
				res++;
			}
		}
		return res;
	}
	else
	{
		uint res=0;
		for(uint count=j;count<ulong_word_size;count++)
		{
			res+=((1UL << count) & list[i]);
		}
		for(uint count=0;count<(k-hole);count++)
		{
			res+=((1UL << count) & list[i+1]);
		}
		return res;
	}
}

void unaryCode::setfild(ulong k,ulong top)
{
	uint i=top/ulong_word_size;
	uint j=top%ulong_word_size;
	uint hole = ulong_word_size-j;
	if(hole>=(k+1))
	{
		list[i]=list[i] |(1UL << (j+k));
		//top=top+k+1;
	}
	else
	{
		uint zeros = k-hole;
		uint i_offset=zeros/ulong_word_size;
		uint j_offset=zeros%ulong_word_size;
		list[i+1+i_offset]=(1UL << j_offset);
		//top=top+k+1;
	}
}
uint unaryCode::rank(uint index)
{
	uint beg=index/s_width;
	uint k=index%s_width;
	beg=beg*s_width;
	return (*f_level)[index/this->f_width]+(*s_level)[index/this->s_width]+this->popcount(beg,k+1);
}
uint unaryCode::select(uint index)
{
	if(index==0)
	{
		return 0;
	}
	ulong left=0;
	ulong right=this->bits_num-1;
	ulong j=0;
	while(left<=right)
	{
		j=(left+right)/2;
		if(this->rank(j)==index)
		{
			break;
		}
		if(this->rank(j)>index)
		{
			right=j-1;
		}
		else
		{
			left=j+1;
		}
	}
	while(j>0 && this->rank(j-1)!=index-1)
	{
		j--;
	}
	return j;
}
uint unaryCode::get(uint index)
{
	if(index == 0)
	{
		return this->head;
	}
	else
	{
	    uint k=select(index);
		return k-rank(k)+1+head;
	}
}
