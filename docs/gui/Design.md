Design
=====

- [Design](#design)
  - [Widget](#widget)
  - [Window](#window)
  - [Button](#button)
  - [Label](#label)
  - [WindowManager](#windowmanager)
  - [Stylesheet](#stylesheet)
- [Dependencies](#dependencies)
- [Drawing](#drawing)
- [Event processing](#event-processing)
- [Layout management](#layout-management)


## Widget
It is a abstract base class for all other gui widgets
Implements basic objects suppport with childs aggregation and it's lifetime management
as well as some basic position and size control


Widget derivate should implement required behaviour of below methods
- onSizeChange
  - By default widget is not handling children sizes. Specific widget like WindowPanel or GridLayout could define own ways of handling children position and size.


## Window
Implements simple closable window (managed by WindowManager). It can contain any `Widget` 

Possible / target implementation:

![](Window.drawio.svg "Window target implementation")

## Button
Simple button class that can have `onClick` method bind to user action. 

## Label
Class for creating text labels

## WindowManager
Manages `Window` instances (added to given window manager) with Z-ordering, focusing and destroying dead windows.

## Stylesheet
Describes visual theme of widgets

# Dependencies

Rough sketch of dependencies

```plantuml
@startuml
class WindowManager

class Widget

class Window
class Button
class Label

Widget <|-- Label

Widget <|-- Window
Widget <|--  Button

Window *--up> WindowManager

Window --> StyleSheet
Button --> StyleSheet
Label --> StyleSheet

@enduml

```
# Drawing
Currently widgets draw themselve on sfml render target.
Consider moving it to separate texture and redraw those only when needed to boost performance.

# Event processing

There is a hierarchy how mouse events are beeing processed.
If user clicks on given widget it iterates over all its children to check if any of them captures the event. If captured, event is not processed further. If any child has not captured the event, event is processed with widget itself.

# Layout management

GUI framework currently supports few layout options:
  - Inset - create a barrier (configured in pixels or percents) around parent object so children size is reduced and centered by a size of this barrier
  - Horizontal - lays objects in horizontal line taking into consideration of each size constraint configured for given column
  - Vertical - same as Horizontal but vertically (rows)
  - Grid - similar to twos above but in 2D. Child objects can be placed on 2D grid. Also padding between columns and between rows can be configured.
