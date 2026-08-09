#include "engine/input/KeyboardReceiver.hpp"
#include "engine/input/KeyboardHandler.hpp"

namespace engine::input
{

KeyboardReceiver::KeyboardReceiver()
: handler_{nullptr}
{}

KeyboardReceiver::~KeyboardReceiver()
{
    if (handler_ != nullptr)
    {
        handler_->unsubscribe(this);
    }
};

void KeyboardReceiver::attach(KeyboardHandler* handler)
{
    handler_ = handler;
}

}  // namespace engine::input
