require './bloops'

b = Bloops.new
ice = b.load "../../sounds/ice.blu"

b.tune ice, "c c# d eb e f f# g ab a bb b + c"

b.play
sleep 1 while !b.stopped?
