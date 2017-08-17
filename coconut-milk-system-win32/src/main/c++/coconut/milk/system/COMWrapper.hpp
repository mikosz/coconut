#ifndef _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_
#define _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_

#include <cassert>
#include <type_traits>

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

	template <class CompatibleT>
	COMWrapper(const COMWrapper<CompatibleT>& other) :
		comObject_(other.get())
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	COMWrapper(const COMWrapper& other) :
		comObject_(other.comObject_)
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	COMWrapper(COMWrapper&& other) :
		COMWrapper()
	{
		swap(other);
	}

	~COMWrapper() {
		reset();
	}

	COMWrapper& operator=(COMWrapper other) {
		swap(other);
		return *this;
	}

	T* operator->() const {
		assert(comObject_ != nullptr);
		return comObject_;
	}

	T& operator*() const {
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

	void swap(COMWrapper& other) {
		std::swap(comObject_, other.comObject_);
	}

	T*& get() { // TODO: WTF? Texture2D::initialise crashes when doing &get()
		return comObject_;
	}

	T* get() const {
		return comObject_;
	}

private:

	T* comObject_;

};

template <class T>
void swap(COMWrapper<T> lhs, COMWrapper<T> rhs) {
	lhs.swap(rhs);
}

} // namespace system
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_ */
