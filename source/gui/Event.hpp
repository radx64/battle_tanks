#ifndef GUI_EVENT_HPP_
#define GUI_EVENT_HPP_

// I know sfml have own event subsystem but I'm redefining it 
// to separate from SFML later on

namespace gui::event
{

    struct MousePosition
    {
        float x;
        float y;
    };

    enum class MouseButton
    {   
        Left,
        Right,
        Middle,
        XButton1,
        XButton2,
    };

    struct MouseMoved
    {
        MousePosition position;
    };

    struct MouseButtonPressed
    {
        MouseButton button;
        MousePosition position;
    };

    struct MouseButtonReleased
    {
        MouseButton button;
        MousePosition position;
    };

}  // namespace gui::event

#endif  // GUI_EVENT_HPP_