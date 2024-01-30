bin_PROGRAMS = 42sh
42sh_SOURCES = main.c

42sh_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -D_POSIX_SOURCE=200809L -fsanitize=address -O3
42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_LDADD = \
    $(top_builddir)/src/parser/libparser.a \
    $(top_builddir)/src/lexer/liblexer.a \
    $(top_builddir)/src/ast/libast.a \
    $(top_builddir)/src/input/libinput.a \
    $(top_builddir)/src/eval/libeval.a \
    $(top_builddir)/src/expansion/libexpansion.a \
    $(top_builddir)/src/utils/Dstring/libdstring.a


SUBDIRS = \
    parser \
    ast \
    input \
    lexer \
    eval \
    expansion \
    utils/Dstring
