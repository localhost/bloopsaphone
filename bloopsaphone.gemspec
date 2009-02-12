Gem::Specification.new do |s|
  s.name = 'bloopsaphone'
  s.version = '0.4'
 
  s.authors = ["why the lucky stiff"]
  s.date = '2009-02-12'
  s.description = 'arcade sounds and chiptunes'
  s.email = 'why@ruby-lang.org'
  s.extensions = ["ext/ruby/extconf.rb"]
  s.extra_rdoc_files = ["README", "COPYING"]
  s.files = ["COPYING", "README", "c/bloopsaphone.c", "c/bloopsaphone.h", "c/notation.c",
    "ext/ruby/extconf.rb", "ext/ruby/rubyext.c", "ext/ruby/test.rb", "ext/ruby/test_load.rb",
    "sounds/dart.blu", "sounds/error.blu", "sounds/ice.blu", "sounds/jump.blu",
    "sounds/pogo.blu", "sounds/stun.blu"]
  s.has_rdoc = false
  s.homepage = 'http://github.com/why/bloopsaphone'
  s.summary = 'arcade sounds and chiptunes'
end
