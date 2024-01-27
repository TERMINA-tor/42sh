import subprocess as sp
import pytest

def run_command(shell_path, command):
    return sp.run([shell_path, "-c", command], capture_output=True, text=True)

def input_string(cmd):
    # Path to your shell executable
    my_shell = "./src/42sh"
    test_command = cmd

    # Running the command using your shell
    output = run_command(my_shell, test_command)

    # Running the command using a standard POSIX shell for comparison
    ref = run_command("/bin/sh", test_command)

    # Comparing the standard outputs
    assert output.stdout == ref.stdout

    # Comparing the standard error outputs
    # assert output.stderr == ref.stderr
    assert output.returncode == ref.returncode


#simple commands

def test_echo():
    input_string("echo abc")

def test_echo_n():
    input_string("echo -n abc")

def test_echo_multiple_args():
    input_string("echo a b c d e f g")

#if commands

def test_basic_if_else():
    input_string("if true;then false;else false;fi")

def test_one_elif():
    input_string("if false; then true; elif true ;then false;fi")

def test_two_elif():
    input_string("if false; then true; elif false ;then false; elif true; then true;fi")

def test_one_elif_one_else():
    input_string("if false; then true; elif false; then true; else true; fi")

#combinaison

def test_one_if_one_else_echo():
    input_string("if false; then echo one;else echo two;fi")

def test_one_if_two_elif_no_else():
    input_string("if false; then echo a; elif false; then echo b; elif true; then echo c; fi")

def test_echo_as_condition():
    input_string("if echo one; then echo done; fi")

#error

def test_one_wrong_argument():
    output = sp.run(["./src/42sh", "toto"], capture_output=True, text=True)

def test_wrong_argc_number():
    output = sp.run(["./src/42sh", "toto", "tata"], capture_output=True, text=True)
    assert output.returncode == 2

def test_wrong_simple_command():
    output = sp.run(["./src/42sh", "-c", "test"], capture_output=True, text=True)
    assert output.returncode == 2

def test_simple_wrong_if():
    output = sp.run(["./src/42sh", "-c", "if"], capture_output=True, text=True)
    assert output.returncode == 2

def test_missing_semicolon():
    output = sp.run(["./src/42sh", "-c", "if true then echo toto; fi"], capture_output=True, text=True)
    assert output.returncode == 2

def test_missing_then():
    output = sp.run(["./src/42sh", "-c", "if true ;then echo toto;"], capture_output=True, text=True)
    assert output.returncode == 2

def test_missing_fi():
    output = sp.run(["./src/42sh", "-c", "if true; then echo toto;"], capture_output=True, text=True)
    assert output.returncode == 2
