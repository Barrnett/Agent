#ifndef _DLIB__H
#define _DLIB__H


struct dlib
{
public:
	void* sym(const char* name) const {
		if (!m_handle)
			return NULL;
		return dlsym(m_handle, name);
	}

	dlib()	{
		m_handle = NULL;
	}
	~dlib()	{
		if(m_handle)
			dlclose(m_handle);
	}
	dlib(const char* name, int flag) {
		m_handle = dlopen(name, flag);
	}
	explicit operator bool() const { return (bool)m_handle; }
private:
	void*	m_handle;
};

#endif
