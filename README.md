# Instructions

## Compilation

Use the `make` command to compile the project:

```bash
make
This will produce ./eval_token.
When given exactly one argument, it will print the tokens.
```

## Testing
Use the `make test` command to launch a pytest testsuite:

```bash
make test
This will lex various strings (if else, simple commands etc...).
```

# Note
Please be aware that there will be a memory leak since the char *value in the token struct is calloc-ed and should be freed.