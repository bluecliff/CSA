/**
 * @file buildcsa.cpp
 * @brief	构造CSA 输入原始字符串序列和后缀数组文件，后缀数组文件要求是二进制整数文件，第一个数为后缀长度n
 * @author li shuangjiang
 * @version 3.0.0
 * @date 2012-12-03
 */
#include <iostream>
#include "csa.h"
#include "def.h"
#include <fstream>
#include <string>
#include <time.h>
using namespace std;
int main(int argc,char** argv)
{
	clock_t beg=clock();
//	string textFile="english.50MB";
//	string saFile="english.50MB.sa";
//	string textFile="test.txt";
//	string saFile="test.txt.sa";
//	string csaFile="english.50MB.csa";
	if(argc!=3)
	{
		cout<<"arguments erro"<<endl;
		return -1;
	}
	string textFile=argv[1];
	string saFile=argv[2];
	string csaFile=saFile+".csa";
	ifstream fin1;
	ifstream fin2;
	ofstream fout;
	fin1.open(textFile.c_str());
	fin2.open(saFile.c_str(),ios::binary);
	fout.open(csaFile.c_str(),ios::binary);
	if(fin1 && fin2 && fout)
	{
		uint n=0;
		uchar* T;
		uint* sa;
		fin2.read((char*)&n,sizeof(n));
		T=new uchar[n];
		sa=new uint[n];
		fin2.read((char*)sa,sizeof(*sa)*n);
		uchar s;
		for(uint i=0;i<n-1;i++)
		{
			s=fin1.get();
			if(s==0)
			{
				cout<<"have a character of 0"<<endl;
				cout<<i<<endl;
				return -1;
			}
			if(s=='A')
			{
				T[i]=1;
			}
			else if(s=='C')
			{
				T[i]=2;
			}
			else if(s=='G')
			{
				T[i]=3;
			}
			else if(s=='T')
			{
				T[i]=4;
			}
			else
			{
				T[i]=5;
			}
		}
		T[n-1]=0;
////		for(uint i=0;i<n;i++)
////		{
////		    cout<<(int)T[i]<<" ";
////		}
////		cout<<endl;
////		for(uint i=0;i<n;i++)
////		{
////		    cout<<sa[i]<<" ";
////		}
		cout<<endl;
		csa c(T,sa,n,6);
		c.write(fout);
		delete[] sa;
		delete[] T;
	}
	else
	{
		cout<<"file opening error!"<<endl;
	}

	fin1.close();
	fin2.close();
	clock_t end=clock();
	cout<<end-beg<<endl;
	return 0;
}
