#include "lexer.h"

#include <ctype.h>
#include <fnmatch.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// compare le mot avec les tokens

static enum token_type input_token(char *c)
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

static void skip_spaces(struct lexer *lexer)
{
    while (lexer->input[lexer->pos] && lexer->input[lexer->pos] == ' ')
        lexer->pos++;
}

static struct token create_token(enum token_type type, char *value)
{
    struct token tok;
    tok.type = type;

    if (type == TOKEN_WORD)
        tok.value = value;
    else
    {
	tok.value = NULL;
        free(value);
    }

    return tok;
}

static struct token handle_special_chars(struct lexer *lexer)
{
    char *temp =
        calloc(2, sizeof(char)); // Allocate 2 bytes: one for the character and
                                 // one for the null terminator
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

static struct token handle_quote(struct lexer *lexer)
{
    int quote_length = 0;
    while (lexer->input[lexer->pos + quote_length]
           != '\'') // +1 to skip the opening quote
    {
        quote_length++;
    }

    char *c =
        calloc(quote_length + 1, sizeof(char)); // +1 for the null terminator
    if (!c)
    {
        fprintf(stderr, "Error: calloc failed\n");
        exit(1);
    }

    int i = 0;
    while (lexer->input[lexer->pos] != '\'')
    {
        c[i] = lexer->input[lexer->pos];
        i++;
        lexer->pos++;
    }
    c[i] = '\0';
    return create_token(input_token(c), c);
}

static struct token handle_word(struct lexer *lexer)
{
    int word_length = 0;
    while (lexer->input[lexer->pos + word_length] != ' '
           && lexer->input[lexer->pos + word_length] != '\0'
           && lexer->input[lexer->pos + word_length] != '\n'
           && lexer->input[lexer->pos + word_length] != ';')
    {
        if (lexer->input[lexer->pos + word_length] == '\'')
        {
            break;
        }
        word_length++;
    }

    char *c =
        calloc(word_length + 1, sizeof(char)); // +1 for the null terminator
    if (!c)
    {
        fprintf(stderr, "Error: calloc failed\n");
        exit(1);
    }

    int i = 0;
    while (i < word_length)
    {
        c[i] = lexer->input[lexer->pos];
        i++;
        lexer->pos++;
    }
    c[i] = '\0';
    return create_token(input_token(c), c);
}

// handle comments
static void handle_comment(struct lexer *lexer)
{
    while (lexer->input[lexer->pos] != '\n' && lexer->input[lexer->pos] != '\0')
    {
        lexer->pos++;
    }
}

static struct token handle_newline(struct lexer *lexer)
{
    lexer->pos++;
    return create_token(TOKEN_EOL, NULL);
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    skip_spaces(lexer);

    if (lexer->input[lexer->pos] == '\0')
    {
        return create_token(TOKEN_EOF, NULL);
    }

    if (lexer->input[lexer->pos] == '\n')
    {
        return handle_newline(lexer);
    }

    if (lexer->input[lexer->pos] == '\n' || lexer->input[lexer->pos] == '\''
        || lexer->input[lexer->pos] == ';')
    {
        return handle_special_chars(lexer);
    }

    if (lexer->pos != 0 && lexer->input[lexer->pos - 1] == '\'')
    {
        return handle_quote(lexer);
    }

    if (lexer->input[lexer->pos] == '#')
    {
        handle_comment(lexer);
        return parse_input_for_tok(lexer);
    }

    return handle_word(lexer);
}

struct token lexer_peek(struct lexer *lexer)
{
    ssize_t pos = lexer->pos;
    struct token tok = parse_input_for_tok(lexer);
    if (tok.value)
	    free(tok.value);
    lexer->pos = pos;
    return tok;
}

struct token lexer_pop(struct lexer *lexer)
{
    struct token tok = parse_input_for_tok(lexer);
    return tok;
}
