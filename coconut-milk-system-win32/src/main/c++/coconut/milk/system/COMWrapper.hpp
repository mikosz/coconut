#ifndef _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_
#define _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_

#include <cassert>
#include <utility>
#include <type_traits>

namespace coconut {
namespace milk {
namespace system {

template <class T>
class COMWrapper {
public:

	COMWrapper() noexcept :
		comObject_(nullptr)
	{
	}

	COMWrapper(T* comObject) noexcept :
		comObject_(comObject)
	{
	}

	COMWrapper(const COMWrapper& other) noexcept :
		comObject_(other.comObject_)
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	COMWrapper(COMWrapper&& other) noexcept :
		COMWrapper()
	{
		swap(other);
	}

	template <class U, class = std::enable_if_t<std::is_convertible_v<U*, T*>>>
	COMWrapper(const COMWrapper<U>& other) noexcept :
		COMWrapper(other.get())
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	~COMWrapper() noexcept {
		reset();
	}

	explicit operator bool() const noexcept {
		return comObject_ != nullptr;
	}

	void swap(COMWrapper& other) noexcept {
		using std::swap;
		swap(comObject_, other.comObject_);
	}

	COMWrapper& operator=(COMWrapper other) noexcept {
		swap(other);
		return *this;
	}

	T* operator->() const noexcept {
		assert(comObject_ != nullptr);
		return comObject_;
	}

	T& operator*() const noexcept {
		assert(comObject_ != nullptr);
		return *comObject_;
	}

	void reset() noexcept {
		if (comObject_) {
			comObject_->Release();
			comObject_ = 0;
		}
	}

	void reset(T* comObject) noexcept {
		reset();
		comObject_ = comObject;
	}

	T*& get() noexcept { // TODO: WTF? Texture2D::initialise crashes when doing &get()
		return comObject_;
	}

	T* get() const noexcept {
		return comObject_;
	}

private:

	T* comObject_;

};

} // namespace system
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_SYSTEM_COMWRAPPER_HPP_ */
