/**
 * @file rank_select.h
 * @brief
 * @author li shuangjiang
 * @version 3.0.0
 * @date 2012-11-26
 */
#ifndef RANK_SELECT_H_
#define RANK_SELECT_H_

#include "../def.h"
#include <fstream>
#include "../compactIntArray/compactIntArray.h"
using namespace std;

#ifndef _BLOCK_WIDTH_
#define _BLOCK_WIDTH_
/**
 * @brief	计算subblock的位宽
 *
 * @param	x	位串的长度
 *
 * @return	subblock的位宽
 */
inline uint widthOfBlock(ulong x)
{
    return uint(floor(log2(x)/2));
}

/**
 * @brief	计算superblock的位宽
 *
 * @param	x	位串的长度
 *
 * @return	superblock的位宽
 */
inline uint widthOfSB(ulong x)
{
    //return uint(floor(log2(x)*log2(x)));
	return uint(floor(log2(x))*widthOfBlock(x));
}
#endif
/**
 * @brief	rank_select类，输入为一个01串，该类紧凑存储01串且提供rank&select操作
 * 使用时，先实力化该类， 用 set函数初始化类的01串，再调用init函数建立rank&select所需要的辅助结构，
 * 否则不能正确返回rank和select的值 
 */
class rank_select
{
	public:
		rank_select();
		/**
		 * @brief	用01串的长度初始化类
		 *
		 * @param	n	01串的长度
		 */
		rank_select(uint n);
		~rank_select();
		/**
		 * @brief	返回index下标处的bool值
		 *
		 * @param	index	下标
		 *
		 * @return	bool值
		 */
		bool get(uint index);
		/**
		 * @brief	设定index处的bool值
		 *
		 * @param	index	下标
		 * @param	value	需要设定的值
		 */
		void set(uint index,bool value);
		/**
		 * @brief	把类数据写到文件
		 *
		 * @param	fout	输出文件流
		 *
		 * @return	是否写成功
		 */
		bool write(ofstream& fout);
		/**
		 * @brief	从文件读取类数据，重新构建类
		 *
		 * @param	fin	输入文件流
		 *
		 * @return	读文件是否成功
		 */
		bool read(ifstream& fin);
		/**
		 * @brief	01串的rank值
		 *
		 * @param	index	下标
		 *
		 * @return	rank值
		 */
		uint rank(uint index);
		/**
		 * @brief	01串的select值
		 *
		 * @param	index	下标
		 *
		 * @return	select值
		 */
		uint select(uint index);
		/**
		 * @brief	已经初始化完01串后需调用本函数，建立rank&select辅助数据结构
		 */
		void init_table();
		/**
		 * @brief	返回01该类存储的01串的长度
		 *
		 * @return	
		 */
		uint size();
	private:
		ulong* list;
		uint listsize;
		uint bits_num;
		compactIntArray* f_level;
		compactIntArray* s_level;
		uint f_level_size;
		uint s_level_size;
		uint f_width;
		uint s_width;
		uint popcount(uint beg,uint k);
};

#endif


