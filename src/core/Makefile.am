bin_PROGRAMS=SpellCorrector
AM_CFLAGS=-I$(top_srcdir)/src/langmod -I$(top_srcdir)/src/candidator -pg -static -O3
#Need to deal with libraries dependencies.
#libcand.a includes functions in liblang.a and libaid.a,so it should be placed at first.
SpellCorrector_LDADD=$(top_builddir)/src/candidator/libcand.a\
					 $(top_builddir)/src/langmod/liblang.a\
					 $(top_builddir)/src/header/libaid.a
SpellCorrector_SOURCES=spellcorrector.c
