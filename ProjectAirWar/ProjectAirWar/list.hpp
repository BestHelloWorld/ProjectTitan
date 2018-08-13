#ifndef __LIST_H
#define __LIST_H

#include "define.h"

class List
{
public:
	List * mNext;

	List():mNext(0)
	{
	}

	template <typename T>
	T * Next()
	{
		if (mNext != 0)
		{
			return (T*)mNext;
		}
		return 0;
	}

	virtual void Push(List * list)
	{
		List * lastList = this;
		while (lastList->Next<List>())
		{
			lastList = lastList->Next<List>();
		}
		lastList->mNext = list;
	}

	virtual void Print()
	{
		List * lastList = this;
		while (lastList->Next<List>())
		{
			LOG_D("Node -> %X", lastList);
			lastList = lastList->Next<List>();
		}
	}
};

class RenderableObject : public List
{
public:
	virtual void Draw()
	{
		if (mNext)
		{
			Next<RenderableObject>()->Draw();
		}
	}

	virtual void Update(FLOAT t)
	{
		if (mNext)
		{
			Next<RenderableObject>()->Update(t);
		}
	}
};

#endif