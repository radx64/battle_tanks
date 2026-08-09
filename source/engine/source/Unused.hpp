#pragma once

#define UNUSED(x) static_cast<void>(x)
#define UNUSED2(x, y) static_cast<void>(x); static_cast<void>(y)
#define UNUSED3(x, y, z) UNUSED2(x,y); static_cast<void>(z)
