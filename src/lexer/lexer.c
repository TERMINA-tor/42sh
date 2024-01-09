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
    struct token *tok = calloc(1, sizeof(struct token));
    char *string = NULL;
    if (!lexer || !lexer->input)
    {
        goto error;
    }
    else
    {
        string = calloc(1, strlen(lexer->input) + 1);
        if (!string)
        {
            free(tok);
            goto error;
        }
        else
        {
            strcpy(string, lexer->input);
            char *s = strtok(string, " ");
            for (size_t i = 0; i < lexer->pos && s != NULL; i++)
            {
                s = strtok(NULL, " ");
            }
            if (s == NULL)
                tok->type = TOKEN_EOF;
            else
            {
                tok->type = input_token(s);
                tok->value = s;
            }
            //if (tok->type == TOKEN_ERROR)
            //    fprintf(stderr, "Error: %s is not a valid token\n", s);
        }
    }
    struct token result = *tok;
    free(tok);
    free(string);
    return result;

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
    lexer->pos++;
    return tok;
}
