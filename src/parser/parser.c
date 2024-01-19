/*

input =
list '\n'
| list EOF
| '\n'
| EOF
;

list = and_or { ';' and_or } [ ';' ] ;

and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;

pipeline = ['!'] command { '|' {'\n'} command } ;

command = simple_command | 
    shell_command { redirection } ; 

simple_command = prefix { prefix } 
    | { prefix } WORD { element } ; 
    
prefix = redirection ; 

redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' ) WORD ;

element = WORD |
    redirection ;

shell_command = 
    rule_if | 
    rule_while | 
    rule_until ; 
    
rule_while = 'while' compound_list 'do' compound_list 'done' ; 
rule_until = 'until' compound_list 'do' compound_list 'done' ; 
rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ; 
else_clause = 'else' compound_list | 
    'elif' compound_list 'then' compound_list [else_clause] ; 
compound_list = {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or } [';'] {'\n'} ;

*/

// i want my parser to be a bottom up parser
// lexer functions are lexer_pop and lexer_peek which are defined in lexer.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "parser.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"

enum parser_status parse_input(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    if (t.type == TOKEN_EOF)
        return status;
    if (t.type == TOKEN_EOL)
    {
        lexer_pop(lexer);
        return status;
    }
    status = parse_list(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_pop(lexer);
    if (t.type == TOKEN_EOF)
        return status;
    if (t.type == TOKEN_EOL)
        return status;
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_list(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    status = parse_and_or(lexer);
    if (status != PARSER_OK)
        return status;
    struct token t = lexer_peek(lexer);
    while (t.type == TOKEN_SEMICOLON)
    {
        lexer_pop(lexer);
        status = parse_and_or(lexer);
        if (status != PARSER_OK)
            return status;
        t = lexer_peek(lexer);
    }
    return status;
}

enum parser_status parse_and_or(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    status = parse_pipeline(lexer);
    if (status != PARSER_OK)
        return status;
    struct token t = lexer_peek(lexer);
    while (t.type == TOKEN_AND || t.type == TOKEN_OR)
    {
        lexer_pop(lexer);
        t = lexer_peek(lexer);
        while (t.type == TOKEN_EOL)
        {
            lexer_pop(lexer);
            t = lexer_peek(lexer);
        }
        status = parse_pipeline(lexer);
        if (status != PARSER_OK)
            return status;
        t = lexer_peek(lexer);
    }
    return status;
}

enum parser_status parse_pipeline(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    if (t.type == TOKEN_NOT)
        lexer_pop(lexer);
    status = parse_command(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    while (t.type == TOKEN_PIPE)
    {
        lexer_pop(lexer);
        t = lexer_peek(lexer);
        while (t.type == TOKEN_EOL)
        {
            lexer_pop(lexer);
            t = lexer_peek(lexer);
        }
        status = parse_command(lexer);
        if (status != PARSER_OK)
            return status;
        t = lexer_peek(lexer);
    }
    return status;
}

enum parser_status parse_command(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    if (t.type == TOKEN_WORD)
    {
        lexer_pop(lexer);
        t = lexer_peek(lexer);
        while (t.type == TOKEN_WORD)
        {
            lexer_pop(lexer);
            t = lexer_peek(lexer);
        }
        return status;
    }
    status = parse_shell_command(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    while (t.type == TOKEN_REDIRECT_INPUT || t.type == TOKEN_REDIRECT_OUTPUT || t.type == TOKEN_APPEND_OUTPUT || t.type == TOKEN_AMPREDIR_OUTPUT || t.type == TOKEN_AMPREDIR_INPUT || t.type == TOKEN_FORCE_OUTPUT_REDIR)
    {
        lexer_pop(lexer);
        status = parse_redirection(lexer);
        if (status != PARSER_OK)
            return status;
        t = lexer_peek(lexer);
    }
    return status;
}

enum parser_status parse_if_command(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    lexer_pop(lexer);
    status = parse_compound_list(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    if (t.type != TOKEN_THEN)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    status = parse_compound_list(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    if (t.type == TOKEN_ELSE)
    {
        lexer_pop(lexer);
        status = parse_compound_list(lexer);
        if (status != PARSER_OK)
            return status;
    }
    else if (t.type == TOKEN_ELIF)
    {
        lexer_pop(lexer);
        status = parse_compound_list(lexer);
        if (status != PARSER_OK)
            return status;
        t = lexer_peek(lexer);
        if (t.type == TOKEN_ELSE)
        {
            lexer_pop(lexer);
            status = parse_compound_list(lexer);
            if (status != PARSER_OK)
                return status;
        }
    }
    t = lexer_peek(lexer);
    if (t.type != TOKEN_FI)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    return status;
}

enum parser_status parse_while_command(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    lexer_pop(lexer);
    status = parse_compound_list(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    if (t.type != TOKEN_DO)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    status = parse_compound_list(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    if (t.type != TOKEN_DONE)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    return status;
}

enum parser_status parse_until_command(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    lexer_pop(lexer);
    status = parse_compound_list(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    if (t.type != TOKEN_DO)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    status = parse_compound_list(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    if (t.type != TOKEN_DONE)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    return status;
}

enum parser_status parse_shell_command(struct lexer *lexer)
{
    struct token t = lexer_peek(lexer);
    if (t.type == TOKEN_IF)
    {
        return parse_if_command(lexer);
    }
    else if (t.type == TOKEN_WHILE)
    {
        return parse_while_command(lexer);
    }
    else if (t.type == TOKEN_UNTIL)
    {
        return parse_until_command(lexer);
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_compound_list(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    while (t.type == TOKEN_EOL)
    {
        lexer_pop(lexer);
        t = lexer_peek(lexer);
    }
    status = parse_and_or(lexer);
    if (status != PARSER_OK)
        return status;
    t = lexer_peek(lexer);
    while (t.type == TOKEN_SEMICOLON || t.type == TOKEN_EOL)
    {
        lexer_pop(lexer);
        t = lexer_peek(lexer);
        while (t.type == TOKEN_EOL)
        {
            lexer_pop(lexer);
            t = lexer_peek(lexer);
        }
        status = parse_and_or(lexer);
        if (status != PARSER_OK)
            return status;
        t = lexer_peek(lexer);
    }
    return status;
}

enum parser_status parse_redirection(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    if (t.type == TOKEN_IONUMBER)
        lexer_pop(lexer);
    t = lexer_peek(lexer);
    if (t.type != TOKEN_REDIRECT_INPUT && t.type != TOKEN_REDIRECT_OUTPUT && t.type != TOKEN_APPEND_OUTPUT && t.type != TOKEN_AMPREDIR_OUTPUT && t.type != TOKEN_AMPREDIR_INPUT && t.type != TOKEN_FORCE_OUTPUT_REDIR)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    t = lexer_peek(lexer);
    if (t.type != TOKEN_WORD)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    return status;
}

enum parser_status parse_prefix(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    if (t.type != TOKEN_REDIRECT_INPUT && t.type != TOKEN_REDIRECT_OUTPUT && t.type != TOKEN_APPEND_OUTPUT && t.type != TOKEN_AMPREDIR_OUTPUT && t.type != TOKEN_AMPREDIR_INPUT && t.type != TOKEN_FORCE_OUTPUT_REDIR)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    t = lexer_peek(lexer);
    if (t.type != TOKEN_WORD)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    return status;
}

enum parser_status parse_element(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    if (t.type == TOKEN_WORD)
    {
        lexer_pop(lexer);
        return status;
    }
    status = parse_redirection(lexer);
    if (status != PARSER_OK)
        return status;
    return status;
}

enum parser_status parse_simple_command(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    while (t.type == TOKEN_REDIRECT_INPUT || t.type == TOKEN_REDIRECT_OUTPUT || t.type == TOKEN_APPEND_OUTPUT || t.type == TOKEN_AMPREDIR_OUTPUT || t.type == TOKEN_AMPREDIR_INPUT || t.type == TOKEN_FORCE_OUTPUT_REDIR)
    {
        lexer_pop(lexer);
        t = lexer_peek(lexer);
        if (t.type != TOKEN_WORD)
            return PARSER_UNEXPECTED_TOKEN;
        lexer_pop(lexer);
        t = lexer_peek(lexer);
    }
    t = lexer_peek(lexer);
    while (t.type == TOKEN_WORD)
    {
        lexer_pop(lexer);
        t = lexer_peek(lexer);
    }
    return status;
}

enum parser_status parse_word(struct lexer *lexer)
{
    enum parser_status status = PARSER_OK;
    struct token t = lexer_peek(lexer);
    if (t.type != TOKEN_WORD)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    return status;
}