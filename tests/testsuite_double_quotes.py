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

#missing quote

def test_bad_weak1():
    file_path = path_dquotes + "bad_weak1"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_weak2():
    file_path = path_dquotes + "bad_weak2"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_weak3():
    file_path = path_dquotes + "bad_weak3"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_weak4():
    file_path = path_dquotes + "bad_weak4"
    return compare(run_ref(file_path), run_cmd(file_path))

#full quoting

def test_simple_full_quoting():
    file_path = path_dquotes + "simple_full_quoting"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_strong_quoting():
    file_path = path_dquotes + "simple_strong_quoting"
    return compare(run_ref(file_path), run_cmd(file_path))

#empty quotes

def test_empty_quotes():
    file_path = path_dquotes + "empty_quotes1"
    return compare(run_ref(file_path), run_cmd(file_path))

#escaped quotes

def test_all_quotes1():
    file_path = path_dquotes + "all_quotes_1"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_all_quotes_2():
    file_path = path_dquotes + "all_quotes_2"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_all_quotes_3():
    file_path = path_dquotes + "all_quotes_3"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_basic_quote_3():
    file_path = path_dquotes + "basic_quote_3"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_dollar_quote():
    file_path = path_dquotes + "dollar_quote"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_dollar_quote2():
    file_path = path_dquotes + "dollar_quote2"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_dollar_quote3():
    file_path = path_dquotes + "dollar_dollar_quote3"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_embedded_quote_1():
    file_path = path_dquotes + "embedded_quote_1"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_embedded_quote_2():
    file_path = path_dquotes + "embedded_quote_2"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_escape1():
    file_path = path_dquotes + "escape1"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_escape_comment():
    file_path = path_dquotes + "escape_comment"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_escape_in_weak():
    file_path = path_dquotes + "escape_in_weak"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_escape_newline1():
    file_path = path_dquotes + "escape_newline1"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_escape_newline2():
    file_path = path_dquotes + "escape_newline2"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_escape_newline3():
    file_path = path_dquotes + "escape_newline3"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_escape_newline4():
    file_path = path_dquotes + "escape_newline4"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_hard_quotes():
    file_path = path_dquotes + "hard_quotes"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_hard_var_4():
    file_path = path_dquotes + "hard_var_4"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_many_vars():
    file_path = path_dquotes + "many_vars"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_multi_line():
    file_path = path_dquotes + "multi_line"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_semicolon_in_arg():
    file_path = path_dquotes + "semicolon_in_arg"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_special_quote_1():
    file_path = path_dquotes + "special_quote_1"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_special_quote_2():
    file_path = path_dquotes + "special_quote_2"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_weird_args():
    file_path = path_dquotes + "weird_args"
    return compare(run_ref(file_path), run_cmd(file_path))
