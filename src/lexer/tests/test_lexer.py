import subprocess as sp
import pytest

lexer = "./tests/test"
path = "./tests/test_files/"

eol = "TOKEN_EOL\n"
eof = "TOKEN_EOF\n"

def run_ref(shell_path, file):
    return sp.run([shell_path, path + file], capture_output=True, text=True)

def run_command(shell_path, file):
    return sp.run([shell_path, "-e", path + file], capture_output=True, text=True)

def assertion(output, awaited):
    assert output.stdout == awaited

#simple tests

def test_empty():
    assertion(run_command(lexer, "simple_test_0"), "TOKEN_EOF\n")

def test_simple_1():
    assertion(run_command(lexer, "simple_test_1"), "echo\ntoto\nTOKEN_EOL\nTOKEN_EOF\n")

def test_simple_2():
    assertion(run_command(lexer, "simple_test_2"), "echo\ntoto\ntata\nTOKEN_EOL\nTOKEN_EOF\n")

def test_simple3():
    awaited = "hello\n"+eol+"this\n"+eol+"is\n"+eol+"a\n"+eol+"test\n"+eol+eol+eof
    assertion(run_command(lexer, "simple_test_3"), awaited)

def test_simple4():
    awaited= "Oct\n31\nTOKEN_SEMICOLON\n==\n" + eol + "Dec\n25\n" + eol + eof
    assertion(run_command(lexer, "simple_test_4"), awaited)

# special tokens

def test_token_and():
    awaited = "word1\nTOKEN_AND\nwrd2\n" + eol + eof
    assertion(run_command(lexer, "test_token_and"), awaited)

def test_token_ampredir_input():
    awaited = "word1\nTOKEN_AMPREDIR_INPUT\nword2\n" + eol + eof
    assertion(run_command(lexer, "test_token_ampredir_input"), awaited)

def test_token_ampredir_output():
    awaited = "word1\nTOKEN_AMPREDIR_OUTPUT\nword2\n" + eol + eof
    assertion(run_command(lexer, "test_token_ampredir_output"), awaited)

def test_test_token_append_output():
    awaited = "word1\nTOKEN_APPEND\nword2\n" + eol + eof
    assertion(run_command(lexer, "test_token_append_output"), awaited)

def test_token_force_output_redir():
    awaited = "word1\nTOKEN_FORCE_REDIR\nword2\n" + eol + eof
    assertion(run_command(lexer, "test_token_force_output_redir"), awaited)

def test_token_newline():
    assertion(run_command(lexer, "test_token_newline"), eol + eol + eof)

def test_token_or():
    awaited = "word1\nTOKEN_OR\nword2\n" + eol + eof
    assertion(run_command(lexer, "test_token_or"), awaited)

def test_token_pipe():
    awaited = "word1\nTOKEN_PIPE\nword2\n" + eol + eof
    assertion(run_command(lexer, "test_token_pipe"), awaited)

def test_token_redirect_input():
    awaited = "word1\nTOKEN_REDIR_LEFT\nword2\n" + eol + eof
    assertion(run_command(lexer, "test_token_redirect_input"), awaited)

def test_token_redirect_input():
    awaited = "word1\nTOKEN_REDIR_RIGHT\nword2\n" + eol + eof
    assertion(run_command(lexer, "test_token_redirect_output"), awaited)

def test_token_semicolon():
    awaited = "hello\n"+eol+"TOKEN_SEMICOLON\n" + eol + eof
    assertion(run_command(lexer, "test_token_semicolon"), awaited)

def test_simple_if_case():
    awaited = "TOKEN_IF\ntrue\nTOKEN_SEMICOLON\nTOKEN_THEN\necho\ntoto\nTOKEN_SEMICOLON\nTOKEN_ELIF\ntrue\nTOKEN_SEMICOLON\nTOKEN_THEN\necho\ntata\nTOKEN_SEMICOLON\nTOKEN_ELSE\necho\ntiti\nTOKEN_SEMICOLON\nTOKEN_FI\n" + eol + eof
    assertion(run_command(lexer, "test_simple_if_case"), awaited)

def test_simple_quote():
    awaited = "echo\n\'bonjourceci$est\\un\\ntest\'\n" + eol + eof
    assertion(run_command(lexer, "test_simple_quote"), awaited)

def test_double_quote():
    awaited = "echo\n\"bonjourceci$est\\un\\ntest\"\n" + eol + eof
    assertion(run_command(lexer, "test_double_quote"), awaited)

#random tests

def test_random_1():
    output_lexer = run_command(lexer, "test_random_1")
    output_ref = run_ref("cat", "test_random_1_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_2():
    output_lexer = run_command(lexer, "test_random_2")
    output_ref = run_ref("cat", "test_random_2_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_3():
    output_lexer = run_command(lexer, "test_random_3")
    output_ref = run_ref("cat", "test_random_3_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_4():
    output_lexer = run_command(lexer, "test_random_4")
    output_ref = run_ref("cat", "test_random_4_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_5():
    output_lexer = run_command(lexer, "test_random_5")
    output_ref = run_ref("cat", "test_random_5_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_6():
    output_lexer = run_command(lexer, "test_random_6")
    output_ref = run_ref("cat", "test_random_6_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_7():
    output_lexer = run_command(lexer, "test_random_7")
    output_ref = run_ref("cat", "test_random_7_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_8():
    output_lexer = run_command(lexer, "test_random_8")
    output_ref = run_ref("cat", "test_random_8_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_9():
    output_lexer = run_command(lexer, "test_random_9")
    output_ref = run_ref("cat", "test_random_9_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_10():
    output_lexer = run_command(lexer, "test_random_10")
    output_ref = run_ref("cat", "test_random_10_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_11():
    output_lexer = run_command(lexer, "test_random_11")
    output_ref = run_ref("cat", "test_random_11_ref")
    assertion(output_lexer, output_ref.stdout)

def test_random_12():
    output_lexer = run_command(lexer, "test_random_12")
    output_ref = run_ref("cat", "test_random_12_ref")
    assertion(output_lexer, output_ref.stdout)

def test_blank_mostlty():
    ouput_lexer = run_command(lexer, "test_mostly_blank")
    output_ref = run_ref("cat", "test_mostly_blank_ref")

def test_only_blank():
    ouput_lexer = run_command(lexer, "test_only_blank")
    output_ref = run_ref("cat", "test_only_blank_ref")

def test_only_blank_and_comments():
    ouput_lexer = run_command(lexer, "test_blank_and_comments")
    output_ref = run_ref("cat", "test_blank_and_comments_ref")
