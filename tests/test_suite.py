import subprocess as sp
import pytest
import os
import sys
shell = "./src/42sh"
path = "step1/"
path_cmd_list = path + "command_lists/"
path_comments = path + "comments/"
path_if_commands = path + "if_commands/"
path_quotes = path + "quotes/"
path_simple_cmd = path + "simple_commands/"

def run_ref(file_path):
    return sp.run(['sh', './' + file_path], capture_output=True, text=True)

def run_cmd(file_path):
    return sp.run([shell, file_path], capture_output=True, text=True)

def compare(output, awaited):
    assert output.stdout == awaited.stdout

#command_lists

def test_many_commands1():
    file_path = path_cmd_list + "test_many_commands1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

test_many_commands1()
