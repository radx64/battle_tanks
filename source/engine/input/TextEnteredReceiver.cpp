#include "engine/input/TextEnteredReceiver.hpp"

#include "engine/input/KeyboardHandler.hpp"

namespace engine::input
{

TextEnteredReceiver::TextEnteredReceiver()
: handler_{nullptr}
{

}

TextEnteredReceiver::~TextEnteredReceiver()
{
    if (handler_ != nullptr)
    {
        handler_->unsubscribe(this);
    } 
}

void TextEnteredReceiver::attach(KeyboardHandler* handler)
{
    handler_ = handler;
}

}  // namespace engine::input