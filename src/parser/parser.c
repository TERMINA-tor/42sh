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

// lexer functions are lexer_pop and lexer_peek which are defined in lexer.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "parser.h"
#include "../lexer/lexer.h"

enum parser_status parse_input(struct lexer *lexer); //
static enum parser_status parse_list(struct lexer *lexer); //
static enum parser_status parse_and_or(struct lexer *lexer); //
static enum parser_status parse_pipeline(struct lexer *lexer); //
static enum parser_status parse_command(struct lexer *lexer);
static enum parser_status parse_compound_list(struct lexer *lexer); //
static enum parser_status parse_redirection(struct lexer *lexer); //
static enum parser_status parse_prefix(struct lexer *lexer); //
static enum parser_status parse_element(struct lexer *lexer); //
static enum parser_status parse_simple_command(struct lexer *lexer); //
static enum parser_status parse_word(struct lexer *lexer); //
static enum parser_status parse_shell_command(struct lexer *lexer); //
static enum parser_status parse_rule_if(struct lexer *lexer); //
static enum parser_status parse_rule_else(struct lexer *lexer); //
static enum parser_status parse_rule_while(struct lexer *lexer);
static enum parser_status parse_rule_until(struct lexer *lexer);

// this function skips all consecutive EOL
static void skip_EOL(struct lexer *lexer)
{
	while (lexer_peek(lexer).type == TOKEN_EOL)
		lexer_pop(lexer);
}

static int is_redirection(struct token t)
{
    return t.type == TOKEN_REDIRECT_INPUT 
	    || t.type == TOKEN_REDIRECT_OUTPUT || t.type == TOKEN_APPEND_OUTPUT 
	    || t.type == TOKEN_AMPREDIR_OUTPUT || t.type == TOKEN_AMPREDIR_INPUT 
	    || t.type == TOKEN_FORCE_OUTPUT_REDIR;
}

enum parser_status parse_input(struct lexer *lexer)
{
    if (parse_list(lexer) == PARSER_OK)
    {
        if (lexer_peek(lexer).type == TOKEN_EOL || lexer_peek(lexer).type == TOKEN_EOF)
            return PARSER_OK;
    }

