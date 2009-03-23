require 'mkmf'
require 'fileutils'

$CFLAGS << " -I../../c -I/opt/local/include "
$LIBS << "-L/opt/local/lib -lm -lportaudio"

%w[../../c/notation.c ../../c/bloopsaphone.c ../../c/bloopsaphone.h].each do |fn|
  abort "!! ERROR !!\n** #{fn} not found; type 'make ruby' in the top directory\n\n" \
    unless File.exists? fn
  FileUtils.cp(fn, ".")
end

have_library("portaudio")
create_makefile("bloops")
