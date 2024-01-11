import subprocess as sp

def test_input_string():
    output = sp.run(["./42sh", "-c", "abc"], capture_output=True, text=True)
    assert output.stdout == "abc\n"

def test_input_file():
    output = sp.run(["./42sh", "-e","tests/toto"], capture_output=True, text=True)
    print(output)
    assert output.stdout == "abc\n"

def test_input_stdin():
    sh = sp.Popen(["./42sh"], stdin=sp.PIPE, stdout=sp.PIPE)
    output = sh.communicate(input = "abc".encode())[0]
    output = output.decode()
    assert output == "abc\n"

def test_input_EOF():
        sh = sp.Popen(["./42sh"], stdin=sp.PIPE, stdout=sp.PIPE)
        output = sh.communicate(input = "".encode())[0]
        output = output.decode()
        assert output == "\n"

