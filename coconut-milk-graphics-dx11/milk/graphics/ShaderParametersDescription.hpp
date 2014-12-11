#ifndef _COCONUT_MILK_GRAPHICS_SHADERPARAMETERSDESCRIPTION_HPP_
#define _COCONUT_MILK_GRAPHICS_SHADERPARAMETERSDESCRIPTION_HPP_

#include <functional>
#include <vector>
#include <memory>

#include <d3d11.h>

#include "Buffer.hpp"
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
		
		ID3D11Buffer* buffer() {
			return buffer_.resource();
		}

		void update(Device& device);

	private:

		Buffer buffer_;

		virtual void doUpdate(void* buffer) = 0;

	};

	class MatrixElement : public Element {
	public:

		typedef std::function<const milk::math::Matrix& ()> RetrievalCallback;

		MatrixElement(Device& device, RetrievalCallback retrievalCallback) :
			Element(device, sizeof(math::Matrix)),
			retrievalCallback_(retrievalCallback)
		{
		}

	private:

		 RetrievalCallback retrievalCallback_;

		 void doUpdate(void* buffer);

	};

	ShaderParametersDescription(ShaderType shaderType) :
		shaderType_(shaderType)
	{
	}

	void update(Device& device);

	void bind(Device& device);

	void push(std::shared_ptr<Element> element);

private:

	ShaderType shaderType_;

	std::vector<std::shared_ptr<Element> > elements_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_SHADERPARAMETERSDESCRIPTION_HPP_ */
