/**
 * @file pass.c
 *
 * @brief This implements the symbol table pass on the AST. It captures the 
 * symbolic context and the symbol name and creates the entries in the AST
 * data structure. It also verifies that symbol references are valid.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-27-2024
 * @copyright Copyright (c) 2024
 */
#include "parse.h"
#include "trace.h"
#include "symbols.h"
#include "sympass.h"


/**
 * @brief This function calls the test pass function. This way, there can be
 * only the elements that are active in the array. There must be a dummy
 * function that does nothing if a node is entered without a handler. There
 * are no actual restrictions on the signature of the function called, but
 * all of the information needed by the pass function must be provided in the
 * AST node. This function should be used as a template for other passes.
 *
 * @param node
 * @param type
 * @return * void
 *
 */
void pre_sym_pass(ast_node* node) {

    ENTER;
    TRACE("node type: %s", nterm_to_str(node));

    // Define the name context for the namespace.
    switch(ast_node_type(node)) {
        case AST_namespace_definition: {  
            ast_namespace_definition* n = (ast_namespace_definition*)node;
            push_sym_context(raw_string(n->name->str)); 
            n->context = copy_root_context();
            const char* s = get_sym_context(n->context);
            create_symtab_node(s, node);
            TRACE("context: %s", s);
        }
        break;
        
        // Define the name context for the class.
        case AST_class_definition: {
            ast_class_definition* n = (ast_class_definition*)node;
            push_sym_context(raw_string(n->name->str)); 
            n->context = copy_root_context();
            const char* s = get_sym_context(n->context);
            create_symtab_node(s, node);
            TRACE("context: %s", s);
        }
        break;

        // A var declaration may be part of a definition with an initializer,
        // but this will define the name in the symbol table.
        case AST_var_decl:
            break;

        // Function declarations are only valid in a class.
        case AST_function_declaration:
            break;

        // Create the unique create name. Can only be declared in a class.
        case AST_create_declaration:
            break;

        // Create the unique destroy name. Can only be declared in a class.
        case AST_destroy_declaration:
            break;

        // Create the name if it has not been declared in a class. Creates a 
        // unique context for the variables that are referenced in the 
        // function, but the names in a class of which the function was 
        // declared are local to this context. Input and output parameters are
        // local to the function, 
        case AST_function_definition:
            break;

        // Create an anonymous name context scope.
        case AST_function_body:
            break;

        // None of these productions define a new name.
        case AST_module:
        case AST_module_item:
        case AST_namespace_item:
        case AST_class_item:
        case AST_compound_name:
        case AST_compound_ref_item:
        case AST_compound_reference:
        case AST_scope_operator:
        case AST_literal_type_name:
        case AST_literal_value:
        case AST_type_name:
        case AST_formatted_strg:
        case AST_string_literal:
        case AST_var_decl_list:
        case AST_var_definition:
        case AST_list_init:
        case AST_dict_init_element:
        case AST_dict_init:
        case AST_array_param:
        case AST_array_reference:
        case AST_function_reference:
        case AST_create_reference:
        case AST_destroy_reference:
        case AST_create_name:
        case AST_destroy_name:
        case AST_create_definition:
        case AST_destroy_definition:
        case AST_function_body_element:
        case AST_expression:
        case AST_expr_primary:
        case AST_expression_list:
        case AST_assignment_item:
        case AST_assignment:
        case AST_while_definition:
        case AST_while_clause:
        case AST_do_clause:
        case AST_for_clause:
        case AST_if_clause:
        case AST_else_clause:
        case AST_elif_clause:
        case AST_try_clause:
        case AST_except_clause:
        case AST_final_clause:
        case AST_break_statement:
        case AST_continue_statement:
        case AST_inline_statement:
        case AST_yield_statement:
        case AST_return_statement:
        case AST_raise_statement:
        case AST_start_function:
        case AST_import_statement:
        case AST_operator:
        case AST_cast_statement:
        case AST_case_item:
        case AST_type_name_list:
        case AST_function_assignment:
        case AST_compound_name_list:
        case AST_array_param_list:
        case AST_class_var_declaration:
        case AST_print_statement:
        case AST_trace_statement:
        case AST_exit_statement:        
            break;

        default:                        
            fatal_error("Unknown non-terminal type in %s: %d", __func__, node->type);
    }
    RET;
}

void post_sym_pass(ast_node* node) {

    ENTER;
    TRACE("node type: %s", nterm_to_str(node));

    switch(ast_node_type(node)) {
        case AST_namespace_definition: { 
            ast_namespace_definition* n = (ast_namespace_definition*)node;
            pop_sym_context(); 
            const char* s = get_sym_context(n->context);
            TRACE("context: %s", s);
            break;
        }

        case AST_class_definition: {
            ast_class_definition* n = (ast_class_definition*)node;
            pop_sym_context(); 
            const char* s = get_sym_context(n->context);
            TRACE("context: %s", s);
            break;
        }

        case AST_module:
        case AST_module_item:
        case AST_namespace_item:
        case AST_class_item:
        case AST_compound_name:
        case AST_compound_ref_item:
        case AST_compound_reference:
        case AST_scope_operator:
        case AST_literal_type_name:
        case AST_literal_value:
        case AST_type_name:
        case AST_formatted_strg:
        case AST_string_literal:
        case AST_var_decl:
        case AST_var_decl_list:
        case AST_var_definition:
        case AST_list_init:
        case AST_dict_init_element:
        case AST_dict_init:
        case AST_array_param:
        case AST_array_reference:
        case AST_function_reference:
        case AST_create_reference:
        case AST_destroy_reference:
        case AST_function_declaration:
        case AST_create_declaration:
        case AST_destroy_declaration:
        case AST_function_definition:
        case AST_create_name:
        case AST_destroy_name:
        case AST_create_definition:
        case AST_destroy_definition:
        case AST_function_body:
        case AST_function_body_element:
        case AST_expression:
        case AST_expr_primary:
        case AST_expression_list:
        case AST_assignment_item:
        case AST_assignment:
        case AST_while_definition:
        case AST_while_clause:
        case AST_do_clause:
        case AST_for_clause:
        case AST_if_clause:
        case AST_else_clause:
        case AST_elif_clause:
        case AST_try_clause:
        case AST_except_clause:
        case AST_final_clause:
        case AST_break_statement:
        case AST_continue_statement:
        case AST_inline_statement:
        case AST_yield_statement:
        case AST_return_statement:
        case AST_raise_statement:
        case AST_start_function:
        case AST_import_statement:
        case AST_operator:
        case AST_cast_statement:
        case AST_case_item:
        case AST_type_name_list:
        case AST_function_assignment:
        case AST_compound_name_list:
        case AST_array_param_list:
        case AST_class_var_declaration:
        case AST_print_statement:
        case AST_trace_statement:
        case AST_exit_statement:
            break;

        default:
            fatal_error("Unknown non-terminal type in %s: %d", __func__, node->type);
    }
    RET;
}
