To compile:
    make
    => Will produce ./eval_token, which when given exactly one argument, will print the tokens.

NOTE:
    will leak since char *value in token struct is calloc-ed, should be freed
