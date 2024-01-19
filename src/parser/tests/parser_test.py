import subprocess

def test_parser(input_string):
    result = subprocess.run(['./test'], input=input_string, text=True, capture_output=True)
    return result.stdout.rstrip('\n').split('\n')

def test_single_command():
    output = test_parser('echo toto')
    assert output == 'OK'

def test_multiple_commands():
    output = test_parser('echo toto; echo tata')
    assert output == 'OK'

def test_bad_();
    output = test_parser(')