#include "gui/keyboard/Utils.hpp"

namespace gui::keyboard
{

std::string_view keyToString(sf::Keyboard::Key key)
{
    #define KEY_LUT_ITEM(x) case sf::Keyboard::x : return #x;
    #define KEY_LUT_ITEM_CUSTOM(x,y)  case sf::Keyboard::x : return y;
 
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
        KEY_LUT_ITEM_CUSTOM(Num0, "0");
        KEY_LUT_ITEM_CUSTOM(Num1, "1");
        KEY_LUT_ITEM_CUSTOM(Num2, "2");
        KEY_LUT_ITEM_CUSTOM(Num3, "3");
        KEY_LUT_ITEM_CUSTOM(Num4, "4");
        KEY_LUT_ITEM_CUSTOM(Num5, "5");
        KEY_LUT_ITEM_CUSTOM(Num6, "6");
        KEY_LUT_ITEM_CUSTOM(Num7, "7");
        KEY_LUT_ITEM_CUSTOM(Num8, "8");
        KEY_LUT_ITEM_CUSTOM(Num9, "9");
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
        KEY_LUT_ITEM_CUSTOM(SemiColon, ":");
        KEY_LUT_ITEM_CUSTOM(Comma, ",");
        KEY_LUT_ITEM_CUSTOM(Period, ".");
        KEY_LUT_ITEM_CUSTOM(Quote, "\"");
        KEY_LUT_ITEM_CUSTOM(Slash, "/");
        KEY_LUT_ITEM_CUSTOM(BackSlash , "\\");
        KEY_LUT_ITEM_CUSTOM(Tilde, "`");
        KEY_LUT_ITEM(Equal);
        KEY_LUT_ITEM(Dash);
        KEY_LUT_ITEM_CUSTOM(Space, " ");
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

}  // namespace gui::keyboard
