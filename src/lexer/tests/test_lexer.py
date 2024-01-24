import subprocess as sp
<<<<<<< HEAD

def test_lexer_one_word():
    output = sp.run(["./eval_token", "echo"], capture_output=True, text=True)
    assert output.stdout == "word\nEOF\n"

def test_lexer_two_word():
    output = sp.run(["./eval_token", "echo abc"], capture_output=True, text=True)
    expected_output = "word\nword\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_multiple_word():
    output = sp.run(["./eval_token", "echo salut salut salut je sais pas salut"], capture_output=True, text=True)
    expected_output = "word\nword\nword\nword\nword\nword\nword\nword\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_if_else():
    output = sp.run(["./eval_token", "if true ;then else ;fi"], capture_output=True, text=True)
    expected_output = "if\nword\n;\nthen\nelse\n;\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_semicolon():
    output = sp.run(["./eval_token", "command1; command2"], capture_output=True, text=True)
    expected_output = "word\n;\nword\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_quote():
    output = sp.run(["./eval_token", "echo 'Hello, World'"], capture_output=True, text=True)
    expected_output = "word\n'\nword\n'\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_elif():
    output = sp.run(["./eval_token", "if true then elif true then else ;fi"], capture_output=True, text=True)
    expected_output = "if\nword\nthen\nelif\nword\nthen\nelse\n;\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_apostrophe_inside_word():
    output = sp.run(["./eval_token", "echo aa'aa'"], capture_output=True, text=True)
    expected_output = "word\nword\n'\nword\n'\nEOF\n"
    assert output.stdout == expected_output
    
def test_lexer_if():
    output = sp.run(["./eval_token", "if true then fi"], capture_output=True, text=True)
    expected_output = "if\nword\nthen\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_if_elif_else():
    output = sp.run(["./eval_token", "if true; then elif true; then else ; fi"], capture_output=True, text=True)
    expected_output = "if\nword\n;\nthen\nelif\nword\n;\nthen\nelse\n;\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_if_elif_elif_else():
    output = sp.run(["./eval_token", "if true; then elif true; then elif true; then else ; fi"], capture_output=True, text=True)
    expected_output = "if\nword\n;\nthen\nelif\nword\n;\nthen\nelif\nword\n;\nthen\nelse\n;\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_if_elif_elif_elif_else():
    output = sp.run(["./eval_token", "if true; then elif true; then elif true; then elif true; then else ; fi"], capture_output=True, text=True)
    expected_output = "if\nword\n;\nthen\nelif\nword\n;\nthen\nelif\nword\n;\nthen\nelif\nword\n;\nthen\nelse\n;\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_comment():
    output = sp.run(["./eval_token", "echo #comment"], capture_output=True, text=True)
    expected_output = "word\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_comment_with_word():
    output = sp.run(["./eval_token", "echo #comment word"], capture_output=True, text=True)
    expected_output = "word\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_comment_words_after_echo():
    output = sp.run(["./eval_token", "echo #comment 'word'"], capture_output=True, text=True)
    expected_output = "word\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_comment_outside_quotes():
    output = sp.run(["./eval_token", "echo 'word' #comment word"], capture_output=True, text=True)
    expected_output = "word\n'\nword\n'\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_comment_inside_quote_and_outside():
    output = sp.run(["./eval_token", "echo 'word #comment' # comment"], capture_output=True, text=True)
    expected_output = "word\n'\nword\n'\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_newline_newline():
    output = sp.run(["./eval_token", "echo tata\n\necho toto"], capture_output=True, text=True)
    expected_output = "word\nword\n\n\nword\nword\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_space_newline():
    output = sp.run(["./eval_token", "if true       \n"], capture_output=True, text=True)
    expected_output = "if\nword\n\n\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_newline_space():
    output = sp.run(["./eval_token", "if      true \n\n       "], capture_output=True, text=True)
    expected_output = "if\nword\n\n\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_newline():
    output = sp.run(["./eval_token", "echo toto\necho tata"], capture_output=True, text=True)
    expected_output = "word\nword\n\n\nword\nword\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_if_newline():
    output = sp.run(["./eval_token", "if true\nthen\nfi"], capture_output=True, text=True)
    expected_output = "if\nword\n\n\nthen\n\n\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_if_else_newline():
    output = sp.run(["./eval_token", "if true\nthen\nelse\nfi"], capture_output=True, text=True)
    expected_output = "if\nword\n\n\nthen\n\n\nelse\n\n\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_if_elif_else_newline():
    output = sp.run(["./eval_token", "if true\nthen\nelif true\nthen\nelse\nfi"], capture_output=True, text=True)
    expected_output = "if\nword\n\n\nthen\n\n\nelif\nword\n\n\nthen\n\n\nelse\n\n\nfi\nEOF\n"
    assert output.stdout == expected_output
=======
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
>>>>>>> main
