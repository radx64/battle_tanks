#pragma once

#include <cstdlib>
#include <cmath>

#include "engine/gui/Button.hpp"
#include "engine/gui/EditBox.hpp"
#include "engine/gui/FontLibrary.hpp"
#include "engine/gui/Label.hpp"
#include "engine/gui/layout/Constraint.hpp"
#include "engine/gui/layout/Grid.hpp"
#include "engine/gui/layout/Horizontal.hpp"
#include "engine/gui/layout/Inset.hpp"
#include "engine/gui/layout/Vertical.hpp"
#include "engine/gui/TextureLibrary.hpp"

namespace calculator
{

class CalculatorWidget : public engine::gui::Widget
{
public:

    enum class Operation
    {
        None,
        Add,
        Subtract,
        Multiply,
        Divide,
    };

    CalculatorWidget()
    {
    }

    void storeValue(const std::string& value)
    {
        acumulator_ = std::atof(value.c_str());
    }

    void storeOperation(const Operation operation)
    {
        operation_ = operation;
    }

    float getResultWith(const std::string& value)
    {
        float result = 0.f;
        float valueFloat = std::atof(value.c_str());

        switch (operation_)
        {
            case Operation::Add:
                result = acumulator_ + valueFloat;
                break;
            case Operation::Subtract:
                result = acumulator_ - valueFloat;
                break;
            case Operation::Multiply:
                result = acumulator_ * valueFloat;
                break;
            case Operation::Divide:
                result = acumulator_ / valueFloat;
                break;
            case Operation::None:
                result = valueFloat;
                break;
        }

        return result;
    }

protected:
    void onRender(sf::RenderTexture&) override
    {
    }

