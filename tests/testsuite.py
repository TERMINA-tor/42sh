import subprocess as sp
import pytest
import os
import sys
import re
shell = "./src/42sh"
path = "tests/step1/"
path_cmd_list = path + "command_lists/"
path_comments = path + "comments/"
path_if_commands = path + "if_commands/"
path_quotes = path + "quotes/"
path_simple_cmd = path + "simple_commands/"
path_random = "tests/random/"
path_expansion = path + "expansion/"

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

def test_many_commands1():
    file_path = path_cmd_list + "test_many_commands1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_many_commands1():
    file_path = path_cmd_list + "test_many_commands2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_no_space():
    file_path = path_cmd_list + "test_no_space.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_sheebang():
    file_path = path_cmd_list + "test_sheebang.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_space2():
    file_path = path_cmd_list + "test_space2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_space():
    file_path = path_cmd_list + "test_space.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

# comments

def test_inline_comment():
    file_path = path_comments + "test_inline_comment.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_just_a_comment():
    file_path = path_comments + "test_just_a_comment.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_not_first():
    file_path = path_comments + "test_not_first.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_quoted():
    file_path = path_comments + "test_quoted.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_sheebang2():
    file_path = path_comments + "test_sheebang.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_space_comments():
    file_path = path_comments + "test_space_comments.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

# if_commands

def test_bad_if1():
    file_path = path_if_commands + "test_bad_if1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_if2():
    file_path = path_if_commands + "test_bad_if2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_if3():
    file_path = path_if_commands + "test_bad_if3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_bad_if3():
    file_path = path_if_commands + "test_bad_if3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_colliding_keywords():
    file_path = path_if_commands + "test_colliding_keywords.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_embeded_list1():
    file_path = path_if_commands + "test_embeded_list1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_embeded_list2():
    file_path = path_if_commands + "test_embeded_list2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_if_false_else():
    file_path = path_if_commands + "test_if_false_else.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_if_false():
    file_path = path_if_commands + "test_if_false.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_if_inside_if_semicolon():
    file_path = path_if_commands + "test_if_inside_if_semicolon.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_if_inside_if():
    file_path = path_if_commands + "test_if_inside_if.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_simple_if_elif_semicolon():
    file_path = path_if_commands + "test_simple_if_elif_semicolon.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_simple_if_elif():
    file_path = path_if_commands + "test_simple_if_elif.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_simple_if_semicolon():
    file_path = path_if_commands + "test_simple_if_semicolon.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_simple_if():
    file_path = path_if_commands + "test_simple_if.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

# simple quotes

def test_empty():
    file_path = path_quotes + "test_empty.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_multiple_quotes():
    file_path = path_quotes + "test_multiple_quotes.sh"
    output = run_cmd(file_path)
    assert output.stdout == '1 \\n 2\n'

#def test_reserved_keyword():
#    file_path = path_quotes + "test_reserved_keyword.sh"
#    return compare(run_ref(file_path), run_cmd(file_path))

def test_space_as_arg():
    file_path = path_quotes + "test_space_as_arg.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_space_in_arg():
    file_path = path_quotes + "test_space_in_arg.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

# simple commands

def test_big_arg_1():
    file_path = path_simple_cmd + "test_big_arg_1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_big_arg_2():
    file_path = path_simple_cmd + "test_big_arg_2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_command_spaces():
    file_path = path_simple_cmd + "test_command_spaces.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_exit_code():
    file_path = path_simple_cmd + "test_exit_code.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_many_args():
    file_path = path_simple_cmd + "test_many_args.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

#def test_not_existant1():
#    file_path = path_simple_cmd + "test_not_existant1.sh"
#    return compare(run_ref(file_path), run_cmd(file_path))

def test_one_arg():
    file_path = path_simple_cmd + "test_one_arg.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

#random tests

def test_random1():
    file_path = path_random + "test_random1.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_random2():
    file_path = path_random + "test_random2.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_random2():
    file_path = path_random + "test_random3.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

# test expansion

def test_at():
    file_path = path_expansion + "test_at.sh"
    output = run_cmd(file_path) 
    assert output.stdout != ''

def test_dollar():
    file_path = path_expansion + "test_dollar.sh"
    output = run_ref(file_path)
    print(output.stdout)
    assert (bool(re.match(r"[{]?[0-9][}]?", output.stdout))) == True

def test_hashtag():
    file_path = path_expansion + "test_hashtag.sh"
    output = run_cmd(file_path)
    assert(bool(re.match(r"[{]?[0-9][}]?", output.stdout))) == True

def test_PWD():
    file_path = path_expansion + "test_PWD.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_star():
    file_path = path_expansion + "test_star.sh"
    output = run_cmd(file_path)
    assert output.stdout != ''

def test_OLDPWD():
    file_path = path_expansion + "test_OLDPWD.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_positional_args_no_args():
    file_path = path_expansion + "test_positional_args.sh"
    output = run_cmd(file_path)
    assert output.stdout == '\n\n{}\n\n\n{}\n\n\n{}\n'

def test_RANDOM():
    file_path = path_expansion + "test_RANDOM.sh"
    output = run_cmd(file_path)
    assert(True)

def test_return_code():
    file_path = path_expansion + "test_return_code.sh"
    return compare(run_ref(file_path), run_cmd(file_path))

def test_uid():
    file_path = path_expansion + "test_uid.sh"
    output = run_cmd(file_path)
    assert(bool(re.match(r"[{]?[0-9][}]?", output.stdout)))

def test_IFS():
    file_path = path_expansion + "test_IFS.sh"
    return compare(run_ref(file_path), run_cmd(file_path))
