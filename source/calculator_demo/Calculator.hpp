#pragma once

#include <cstdlib>
#include <cmath>

#include "gui/Button.hpp"
#include "gui/EditBox.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/Label.hpp"
#include "gui/layout/Constraint.hpp"
#include "gui/layout/Grid.hpp"
#include "gui/layout/Horizontal.hpp"
#include "gui/layout/Inset.hpp"
#include "gui/layout/Vertical.hpp"
#include "gui/TextureLibrary.hpp"

namespace calculator
{

class CalculatorComponent : public gui::Component
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

    CalculatorComponent()
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


void appendTextToEditBox(gui::EditBox* editBox, const std::string& text)
{
    auto currentText = editBox->getText();
    currentText += text;
    editBox->setText(currentText);
}

std::unique_ptr<gui::Component> createCalculator(const sf::Vector2f& initalSize)
{
    auto verticalLayout = gui::layout::Vertical::create();
    verticalLayout->setSize(initalSize);

    /* EDIT BOX*/
    auto editBox = gui::EditBox::create();
    editBox->setAlignment(gui::Alignment::Right);
    gui::EditBox* editBoxPtr = editBox.get();
    verticalLayout->addChild(std::move(editBox));
    verticalLayout->setRowSize(0, gui::layout::Constraint::Pixels(40));

    auto horizontalLayout = gui::layout::Horizontal::create();
    horizontalLayout->setPadding(10);

    /* TOP BUTTONS */
    auto calculatorComponent = std::make_unique<CalculatorComponent>();
    CalculatorComponent* calculatorComponentPtr = calculatorComponent.get(); 

    auto backspaceButton = gui::TextButton::create("Backspace");
    backspaceButton->onClick([editBoxPtr](){
        auto currentText = editBoxPtr->getText();
        if (currentText.size() > 0)
        {
            currentText.erase(currentText.size() - 1);
            editBoxPtr->setText(currentText);
        }
    });
    auto CEButton = gui::TextButton::create("CE");
    CEButton->onClick([editBoxPtr](){editBoxPtr->setText("");});

    auto CButton = gui::TextButton::create("C");
    CButton->onClick([editBoxPtr, calculatorComponentPtr](){
        editBoxPtr->setText("");
        calculatorComponentPtr->storeValue("0");
        calculatorComponentPtr->storeOperation(CalculatorComponent::Operation::None);
    });

    horizontalLayout->addChild(std::move(calculatorComponent));  // This is a trick to store calculatorComponent in the layout :D
    horizontalLayout->addChild(std::move(backspaceButton));
    horizontalLayout->addChild(std::move(CEButton));
    horizontalLayout->addChild(std::move(CButton));

    auto inset = gui::layout::Inset::create(gui::layout::Constraint::Pixels(20.f));
    inset->addChild(std::move(horizontalLayout));
    verticalLayout->addChild(std::move(inset));
    verticalLayout->setRowSize(1, gui::layout::Constraint::Pixels(60));

    /* 2D KEYBOARD */
    auto gridLayout = gui::layout::Grid::create(5, 4);
    gridLayout->setHorizontalPadding(15);
    gridLayout->setVerticalPadding(15);

    auto button7 = gui::TextButton::create("7");
    button7->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "7");});

    auto button8 = gui::TextButton::create("8");
    button8->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "8");});

    auto button9 = gui::TextButton::create("9");
    button9->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "9");});

    auto buttonDiv = gui::TextButton::create("/");
    buttonDiv->onClick([editBoxPtr, calculatorComponentPtr](){
        calculatorComponentPtr->storeValue(editBoxPtr->getText());
        calculatorComponentPtr->storeOperation(CalculatorComponent::Operation::Divide);
        editBoxPtr->setText("");});

    auto buttonSqrt = gui::TextButton::create("sqrt");
    buttonSqrt->onClick([editBoxPtr](){
        float value = std::atof(editBoxPtr->getText().c_str());
        value = std::sqrt(value);
        editBoxPtr->setText(std::to_string(value));});    

    gridLayout->addChild(std::move(button7));
    gridLayout->addChild(std::move(button8));
    gridLayout->addChild(std::move(button9));
    gridLayout->addChild(std::move(buttonDiv));
    gridLayout->addChild(std::move(buttonSqrt));

    auto button4 = gui::TextButton::create("4");
    button4->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "4");});

    auto button5 = gui::TextButton::create("5");
    button5->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "5");});

    auto button6 = gui::TextButton::create("6");
    button6->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "6");});

    auto buttonMul = gui::TextButton::create("*");
    buttonMul->onClick([editBoxPtr, calculatorComponentPtr](){
        calculatorComponentPtr->storeValue(editBoxPtr->getText());
        calculatorComponentPtr->storeOperation(CalculatorComponent::Operation::Multiply);
        editBoxPtr->setText("");});

    auto buttonPercent = gui::TextButton::create("%");
    buttonPercent->onClick([editBoxPtr](){
        float value = std::atof(editBoxPtr->getText().c_str());
        value /= 100.f;
        editBoxPtr->setText(std::to_string(value));});

    gridLayout->addChild(std::move(button4));
    gridLayout->addChild(std::move(button5));
    gridLayout->addChild(std::move(button6));
    gridLayout->addChild(std::move(buttonMul));
    gridLayout->addChild(std::move(buttonPercent));

    auto button1 = gui::TextButton::create("1");
    button1->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "1");});

    auto button2 = gui::TextButton::create("2");
    button2->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "2");});

    auto button3 = gui::TextButton::create("3");
    button3->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "3");});

    auto buttonSub = gui::TextButton::create("-");
    buttonSub->onClick([editBoxPtr, calculatorComponentPtr](){
        calculatorComponentPtr->storeValue(editBoxPtr->getText());
        calculatorComponentPtr->storeOperation(CalculatorComponent::Operation::Subtract);
        editBoxPtr->setText("");});
    auto button1overX = gui::TextButton::create("1/x");

    button1overX->onClick([editBoxPtr](){
        float value = std::atof(editBoxPtr->getText().c_str());
        value = 1.f/value;
        editBoxPtr->setText(std::to_string(value));});    

    gridLayout->addChild(std::move(button1));
    gridLayout->addChild(std::move(button2));
    gridLayout->addChild(std::move(button3));
    gridLayout->addChild(std::move(buttonSub));
    gridLayout->addChild(std::move(button1overX));

    auto button0 = gui::TextButton::create("0");
    button0->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, "0");});

    auto buttonPlusMinus = gui::TextButton::create("+/-");
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

    auto buttonDot = gui::TextButton::create(".");
    buttonDot->onClick([editBoxPtr](){appendTextToEditBox(editBoxPtr, ".");});

    auto buttonAdd = gui::TextButton::create("+");
    buttonAdd->onClick([editBoxPtr, calculatorComponentPtr](){
        calculatorComponentPtr->storeValue(editBoxPtr->getText());
        calculatorComponentPtr->storeOperation(CalculatorComponent::Operation::Add);
        editBoxPtr->setText("");});

    auto buttonEqual = gui::TextButton::create("=");
    buttonEqual->onClick([editBoxPtr, calculatorComponentPtr](){
        auto result = calculatorComponentPtr->getResultWith(editBoxPtr->getText());
        const std::string resultString = std::to_string(result);
        editBoxPtr->setText(resultString);
        calculatorComponentPtr->storeValue(resultString);
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
