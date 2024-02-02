import subprocess as sp
import pytest
import os
import sys
import re

shell = "./src/42sh"
path3 = "tests/step3/"
path_cd = path3 + "builtins/"

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

def test_cd_current():
    file_path = path_cd + "test_cd_now.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_cd_previous():
    file_path = path_cd + "test_cd_previous.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_cd_root():
    file_path = path_cd + "test_cd_root.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_cd_user():
    file_path = path_cd + "test_cd_user.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_cd_many_double_dot():
    file_path = path_cd + "test_cd_many_double_dot.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_cd_minus():
    file_path = path_cd + "test_cd_minus.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
