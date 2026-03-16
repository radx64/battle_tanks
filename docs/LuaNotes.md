Notes on Lua Scripting in the Game
=====

I've managed to get some basic Lua scripting working in simple example, and here are some notes on my findings and thoughts on how it could be improved.

I have a basic understanding of how set up Lua environement, bind functions and handle coroutines, but i need to decide on the best way to structure the scripting system and how to handle blocking and non-blocking function calls.

Currently there is no easy way to tell from lua script if function call is blocking or not.

```lua
  set_turret_heading(90.0)
```
is blocking and yields the script until finished but maybe the better approach would be to have some wait function that can be used to wait for the turret to reach the desired heading. This would allow for more flexible scripting and better control over the flow of the script.

```lua
    set_turret_heading(90.0)
    set_tank_heading(180.0)
    wait_for_turret_heading(90.0)
    wait_for_tank_heading(180.0)
```
or even more general
```lua
    set_turret_heading(90.0)
    set_tank_heading(180.0)
    wait_for(all(turret_heading_reached(90.0), tank_heading_reached(180.0)))
```

in above scripts the turret and tank can start moving at the same time and the script will wait for both to reach their desired headings before proceeding. This would make the scripting more efficient and allow for more complex behaviors.

Or maybe we can have both options, blocking and non-blocking, and let the scripter decide which one to use based on their needs. This would provide more flexibility and allow for a wider range of scripting styles.

```lua
    set_turret_heading(90.0) -- blocking
    set_tank_heading(180.0) -- blocking
    set_turret_heading_non_blocking(90.0) -- non-blocking
    set_tank_heading_non_blocking(180.0) -- non-blocking
    wait_for(all(turret_heading_reached(90.0), tank_heading_reached(180.0)))
```
For begineers, the blocking approach might be easier to understand and use, while more advanced users might prefer the non-blocking approach for better performance and flexibility. It would be good to provide clear documentation and examples for both approaches to help users understand how to use them effectively.
As an example in Colobot (game I've played as a child) most functions were blocking and the scripting was very straightforward and easy to understand for beginners. However, it could be limiting for more complex behaviors and might not be the best approach for a more advanced scripting system. It would be good to provide both options and let the scripter choose which one to use based on their needs and preferences.
