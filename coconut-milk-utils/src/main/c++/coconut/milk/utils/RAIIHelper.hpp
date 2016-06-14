#ifndef _COCONUT_MILK_UTILS_RAIIHELPER_HPP_
#define _COCONUT_MILK_UTILS_RAIIHELPER_HPP_

#include <functional>

namespace coconut {
namespace milk {
namespace utils {

// TODO: probably useless in c++11 with unique_ptr deleters
class RAIIHelper {
public:

	typedef std::function<void ()> TextureCallback;

	RAIIHelper() {
	}

	RAIIHelper(TextureCallback callback) :
		textureCallback_(callback)
	{
	}

	// TODO: probably the way copying is done should be handled by a template parameter
	RAIIHelper(RAIIHelper& other) {
		textureCallback_.swap(other.textureCallback_);
	}

	~RAIIHelper() {
		if (textureCallback_) {
			textureCallback_();
		}
	}

	// TODO: probably the way copying is done should be handled by a template parameter
	RAIIHelper& operator=(RAIIHelper& other) {
		if (this != &other) {
			textureCallback_.swap(other.textureCallback_);
		}
		return *this;
	}

	void reset() {
		textureCallback_ = TextureCallback();
	}

private:

	TextureCallback textureCallback_;

};

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_RAIIHELPER_HPP_ */
