#
#         -=) warp-o-mat (=-
#  tune coded/composed by freQvibez
#         (c) 2009 Alex Brem
#
# exclusively for why's BloopSaphone
#
#       from Farbrausch with ♥
#

module LousyBloopMachine

  class Tune

    BPM = 67

    ###
     ### sequences
      ###

    SEQUENCES = {
      :bass => %w^
        01 02 01 02 01 02 01 02
        01 02 01 02 01 02 01 02

        01 02 01 02 01 02 01 02
        01 02 01 02 01 02 01 02
      ^,
      :base => %w^
        00 01 01 01 01 01 01 01
        01 01 01 01 01 01 01 01

        01 01 01 01 01 01 01 01
        01 01 01 01 01 01 01 01
      ^,
      :snare => %w^
        00 00 00 00 01 01 01 02
        01 01 01 02 01 01 01 02

        01 01 01 02 01 01 01 02
        01 01 01 02 01 01 01 02
      ^,
      :hihat => %w^
        00 00 01 01 01 01 01 01
        01 01 01 01 01 01 01 01

        01 01 01 01 01 01 01 01
        01 01 01 01 01 01 01 01
      ^,
      :rhodes_1 => %w^
        00 00 00 00
        01 02 03 04

        01 02 03 04
        01 02 03 04
      ^,
      :rhodes_2 => %w^
        00 00 00 00
        01 02 03 04

        01 02 03 04
        01 02 03 04
      ^,
      :rhodes_3 => %w^
        00 00 00 00
        01 02 03 04

        01 02 03 04
        01 02 03 04
      ^,
      :rhodes_4 => %w^
        00 00 00 00
        01 02 03 04

        01 02 03 04
        01 02 03 04
      ^,
      :silent => %w^
        00
        00

        01
        01
      ^,
      :naughty => %w^
        00
        00

        00
        01
      ^
    }

    ###
     ### patterns
      ###

    PATTERNS = {

      :bass => {
        01 => %q^
          32a1[attack 0.1][sustain 0.05] 32a
          32a 32a
          32a 32a2
          32a1 32a

          32a[sustain 0.1] 32a
          32a 32a
          32a[sustain 0.15] 32a
          32a2[sustain 0.1] 32a1
        ^,
        02 => %q^
          8 16a[attack 0.2][psweep 0.5][square 0.2] 16a[attack 0.1]
          8 16a 16

          16a 8 16a
          16 16a 16 16a

          8 16a 32a 16a 32a
          32a 32 16a 32
        ^
      },

      :base => {
        00 => %q^
          1
        ^,
        01 => %q^
          8d2 8d 8d 8d
          8d 8d 8d 8d
        ^
      },

      :snare => {
        00 => %q^
          1
        ^,
        01 => %q^
          8 8a 8 8a
          8 8a 8 8a
        ^,
        02 => %q^
          8 8a 8 8a
          16 16 8a 32 32a[volume 0.05] 16 8a[volume 0.25]
        ^
      },

      :hihat => {
        00 => %q^
          1
        ^,
        01 => %q^
          16 16a 16 16a 16 16a 16 16a
          16 16a 16 16a 16 16a 16 16a
        ^
      },

      :rhodes_1 => {
        00 => %q^
          1     1
        ^,
        01 => %q^
          1c4   1e4
        ^,
        02 => %q^
          1c4   1e4
        ^,
        03 => %q^
          1d4   1c4
        ^,
        04 => %q^
          1e4   1eb4
        ^
      },

      :rhodes_2 => {
        00 => %q^
          1     1
        ^,
        01 => %q^
          1e4   1g4
        ^,
        02 => %q^
          1e4   1g4
        ^,
        03 => %q^
          1g4   1d4
        ^,
        04 => %q^
          1gb4  1gb4
        ^
      },

      :rhodes_3 => {
        00 => %q^
          1     1
        ^,
        01 => %q^
          1g4   1b4
        ^,
        02 => %q^
          1g4   1b4
        ^,
        03 => %q^
          1a4   1g4
        ^,
        04 => %q^
          1g4   1g4
        ^
      },

      :rhodes_4 => {
        00 => %q^
          1     1
        ^,
        01 => %q^
          1b4   1d4
        ^,
        02 => %q^
          1b4   1d4
        ^,
        03 => %q^
          1c4   1b4
        ^,
        04 => %q^
          1c4   1b4
        ^
      },

      :silent => {
        00 => %q^
          1 1 1 1
          1 1 1 1
        ^,
        01 => %q^
          1b4   1d5
              2b4   2g4   1e4

          1c4   2gb5    2b4
              1c4   1b3
        ^
      },

      :naughty => {
        00 => %q^
          1 1 1 1
          1 1 1 1
        ^,
        01 => %q^
          2

          1b4     1e4
                2b3   2d4 2b4

          1a4   2gb4    2a3
              1eb4   1gb4
        ^
      }

    }

  end

  ###
   ### playroutine
    ###

  require 'yaml'
  require './bloops'

  extend self

  def init
    @bloops ||= Bloops.new
    @bloops.tempo = Tune::BPM

    return if @sounds

    @sounds = {}
    YAML.load(DATA.read).each do |track,instrument|
      @sounds[track] = @bloops.sound instrument['sound'].split("::").inject(Object) { |c1,c2| c1.const_get(c2) }
      instrument.reject{|k,v| k == 'sound'}.each do |sound,value|
        @sounds[track].send "#{sound}=", value
      end
    end

    Tune::SEQUENCES.each do |track,sequences|
      instr = track.to_s.split('_')[0]
      next unless @sounds[instr]
      next unless Tune::PATTERNS[track]

      notes = ''
      sequences.each do |seq|
        seq = seq.to_i
        next unless Tune::PATTERNS[track][seq]
        notes << Tune::PATTERNS[track][seq]
      end
      @bloops.tune @sounds[instr], notes
    end
  end

  def play
    init unless @bloops
    @bloops.play
    sleep 0.05 while !@bloops.stopped?
  end

  def play_endless
    while true do play; end
  end

