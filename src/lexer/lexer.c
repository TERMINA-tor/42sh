#include "lexer.h"

#include <ctype.h>
#include <fnmatch.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// compare le mot avec les tokens

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

// créer un lexer

struct lexer *lexer_new(const char *input)
{
    struct lexer *lex = malloc(sizeof(struct lexer));
    if (!lex)
        return NULL;

    lex->input = input;
    lex->pos = 0;

    return lex;
}

// libérer le lexer
void lexer_free(struct lexer *lexer)
{
    if (!lexer)
        return;
    free(lexer);
}

/*
    récupérer le prochain token dans le lexer (sans le supprimer) et le renvoyer
    si le lexer est NULL, renvoyer un token d'erreur
    si le lexer est vide, renvoyer un token EOF
    si le lexer est plein, renvoyer un token d'erreur
    si le token est un mot, renvoyer un token WORD

    la fonction skip_spaces permet de passer les espaces dans le lexer
    la create_token permet de créer un token
    la handle_special_chars permet de gérer les caractères spéciaux
    la handle_quote permet de gérer les quotes
    la handle_word permet de gérer les mots

    la fonction parse_input_for_tok permet de récupérer le prochain token dans
   le lexer
*/

void skip_spaces(struct lexer *lexer)
{
    while (lexer->input[lexer->pos] == ' ')
        lexer->pos++;
}

struct token create_token(enum token_type type, char *value)
{
    struct token tok;
    tok.type = type;
    tok.value = value;
    return tok;
}

struct token handle_special_chars(struct lexer *lexer)
{
    char *temp = calloc(1, sizeof(char));
    if (!temp)
    {
        fprintf(stderr, "Error: calloc failed\n");
        exit(1);
    }

    temp[0] = lexer->input[lexer->pos];
    temp[1] = '\0';

    struct token tok = create_token(input_token(temp), temp);
    lexer->pos++;
    return tok;
}

struct token handle_quote(struct lexer *lexer)
{
    char *c = calloc(1, sizeof(char));
    if (!c)
    {
        fprintf(stderr, "Error: calloc failed\n");
        exit(1);
    }

    int i = 0;
    lexer->pos++;
    while (lexer->input[lexer->pos] != '\'')
    {
        c[i] = lexer->input[lexer->pos];
        i++;
        lexer->pos++;
    }
    c[i] = '\0';
    return create_token(input_token(c), c);
}

struct token handle_word(struct lexer *lexer)
{
    char *c = calloc(1, sizeof(char));
    if (!c)
    {
        fprintf(stderr, "Error: calloc failed\n");
        exit(1);
    }

    int i = 0;
    while (lexer->input[lexer->pos] != ' ' && lexer->input[lexer->pos] != '\0'
           && lexer->input[lexer->pos] != '\n'
           && lexer->input[lexer->pos] != ';')
    {
        if (lexer->input[lexer->pos] == '\'')
        {
            break;
        }
        c[i] = lexer->input[lexer->pos];
        i++;
        lexer->pos++;
    }
    c[i] = '\0';
    return create_token(input_token(c), c);
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    skip_spaces(lexer);

    if (lexer->input[lexer->pos] == '\0')
    {
        return create_token(TOKEN_EOF, NULL);
    }

    if (lexer->input[lexer->pos] == '\n' || lexer->input[lexer->pos] == '\''
        || lexer->input[lexer->pos] == ';')
    {
        return handle_special_chars(lexer);
    }

    if (lexer->input[lexer->pos - 1] == '\'')
    {
        return handle_quote(lexer);
    }

    return handle_word(lexer);
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