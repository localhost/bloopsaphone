require 'mkmf'
require 'fileutils'

$CFLAGS << " -I../../c #{ENV['CFLAGS']}"
$LIBS << " -lm -lportaudio #{ENV['LDFLAGS']}"

%w[notation.c bloopsaphone-internal.h bloopsaphone.c bloopsaphone.h].each do |fn|
  fn = "../../c/#{fn}"
  abort "!! ERROR !!\n** #{fn} not found; type 'make ruby' in the top directory\n\n" \
    unless File.exists? fn
  FileUtils.cp(fn, ".")
end

have_library("portaudio")
create_makefile("bloops")
