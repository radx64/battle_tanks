#include "engine/KeyboardReceiver.hpp"
#include "engine/KeyboardHandler.hpp"

namespace engine
{
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

}  // namespace engine
