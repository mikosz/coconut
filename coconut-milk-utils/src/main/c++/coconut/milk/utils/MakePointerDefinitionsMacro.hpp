#ifndef _COCONUT_MILK_UTILS_MAKEPOINTERDEFINITIONSMACRO_HPP_
#define _COCONUT_MILK_UTILS_MAKEPOINTERDEFINITIONSMACRO_HPP_

#include <memory>

#define CCN_MAKE_POINTER_DEFINITIONS(className) \
	using className##UniquePtr = std::unique_ptr<className>; \
	using Const##className##UniquePtr = std::unique_ptr<const className>; \
	using className##SharedPtr = std::shared_ptr<className>; \
	using Const##className##SharedPtr = std::shared_ptr<const className>;

#endif /* _COCONUT_MILK_UTILS_MAKEPOINTERDEFINITIONSMACRO_HPP_ */
