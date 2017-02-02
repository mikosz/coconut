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
		comObject_(nullptr)
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

	COMWrapper(COMWrapper&& other) :
		comObject_(other.comObject_)
	{
		other.comObject_ = nullptr;
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

	COMWrapper& operator =(COMWrapper&& other) {
		if (&other != this) {
			reset();
			comObject_ = other.comObject_;
			other.comObject_ = nullptr;
		}

		return *this;
	}

	T* operator ->() const {
		assert(comObject_ != nullptr);
		return comObject_;
	}

	T& operator *() const {
		assert(comObject_ != nullptr);
		return *comObject_;
	}

	operator T*() const {
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

	T* get() const {
		return comObject_;
	}

private:

	T* comObject_;

};

} // namespace system
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_ */
