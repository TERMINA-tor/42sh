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