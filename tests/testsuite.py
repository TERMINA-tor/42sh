import subprocess as sp
import pytest

def run_command(shell_path, command):
    return sp.run([shell_path, "-c", command], capture_output=True, text=True)

def input_string(cmd):
    # Path to your shell executable
    my_shell = "./42sh"
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

def test_echo_ne():
    input_string("echo -ne abc")

def test_echo_e():
    input_string("echo -e abc")

def test_echo_n():
    input_string("echo -n abc")

def test_echo_multiple_args():
    input_string("echo -ne a b c d e f g")

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
