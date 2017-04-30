#ifndef _COCONUT_SHELL_GLOBALS_HPP_
#define _COCONUT_SHELL_GLOBALS_HPP_

namespace coconut {
namespace shell {

#if defined(NDEBUG)
const bool DEBUG = false;
#else
const bool DEBUG = true;
#endif /* DEBUG */

} // namespace shell
} // namespace coconut

#endif /* _COCONUT_SHELL_GLOBALS_HPP_ */
