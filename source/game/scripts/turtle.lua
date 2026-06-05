function main()
    while true do
        say("Turtle will move in a squareish pattern...")
        sleep(1)
        for i = 0, 4 do
            move_to(800 + i * 50, 500 + i * 50)
            move_to(900 + i * 50, 500 + i * 50)
            move_to(900 + i * 50, 600 + i * 50)
            move_to(800 + i * 50, 600 + i * 50)
        end
    end
end
