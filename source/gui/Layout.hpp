#pragma once

#include "gui/Component.hpp"

#include <optional>

namespace gui
{

class Layout : public Component
{
public:
    void onRender(sf::RenderTexture& renderWindow) override;
};

class FillLayout : public Layout
{
public:
    void addChild(std::unique_ptr<Component> child) override;

protected:
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
    void onSizeChange() override;
};

class DimensionRatioScaler
{
public:
    /* TODO:
    - move this to separate file
    - write some UTs
    - implement corner cases handling and error logging
    - consider defining some percentage suffix with operator""_percent"
    */

    DimensionRatioScaler() = default;

    void setSize(const float size)
    {
        size_ = size;
    }
    
    void setElementCount(const size_t count)
    {
        elements_.resize(count);
    }

    void setElementSize(const size_t index, const float ratio)
    {
        if (index >= elements_.size())
        {
            //loger error
            return;
        }
        elements_[index] = ratio;
    }

    void addElementAtIndex(const size_t index)
    {
        if (index > elements_.size())
        {
            //loger error
            return;
        }
        elements_.insert(std::begin(elements_) + index, std::nullopt);
    }

    void removeElementAtIndex(const size_t index)
    {
        if (index >= elements_.size())
        {
            //loger error
            return;
        }
        elements_.erase(std::begin(elements_) + index);
    }

    float getElementSize(const size_t index) const
    {
        if (index >= elements_.size())
        {
            //loger error
            return 0.f;
        }

        auto elementSize = elements_[index];

        if (elementSize.has_value())
        {
            return elementSize.value() * size_;
        }
        else
        {
            //return average size of empty elements
            auto emptyElements = std::count(std::cbegin(elements_), std::cend(elements_), std::nullopt);
            if (emptyElements == 0)
            {
                // log error or handle case where there are no empty elements so element might not be visible
                return 0.f;
            }

            float usedRatioSum = 0.f;
            for (const auto& element : elements_)
            {
                if (element.has_value())
                {
                    usedRatioSum += element.value();
                }
            }

            float remainingSize = size_ * (1.f - usedRatioSum);
            return remainingSize / emptyElements;
            return size_ / emptyElements;
        }
    }

protected:
    std::vector<std::optional<float>> elements_;
    float size_;
};

class GridLayout : public Layout
{
/*  TODO:
    - Add support for defining grid size in ratio
*/

public:
    GridLayout(size_t width, size_t height);

    void addChild(std::unique_ptr<Component> child) override;
    bool addColumn(const size_t position);
    bool removeColumn(const size_t position);
    void setColumnSize(const size_t column, const float ratio);
    bool addRow(const size_t position);
    bool removeRow(const size_t position);
    void setRowSize(const size_t position, const float ratio);
    size_t getWidth() const;
    size_t getHeight() const;

protected:
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
    void onSizeChange() override;

    void recalculateChildrenBounds();

    size_t width_;
    size_t height_;

    DimensionRatioScaler columnSize_;
    DimensionRatioScaler rowSize_;

    std::vector<std::vector<Component*>> grid_;
};

class BaseLineLayout : public Layout
{
public:
    void addChild(std::unique_ptr<Component> child) override;

protected:
    virtual sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const = 0;
    virtual sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const = 0;
    void recalculateChildrenBounds();
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
};

class HorizontalLayout : public BaseLineLayout
{

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const override;
    sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const override;
};

class VerticalLayout : public BaseLineLayout
{

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const override;
    sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const override;
};

}  // namespace gui
