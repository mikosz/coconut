#ifndef _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_
#define _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_

#include <cassert>

namespace coconut {
namespace milk {
namespace system {

template <class T>
class COMWrapper {
public:

	COMWrapper() :
		comObject_(0)
	{
	}

	COMWrapper(T* comObject) :
		comObject_(comObject)
	{
	}

	COMWrapper(const COMWrapper& other) :
		comObject_(other.comObject_)
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	~COMWrapper() {
		reset();
	}

	COMWrapper& operator =(const COMWrapper& other) {
		if (&other != this) {
			reset();
			comObject_ = other.comObject_;
			if (comObject_) {
				comObject_->AddRef();
			}
		}

		return *this;
	}

	T* operator ->() {
		assert(comObject_);
		return comObject_;
	}

	const T* operator ->() const {
		assert(comObject_);
		return comObject_;
	}

	T*& operator *() {
		return comObject_;
	}

	const T*& operator *() const {
		return comObject_;
	}

	operator T*() {
		return get();
	}

	operator const T*() const {
		return get();
	}

	void reset() {
		if (comObject_) {
			comObject_->Release();
			comObject_ = 0;
		}
	}

	void reset(T* comObject) {
		reset();
		comObject_ = comObject;
	}

	T*& get() {
		return comObject_;
	}

	const T* get() const {
		return comObject_;
	}

private:

	T* comObject_;

};

} // namespace system
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_ */
