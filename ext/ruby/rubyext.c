//
// rubyext.c
// the ruby binding to bloopsaphone
//
// (c) 2009 why the lucky stiff
//
#include <ruby.h>
#include "bloopsaphone.h"

VALUE
rb_bloops_alloc()
{
}

void
Init_bloopsaphone()
{
  VALUE cBloops = rb_define_class("Bloopsaphone");
  rb_define_alloc_func(cBloops, rb_bloops_alloc);
}
