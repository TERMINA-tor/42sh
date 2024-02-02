import subprocess as sp
import pytest
import os
import sys
import re
shell = "./src/42sh"
path = "tests/"
path_builtins = path + "builtins/"

def run_ref(file_path):
    return sp.run(['sh', './' + file_path], capture_output=True, text=True)

def run_cmd(file_path):
    return sp.run([shell, file_path], capture_output=True, text=True)

def compare(output, awaited):
    print(output.stdout, awaited.stdout)
    if (output.returncode != 0):
        assert output.returncode == awaited.returncode
    else:
        assert output.stdout == awaited.stdout

#command_lists

def test_while0():
    file_path = path_builtins + "test_while0.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while1():
    file_path = path_builtins + "test_while1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while2():
    file_path = path_builtins + "test_while2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while3():
    file_path = path_builtins + "test_while3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while4():
    file_path = path_builtins + "test_while4.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while5():
    file_path = path_builtins + "test_while5.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while6():
    file_path = path_builtins + "test_while6.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while7():
    file_path = path_builtins + "test_while7.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while8():
    file_path = path_builtins + "test_while8.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while9():
    file_path = path_builtins + "test_while9.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while10():
    file_path = path_builtins + "test_while10.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while11():
    file_path = path_builtins + "test_while11.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while12():
    file_path = path_builtins + "test_while12.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_while13():
    file_path = path_builtins + "test_while13.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until0():
    file_path = path_builtins + "test_until0.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until1():
    file_path = path_builtins + "test_until1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until2():
    file_path = path_builtins + "test_until2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until3():
    file_path = path_builtins + "test_until3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until4():
    file_path = path_builtins + "test_until4.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until5():
    file_path = path_builtins + "test_until5.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until6():
    file_path = path_builtins + "test_until6.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until7():
    file_path = path_builtins + "test_until7.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_until8():
    file_path = path_builtins + "test_until7.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_break0():
    file_path = path_builtins + "test_break0.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_break1():
    file_path = path_builtins + "test_break1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_break2():
    file_path = path_builtins + "test_break3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_break3():
    file_path = path_builtins + "test_break3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_continue0():
    file_path = path_builtins + "test_continue0.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_continue1():
    file_path = path_builtins + "test_continue1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_continue2():
    file_path = path_builtins + "test_continue3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_continue3():
    file_path = path_builtins + "test_continue3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_export0():
    file_path = path_builtins + "test_export0.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_export1():
    file_path = path_builtins + "test_export1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_export2():
    file_path = path_builtins + "test_export2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
def test_unset0():
    file_path = path_builtins + "test_unset0.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
