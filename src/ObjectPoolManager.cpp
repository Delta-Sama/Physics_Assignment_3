#include "ObjectPoolManager.h"
#include <cassert>

template <class T> PoolObject<T>* ObjectPool<T>::pool;
template <class T> int ObjectPool<T>::m_size;

template <class T>
void ObjectPool<T>::Init(int size)
{
	assert(size > 0);
	
	pool = new PoolObject<T>[size];
	m_size = size;

	for (int i = 0; i < m_size; i++)
	{
		pool[i].object = new T();
		
	}
}

template <class T>
void ObjectPool<T>::Update()
{
	for (int i = 0; i < m_size; i++)
	{
		pool[i].object->update();
	}
}

template <class T>
void ObjectPool<T>::Clean()
{
	for (int i = 0; i < m_size; i++)
	{
		delete pool[i].object;
		pool[i].object = nullptr;
	}
}