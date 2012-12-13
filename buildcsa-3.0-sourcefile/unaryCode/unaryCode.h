/**
 * @file unaryCode.h
 * @brief	unaryCode类的声明
 * @author li shuangjiang
 * @version 3.0.0
 * @date 2012-11-16
 */
#ifndef UNARY_CODE_H_
#define UNARY_CODE_H_

#include "../def.h"
#include <fstream>
#include "../compactIntArray/compactIntArray.h"
#include <string.h>
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
 * @brief	unary编码类
 */
class unaryCode
{
	public:
		/**
		 * @brief	构造函数
		 */
		unaryCode();
		/**
		 * @brief	构造函数
		 *
		 * @param	n	要保存的数组的长度
		 * @param	table	要保存的数组
		 */
		unaryCode(uint n,uint* table);
		/**
		 * @brief	析构函数
		 */
		~unaryCode();
		/**
		 * @brief	返回保存数组的第index个值
		 *
		 * @param	index	下标
		 *
		 * @return	存入的第index个数的值
		 */
		uint get(uint index);
		/**
		 * @brief	把该类写入文件
		 *
		 * @param	fout	输出文件留
		 *
		 * @return	写文件成功与否
		 */
		bool write(ofstream& fout);
		/**
		 * @brief	读文件，重建该类
		 *
		 * @param	fin	输入流
		 *
		 * @return	读文件成功与否
		 */
		bool read(ifstream& fin);
		/**
		 * @brief	返回该类存储的递增序列的序列长度
		 *
		 * @return	
		 */
		uint size();
	private:
		/**
		 * @brief	数组的第一个数
		 */
		uint head;
		/**
		 * @brief	内部维护的存储01串的数组
		 */
		ulong* list;
		/**
		 * @brief	list长度
		 */
		uint listsize;
		/**
		 * @brief	01串的总位数
		 */
		ulong bits_num;
		/**
		 * @brief	保存数组的大小
		 */
		uint n;
		/**
		 * @brief	内部维护的指针，标记list当前的顶部元素
		 */
//		ulong top;

		/**
		 * @brief	一级rank索引，采用紧凑数据类型存储
		 */
		compactIntArray* f_level;
		/**
		 * @brief	二级rank索引，采用紧凑数据类型存储
		 */
		compactIntArray* s_level;

		/**
		 * @brief	一级rank索引的规模
		 */
		uint f_level_size;
		/**
		 * @brief	二级rank索引的规模
		 */
		uint s_level_size;
		/**
		 * @brief	一级目录存储的超级块位模式的宽度
		 */
		uint f_width;
		/**
		 * @brief	二级目录存储的小块的位模式的宽度
		 */
		uint s_width;

		/**
		 * @brief	rank函数
		 *
		 * @param	index	下标
		 *
		 * @return	rank[index]值
		 */
		uint rank(uint index);
		/**
		 * @brief	select函数
		 *
		 * @param	index 下标
		 *
		 * @return	select[index]值
		 */
		uint select(uint index);
		/**
		 * @brief	设定从top开始的k位list表
		 *
		 * @param	k	设定k的一元编码方案
		 * @param	top	从list的top位开始设定
		 */
		void setfild(ulong k,ulong top);
		/**
		 * @brief	设定两级目录表
		 *
		 * @param	f_width	一级目录 存储的01串的长度
		 * @param	s_width	二级目录存储的01串的长度
		 */
		void settable(uint f_width,uint s_width);
		/**
		 * @brief	计算list中从beg开始的k位串中1的个数
		 *
		 * @param	beg	开始位置
		 * @param	k	计算的范围
		 *
		 * @return	1的个数
		 */
		uint popcount(uint beg,uint k);
};


#endif
