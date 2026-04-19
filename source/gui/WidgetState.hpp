#pragma once
namespace gui
{

struct WidgetState
{
    bool hovered  = false;
    bool pressed  = false;
    // TODO consider introducting focused into WidgetState 
    // Also WidgetState could be part of a Widget class
    // bool focused  = false; 
    bool disabled = false;
    // TODO: add disabled in the future
};

}  // namespace gui
