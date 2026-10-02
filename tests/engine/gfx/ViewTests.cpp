#include <gtest/gtest.h>

#include "engine/gfx/View.hpp"

namespace engine::gfx
{

TEST(ViewTests, shouldStoreCenterAndSize)
{
    const View view{engine::Vec2f{100.f, 50.f}, engine::Vec2f{800.f, 600.f}};

    EXPECT_FLOAT_EQ(view.getCenter().x, 100.f);
    EXPECT_FLOAT_EQ(view.getCenter().y, 50.f);
    EXPECT_FLOAT_EQ(view.getSize().x, 800.f);
    EXPECT_FLOAT_EQ(view.getSize().y, 600.f);
}

TEST(ViewTests, shouldZoomByScalingSizeAndLeaveCenterUntouched)
{
    View view{engine::Vec2f{10.f, 20.f}, engine::Vec2f{200.f, 100.f}};

    view.zoom(2.f);

    EXPECT_FLOAT_EQ(view.getSize().x, 400.f);
    EXPECT_FLOAT_EQ(view.getSize().y, 200.f);
    EXPECT_FLOAT_EQ(view.getCenter().x, 10.f);
    EXPECT_FLOAT_EQ(view.getCenter().y, 20.f);

    view.zoom(0.5f);

    EXPECT_FLOAT_EQ(view.getSize().x, 200.f);
    EXPECT_FLOAT_EQ(view.getSize().y, 100.f);
}

TEST(ViewTests, shouldMoveByAddingOffsetToCenter)
{
    View view{engine::Vec2f{10.f, 20.f}, engine::Vec2f{200.f, 100.f}};

    view.move({5.f, -8.f});
    view.move({1.f, 2.f});

    EXPECT_FLOAT_EQ(view.getCenter().x, 16.f);
    EXPECT_FLOAT_EQ(view.getCenter().y, 14.f);
    EXPECT_FLOAT_EQ(view.getSize().x, 200.f);
    EXPECT_FLOAT_EQ(view.getSize().y, 100.f);
}

}  // namespace engine::gfx
