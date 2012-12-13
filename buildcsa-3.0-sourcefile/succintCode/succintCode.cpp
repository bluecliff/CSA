
#include "succintCode.h"
#include "../def.h"
succintCode::succintCode()
{
	_size=0;
	w=0;
	z=0;
	qt=NULL;
	rt=NULL;
}
succintCode::succintCode(uint n,ulong* list)
{
	this->_size=n;
	z=log2(n);
	w=log2(list[n-1])+1;
	uint* r=new uint[n];
	uint* q=new uint[n];
	for(uint i=0;i<n;i++)
	{
		ulong temp=(list[i]>>(w-z));
		q[i]=temp;
		temp=(list[i]<<(ulong_word_size-w+z))>>(ulong_word_size-w+z);
		r[i]=temp;
	}
	rt=new compactIntArray(r,n,pow(2,w-z));
	qt=new unaryCode(n,q);
	delete[] r;
	delete[] q;
}
succintCode::~succintCode()
{
	_size=0;
	if(qt!=NULL)
	{
		delete qt;
		qt=NULL;
	}
	if(rt!=NULL)
	{
		delete rt;
		rt=NULL;
	}
}
ulong succintCode::get(uint index)
{
	uint r=rt->get(index);
	uint q=qt->get(index);
	ulong res=q;
	res=((res<<(w-z)) + r);
	return res;
}
bool succintCode::write(ofstream& fout)
{
	if(fout)
	{
		fout.write((char*)&_size,sizeof(_size));
        fout.write((char*)&z,sizeof(z));
		fout.write((char*)&w,sizeof(w));
		if(rt->write(fout) && qt->write(fout))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool succintCode::read(ifstream& fin)
{
	if(fin)
	{
		fin.read((char*)&_size,sizeof(_size));
		fin.read((char*)&z,sizeof(z));
		fin.read((char*)&w,sizeof(w));
        rt=new compactIntArray();
        qt=new unaryCode();
		if(rt->read(fin) && qt->read(fin))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
uint succintCode::size()
{
	return _size;
}
