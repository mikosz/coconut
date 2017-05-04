#ifndef _COCONUT_MILK_UTILS_SLICEIDENTIFIER_HPP_
#define _COCONUT_MILK_UTILS_SLICEIDENTIFIER_HPP_

#include <string>
#include <locale>

namespace coconut {
namespace milk {
namespace utils {

// TODO: remove after fixing ResourceFactory
template <template<class...> class ResultType>
ResultType<std::string> sliceIdentifier(const std::string& id) {
	ResultType<std::string> result;
	
	std::locale locale;

	std::string next;
	next.reserve(id.length());

	for (auto c : id) {
		if (std::isupper(c, locale) && !next.empty()) {
			result.emplace_back(next);
			next.clear();
		} else if (c == '_' && !next.empty()) {
			result.emplace_back(next);
			next.clear();
			continue;
		}

		next.push_back(std::tolower(c, locale));
	}

	if (!next.empty()) {
		result.emplace_back(next);
	}

	return result;
}

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_SLICEIDENTIFIER_HPP_ */
