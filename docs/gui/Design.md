Design
=====

## Component
It is a abstract base class for all other gui components
Implements basic objects suppport with childs aggregation and it's lifetime management
as well as some basic position and size control

![](Component.drawio.svg "Component diagram")



## Window
Implements simple closable window (managed by WindowManager). It can contain any `Component` 

Possible / target implementation:

![](Window.drawio.svg "Window target implementation")

## Button
Simple button class that can have `onClick` method bind to user action. 

## Label
Class for creating text labels

## WindowManager
Manages `Window` instances (added to given window manager) with Z-ordering, focusing and destroying dead windows.

## Stylesheet
Describes visual theme of components

# Dependencies

Rough sketch of dependencies

```plantuml
@startuml
class WindowManager

class Component

class Window
class Button
class Label

Component <|-- Label

Component <|-- Window
Component <|--  Button

Window *--up> WindowManager

Window --> StyleSheet
Button --> StyleSheet
Label --> StyleSheet


@enduml

```