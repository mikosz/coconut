#ifndef _COCONUT_MILK_UTILS_MAKEPOINTERDEFINITIONSMACRO_HPP_
#define _COCONUT_MILK_UTILS_MAKEPOINTERDEFINITIONSMACRO_HPP_

#include <memory>

#define MAKE_POINTER_DEFINITIONS(className) \
	typedef std::unique_ptr<className> className##UniquePtr; \
	typedef std::unique_ptr<const className> Const##className##UniquePtr; \
	typedef std::shared_ptr<className> className##SharedPtr; \
	typedef std::shared_ptr<const className> Const##className##SharedPtr;

#endif /* _COCONUT_MILK_UTILS_MAKEPOINTERDEFINITIONSMACRO_HPP_ */
