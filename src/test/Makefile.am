noinst_PROGRAMS=spellcorrector_test langmod_test
AM_CFLAGS=-I$(top_srcdir)/src/candidator/ -I$(top_srcdir)/src/langmod/ -I$(top_srcdir)/src/header/
spellcorrector_test_SOURCES=spellcorrector_test.c
spellcorrector_test_LDADD=$(top_builddir)/src/candidator/libcand.a $(top_builddir)/src/langmod/liblang.a $(top_builddir)/src/header/libaid.a
langmod_test_SOURCES=langmod_test.c
langmod_test_LDADD=$(top_builddir)/src/langmod/liblang.a $(top_builddir)/src/header/libaid.a
