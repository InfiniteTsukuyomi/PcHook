#pragma once

#define INIT_SINGLETON(T)		T* Singleton<T>::m_instance = 0; std::mutex Singleton<T>::m_single_mutex;

template<class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (!m_instance)
		{
			std::lock_guard<std::mutex> lock(m_single_mutex);
			if (!m_instance)
			{
				m_instance = new T;
			}			
		}
		return m_instance;
	}

private:
	static T* m_instance;
	static std::mutex m_single_mutex;
};
