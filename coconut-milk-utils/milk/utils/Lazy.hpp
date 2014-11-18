#ifndef _COCONUT_MILK_UTILS_LAZY_HPP_
#define _COCONUT_MILK_UTILS_LAZY_HPP_

#include <functional>

namespace coconut {
namespace milk {
namespace utils {

template <class T>
class Lazy {
public:

	typedef std::function<void (T&)> EvaluateCallback;

	Lazy(EvaluateCallback evaluateCallback) :
		valid_(false),
		evaluateCallback_(evaluateCallback)
	{
	}

	void invalidate() {
		valid_ = false;
	}

	const T& get() const {
		if (!valid_) {
			evaluateCallback_(value_);
			valid_ = true;
		}
		return value_;
	}

	T& get() {
		if (!valid_) {
			evaluateCallback_(value_);
			valid_ = true;
		}
		return value_;
	}

private:

	mutable bool valid_;

	EvaluateCallback evaluateCallback_;

	mutable T value_;

};

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_LAZY_HPP_ */
