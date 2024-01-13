bin_PROGRAMS = src/42sh
42sh_SOURCES = src/main.c

42sh_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -D_POSIX_SOURCE=200809L
42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_LDADD = \
    $(top_builddir)/src/parser/libparser.a \
    $(top_builddir)/src/lexer/liblexer.a \
    $(top_builddir)/src/ast/libast.a \
    $(top_builddir)/src/pretty_print_ast/libpretty_print_ast.a \
    $(top_builddir)/src/input/libinput.a


SUBDIRS = \
    src/parser \
    src/ast \
    src/pretty_print_ast \
    src/input \
    src/lexer
