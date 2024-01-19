import subprocess

def run_lexer(input_string):
    result = subprocess.run(['./test'], input=input_string, text=True, capture_output=True)
    return result.stdout.rstrip('\n').split('\n')

def test_single_command():
    output = run_lexer('echo hello')
    assert output == ['echo', 'hello', 'EOF']

def test_multiple_commands():
    output = run_lexer('echo hello; echo world')
    assert output == ['echo', 'hello', ';', 'echo', 'world', 'EOF']

def test_variable_assignment():
    output = run_lexer('x=hello')
    assert output == ['x', '=', 'hello', 'EOF']

def test_variable_usage():
    output = run_lexer('echo $x')
    assert output == ['echo', '$x', 'EOF']

def test_if_statement():
    output = run_lexer('if true; then echo yes; fi')
    assert output == ['if', 'true', ';', 'then', 'echo', 'yes', ';', 'fi', 'EOF']

def test_if_else_statement():
    output = run_lexer('if true; then echo yes; else echo no; fi')
    assert output == ['if', 'true', ';', 'then', 'echo', 'yes', ';', 'else', 'echo', 'no', ';', 'fi', 'EOF']

def test_if_elif_else_statement():
    output = run_lexer('if true; then echo yes; elif false; then echo no; else echo maybe; fi')
    assert output == ['if', 'true', ';', 'then', 'echo', 'yes', ';', 'elif', 'false', ';', 'then', 'echo', 'no', ';', 'else', 'echo', 'maybe', ';', 'fi', 'EOF']

def test_while_loop():
    output = run_lexer('while true; do echo yes; done')
    assert output == ['while', 'true', ';', 'do', 'echo', 'yes', ';', 'done', 'EOF']

def test_until_loop():
    output = run_lexer('until true; do echo yes; done')
    assert output == ['until', 'true', ';', 'do', 'echo', 'yes', ';', 'done', 'EOF']

def test_nested_loops():
    output = run_lexer('for i in 1 2 3; do while true; do echo $i; done; done')
    assert output == ['for', 'i', 'in', '1', '2', '3', ';', 'do', 'while', 'true', ';', 'do', 'echo', '$i', ';', 'done', ';', 'done', 'EOF']

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
    output = run_lexer('echo hello#this is a comment')
    assert output == ['echo', 'hello', 'EOF']