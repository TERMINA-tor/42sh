import subprocess as sp
import pytest
import os
import sys
import re
shell = "./src/42sh"
path = "tests/step1/"
path_redirs = path + "redirs/"
path_pipelines = path + "pipelines/"

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

# test redirs
        
def test_bad_only_output_redir():
    file_path = path_redirs + "bad_only_output_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_only_input_redir():
    file_path = path_redirs + "bad_only_input_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_only_append_redir():
    file_path = path_redirs + "bad_only_append_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_output_redir():
    file_path = path_redirs + "bad_output_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_input_redir():
    file_path = path_redirs + "bad_input_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_append_redir():
    file_path = path_redirs + "bad_append_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_greater_and_redir():
    file_path = path_redirs + "bad_greater_and_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_output_pipe_redir():
    file_path = path_redirs + "bad_output_pipe.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_multiple_redir():
    file_path = path_redirs + "bad_multiple_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_only_output():
    file_path = path_redirs + "only_output_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_only_input():
    file_path = path_redirs + "only_input_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_only_append():
    file_path = path_redirs + "only_append_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_multiple_redir():
    file_path = path_redirs + "multiple_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_output_redir():
    file_path = path_redirs + "output_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_input_redir():
    file_path = path_redirs + "input_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_append_output_redir():
    file_path = path_redirs + "append_output_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_greater_and_redir():
    file_path = path_redirs + "greater_and_redir.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_output_pipe_redir():
    file_path = path_redirs + "output_pipe.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

# test pipelines

def test_bad_only_pipe():
    file_path = path_pipelines + "bad_only_pipe.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_pipeline_begin():
    file_path = path_pipelines + "bad_pipeline_begin.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_pipeline_end():
    file_path = path_pipelines + "bad_pipeline_end.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_multiple_pipes():
    file_path = path_pipelines + "bad_multiple_pipes.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_pipeline():
    file_path = path_pipelines + "pipeline1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_if_pipeline():
    file_path = path_pipelines + "if_pipe.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_multiple_pipes():
    file_path = path_pipelines + "multiple_pipes.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_pipe_redir1():
    file_path = path_pipelines + "pipe_redir1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_pipe_redir2():
    file_path = path_pipelines + "pipe_redir2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))