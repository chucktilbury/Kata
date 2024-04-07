############################################################
#
# WARNING! Do not run this script unless you intend to
# destroy the entire parser test and start over from scratch.
#
# This remains here for the purpose of documentation. It is
# NOT intended to be used for any other purpose.
#
############################################################

print("A wise person will reflect and investigate before typing.")
exit(1)


import shutil, sys, os
from pprint import pprint as p

compound_list = [
    "compound_name",
    "compound_name_list",
    "compound_ref_item",
    "compound_reference"]

except_list = [
    "try_clause",
    "except_clause_mid",
    "except_clause_final",
    "except_clause"]

expr_list = [
    "expression",
    "expr_primary",
    "expression_list",
    "assignment_item",
    "assignment",
    "cast_statement",
    "operator"]

flow_list = [
    "if_clause",
    "else_clause_item",
    "else_clause",
    "switch_clause",
    "case_clause",
    "default_clause",
    "case_body"]

func_list = [
    "function_reference",
    "create_reference",
    "destroy_reference",
    "function_declaration",
    "create_declaration",
    "destroy_declaration",
    "function_definition",
    "create_name",
    "destroy_name",
    "create_definition",
    "destroy_definition",
    "function_body",
    "start_function",
    "function_assignment"]

func_body_list = [
    "function_body_element",
    "break_statement",
    "continue_statement",
    "trace_statement",
    "inline_statement",
    "yield_statement",
    "type_statement",
    "exit_statement",
    "print_statement",
    "return_statement",
    "raise_statement"]

import_list = ["import_statement"]

list_list = [
    "list_init",
    "dict_init_element",
    "dict_init",
    "array_param",
    "array_param_list",
    "array_reference"]

literals_list = [
    "scope_operator",
    "literal_type_name",
    "literal_value",
    "type_name",
    "type_name_list"]

loop_list = [
    "while_definition",
    "while_clause",
    "do_clause",
    "for_clause"]

module_list = [
    "module",
    "module_item",
    "namespace_item",
    "namespace_definition",
    "class_item",
    "class_definition",
    "class_var_declaration"]

strg_list = [
    "string_literal",
    "string_expr_item",
    "string_expr",
    "formatted_strg"]

var_list = [
    "var_decl",
    "var_decl_list",
    "var_definition"]

dir_list = {
    "compound": compound_list,
    "except": except_list,
    "expr": expr_list,
    "flow": flow_list,
    "func": func_list,
    "func_body": func_body_list,
    "import": import_list,
    "list": list_list,
    "literals": literals_list,
    "loop": loop_list,
    "module": module_list,
    "strg": strg_list,
    "vars": var_list}


def clean_dirs() :

    for item in dir_list:
        #sys.stdout.write("clean directory: "+item)
        try:
            shutil.rmtree(item)
            #sys.stdout.write(" (ok)\n")
        except:
            #sys.stdout.write(" (pass)\n")
            pass

def touch_dummy(dir) :

    d = os.path.join(dir, "dummy_test.simp")
    fp = open(d, "w")
    fp.write("; dummy_test.simp\n\n")
    fp.close()

def make_dirs():

    with open("test_list.txt", "w") as root_fp:
        root_fp.write("# this is the list of groups of tests.\n\n")

        for item in dir_list:
            root_fp.write(item+":run\n")
            os.mkdir(item)

            with open(os.path.join(item, "test_list.txt"), "w") as branch_fp:
                branch_fp.write("# this is a list of classes of tests.\n\n")
                for name in dir_list[item] :
                    branch_fp.write(name+":run\n")
                    bdir = os.path.join(item, name)
                    os.mkdir(bdir)

                    with open(os.path.join(bdir, "test_list.txt"), "w") as tfp :
                        tfp.write("# this is a list of actual tests.\n\n")
                        tfp.write("dummy_test:run # tests with correct syntax\n")

                    tdir = os.path.join(bdir, "tests")
                    os.mkdir(tdir)
                    with open(os.path.join(tdir, "dummy_test.simp"), "w") as tfp :
                        tfp.write(";-----------------------------------------\n"
                                  "; this is a dummy test for development.\n"
                                  ";-----------------------------------------\n\n"
                                  "public\n\n")

                    tdir = os.path.join(bdir, "results")
                    os.mkdir(tdir)
                    with open(os.path.join(tdir, "dummy_test.res"), "w") as tfp :
                        pass

        root_fp.write("\n")

clean_dirs()
make_dirs()
#p(dir_list)
