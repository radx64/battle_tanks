function main()
    while true do
        for i = 0, 3, 1 do
            set_turret_heading(i * 90.0);
            print("[LUA]> Turret heading set to " .. (i * 90.0) .. " degrees");
            sleep(2);
            set_tank_heading(i * 90.0);
            print("[LUA]> Heading set to " .. (i * 90.0) .. " degrees");
            sleep(2);
        end
    end
end
