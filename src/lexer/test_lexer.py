import subprocess

def run_lexer(input_string):
    result = subprocess.run(['./test'], input=input_string, text=True, capture_output=True)
    return result.stdout.split('\n')

def test_if_statement():
    output = run_lexer('if true; then echo yes; fi')
    assert output == ['if', 'true', ';', 'then', 'echo', 'yes', ';', 'fi', 'EOF']

def test_while_loop():
    output = run_lexer('while true; do echo yes; done')
    assert output == ['while', 'true', ';', 'do', 'echo', 'yes', ';', 'done', 'EOF']

def test_for_loop():
    output = run_lexer('for i in 1 2 3; do echo $i; done')
    assert output == ['for', 'i', 'in', '1', '2', '3', ';', 'do', 'echo', '$i', ';', 'done', 'EOF']

def test_redirection():
    output = run_lexer('echo hello > file.txt')
    assert output == ['echo', 'hello', '>', 'file.txt', 'EOF']

def test_pipes():
    output = run_lexer('cat file.txt | grep hello')
    assert output == ['cat', 'file.txt', '|', 'grep', 'hello', 'EOF']

def test_comments():
    output = run_lexer('echo hello # this is a comment')
    assert output == ['echo', 'hello', '#', 'this is a comment', 'EOF']