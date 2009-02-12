require './bloops'

# the song object
b = Bloops.new
b.tempo = 320

# an instrument
sound = b.sound Bloops::SAWTOOTH

# assign a track to the song
b.tune sound, "c5 c6 b4 b5 d5 d6 e5 e6"

# make it go
b.play
while !b.stopped?
  sleep 1
end

# a percussion
sound = b.sound Bloops::NOISE
sound.repeat = 0.6

# assign a track to the song
b.tune sound, "4 4 b4 4 d5 4 e5 e6"

# make it go
b.play
while !b.stopped?
  sleep 1
end
