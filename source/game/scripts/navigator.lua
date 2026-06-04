function main()
    local current_waypoint_idx = 1;
    while true do
        local waypoints = get_waypoints();
        if #waypoints == 0 then
            print("[LUA]> No waypoints found, waiting...");
            sleep(2);
            goto continue;
        end

        local waypoint = waypoints[current_waypoint_idx];
        if waypoint ~= nil then
            local pos = get_tank_position();
            local dx = waypoint.x - pos.x;
            local dy = waypoint.y - pos.y;
            local distance = math.sqrt(dx * dx + dy * dy);
            if distance > 30.0 then
                local direction = math.atan(dx, -dy);
                local heading = math.deg(direction);
                local throttle = math.min(1.0, distance * 0.01);
                set_tank_heading(heading);
                set_turret_heading(heading);
                set_throttle(throttle);
                print("[LUA]> Moving toward waypoint " .. waypoint.x .. ", " .. waypoint.y .. " with throttle " .. throttle);
            else
                current_waypoint_idx = current_waypoint_idx + 1;
                if current_waypoint_idx > #waypoints then
                    current_waypoint_idx = 1;
                end
                set_throttle(0.0);
                print("[LUA]> Waypoint reached, switching to waypoint " .. current_waypoint_idx);
                fire_cannon();
                fire_cannon(); -- checking if cannon cooldown works, should only fire once
                fire_cannon();
            end
            sleep(0.02);
        else
            print("[LUA]> No more waypoints, resetting to first waypoint.");
            current_waypoint_idx = 1;
            sleep(2);
        end

        ::continue::
    end

end
