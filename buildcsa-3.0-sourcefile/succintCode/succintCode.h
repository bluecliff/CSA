/**
 * @file succintCode.h
 * @brief	对递增序列的一种压缩存储，且支持常量时间访问递增序列的任意数据
 * @author li shuangjiang
 * @version 3.0.0
 * @date 2012-11-28
 */
#ifndef SUCCINT_CODE_H_
#define SUCCINT_CODE_H_

#include "../compactIntArray/compactIntArray.h"
#include "../unaryCode/unaryCode.h"

class succintCode
{
	public:
		succintCode();
		succintCode(uint n,ulong* list);
		~succintCode();
		ulong get(uint index);
		bool write(ofstream& fout);
		bool read(ifstream& fin);
		uint size();
	private:
		uint _size;
		uint w;
		uint z;
		compactIntArray* rt;
		unaryCode* qt;
};

#endif

