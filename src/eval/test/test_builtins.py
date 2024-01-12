import subprocess as sp

def run_echo_command(args):
    """ Utility function to run the echo command with given arguments. """
    cmd = ["./builtin_echo"] + args
    return sp.run(cmd, capture_output=True, text=True)

def test_echo_simple():
    output = run_echo_command(["hello", "world"])
    assert output.stdout == "hello world\n"

def test_echo_with_n_option():
    output = run_echo_command(["-n", "hello", "world"])
    assert output.stdout == "hello world"

def test_echo_with_e_option():
    output = run_echo_command(["-e", "hello\\nworld"])
    assert output.stdout == "hello\nworld\n"

def test_echo_with_E_option():
    output = run_echo_command(["-E", "hello\\nworld"])
    assert output.stdout == "hello\\nworld\n"

def test_echo_with_ne_options():
    output = run_echo_command(["-ne", "hello\\nworld"])
    assert output.stdout == "hello\nworld"

def test_echo_with_nE_options():
    output = run_echo_command(["-nE", "hello\\nworld"])
    assert output.stdout == "hello\\nworld"

