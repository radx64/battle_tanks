#include "gui/layout/Inset.hpp"

namespace gui::layout
{

std::unique_ptr<Inset> Inset::create(const SizeConstraint& constraint)
{
    return std::unique_ptr<Inset>{new Inset{constraint}};
}

void Inset::set(const SizeConstraint& constraint)
{
    constraint_ = constraint;
}

Inset::Inset(const SizeConstraint& constraint)
: constraint_(constraint)
{
}

void Inset::onSizeChange()
{
    auto size = getSize();
    sf::Vector2f positionOffset{0.0f, 0.0f};

    if (constraint_.getType() == SizeConstraint::Type::Fixed)
    {
        if (constraint_.getUnit() == SizeConstraint::Unit::Pixels)
        {
            size.x -= constraint_.getValue();
            size.y -= constraint_.getValue();

            positionOffset.x = constraint_.getValue() / 2.0f;
            positionOffset.y = constraint_.getValue() / 2.0f;
        }
        else if (constraint_.getUnit() == SizeConstraint::Unit::Percentage)
        {
            auto layoutSize = size;

            size.x *= constraint_.getValue() / 100.0f;
            size.y *= constraint_.getValue() / 100.0f;

            positionOffset.x = (layoutSize.x - size.x) / 2.0f;
            positionOffset.y = (layoutSize.y - size.y) / 2.0f;
            
        }
    }
    
    size.x = std::max(0.0f, size.x);
    size.y = std::max(0.0f, size.y);

    positionOffset.x = std::max(0.0f, positionOffset.x);
    positionOffset.y = std::max(0.0f, positionOffset.y);
    
    for (auto& child : children_)
    {
        child->setSize(size);
        child->setPosition(positionOffset);
    }
}

}  // namespace gui::layout
