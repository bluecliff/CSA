/**
 * @file compactIntArray.cpp
 * @brief	紧凑存储整数数组类的实现，实现把给定整数范围和数组大小的整数数组紧凑存储
 * @author li shuangjiang
 * @version 0.0.1
 * @date 2012-10-30
 */
#include "compactIntArray.h"
#include <cmath>
#include <string.h>


compactIntArray::compactIntArray()
{
    this->array=NULL;
    this->size=0;
    this->wordsize=0;
    this->innersize = 0;
}

/**
 * @brief	构造函数
 *
 * @param	arr	需要紧凑存储的整数数组指针或者数组名
 * @param	n	需要紧凑存储的整数数组的大小
 * @param	max	需要紧凑存储的整数数组中的最大数
 */
compactIntArray::compactIntArray(uint* arr,uint n,uint max)
{
    this->wordsize=(uint)log2(max)+1;
    this->size=n;
    this->innersize=(n*this->wordsize)/(8*sizeof(ulong))+1;
    this->array=new ulong[this->innersize];
    memset(this->array,0,sizeof(ulong)*this->innersize);
    this->init(arr);
}

/**
 * @brief	构造函数，初始化为0数组
 *
 * @param	n	需要紧凑存储的整数数组指针或者数组名
 * @param	max	需要紧凑存储的整数数组中的最大数
 */
compactIntArray::compactIntArray(uint n, uint max)
{
    this->size = n;
    this->wordsize = (uint)log2(max) + 1;
    this->innersize = (n * this->wordsize) / (8 * sizeof(ulong)) + 1;
    this->array = new ulong[this->innersize];
    memset(this->array,0,sizeof(ulong)*this->innersize);
}

/**
 * @brief	复制构造函数
 *
 * @param	object.size
 */
compactIntArray::compactIntArray(compactIntArray& object):size(object.size), wordsize(object.wordsize), innersize(object.innersize)
{
    this->array = new ulong[innersize];
    memcpy(this->array, object.array, innersize);
}

/**
 * @brief	析构函数，释放动态申请的内存
 */
compactIntArray::~compactIntArray()
{
    if(this->array)
    {
        delete[] this->array;
        this->array=NULL;
    }
}

void compactIntArray::init(uint* arr)
{
    for(uint i=0;i<this->size;i++)
    {
        this->setfiled(i,arr[i]);
    }
}

bool compactIntArray::write(std::ofstream& fout)
{
    fout.write((char*)&size, sizeof(size));
    fout.write((char*)&wordsize, sizeof(wordsize));
    fout.write((char*)&innersize, sizeof(innersize));

    fout.write((char*)array, sizeof(*array) * innersize);
    if(fout.bad())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool compactIntArray::read(std::ifstream& fin)
{
    fin.read((char*)&size, sizeof(size));
    fin.read((char*)&wordsize, sizeof(wordsize));
    fin.read((char*)&innersize, sizeof(innersize));
    this->array = new ulong[this->innersize];
    fin.read((char*)array, sizeof(*array) * innersize);

    if(fin.bad() || fin.fail())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void compactIntArray::setfiled(uint index,ulong value)
{
    uint i=(index*this->wordsize)/(sizeof(ulong)*8);
    uint j=(index*this->wordsize)%(sizeof(ulong)*8);
    uint hole=(sizeof(ulong)*8)-j;
    if(hole >= this->wordsize)
    {
        this->array[i]=(value<<j) | (this->array[i]);
    }
    else
    {
        this->array[i]=(value<<j) | (this->array[i]);
        this->array[i+1]=(value >> hole) | (this->array[i+1]);
    }
}

ulong compactIntArray::getfiled(uint index) const
{
    uint i=(index*this->wordsize)/(sizeof(ulong)*8);
    uint j=(index*this->wordsize)%(sizeof(ulong)*8);
    uint hole=(sizeof(ulong)*8)-j;
    if(hole >= this->wordsize)
    {
        ulong res=this->array[i];
        ulong temp=(1UL<<this->wordsize)-1;
        res=((res >> j) & temp);
        return res;
    }
    else
    {
		ulong res=(this->array[i] >> j);
        ulong temp=(1UL<<(this->wordsize-hole))-1;
        temp=temp & this->array[i+1];
        res=res | (temp<<hole);
        return res;
    }
}

uint compactIntArray::getsize()
{
	return this->size;
}

void compactIntArray::set(uint index, ulong value)
{


    uint i=(index*this->wordsize)/(sizeof(ulong)*8);
    uint j=(index*this->wordsize)%(sizeof(ulong)*8);
    uint hole=(sizeof(ulong)*8)-j;
    if(hole >= this->wordsize)
    {
       // this->array[i]=(value<<j) | (this->array[i]);
    	ulong mask1 = 0UL-1UL;
		ulong mask2 = mask1;
		mask1 = (j+wordsize)<(sizeof(ulong)*8)?(mask1 << (j+wordsize)):0;
		mask2 = (j==0)? 0:(mask2 >> (sizeof(ulong)*8-j));
	    this->array[i] = this->array[i] & (mask1 | mask2);
//		this->array[i] = (value<<j) | (this->array[i]);
		this->array[i] = (value << j) | (this->array[i]);
	}
    else
    {

    	ulong mask1 = 0UL-1UL;
		ulong mask2 = mask1;
		mask1 = mask1 >> (sizeof(ulong)*8-j);
		mask2 = mask2 << (wordsize-hole);
		this->array[i]=this->array[i] & mask1;
		this->array[i+1]=this->array[i+1] & mask2;
        this->array[i]=(value<<j) | (this->array[i]);
        this->array[i+1]=(value >> hole) | (this->array[i+1]);
    }
}

uint compactIntArray::get(uint index) const
{
	return this->getfiled(index);
}
