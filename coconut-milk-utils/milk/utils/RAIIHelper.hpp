#ifndef _COCONUT_MILK_UTILS_RAIIHELPER_HPP_
#define _COCONUT_MILK_UTILS_RAIIHELPER_HPP_

#include <functional>

namespace coconut {
namespace milk {
namespace utils {

class RAIIHelper {
public:

	typedef std::function<void ()> Callback;

	RAIIHelper(Callback callback) :
		callback_(callback)
	{
	}

	~RAIIHelper() {
		if (callback_) {
			callback_();
		}
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
