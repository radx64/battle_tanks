function main()
    set_led_blinking(0) -- disable automatic blinking
    while true do
        for j = 0, 255, 5 do
            set_led(true) -- Turn LED on
            set_led_color(j, 0, 0) -- Set LED color to red with increasing intensity
            sleep(0.01) -- Wait for 10 milliseconds
        end
        for j = 0, 255, 5 do
            set_led(true) -- Turn LED on
            set_led_color(0, j, 0) -- Set LED color to green with increasing intensity
            sleep(0.01) -- Wait for 10 milliseconds
        end
        for j = 0, 255, 5 do
            set_led(true) -- Turn LED on
            set_led_color(0, 0, j) -- Set LED color to blue with increasing intensity
            sleep(0.01) -- Wait for 10 milliseconds
        end

        for j = 0, 4, 1 do
            for i = 0, 4, 1 do
                set_led_color(255, 0, 0) -- Set LED color to red
                set_led(true) -- Turn LED on
                sleep(0.5) -- Wait for 500 milliseconds
                set_led(false) -- Turn LED off
                sleep(0.5) -- Wait for 500 milliseconds
            end
            set_led_color(0, 255, 0) -- Set LED color to green
            set_led_blinking(0.0)
            for i = 0, 4, 1  do
                set_led(true) -- Turn LED on
                sleep(0.2) -- Wait for 200 milliseconds
                set_led(false) -- Turn LED off 
                sleep(0.2) -- Wait for 200 milliseconds
            end
        end
    end
end
