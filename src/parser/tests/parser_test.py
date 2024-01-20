import subprocess

def run_parser(input_string):
    result = subprocess.run(['./test'], input=input_string, text=True, capture_output=True)
    return result.stdout.rstrip('\n').split('\n')

def test_single_command():
    output = run_parser('echo toto')
    assert output[0] == 'OK'

def test_multiple_commands():
    output = run_parser('echo toto; echo tata')
    assert output[0] == 'OK'

def test_if():
    output = run_parser('if echo toto; then echo tata; fi')
    assert output[0] == 'OK'

def test_if_else():
    output = run_parser('if echo toto; then echo tata; else echo tutu; fi')
    assert output[0] == 'OK'

def test_if_else_if():
    output = run_parser('if echo toto; then echo tata; elif echo tutu; then echo tete; fi')
    assert output[0] == 'OK'

def test_bad_if():
    output = run_parser('if echo toto then echo tata; fi')
    assert output[0] == 'error'

def test_bad_if_else():
    output = run_parser('if echo toto; then echo tata else echo tutu; fi')
    assert output[0] == 'error'

def test_bad_if_else_if():
    output = run_parser('if echo toto; then echo tata; elif echo tutu then echo tete; fi')
    assert output[0] == 'error'

def test_while():
    output = run_parser('while echo toto; do echo tata; done')
    assert output[0] == 'OK'

def test_bad_while():
    output = run_parser('while echo toto; do echo tata done')
    assert output[0] == 'error'

def test_for():
    output = run_parser('for i in 1 2 3; do echo $i; done')
    assert output[0] == 'OK'

def test_bad_for():
    output = run_parser('for i in 1 2 3; do echo $i done')
    assert output[0] == 'error'

def test_nested_for():
    output = run_parser('for i in 1 2 3; do for j in 1 2 3; do echo $i $j; done; done')
    assert output[0] == 'OK'

def test_nested_while():
    output = run_parser('while echo toto; do while echo tata; do echo tutu; done; done')
    assert output[0] == 'OK'

def test_nested_for_while():
    output = run_parser('for i in 1 2 3; do while echo tata; do echo tutu; done; done')
    assert output[0] == 'OK'

def test_nested_while_for():
    output = run_parser('while echo toto; do for i in 1 2 3; do echo $i; done; done')
    assert output[0] == 'OK'

def test_bad_nested_for():
    output = run_parser('for i in 1 2 3; do for j in 1 2 3; do echo $i $j done; done')
    assert output[0] == 'error'

def test_bad_nested_while():
    output = run_parser('while echo toto; do while echo tata; do echo tutu done; done')
    assert output[0] == 'error'

def test_bad_nested_for_while():
    output = run_parser('for i in 1 2 3; do while echo tata; do echo tutu done; done')
    assert output[0] == 'error'

def test_bad_nested_while_for():
    output = run_parser('while echo toto; do for i in 1 2 3; do echo $i done; done')
    assert output[0] == 'error'

def test_pipe():
    output = run_parser('echo toto | cat -e')
    assert output[0] == 'OK'

def test_bad_pipe():
    output = run_parser('echo toto | cat -e |')
    assert output[0] == 'error'

def test_redirection():
    output = run_parser('echo toto > file')
    assert output[0] == 'OK'

def test_bad_redirection():
    output = run_parser('echo toto >')
    assert output[0] == 'error'

def test_redirection_append():
    output = run_parser('echo toto >> file')
    assert output[0] == 'OK'

def test_bad_redirection_append():
    output = run_parser('echo toto >>')
    assert output[0] == 'error'

def test_redirection_input():
    output = run_parser('cat < file')
    assert output[0] == 'OK'

def test_bad_redirection_input():
    output = run_parser('cat <')
    assert output[0] == 'error'

def test_negation():
    output = run_parser('! echo toto')
    assert output[0] == 'OK'

def test_bad_negation():
    output = run_parser('! echo toto !')
    assert output[0] == 'error'