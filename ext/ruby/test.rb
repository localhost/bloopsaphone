require './bloops'

# the song object
b = Bloops.new
b.tempo = 320

# an instrument
saw = b.sound Bloops::SAWTOOTH

# assign a track to the song
b.tune saw, "c5 c6 b4 b5 d5 d6 e5 e6"

# make it go
b.play
sleep 1 while !b.stopped?

# a percussion
beat = b.sound Bloops::NOISE
beat.repeat = 0.6

# assign a track to the song
b.tune beat, "4 4 b4 4 d5 4 e5 e6"

# make it go
b.play
sleep 1 while !b.stopped?
