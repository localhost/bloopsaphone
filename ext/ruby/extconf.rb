require 'mkmf'

$CFLAGS << " -I../../c "

%w[notation.c bloopsaphone.c].each do |fn|
  abort "!! ERROR !!\n** #{fn} not found; type 'make ruby' in the top directory\n\n" \
    unless File.exists? fn
end

have_library("portaudio")
create_makefile("bloops")
