#ifndef _COCONUT_PULP_RENDERER_SHADER_CALLBACKUPDATEABLEPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CALLBACKUPDATEABLEPARAMETER_HPP_

#include <functional>

#include "UpdateableParameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class SourceType, class DataType>
class CallbackUpdateableParameter : public UpdateableParameter<SourceType> {
public:
	
	typedef std::function<void (const SourceType&, DataType&)> Callback;

	CallbackUpdateableParameter(
		milk::graphics::Device& graphicsDevice,
		Callback callback
		) :
		UpdateableParameter(graphicsDevice, sizeof(DataType)),
		callback_(callback)
	{
	}

	void update(
		milk::graphics::Device& graphicsDevice,
		const SourceType& data
		) override {
		milk::graphics::Buffer::LockedData bufferData =
			buffer().lock(graphicsDevice, milk::graphics::Buffer::LockPurpose::WRITE_DISCARD);
		callback_(data, *reinterpret_cast<DataType*>(bufferData.data));
	}

private:

	Callback callback_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_CALLBACKUPDATEABLEPARAMETER_HPP_ */
