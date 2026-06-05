function main()
    local current_waypoint_idx = 1
    set_led(true)
    set_led_color(0, 255, 0)
    while true do
        local waypoints = get_waypoints()
        if #waypoints == 0 then
            sleep(2)
            goto continue
        end

        local waypoint = waypoints[current_waypoint_idx]
        if waypoint ~= nil then
                local pos = get_tank_position()
                local dx = waypoint.x - pos.x
                local dy = waypoint.y - pos.y
                local distance = math.sqrt(dx * dx + dy * dy)

                if distance > 30.0 then
                    local direction = math.atan(dx, -dy)
                    local heading = math.deg(direction)
                    set_turret_heading(heading)
                    print("[LUA]> Moving toward waypoint " .. waypoint.x .. ", " .. waypoint.y .. "")
                    -- blocking call: drives the tank until it's within threshold
                    move_to(waypoint.x, waypoint.y)
                end

                -- Arrived (either was already within threshold or move_to returned)
                say("Waypoint " .. current_waypoint_idx .. " reached")
                current_waypoint_idx = current_waypoint_idx + 1
                if current_waypoint_idx > #waypoints then
                    current_waypoint_idx = 1
                end
                set_throttle(0.0)
                set_led_color(255, 0, 0) -- Set LED color to red
                fire_cannon()
                fire_cannon() -- checking if cannon cooldown works, should only fire once
                sleep(0.02)
                set_led_color(0, 255, 0) -- Set LED color to green
        else
            print("[LUA]> No more waypoints, resetting to first waypoint.")
            current_waypoint_idx = 1
            sleep(2)
        end

        ::continue::
    end

end
