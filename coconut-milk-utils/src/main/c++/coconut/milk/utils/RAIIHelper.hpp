#ifndef _COCONUT_MILK_UTILS_RAIIHELPER_HPP_
#define _COCONUT_MILK_UTILS_RAIIHELPER_HPP_

#include <functional>

namespace coconut {
namespace milk {
namespace utils {

// TODO: probably useless in c++11 with unique_ptr deleters
class RAIIHelper {
public:

	using Callback = std::function<void ()>;

	RAIIHelper() {
	}

	RAIIHelper(Callback callback) :
		callback_(callback)
	{
	}

	// TODO: probably the way copying is done should be handled by a template parameter
	RAIIHelper(RAIIHelper& other) {
		callback_.swap(other.callback_);
	}

	~RAIIHelper() {
		if (callback_) {
			callback_();
		}
	}

	// TODO: probably the way copying is done should be handled by a template parameter
	RAIIHelper& operator=(RAIIHelper& other) {
		if (this != &other) {
			callback_.swap(other.callback_);
		}
		return *this;
	}

	void reset() {
		callback_ = Callback();
	}

private:

	Callback callback_;

};

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_RAIIHELPER_HPP_ */
