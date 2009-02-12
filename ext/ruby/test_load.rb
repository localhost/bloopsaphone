require './bloops'

b = Bloops.new

# ice #1
puts "** playing scale using ice.blu"
ice = b.load "../../sounds/ice.blu"
b.tune ice, "c c# d eb e f f# g ab a bb b + c"

b.play
sleep 1 while !b.stopped?

b.clear

# ice #2
puts "** same scale built from ruby"
ice2 = b.sound Bloops::SQUARE
ice2.punch = 0.441
ice2.sustain = 0.067
ice2.decay = 0.197
ice2.freq = 0.499
b.tune ice2, "c c# d eb e f f# g ab a bb b + c"

b.play
sleep 1 while !b.stopped?
