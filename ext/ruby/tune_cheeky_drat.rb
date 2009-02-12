# cheeky drat
#   by Alex "freQvibez" Brem
# exclusivele composed in TextMate
#   for _why's BloopSaphone!

require './bloops'

b = Bloops.new
b.tempo = 171

bass = b.sound Bloops::SQUARE
bass.volume = 0.5
bass.sustain = 0.1
bass.attack = 0.1
bass.decay = 0.3
# bass.hpf = 0.05
# bass.resonance = 0.2
# bass.phase = 0.5
# bass.psweep = -0.05

base = b.sound Bloops::NOISE
base.sustain = 0.1
base.decay = 0.1
base.lpf = 0.2
base.resonance = 0.7
# base.dslide = -0.2

snare = b.sound Bloops::NOISE
snare.attack = 0.075
snare.sustain = 0
snare.decay = 0.3
snare.hpf = 0.55
snare.resonance = 0.4
snare.dslide = -0.4

chord = b.sound Bloops::SQUARE
chord.volume = 0.3
chord.attack = 0.05
chord.sustain = 0.6
chord.decay = 0.9
chord.phase = 0.8
chord.psweep = -0.2

# lead = b.sound Bloops::SAWTOOTH
lead = b.sound Bloops::SINE
lead.volume = 0.4
lead.attack = 0.3
lead.sustain = 0.15
lead.decay = 0.8
lead.vibe = 0.035
lead.vspeed = 0.35
lead.vdelay = -0.5
lead.hpf = 0.2
lead.hsweep = -0.05
lead.resonance = 0.55
lead.phase = 0.4
lead.psweep = -0.05

# seq1 = %q( a2 a3 a2 a3 a2 a3 a2 a3 )
# seq2 = %q( d3 e4 d3 e4 g3 d4 gb3 d4 )
# mel1 = %q( )

# assign a track to the song
# b.tune sound, "c5 c6 b4 b5 d5 d6 e5 e6"
# sound.repeat = 0.1
# b.tune sound, seq1
# b.tune sound, seq2

# b.track_at seq1, 0
# bass.arp = 0.2
# bass.arp = 0.7
# bass.arp = 0.6
# bass.aspeed = 0.45

#8 4d2 8d 8d 4d 4d 4a1 8a 8a 4a 8a
#8 4a1 8a 8a 4a 4a 4d2 9d 7d 4d 8e
#8 4a1 8a 8a 4a 4a 4c2 9c 7c 4c 8e

b.tune bass, %q^
  8 4a1 8a 8a 4a 4a 4c2 9c 7c 4c 8e
  8 4g2 8g 8g 4g 4g 4c2 8c 8c 4c 8c
  8 4d2 8d 8d 4d 4d 4a1 8a 8a 4a 8a
  8 4g2 8g 8g 4g 4d 4d2 9e3 7c1 8 4c2
  
  8 4d2 8d 8d 4d 4d 4a1 8a 8a 4a 8a
  8 4g2 8g 8g 4g 4g 4c2 8c 8c2 4c 8c
  8 4d2 8d 8d 4d 4d 4a1 8a 8a 4a 8a
  8 4g2 8g 8g 4g 4g 4c2 8c 8c 4c 8c

  8 4a1 8a 8a 4a 4a 4d2 9d 7d 4d 8e
  8 4g2 8g 8g 4g 4g 4c2 8c 8c 4c 8c
  8 4d2 8d 8d 4d 4d 4a1 8a 8a 4a 8a
  8 4g2 8g 8g 4g 4d 4d2 9e3 7c1 8 4c2

  8 4d2 8d 8d 4d 4d 4a1 8a 8a 4a 8a
  8 4g2 8g 8g 4g 4g 4c2 8c 8c2 4c 8c
  8 4d2 8d 8d 4d 4d 4a1 8a 8a 4a 8a
  8 4g2 8g 8g 4g 4g 4c2 8c 8c 4c 8c
^

# b.tune base, %q^
#   4a1 4 4a 4 4a 4 4a 4
#   4a1 4 4a 4 4a 4 4a 4
#   4a1 4 4a 4 4a 4 4a 4
#   4a1 4 4a 4 4a 4 4a 4
# ^

b.tune snare, %q^
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 8a2 8a2
  
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 8a2 8a2

  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 8a2 8a2

  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 4a2
  4 4a2 4 4a2 4 4a2 4 8a2 8a2
^

b.tune chord, %q^
  1a2 2a3 2 1 2g3 2
  1 2a3 2 1 2g3 2

  1a2 2a3 2 1 2g3 2
  1 2a3 2 1 2g3 2

  1a2 2a3 2 1 2g3 2
  1 2a3 2 1 2g3 2

  1a2 2a3 2 1 2g3 2
  1 2a3 2 1 2g3 2
^

b.tune chord, %q^
  2 2c4 2 1 2b4 1
  2 2c4 2 1 2b4 1

  2 2c4 2 1 2b4 1
  2 2c4 2 1 2b4 1

  2 2c4 2 1 2b4 1
  2 2c4 2 1 2b4 1

  2 2c4 2 1 2b4 1
  2 2c4 2 1 2b4 1
^

b.tune chord, %q^
  2 1 2e4 2 1 2d4
  2 1 2e4 2 1 2d4

  2 1 2e4 2 1 2d4
  2 1 2e4 2 1 2d4

  2 1 2e4 2 1 2d4
  2 1 2e4 2 1 2d4

  2 1 2e4 2 1 2d4
  2 1 2e4 2 1 2d4
^

# 2 4
b.tune lead, %q^
  1 1
  1 1
  1 1
  1 4

  2g3 1a4 2
  2c5 1e4 2 1
  1a4 2 4 2e4 1d4
  2

  2a3 1b4 2
  2d5 2g4 1c5
  1a4 1e5
  1b4 2 4 8 8d4  

  2g3 1a4 2
  2c5 1e4 3d4 5g4
  1a4 1e4
  1d4 1
^

# 5eb4
# 2a3 1b4 2
# 2d5 1e4 2
# 1a4 1c5
# 1b4 1

# 2g3 1a4 2
# 2c5 1e4 2
# 1a4 1e4
# 1d4 1

b.play
while !b.stopped?
  sleep 1
end
