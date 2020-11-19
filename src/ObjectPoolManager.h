#pragma once
#ifndef __OBJECTPOOL_H__
#define __OBJECTPOOL_H__

template <class T>
struct PoolObject
{
	T* object;
	bool inUse;
};

template <class T>
class ObjectPool
{
public:
	static void Init(int size);
	static void Update();
	static void Clean();
	
private:
	ObjectPool() {}
	
	static PoolObject<T>* pool;
	static int m_size;
};

template<typename T>
using Pool = ObjectPool<T>;

#endif
