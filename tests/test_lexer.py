import subprocess as sp

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
    output = sp.run(["./eval_token", "if true then else fi"], capture_output=True, text=True)
    expected_output = "if\nword\nthen\nelse\nfi\nEOF\n"
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
    output = sp.run(["./eval_token", "if true then elif true then else fi"], capture_output=True, text=True)
    expected_output = "if\nword\nthen\nelif\nword\nthen\nelse\nfi\nEOF\n"
    assert output.stdout == expected_output

def test_lexer_apostrophe_inside_word():
    output = sp.run(["./eval_token", "echo aa'aa'"], capture_output=True, text=True)
    expected_output = "word\nword\n'\nword\n'\nEOF\n"
    assert output.stdout == expected_output