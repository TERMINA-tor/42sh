lib_LIBRARIES = libparser.a

libparser_a_SOURCES = parser.h parser_compound_list.c parser_input.c parser_loops.c parser_rule_shell.c parser_redirections.c parser_redirections.h 
libparser_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -D_POSIX_C_SOURCE=200809L -fsanitize=address
libparser_a_CPPFLAGS = -I$(top_srcdir)