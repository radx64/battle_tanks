#include "engine/input/MouseReceiver.hpp"
#include "engine/input/MouseHandler.hpp"

namespace engine::input
{

MouseReceiver::MouseReceiver()
: handler_{nullptr}
{}

MouseReceiver::~MouseReceiver()
{
    if (handler_ != nullptr)
    {
        handler_->unsubscribe(this);
    }
};

void MouseReceiver::attach(MouseHandler* handler)
{
    handler_ = handler;
}

}  // namespace engine::input
