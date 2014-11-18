#ifndef _COCONUT_MILK_GRAPHICS_SHADERPARAMETERSDESCRIPTION_HPP_
#define _COCONUT_MILK_GRAPHICS_SHADERPARAMETERSDESCRIPTION_HPP_

#include <functional>
#include <vector>
#include <memory>

#include "Renderable.hpp"
#include "milk/math/Matrix.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class ShaderParametersDescription {
public:

	enum ShaderType {
		VERTEX_SHADER,
		PIXEL_SHADER,
	};

	class Element {
	public:

		virtual ~Element() {
		}
		
		ID3D11Buffer* data(const Renderable& renderable) {
			return buffer_;
		}

	protected:

		virtual void update(const Renderable& renderable) = 0;

	private:

		ID3D11Buffer* buffer_;

	};

	class MatrixElement : public Element {
	public:

		typedef std::function<const milk::math::Matrix& (const Renderable&)> RetrievalCallback;

		MatrixElement(RetrievalCallback retrievalCallback) :
			retrievalCallback_(retrievalCallback)
		{
		}

	protected:

		void update(const Renderable& renderable) override;

	private:

		 RetrievalCallback retrievalCallback_;

	};

	ShaderParametersDescription(ShaderType shaderType) :
		shaderType_(shaderType)
	{
	}

	void bind(Device& device, const Renderable& renderable);

	void push(std::shared_ptr<Element> element);

private:

	ShaderType shaderType_;

	std::vector<std::shared_ptr<Element> > elements_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_SHADERPARAMETERSDESCRIPTION_HPP_ */
