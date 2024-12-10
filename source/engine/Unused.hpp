#pragma once

// TODO move it to some utils
#define UNUSED(x) static_cast<void>(x)

// TODO think about recursive variadic macro expansion in the future :)
#define UNUSED2(x, y) static_cast<void>(x); static_cast<void>(y)
#define UNUSED3(x, y, z) UNUSED2(x,y); static_cast<void>(z)
