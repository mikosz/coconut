#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class... UpdateArguments>
class Parameter {
public:

	Parameter(size_t size) :
		size_(size)
	{
	}

	Parameter(const Parameter&) = delete;

	virtual ~Parameter() = default;

	void operator=(const Parameter&) = delete;

	virtual void update(void* buffer, const UpdateArguments&... data) = 0;

	size_t size() const {
		return size_;
	}

private:

	size_t size_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_ */
