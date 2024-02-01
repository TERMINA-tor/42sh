#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dstring.h"

void canonicalize_path(struct Dstring **curpath)
{
    struct Dstring *temp_path =
        Dstring_new(); // Copie temporaire du chemin à traiter
    Dstring_concat_string(temp_path, *curpath);
    char **canonical_components = (char **)malloc(
        100 * sizeof(char *)); // Tableau dynamique pour stocker les composants
                               // du chemin canonique
    int index_limit = 100;
    int component_count = 0;
    char *token = strtok(temp_path->value, "/"); // Tokenize le chemin par '/'
    while (token != NULL)
    {
        if (strcmp(token, ".") == 0)
        {
            // Ignore les composants "dot"
        }
        else if (strcmp(token, "..") == 0)
        {
            if (component_count > 0)
            { // Remonter d'un niveau pour les composants "dot-dot"
                if (strcmp(canonical_components[component_count - 1], "..") != 0
                    && strcmp(canonical_components[component_count - 1], "/")
                        != 0)
                { // Vérifie si le composant précédent est ni root ni dot-dot
                    free(canonical_components[component_count - 1]);
                    component_count--;
                }
            }
        }
        else
        {
            if (component_count >= index_limit)
            {
                index_limit += 100;
                canonical_components =
                    realloc(canonical_components, index_limit * sizeof(char *));
            }
            canonical_components[component_count++] =
                strdup(token); // Stocke les autres composants
        }
        token = strtok(NULL, "/");
    }
    Dstring_free(*curpath); // Reconstruit le chemin canonique
    struct Dstring *final = Dstring_new();
    if (component_count == 0)
        Dstring_concat(final, "/");
    for (int i = 0; i < component_count; ++i)
    {
        Dstring_concat(final, "/");
        Dstring_concat(final, canonical_components[i]);
    }
    *curpath = final; // Libère la mémoire utilisée pour le tableau dynamique
    for (int i = 0; i < component_count; ++i)
        free(canonical_components[i]);
    free(canonical_components); // Libère la mémoire utilisée pour la copie
                                // temporaire
    Dstring_free(temp_path);
}

void option_cdpath(struct Dstring **curpath, char *input)
{
    char *cdpath =
        getenv("CDPATH"); // Chercher dans CDPATH si le chemin est relatif
    if (cdpath == NULL)
    {
        // CDPATH non défini
        Dstring_concat(*curpath, input);
    }
    else
    {
        char *token = strtok(cdpath, ":");
        while (token != NULL)
        {
            // Construire le chemin résultant
            struct Dstring *temp_path = Dstring_new();
            Dstring_concat(temp_path, token);
            if (temp_path->value[temp_path->size - 1] != '/')
                Dstring_append(temp_path, '/');
            Dstring_concat(temp_path, input);
            // Vérifier si le chemin résultant existe
            if (access(temp_path->value, F_OK) == 0)
            {
                // Le chemin existe, copier le chemin dans result et sortir de
                // la boucle
                Dstring_concat_string(*curpath, temp_path);
                Dstring_free(temp_path);
                break;
            }
            Dstring_free(temp_path);
            // Passer au chemin suivant dans CDPATH
            token = strtok(NULL, ":");
        }
    }
    if (!(*curpath)->value)
        Dstring_concat(*curpath, input);
}

void build_with_pwd(struct Dstring **curpath)
{
    struct Dstring *final = Dstring_new();
    char *pwd = getenv("PWD");
    if (pwd)
    {
        Dstring_concat(final, pwd);
        if (final->value[final->size - 1] != '/')
            Dstring_append(final, '/');
        Dstring_concat_string(final, *curpath);
        Dstring_free(*curpath);
        *curpath = final;
    }
}

int path(struct Dstring **curpath, char *args[], int nb_args)
{
    // Si aucun répertoire n'est spécifié, ou si HOME n'est pas défini
    if (nb_args == 0)
    {
        char *home = getenv("HOME");
        if (home == NULL || strlen(home) == 0)
            Dstring_concat(*curpath, "/home");
        else
            Dstring_concat(*curpath, home);
    }
    else
    {
        char *input = args[1];

        if (strcmp(input, "-") == 0)
        {
            char *oldpwd = getenv("OLDPWD");
            char *pwd = getenv("PWD");
            if (oldpwd == NULL)
            {
                fprintf(stderr, "cd: OLDPWD not set\n");
                return 1;
            }
            printf("%s", oldpwd);
            setenv("OLDPWD", pwd, 1);
            Dstring_concat(*curpath, oldpwd);

            if (chdir((*curpath)->value) != 0)
            { // Définir PWD
                perror("cd");
                Dstring_free(*curpath);
                return 1;
            }

            Dstring_free(*curpath);
            return 2;
        }
        else if (*input == '/')
        {
            Dstring_concat(*curpath, input);
        }
        else
        {
            if (strcmp(input, "..") == 0
                || strcmp(input, ".")
                    == 0) // Si le chemin commence par "." ou ".."
                Dstring_concat(*curpath, input);
            else
                option_cdpath(&(*curpath), input);
        }
    }
    return 0;
}

int builtin_cd(char **args, int nb_args)
{
    if (nb_args > 2) // si trop d'argument
        return 1;

    struct Dstring *curpath = Dstring_new();
    int result = path(&curpath, args, nb_args);
    if (result == 1)
        return 1;
    else if (result == 2)
        return 0;

    if (curpath->value[0] != '/')
        build_with_pwd(&curpath);

    Dstring_append(curpath, '\0');
    canonicalize_path(&curpath);
    Dstring_append(curpath, '\0');
    if (chdir(curpath->value) != 0)
    { // Définir PWD
        perror("cd");
        Dstring_free(curpath);
        return 2;
    }
    else
    {
        setenv("PWD", curpath->value, 1); // Mettre à jour OLDPWD
        const char *oldpwd = getenv("PWD");
        if (oldpwd != NULL)
        {
            setenv("OLDPWD", oldpwd, 1);
        }
    }

    Dstring_free(curpath);
    return 0;
}
