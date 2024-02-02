import subprocess as sp
import pytest
import os
import sys
import re
shell = "./src/42sh"
path = "tests/"
path_dquotes = path + "step2/"

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

def test_bad_weak3():
    file_path = path_dquotes + "bad_weak3"
    return compare(run_ref(file_path), run_cmd(file_path))
