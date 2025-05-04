#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/layout/Inset.hpp"

#include "tests/mocks/ComponentSpy.hpp"

namespace gui::layout
{

auto makeSpy()
{
    auto spy = std::make_unique<mocks::ComponentSpy>();
    EXPECT_CALL(*spy, die()).Times(1);
    return spy;
}

TEST(InsetShould, offsetChildComponentFromItsParentByPixels)
{
    auto sut = Inset::create(SizeConstraint::Pixels(10));
    auto child = makeSpy();

    auto child_ptr = child.get();

    sut->addChild(std::move(child));

    sut->setSize(sf::Vector2f(100.0f, 100.0f));
    
    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(90.0f, 90.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(5.0f, 5.0f));

    sut->set(SizeConstraint::Pixels(20));

    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(80.0f, 80.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(10.0f, 10.0f));
}

TEST(InsetShould, offsetChildComponentFromItsParentByPercentage)
{
    auto sut = Inset::create(SizeConstraint::Percent(10));
    auto child = makeSpy();

    auto child_ptr = child.get();

    sut->addChild(std::move(child));

    sut->setSize(sf::Vector2f(100.0f, 100.0f));
    
    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(10.0f, 10.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(45.0f, 45.0f));

    sut->set(SizeConstraint::Percent(20));

    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(20.0f, 20.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(40.0f, 40.0f));
}

TEST(InsetShould, doNotoffsetChildComponentFromItsParentWhenAuto)
{
    auto sut = Inset::create(SizeConstraint::Auto());
    auto child = makeSpy();

    auto child_ptr = child.get();

    sut->addChild(std::move(child));

    sut->setSize(sf::Vector2f(100.0f, 100.0f));
    
    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(100.0f, 100.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(0.0f, 0.0f));

    sut->set(SizeConstraint::Auto());

    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(100.0f, 100.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(0.0f, 0.0f));
}

TEST(InsetShould, setChildComponentSizeToParentWhenInsetIsZeroPixels)
{
    auto sut = Inset::create(SizeConstraint::Pixels(0));
    auto child = makeSpy();

    auto child_ptr = child.get();

    sut->addChild(std::move(child));

    sut->setSize(sf::Vector2f(100.0f, 100.0f));
    
    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(100.0f, 100.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(0.0f, 0.0f));

    sut->set(SizeConstraint::Pixels(0));

    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(100.0f, 100.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(0.0f, 0.0f));
}

TEST(InsetShould, setChildComponentSizeToZeroWhenInsetIsZeroPercent)
{
    auto sut = Inset::create(SizeConstraint::Percent(0));
    auto child = makeSpy();

    auto child_ptr = child.get();

    sut->addChild(std::move(child));

    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(0.0f, 0.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(0.0f, 0.0f));

    sut->set(SizeConstraint::Percent(0));

    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(0.0f, 0.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(0.0f, 0.0f)); 
}

TEST(InsetShould, setChildComponentSizeProperlyWhenInsetIsNegativeInPixels)
{
    auto sut = Inset::create(SizeConstraint::Pixels(-10));
    auto child = makeSpy();

    auto child_ptr = child.get();

    sut->addChild(std::move(child));

    sut->setSize(sf::Vector2f(100.0f, 100.0f));
    
    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(110.0f, 110.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(-5.0f, -5.0f));

    sut->set(SizeConstraint::Pixels(-20));

    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(120.0f, 120.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(-10.0f, -10.0f));
}

TEST(InsetShould, setChildComponentSizeToZeroWhenInsetIsNegativeInPercent)
{
    auto sut = Inset::create(SizeConstraint::Percent(-10));
    auto child = makeSpy();

    auto child_ptr = child.get();

    sut->addChild(std::move(child));

    sut->setSize(sf::Vector2f(100.0f, 100.0f));
    
    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(0.0f, 0.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(0.0f, 0.0f));

    sut->set(SizeConstraint::Percent(-20));

    EXPECT_EQ(child_ptr->getSize(), sf::Vector2f(0.0f, 0.0f));
    EXPECT_EQ(child_ptr->getPosition(), sf::Vector2f(0.0f, 0.0f));
}


}  // namespace gui::layout
