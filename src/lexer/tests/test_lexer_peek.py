import subprocess as sp

def test_lexer_peek_one_word():
    output = sp.run(["./eval_peek", "echo"], capture_output=True, text=True)
    expected_output = "word\n" * len("echo")
    assert output.stdout == expected_output

def test_lexer_peek_two_word():
    output = sp.run(["./eval_peek", "echo abc"], capture_output=True, text=True)
    expected_output = "word\n" * len("echo abc")
    assert output.stdout == expected_output

def test_lexer_peek_multiple_word():
    output = sp.run(["./eval_peek", "echo salut salut salut je sais pas salut"], capture_output=True, text=True)
    expected_output = "word\n" * len("echo salut salut salut je sais pas salut")
    assert output.stdout == expected_output

def test_lexer_peek_if_else():
    output = sp.run(["./eval_peek", "if true ;then else ;fi"], capture_output=True, text=True)
    expected_output = "if\n" * len("if true ;then else ;fi")
    assert output.stdout == expected_output

def test_lexer_peek_semicolon():
    output = sp.run(["./eval_peek", "command1; command2"], capture_output=True, text=True)
    expected_output = "word\n" * len("command1; command2")
    assert output.stdout == expected_output

def test_lexer_peek_quote():
    output = sp.run(["./eval_peek", "echo 'Hello, World'"], capture_output=True, text=True)
    expected_output = "word\n" * len("echo 'Hello, World'")
    assert output.stdout == expected_output

def test_lexer_peek_elif():
    output = sp.run(["./eval_peek", "if true then elif true then else ;fi"], capture_output=True, text=True)
    expected_output = "if\n" * len("if true then elif true then else ;fi")
    assert output.stdout == expected_output

def test_lexer_peek_apostrophe_inside_word():
    output = sp.run(["./eval_peek", "echo aa'aa'"], capture_output=True, text=True)
    expected_output = "word\n" * len("echo aa'aa'")
    assert output.stdout == expected_output
    
def test_lexer_peek_if():
    output = sp.run(["./eval_peek", "if true then fi"], capture_output=True, text=True)
    expected_output = "if\n" * len("if true then fi")
    assert output.stdout == expected_output

def test_lexer_peek_if_elif_else():
    output = sp.run(["./eval_peek", "if true; then elif true; then else ; fi"], capture_output=True, text=True)
    expected_output = "if\n" * len("if true; then elif true; then else ; fi")
    assert output.stdout == expected_output

def test_lexer_peek_if_elif_elif_else():
    output = sp.run(["./eval_peek", "if true; then elif true; then elif true; then else ; fi"], capture_output=True, text=True)
    expected_output = "if\n" * len("if true; then elif true; then elif true; then else ; fi")
    assert output.stdout == expected_output

def test_lexer_peek_if_elif_elif_elif_else():
    output = sp.run(["./eval_peek", "if true; then elif true; then elif true; then elif true; then else ; fi"], capture_output=True, text=True)
    expected_output = "if\n" * len("if true; then elif true; then elif true; then elif true; then else ; fi")
    assert output.stdout == expected_output