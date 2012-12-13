/**
 * @file def.h
 * @brief	the basic included file, define some macros ,gloable variables and inline functions
 * @author li shuangjiang
 * @version 3.0.0
 * @date 2012-11-16
 */
#ifndef DEF_H_
#define DEF_H_

#include<cmath>
typedef unsigned int uint;
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned char uchar;

const uint ulong_word_size=sizeof(ulong)*8;
const uint word_byte=sizeof(ulong);
inline uint log2(ulong x)
{
    return (uint)(log(x)/log(2));
}
inline uint log2(uint x)
{
	return (uint)(log(x)/log(2));
}	
inline ulong mypow(ulong base,uint exp)
{
    return (ulong)pow(base,exp);
}
inline ulong mypow(uint base,uint exp)
{
	return (ulong)pow(base,exp);
}


/*inline ul sizeOfBlock(ul x)
{
    return ul(floor(log2(x)/2));
}

inline ul sizeOfSB(ul x,ul b)
{
    return ul(b*floor(log2(x)));
}
*/
#endif
