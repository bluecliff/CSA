/**
 * @file rank_select.cpp
 * @brief	实现rank_select类
 * @author li shuangjiang
 * @version 3.0.0
 * @date 2012-11-26
 */
#include "rank_select.h"
#include <string.h>
rank_select::rank_select()
{
	list=NULL;
	listsize=0;
	bits_num=0;
	f_level_size=0;
	s_level_size=0;
	f_width=0;
	s_width=0;
	f_level=NULL;
	s_level=NULL;
}
rank_select::rank_select(uint n)
{
	bits_num=n;
	listsize=n/ulong_word_size+1;
	list=new ulong[listsize];
	memset(this->list,0,listsize*sizeof(ulong));
	f_width=widthOfSB(bits_num);
	s_width=widthOfBlock(bits_num);
	f_level_size=bits_num/f_width+1;
	s_level_size=bits_num/s_width+1;
	f_level=new compactIntArray(f_level_size,bits_num+1);
	s_level=new compactIntArray(s_level_size,log2(bits_num)*log2(bits_num)+1);
}
rank_select::~rank_select()
{
	if(list!=NULL)
	{
		delete[] list;
		list=NULL;
	}
	if(f_level!=NULL)
	{
		delete f_level;
		f_level=NULL;
	}
	if(s_level!=NULL)
	{
		delete s_level;
		s_level=NULL;
	}
}
uint rank_select::size()
{
    return this->bits_num;
}
bool rank_select::write(ofstream& fout)
{
	if(fout)
	{
		fout.write((char*)&bits_num,sizeof(bits_num));
		fout.write((char*)list,sizeof(*list)*listsize);
		f_level->write(fout);
		s_level->write(fout);
		return true;
	}
	else
	{
		return false;
	}
}
bool rank_select::read(ifstream& fin)
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
		fin.read((char*)&bits_num,sizeof(bits_num));
		listsize=bits_num/ulong_word_size+1;
		list=new ulong[listsize];
		memset(this->list,0,listsize*sizeof(ulong));
		f_width=widthOfSB(bits_num);
		s_width=widthOfBlock(bits_num);
		f_level_size=bits_num/f_width+1;
		s_level_size=bits_num/s_width+1;
		f_level=new compactIntArray(f_level_size,bits_num+1);
		s_level=new compactIntArray(s_level_size,log2(bits_num)*log2(bits_num)+1);
		fin.read((char*)list,sizeof(*list)*listsize);
		f_level->read(fin);
		s_level->read(fin);
		return true;
	}
	else
	{
		return false;
	}
}
void rank_select::set(uint index,bool value)
{
    if(value)
    {
        uint i=index/ulong_word_size;
        uint j=index%ulong_word_size;
        list[i]=(list[i] | (1UL << j));
    }
    else
    {
        uint i=index/ulong_word_size;
        uint j=index%ulong_word_size;
        list[i]=(list[i] | (1UL << j));
        list[i]=(list[i] ^ (1UL << j));
    }
}
bool rank_select::get(uint index)
{
	uint i=index/ulong_word_size;
	uint j=index%ulong_word_size;
	return (list[i] & (1UL << j));
}
void rank_select::init_table()
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
}
uint rank_select::popcount(uint beg,uint k)
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
uint rank_select::rank(uint index)
{
	uint beg=index/s_width;
	uint k=index%s_width;
	beg=beg*s_width;
	return (*f_level)[index/this->f_width]+(*s_level)[index/this->s_width]+this->popcount(beg,k+1);
}
uint rank_select::select(uint index)
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
