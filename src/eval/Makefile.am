lib_LIBRARIES = libeval.a

libeval_a_SOURCES = basic_builtins.c builtins.h evaluation_process.c evaluation_process.h eval_redirections.c eval_redirections.h advanced_builtins.c eval_pipe_negation.c eval_pipe_negation.h cd_builtin.c
libeval_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -D_POSIX_C_SOURCE=200809L
libeval_a_CPPFLAGS = -I$(top_srcdir)