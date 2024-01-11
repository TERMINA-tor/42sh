#include "lexer.h"

#include <ctype.h>
#include <fnmatch.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//compare le mot avec les tokens

enum token_type input_token(char *c)
{
    if (strcmp(c, "if") == 0)
        return TOKEN_IF;
    else if (strcmp(c, "else") == 0)
        return TOKEN_ELSE;
    else if (strcmp(c, "elif") == 0)
        return TOKEN_ELIF;
    else if (strcmp(c, "then") == 0)
        return TOKEN_THEN;
 	else if (strcmp(c, "fi") == 0)
		return TOKEN_FI;
 	else if (strcmp(c, "'") == 0)
        return TOKEN_QUOTE;
    else if (strcmp(c, "\n") == 0)
        return TOKEN_EOL;
    else if (strcmp(c, "") == 0)
        return TOKEN_EOF;
    else if (strcmp(c, ";") == 0)
        return TOKEN_SEMICOLON;
    else
        return TOKEN_WORD;

}


//créer un lexer

struct lexer *lexer_new(const char *input)
{
    struct lexer *lex = malloc(sizeof(struct lexer));
    if (!lex)
        return NULL;

    lex->input = input;
    lex->pos = 0;

    return lex;
}


//libérer le lexer
void lexer_free(struct lexer *lexer)
{
    if (!lexer)
        return;
    free(lexer);
}


//récupérer le prochain token dans le lexer (sans le supprimer) et le renvoyer
//si le lexer est NULL, renvoyer un token d'erreur
//si le lexer est vide, renvoyer un token EOF
//si le lexer est plein, renvoyer un token d'erreur
//si le token est un mot, renvoyer un token WORD

struct token parse_input_for_tok(struct lexer *lexer)
{
    while (lexer->input[lexer->pos] == ' ')
        lexer->pos++;

    struct token *tok = calloc(1, sizeof(struct token));
    if (!tok)
        goto error;

    if (lexer->input[lexer->pos] == '\0')
    {
        tok->type = TOKEN_EOF;
        tok->value = NULL;

        return *tok;
    }

    if (lexer->input[lexer->pos] == '\n' || lexer->input[lexer->pos] == '\'' || lexer->input[lexer->pos] == ';')
    {
        char temp[2] = {lexer->input[lexer->pos], '\0'};
    
        tok->type = input_token(temp);
        tok->value = temp;

        struct token ret = *tok;
        free(tok);
        lexer->pos++;
        return ret;
    }

    //loop thru input until we find a token
    //if we find a token, return it

    char *c = calloc(1, sizeof(char));
    if (!c)
        goto error;
    
    int i = 0;
    while (lexer->input[lexer->pos] != ' ' && lexer->input[lexer->pos] != '\0' && lexer->input[lexer->pos] != '\n' && lexer->input[lexer->pos] != '\'' && lexer->input[lexer->pos] != ';')
    {
        c[i] = lexer->input[lexer->pos];
        i++;
        lexer->pos++;
    }
    c[i] = '\0';

    tok->type = input_token(c);
    tok->value = c;

    struct token ret = *tok;
    free(tok);
    return ret;

    error:
        fprintf(stderr, "Error: calloc failed\n");
        exit(1);
}

struct token lexer_peek(struct lexer *lexer)
{
    return parse_input_for_tok(lexer);
}

struct token lexer_pop(struct lexer *lexer)
{
    struct token tok = parse_input_for_tok(lexer);
    return tok;
}
