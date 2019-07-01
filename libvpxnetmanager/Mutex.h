#ifndef Foundation_Mutex_INCLUDED
#define Foundation_Mutex_INCLUDED

#include <pthread.h>


class MutexEx
	/// A Mutex (mutual exclusion) is a synchronization 
	/// mechanism used to control access to a shared resource
	/// in a concurrent (multithreaded) scenario.
	/// Mutexes are recursive, that is, the same mutex can be 
	/// locked multiple times by the same thread (but, of course,
	/// not by other threads).
	/// Using the ScopedLock class is the preferred way to automatically
	/// lock and unlock a mutex.
{
public:
	
		/// creates the Mutex.
	MutexEx()
	{
		pthread_mutex_init(&m_mutex, NULL);
	}
		
		/// destroys the Mutex.
	~MutexEx()
	{
		pthread_mutex_destroy(&m_mutex);
	}

		/// Locks the mutex. Blocks if the mutex
		/// is held by another thread.
	void lock()
	{
		pthread_mutex_lock(&m_mutex);
	}
		

		/// Unlocks the mutex so that it can be acquired by
		/// other threads.
	void unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}
	
private:
	pthread_mutex_t m_mutex;
};

class Lock {
	public:
		Lock(MutexEx& m) : mutex(m) {m.lock();}
		~Lock() {mutex.unlock();}
	protected:
		MutexEx& mutex;
};

#endif // Foundation_Mutex_INCLUDED
