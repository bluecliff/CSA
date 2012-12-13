/**
 * @file compactIntArray.h
 * @brief	紧凑存储整数数组的类型定义
 * @author li shuangjiang
 * @version 0.0.2
 * @date 2012-10-30
 */
#ifndef GENERAL_INT_H_
#define GENERAL_INT_H_

#include <fstream>
typedef unsigned long int ulong;
typedef unsigned int uint;

/**
 * @brief	紧凑存储给定大小和范围的整数数组，总存储空间为nlogn位
 */
class compactIntArray
{
    public:

		//实现[]操作符重载时作为左值的特殊情况
		class Reference
		{
			friend class compactIntArray;
			public:
				void operator = (uint value)
				{
					_pCompactIntArray->set(index,value);
					//return (*this);
				}
				operator uint() const{ return _pCompactIntArray->get(index);};
			private:
				Reference(compactIntArray& _compact_int_array,uint _index):_pCompactIntArray(&_compact_int_array),index(_index)
				{
					//constructor
				}
				compactIntArray* _pCompactIntArray;
				uint index;
		};
			/**
			 * @brief	默认构造函数，初始化为空数组
			 */
        compactIntArray();
		/**
		 * @brief	构造函数，初始化为0数组
		 *
		 * @param	n	需要紧凑存储的整数数组长度
		 * @param	max	需要紧凑存储的整数数组中的最大数
		 */
        compactIntArray(uint n, uint max);
		/**
		 * @brief	构造函数
		 *
		 * @param	arr	需要紧凑存储的整数数组指针或者数组名
		 * @param	n	需要紧凑存储的整数数组的大小
		 * @param	max	需要紧凑存储的整数数组中的最大数
		 */
        compactIntArray(uint* arr, uint n,uint max);
		/**
		 * @brief	复制构造函数
		 *
		 * @param	object
		 */
        compactIntArray(compactIntArray& object);
		/**
		 * @brief	析构函数，释放动态申请的内存
		 */
        ~compactIntArray();

		/**
		 * @brief	重载[]操作符，作为左值使用
		 *
		 * @param	index	要赋值的数组下标
		 *
		 * @return	内部引用值
		 */
		Reference operator[](uint index)
		{
			return (Reference(*this,index));
		}

		uint operator[](uint index) const
		{
			return this->get(index);
		}
		/**
		 * @brief	写文件函数
		 *
		 * @param	fout	输出文件流，二进制模式
		 *
		 * @return	写文件是否成功
		 */
        bool write(std::ofstream& fout);
		/**
		 * @brief	从文件读取重构紧凑数组
		 *
		 * @param	fin	输入文件流，二进制模式
		 *
		 * @return	读取紧凑数组是否成功
		 */
        bool read(std::ifstream& fin);
		/**
		 * @brief	获取紧凑数组的大小,即存了多少个整数
		 *
		 * @return	返回紧凑数组的大小
		 */
		uint getsize();
		/**
		 * @brief	为数组第index个索引赋值
		 *
		 * @param	index	索引，下标
		 *
		 * @param	value	要赋的值
		 */
		void set(uint index,ulong value);
		/**
		 * @brief	获取数组第index个值
		 *
		 * @param	index	索引，下标
		 *
		 * @return	获取的值
		 */
		uint get(uint index) const;
    private:
		/**
		 * @brief	成员变量，类内部完成紧凑存储的数组指针
		 */
        ulong* array;
		/**
		 * @brief	成员变量，紧凑数组的长度
		 */
        uint size;
		/**
		 * @brief	成员变量，紧凑数组存储一个整数所用的位数
		 */
        uint wordsize;
		/**
		 * @brief	内部数组 array 的长度，为类实际分配的内存大小
		 */
        uint innersize;

		/**
		 * @brief	初始化函数，用 arr 数组初始化内部数组 array
		 *
		 * @param	arr 需要紧凑存储的数组
		 */
        void init(uint* arr);
		/**
		 * @brief	设定内部数组 array 的第index个数，类似给第index个数赋值
		 *
		 * @param	index	紧凑数组要赋值的索引
		 * @param	value	要赋的值
		 */
        void setfiled(uint index,ulong value);

		/**
		 * @brief	读取内部数组 array 中的第index个整数
		 *
		 * @param	index	要读取的紧凑数组的索引（下标）
		 *
		 * @return	获取的紧凑数组第index位置的整数值
		 */
        ulong getfiled(uint index) const;
};
#endif
