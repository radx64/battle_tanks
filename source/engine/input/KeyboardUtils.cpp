#include "engine/input/KeyboardUtils.hpp"

namespace engine::input
{

std::string_view keyToString(sf::Keyboard::Key key)
{
    #define KEY_LUT_ITEM(x) case sf::Keyboard::x : return #x;

    switch (key)
    {
        KEY_LUT_ITEM(Unknown);
        KEY_LUT_ITEM(A);
        KEY_LUT_ITEM(B);
        KEY_LUT_ITEM(C);
        KEY_LUT_ITEM(D);
        KEY_LUT_ITEM(E);
        KEY_LUT_ITEM(F);
        KEY_LUT_ITEM(G);
        KEY_LUT_ITEM(H);
        KEY_LUT_ITEM(I);
        KEY_LUT_ITEM(J);
        KEY_LUT_ITEM(K);
        KEY_LUT_ITEM(L);
        KEY_LUT_ITEM(M);
        KEY_LUT_ITEM(N);
        KEY_LUT_ITEM(O);
        KEY_LUT_ITEM(P);
        KEY_LUT_ITEM(Q);
        KEY_LUT_ITEM(R);
        KEY_LUT_ITEM(S);
        KEY_LUT_ITEM(T);
        KEY_LUT_ITEM(U);
        KEY_LUT_ITEM(V);
        KEY_LUT_ITEM(W);
        KEY_LUT_ITEM(X);
        KEY_LUT_ITEM(Y);
        KEY_LUT_ITEM(Z);
        KEY_LUT_ITEM(Num0);
        KEY_LUT_ITEM(Num1);
        KEY_LUT_ITEM(Num2);
        KEY_LUT_ITEM(Num3);
        KEY_LUT_ITEM(Num4);
        KEY_LUT_ITEM(Num5);
        KEY_LUT_ITEM(Num6);
        KEY_LUT_ITEM(Num7);
        KEY_LUT_ITEM(Num8);
        KEY_LUT_ITEM(Num9);
        KEY_LUT_ITEM(Escape);
        KEY_LUT_ITEM(LControl);
        KEY_LUT_ITEM(LShift);
        KEY_LUT_ITEM(LAlt);
        KEY_LUT_ITEM(LSystem);
        KEY_LUT_ITEM(RControl);
        KEY_LUT_ITEM(RShift);
        KEY_LUT_ITEM(RAlt);
        KEY_LUT_ITEM(RSystem);
        KEY_LUT_ITEM(Menu);
        KEY_LUT_ITEM(LBracket);
        KEY_LUT_ITEM(RBracket);
        KEY_LUT_ITEM(SemiColon);
        KEY_LUT_ITEM(Comma);
        KEY_LUT_ITEM(Period);
        KEY_LUT_ITEM(Quote);
        KEY_LUT_ITEM(Slash);
        KEY_LUT_ITEM(BackSlash);
        KEY_LUT_ITEM(Tilde);
        KEY_LUT_ITEM(Equal);
        KEY_LUT_ITEM(Dash);
        KEY_LUT_ITEM(Space);
        KEY_LUT_ITEM(Return);
        KEY_LUT_ITEM(BackSpace);
        KEY_LUT_ITEM(Tab);
        KEY_LUT_ITEM(PageUp);
        KEY_LUT_ITEM(PageDown);
        KEY_LUT_ITEM(End);
        KEY_LUT_ITEM(Home);
        KEY_LUT_ITEM(Insert);
        KEY_LUT_ITEM(Delete);
        KEY_LUT_ITEM(Add);
        KEY_LUT_ITEM(Subtract);
        KEY_LUT_ITEM(Multiply);
        KEY_LUT_ITEM(Divide);
        KEY_LUT_ITEM(Left);
        KEY_LUT_ITEM(Right);
        KEY_LUT_ITEM(Up);
        KEY_LUT_ITEM(Down);
        KEY_LUT_ITEM(Numpad0);
        KEY_LUT_ITEM(Numpad1);
        KEY_LUT_ITEM(Numpad2);
        KEY_LUT_ITEM(Numpad3);
        KEY_LUT_ITEM(Numpad4);
        KEY_LUT_ITEM(Numpad5);
        KEY_LUT_ITEM(Numpad6);
        KEY_LUT_ITEM(Numpad7);
        KEY_LUT_ITEM(Numpad8);
        KEY_LUT_ITEM(Numpad9);
        KEY_LUT_ITEM(F1);
        KEY_LUT_ITEM(F2);
        KEY_LUT_ITEM(F3);
        KEY_LUT_ITEM(F4);
        KEY_LUT_ITEM(F5);
        KEY_LUT_ITEM(F6);
        KEY_LUT_ITEM(F7);
        KEY_LUT_ITEM(F8);
        KEY_LUT_ITEM(F9);
        KEY_LUT_ITEM(F10);
        KEY_LUT_ITEM(F11);
        KEY_LUT_ITEM(F12);
        KEY_LUT_ITEM(F13);
        KEY_LUT_ITEM(F14);
        KEY_LUT_ITEM(F15);
        KEY_LUT_ITEM(Pause);
        default : return "Unknown";
    }

    #undef KEY_LUT_ITEM
}

}  // namespace engine::input