end

LousyBloopMachine.play_endless if $0 == __FILE__

###
 ### instruments
  ###

__END__

bass:
  sound: Bloops::SQUARE
  volume: 0.9
  attack: 0.1
  decay: 0.15
  sustain: 0.05
  square: 0.05
  phase: 0.5
  psweep: -0.255

base:
  sound: Bloops::SINE
  volume: 0.6
  attack: 0.0
  decay: 0.25
  sustain: 0.15
  lpf: 0.45
  resonance: 0.4
  dslide: -0.3

snare:
  sound: Bloops::NOISE
  volume: 0.25
  attack: 0.01
  decay: 0.305
  sustain: 0
  hpf: 0.65
  resonance: 0.24
  dslide: -0.452

hihat:
  sound: Bloops::NOISE
  volume: 0.25
  attack: 0.150
  decay: 0.105
  sustain: 0.205
  hpf: 0.95

rhodes:
  sound: Bloops::SAWTOOTH
  volume: 0.09
  attack: 0.55
  decay: 1.0
  sustain: 0.45
  lpf: 0.55
  lsweep: -0.005
  resonance: 0.35
  vibe: 0.035
  vspeed: 0.292
  phase: 0.305
  psweep: -0.025
  vdelay: 0.9

silent:
  sound: Bloops::SINE
  volume: 0.45
  attack: 0.35
  decay: 0.95
  sustain: 0.85
  lpf: 0.25
  lsweep: -0.025
  hpf: 0.25
  hsweep: -0.025
  resonance: 0.75
  vibe: 0.055
  vspeed: -0.325
  phase: 0.505
  psweep: -0.025

naughty:
  sound: Bloops::SQUARE
  volume: 0.15
  attack: 0.45
  decay: 0.95
  sustain: 0.85
  lpf: 0.25
  lsweep: -0.025
  hpf: 0.25
  hsweep: -0.025
  resonance: 0.65
  vibe: 0.065
  vspeed: -0.325
  phase: 0.305
  psweep: -0.025
  square: 0.75
  phase: 0.0
  psweep: 0.555