    float acumulator_;
    Operation operation_;
};


void appendTextToEditBox(engine::gui::EditBox* editBox, const std::string& text)
{
    auto currentText = editBox->getText();
    currentText += text;
    editBox->setText(currentText);
}

std::unique_ptr<engine::gui::Widget> createCalculator(const sf::Vector2f& initalSize)
{
    auto verticalLayout = engine::gui::layout::Vertical::create();
    verticalLayout->setSize(initalSize);

    /* EDIT BOX*/
    auto editBox = engine::gui::EditBox::create();
    editBox->setAlignment(engine::gui::Alignment::Right);
    engine::gui::EditBox* editBoxPtr = editBox.get();
    verticalLayout->addChild(std::move(editBox));
    verticalLayout->setRowSize(0, engine::gui::layout::Constraint::Pixels(40));

    auto horizontalLayout = engine::gui::layout::Horizontal::create();
    horizontalLayout->setPadding(10);

    /* TOP BUTTONS */
    auto calculatorWidget = std::make_unique<CalculatorWidget>();
    CalculatorWidget* calculatorWidgetPtr = calculatorWidget.get(); 

    auto backspaceButton = engine::gui::TextButton::create("Backspace");
    backspaceButton->onClick([editBoxPtr](){
        auto currentText = editBoxPtr->getText();
        if (currentText.size() > 0)
        {
            currentText.erase(currentText.size() - 1);
            editBoxPtr->setText(currentText);
        }
    });
    auto CEButton = engine::gui::TextButton::create("CE");
    CEButton->onClick([editBoxPtr](){editBoxPtr->setText("");});

    auto CButton = engine::gui::TextButton::create("C");
    CButton->onClick([editBoxPtr, calculatorWidgetPtr](){
        editBoxPtr->setText("");
        calculatorWidgetPtr->storeValue("0");
        calculatorWidgetPtr->storeOperation(CalculatorWidget::Operation::None);
    });

    horizontalLayout->addChild(std::move(calculatorWidget));  // This is a trick to store calculatorWidget in the layout :D
    horizontalLayout->addChild(std::move(backspaceButton));
    horizontalLayout->addChild(std::move(CEButton));
    horizontalLayout->addChild(std::move(CButton));

    auto inset = engine::gui::layout::Inset::create(engine::gui::layout::Constraint::Pixels(20.f));
    inset->addChild(std::move(horizontalLayout));
    verticalLayout->addChild(std::move(inset));
    verticalLayout->setRowSize(1, engine::gui::layout::Constraint::Pixels(60));

    /* 2D KEYBOARD */
    auto gridLayout = engine::gui::layout::Grid::create(5, 4);
    gridLayout->setHorizontalPadding(15);
    gridLayout->setVerticalPadding(15);

    auto button7 = engine::gui::TextButton::create("7");
    button7->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "7");});

    auto button8 = engine::gui::TextButton::create("8");
    button8->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "8");});

    auto button9 = engine::gui::TextButton::create("9");
    button9->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "9");});

    auto buttonDiv = engine::gui::TextButton::create("/");
    buttonDiv->onClick([editBoxPtr, calculatorWidgetPtr](){
        calculatorWidgetPtr->storeValue(editBoxPtr->getText());
        calculatorWidgetPtr->storeOperation(CalculatorWidget::Operation::Divide);
        editBoxPtr->setText("");});

    auto buttonSqrt = engine::gui::TextButton::create("sqrt");
    buttonSqrt->onClick([editBoxPtr](){
        float value = std::atof(editBoxPtr->getText().c_str());
        value = std::sqrt(value);
        editBoxPtr->setText(std::to_string(value));});    

    gridLayout->addChild(std::move(button7));
    gridLayout->addChild(std::move(button8));
    gridLayout->addChild(std::move(button9));
    gridLayout->addChild(std::move(buttonDiv));
    gridLayout->addChild(std::move(buttonSqrt));

    auto button4 = engine::gui::TextButton::create("4");
    button4->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "4");});

    auto button5 = engine::gui::TextButton::create("5");
    button5->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "5");});

    auto button6 = engine::gui::TextButton::create("6");
    button6->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "6");});

    auto buttonMul = engine::gui::TextButton::create("*");
    buttonMul->onClick([editBoxPtr, calculatorWidgetPtr](){
        calculatorWidgetPtr->storeValue(editBoxPtr->getText());
        calculatorWidgetPtr->storeOperation(CalculatorWidget::Operation::Multiply);
        editBoxPtr->setText("");});

    auto buttonPercent = engine::gui::TextButton::create("%");
    buttonPercent->onClick([editBoxPtr](){
        float value = std::atof(editBoxPtr->getText().c_str());
        value /= 100.f;
        editBoxPtr->setText(std::to_string(value));});

    gridLayout->addChild(std::move(button4));
    gridLayout->addChild(std::move(button5));
    gridLayout->addChild(std::move(button6));
    gridLayout->addChild(std::move(buttonMul));
    gridLayout->addChild(std::move(buttonPercent));

    auto button1 = engine::gui::TextButton::create("1");
    button1->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "1");});

    auto button2 = engine::gui::TextButton::create("2");
    button2->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "2");});

    auto button3 = engine::gui::TextButton::create("3");
    button3->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "3");});

    auto buttonSub = engine::gui::TextButton::create("-");
    buttonSub->onClick([editBoxPtr, calculatorWidgetPtr](){
        calculatorWidgetPtr->storeValue(editBoxPtr->getText());
        calculatorWidgetPtr->storeOperation(CalculatorWidget::Operation::Subtract);
        editBoxPtr->setText("");});
    auto button1overX = engine::gui::TextButton::create("1/x");

    button1overX->onClick([editBoxPtr](){
        float value = std::atof(editBoxPtr->getText().c_str());
        value = 1.f/value;
        editBoxPtr->setText(std::to_string(value));});    

    gridLayout->addChild(std::move(button1));
    gridLayout->addChild(std::move(button2));
    gridLayout->addChild(std::move(button3));
    gridLayout->addChild(std::move(buttonSub));
    gridLayout->addChild(std::move(button1overX));

    auto button0 = engine::gui::TextButton::create("0");
    button0->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "0");});

    auto buttonPlusMinus = engine::gui::TextButton::create("+/-");
    buttonPlusMinus->onClick([editBoxPtr](){
        auto currentText = editBoxPtr->getText();
        if (currentText[0] == '-')
        {
            currentText.erase(0, 1);
        }
        else
        {
            currentText = "-" + currentText;
        }
        editBoxPtr->setText(currentText);
    });

    auto buttonDot = engine::gui::TextButton::create(".");
    buttonDot->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, ".");});

    auto buttonAdd = engine::gui::TextButton::create("+");
    buttonAdd->onClick([editBoxPtr, calculatorWidgetPtr](){
        calculatorWidgetPtr->storeValue(editBoxPtr->getText());
        calculatorWidgetPtr->storeOperation(CalculatorWidget::Operation::Add);
        editBoxPtr->setText("");});

    auto buttonEqual = engine::gui::TextButton::create("=");
    buttonEqual->onClick([editBoxPtr, calculatorWidgetPtr](){
        auto result = calculatorWidgetPtr->getResultWith(editBoxPtr->getText());
        const std::string resultString = std::to_string(result);
        editBoxPtr->setText(resultString);
        calculatorWidgetPtr->storeValue(resultString);
    });

    gridLayout->addChild(std::move(button0));
    gridLayout->addChild(std::move(buttonPlusMinus));
    gridLayout->addChild(std::move(buttonDot));
    gridLayout->addChild(std::move(buttonAdd));
    gridLayout->addChild(std::move(buttonEqual));

    verticalLayout->addChild(std::move(gridLayout));

    return verticalLayout;
}

}  // namespace calculator
