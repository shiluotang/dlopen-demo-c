ACLOCAL_AMFLAGS = -I m4

JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64
AM_CPPFLAGS = -Wall -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
AM_CFLAGS	=
AM_CXXFLAGS	=
AM_FFLAGS	=
AM_GCJFLAGS	=
AM_LDFLAGS	= -ldl

lib_LTLIBRARIES = libdynamic-library.la
libdynamic_library_la_SOURCES = \
src/dynamic_library.c \
src/jvm.c

check_PROGRAMS =

check_PROGRAMS += test-dlopen
test_dlopen_SOURCES = tests/test-dlopen.c
test_dlopen_LDADD = libdynamic-library.la

check_PROGRAMS += test-jvm
test_jvm_SOURCES = tests/test-jvm.c
test_jvm_LDADD = libdynamic-library.la

TESTS = $(check_PROGRAMS)
