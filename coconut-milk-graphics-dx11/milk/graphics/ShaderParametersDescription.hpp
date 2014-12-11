#ifndef _COCONUT_MILK_GRAPHICS_SHADERPARAMETERSDESCRIPTION_HPP_
#define _COCONUT_MILK_GRAPHICS_SHADERPARAMETERSDESCRIPTION_HPP_

#include <functional>
#include <vector>
#include <memory>

#include <d3d11.h>

#include "Buffer.hpp"
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

		Element(Device& device, size_t size);

		virtual ~Element() {
		}
		
		ID3D11Buffer* buffer(const Renderable& renderable) {
			return buffer_.resource();
		}

		void update(Device& device, const Renderable& renderable);

	private:

		Buffer buffer_;

		virtual void doUpdate(void* buffer, const Renderable& renderable) = 0;

	};

	class MatrixElement : public Element {
	public:

		typedef std::function<const milk::math::Matrix& (const Renderable&)> RetrievalCallback;

		MatrixElement(Device& device, RetrievalCallback retrievalCallback) :
			Element(device, sizeof(math::Matrix)),
			retrievalCallback_(retrievalCallback)
		{
		}

	private:

		 RetrievalCallback retrievalCallback_;

		 void doUpdate(void* buffer, const Renderable& renderable);

	};

	ShaderParametersDescription(ShaderType shaderType) :
		shaderType_(shaderType)
	{
	}

	void update(Device& device, const Renderable& renderable);

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
