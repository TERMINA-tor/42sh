bin_PROGRAMS = 42sh
42sh_SOURCES = src/main.c \
    src/parser/parser.c \
    src/lexer/lexer.c

42sh_HEADERS = src/parser/parser.h \
    src/lexer/lexer.h

check_PROGRAMS = test_parser test_lexer
check-local: 
    python3 -m pytest tests/test_parser.py

42sh_CPPFLAGS = -I$(top_srcdir)/src
42sh_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
42sh_LDADD = \
    $(top_builddir)/src/parser/libparser.a \
    $(top_builddir)/src/lexer/liblexer.a

SUBDIRS = \
    src/parser \
    src/lexer

test_parser_SOURCES = tests/test_parser.c \
    src/parser/parser.c

test_parser_CPPFLAGS = -I$(top_srcdir)/src
test_parser_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic

test_lexer_SOURCES = tests/test_lexer.c \
    src/lexer/lexer.c

test_lexer_CPPFLAGS = -I$(top_srcdir)/src
test_lexer_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