    if (lexer_peek(lexer).type == TOKEN_EOL || lexer_peek(lexer).type == TOKEN_EOF)
        return PARSER_OK;
    
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_list(struct lexer *lexer)
{
    if (parse_and_or(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    
    struct token next = lexer_peek(lexer);
    while (next.type == TOKEN_SEMICOLON)
    {
        lexer_pop(lexer);
        if (parse_and_or(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        next = lexer_peek(lexer);
    }

    return PARSER_OK;
}

static enum parser_status parse_and_or(struct lexer *lexer)
{
    if (parse_pipeline(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    
    struct token next = lexer_peek(lexer);
    while (next.type == TOKEN_AND || next.type == TOKEN_OR)
    {
        lexer_pop(lexer);
        skip_EOL(lexer); 
        if (parse_pipeline(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        next = lexer_peek(lexer);
    }

    return PARSER_OK;
}

static enum parser_status parse_pipeline(struct lexer *lexer)
{
    struct token next = lexer_peek(lexer);
    if (next.type == TOKEN_NOT)
        lexer_pop(lexer);
    
    if (parse_command(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    
    next = lexer_peek(lexer);
    while (next.type == TOKEN_PIPE)
    {
        lexer_pop(lexer);
        skip_EOL(lexer);
        if (parse_command(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        next = lexer_peek(lexer);
    }

    return PARSER_OK;
}

static enum parser_status parse_command(struct lexer *lexer)
{
    if (parse_simple_command(lexer) == PARSER_OK)
        return PARSER_OK;

    if (parse_shell_command(lexer) == PARSER_OK)
    {
        struct token next = lexer_peek(lexer);
        while (is_redirection(next))
        {
            if (parse_redirection(lexer) != PARSER_OK)
                return PARSER_UNEXPECTED_TOKEN;
            next = lexer_pop(lexer);
        }
        if (next.type == TOKEN_WORD)
            free(next.value);
        return PARSER_OK;
    }

    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_simple_command(struct lexer *lexer)
{
    int nb_prefix = 0;
    while (parse_prefix(lexer) == PARSER_OK)
	    nb_prefix ++;

    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
	    lexer_pop(lexer);
	    while (parse_element(lexer) == PARSER_OK)
		    continue;
	    return PARSER_OK;
    }
    return nb_prefix >= 1 ? PARSER_OK : PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_rule_while(struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_WHILE)
        return PARSER_UNEXPECTED_TOKEN;
    
    lexer_pop(lexer);
    if (parse_compound_list(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    
    if (lexer_peek(lexer).type != TOKEN_DO)
        return PARSER_UNEXPECTED_TOKEN;
    
    lexer_pop(lexer);
    if (parse_compound_list(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    if (lexer_peek(lexer).type != TOKEN_DONE)
        return PARSER_UNEXPECTED_TOKEN;
    
    lexer_pop(lexer);
    return PARSER_OK;
}

static enum parser_status parse_rule_until(struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_UNTIL)
        return PARSER_UNEXPECTED_TOKEN;
    
    lexer_pop(lexer);
    if (parse_compound_list(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    
    if (lexer_peek(lexer).type != TOKEN_DO)
        return PARSER_UNEXPECTED_TOKEN;
    
    lexer_pop(lexer);
    if (parse_compound_list(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    
    if (lexer_peek(lexer).type != TOKEN_DONE)
        return PARSER_UNEXPECTED_TOKEN;
    
    lexer_pop(lexer);
    return PARSER_OK;
}

static enum parser_status parse_rule_else(struct lexer *lexer)
{ 
    if (lexer_peek(lexer).type == TOKEN_ELSE)
    {
        lexer_pop(lexer);
        if (parse_compound_list(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        return PARSER_OK;
    }
    else if (lexer_peek(lexer).type == TOKEN_ELIF)
    {
        lexer_pop(lexer);
        if (parse_compound_list(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        if (lexer_peek(lexer).type != TOKEN_THEN)
            return PARSER_UNEXPECTED_TOKEN;
        lexer_pop(lexer);
        if (parse_compound_list(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        
	parse_rule_else(lexer); // we do not care about the return value
           
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_compound_list(struct lexer *lexer)
{
    struct token next;

    skip_EOL(lexer);
    
    if (parse_and_or(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    
    next = lexer_peek(lexer);
    while (next.type == TOKEN_SEMICOLON || next.type == TOKEN_EOL)
    {
        lexer_pop(lexer);

	skip_EOL(lexer);

        if (parse_and_or(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        next = lexer_peek(lexer);
    }

    if (next.type == TOKEN_SEMICOLON)
        lexer_pop(lexer);
    
    skip_EOL(lexer); 
    
    return PARSER_OK;
}

static enum parser_status parse_rule_if(struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_IF)
        return PARSER_UNEXPECTED_TOKEN;

    lexer_pop(lexer);
    if (parse_compound_list(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    if (lexer_peek(lexer).type != TOKEN_THEN)
        return PARSER_UNEXPECTED_TOKEN;

    lexer_pop(lexer);
    if (parse_compound_list(lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

	
    parse_rule_else(lexer);

    if (lexer_peek(lexer).type != TOKEN_FI)
        return PARSER_UNEXPECTED_TOKEN;

    lexer_pop(lexer);    
    return PARSER_OK;
}

static enum parser_status parse_shell_command(struct lexer *lexer)
{
    if (parse_rule_if(lexer) == PARSER_OK)
        return PARSER_OK;
    
    if (parse_rule_while(lexer) == PARSER_OK)
        return PARSER_OK;
    
    if (parse_rule_until(lexer) == PARSER_OK)
        return PARSER_OK;
    
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_redirection(struct lexer *lexer)
{
    struct token next = lexer_peek(lexer);

    if (is_redirection(next))
    {
        lexer_pop(lexer);
        if (lexer_peek(lexer).type != TOKEN_WORD)
            return PARSER_UNEXPECTED_TOKEN;
        lexer_pop(lexer);
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_prefix(struct lexer *lexer)
{
    return parse_prefix(lexer);
}

static enum parser_status parse_element(struct lexer *lexer)
{
    struct token next = lexer_peek(lexer);
    if (next.type == TOKEN_WORD)
    {
        if (parse_word(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        return PARSER_OK;
    }
    else if (is_redirection(next))
    {
        if (parse_redirection(lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_word(struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_WORD)
        return PARSER_UNEXPECTED_TOKEN;
    
    return PARSER_OK;
} 
