
%header {}

%source {}

%tokens {
    NAMESPACE
    CLASS
    CREATE
    DESTROY
    IF
    ELSE
    WHILE
    DO
    FOR
    IN
    TO
    TRY
    EXCEPT
    RAISE
    RETURN
    EXIT
    SWITCH
    CASE
    YIELD
    TRACE
    PRINT
    IMPORT
    TRUE
    FALSE
    BREAK
    CONTINUE
    INLINE
    ENTRY
    DEFAULT
    DOT
    COMMA
    COLON
    AS
    CPAREN
    OPAREN
    OBLOCK
    CBLOCK
    OBRACE
    CBRACE
    STRG_CONST@
    SYMBOL@
    FLOAT_CONST@
    INT_CONST@
    UNSIGNED_CONST@
    PUBLIC@
    PRIVATE@
    PROTECTED@
    INTEGER@
    UNSIGNED@
    FLOAT@
    STRING@
    DICT@
    LIST@
    BOOLEAN@
    NOTHING@
    ASSIGN@
    ADD@
    SUB@
    MUL@
    DIV@
    MOD@
    POW@
    NOT@
    OR@
    AND@
    EQU@
    NEQU@
    LORE@
    GORE@
    OPOINT@
    CPOINT@
    ADD_ASSIGN@
    SUB_ASSIGN@
    MUL_ASSIGN@
    DIV_ASSIGN@
    MOD_ASSIGN@
}

%grammar {

    module {
        : module_body
    }

    module_element {
        : namespace_element
        : import_statement
        : entry_definition
    }

    module_body {
        : module_element
        : module_body module_element
    }

    entry_definition {
        : ENTRY func_block
    }

    scope {
        : PUBLIC
        : PRIVATE
        : PROTECTED
    }

    type_name {
        : compound_name
        : FLOAT
        : INTEGER
        : UNSIGNED
        : STRING
        : DICT
        : LIST
        : BOOLEAN
        : NOTHING
    }

    compound_name {
        : SYMBOL
        : compound_name DOT SYMBOL
    }

    compound_reference {
        : compound_reference_element
        : compound_reference DOT compound_reference_element
    }

    compound_reference_element {
        : SYMBOL
        : func_reference
        : array_reference
    }

    formatted_string {
        : STRG_CONST
        : STRG_CONST OPAREN CPAREN
        : STRG_CONST OPAREN expr_list CPAREN
    }

    func_reference {
        : compound_name OPAREN CPAREN
        : compound_name OPAREN expr_list CPAREN
    }

    array_reference {
        : compound_name array_reference_list
    }

    array_reference_list {
        : OBRACE expression CBRACE
        : array_reference_list OBRACE expression CBRACE
    }

    import_statement {
        : IMPORT compound_name
        : IMPORT compound_name AS SYMBOL
    }

    namespace_definition {
        : NAMESPACE SYMBOL OBLOCK namespace_block CBLOCK
    }

    namespace_block {
        : namespace_element
        : namespace_block namespace_element
    }

    namespace_element {
        : namespace_definition
        : class_definition
        : func_definition
        : ctor_definition
        : dtor_definition
        : var_declaration
        : scope
    }

    class_definition {
        : CLASS SYMBOL class_parameters class_block
    }

    class_parameters {
        : OPAREN CPAREN
        : OPAREN compound_name CPAREN
    }

    class_block {
        : OBLOCK CBLOCK
        : OBLOCK class_body CBLOCK
    }

    class_body {
        : class_body_element
        : class_body class_body_element
    }

    class_body_element {
        : var_declaration
        : func_declaration
        : scope
    }

    var_declaration {
        : type_name SYMBOL
    }

    func_declaration {
        : type_name SYMBOL func_decl_parms
        : CREATE func_decl_parms
        : DESTROY
    }

    func_decl_parms {
        : OPAREN CPAREN
        : OPAREN func_decl_parms_list CPAREN
    }

    func_decl_parms_list {
        : func_decl_parms_elem
        : func_decl_parms_list COMMA func_decl_parms_elem
    }

    func_decl_parms_elem {
        : type_name SYMBOL
    }

    bool_literal {
        : TRUE
        : FALSE
    }

    primary {
        : compound_reference
        : const_expression
        : cast_expr
        : OPAREN expression CPAREN
    }

    const_expression {
        : formatted_string
        : bool_literal
        : FLOAT_CONST
        : INT_CONST
        : UNSIGNED_CONST
    }

    expression {
        : expr_or
    }

    expr_or {
        : expr_and OR expr_or
    }

    expr_and {
        : expr_equ AND expr_and
    }

    expr_equ {
        : expr_comp EQU expr_equ
        : expr_comp NEQU expr_equ
    }

    expr_comp {
        : expr_term LORE expr_comp
        : expr_term GORE expr_comp
        : expr_term CPOINT expr_comp
        : expr_term OPOINT expr_comp
    }

    expr_term {
        : expr_pow ADD expr_term
        : expr_pow SUB expr_term
    }

    expr_pow {
        : expr_fact POW expr_pow
    }

    expr_fact {
        : expr_unary MUL expr_fact
        : expr_unary DIV expr_fact
        : expr_unary MOD expr_fact
    }

    expr_unary {
        : NOT expression
        : SUB expression
        : primary
    }

    cast_expr {
        : OPOINT type_name CPOINT expression
    }

    expr_list {
        : expression
        : expr_list COMMA expression
    }

    member {
        : SYMBOL COLON
    }

    func_definition {
        : type_name member SYMBOL func_decl_parms func_block
    }

    ctor_definition {
        : member CREATE func_decl_parms func_block
    }

    dtor_definition {
        : member DESTROY func_block
    }

    func_block {
        : OBLOCK CBLOCK
        : OBLOCK func_content CBLOCK
    }

    assignment_oper {
        : ASSIGN
        : ADD_ASSIGN
        : SUB_ASSIGN
        : MUL_ASSIGN
        : DIV_ASSIGN
        : MOD_ASSIGN
    }

    assignment {
        : assignment_oper expression
    }

    data_definition {
        : var_declaration
        : var_declaration assignment
    }

    func_content {
        : func_content_elem
        : func_content func_content_elem
    }

    func_content_elem {
        : func_block
        : data_definition
        : compound_reference
        : compound_reference assignment
        : if_else_clause
        : while_clause
        : do_clause
        : for_clause
        : switch_case_clause
        : try_except_clause
        : return_statement
        : raise_statement
        : exit_statement
        : yield_statement
        : trace_statement
        : print_statement
        : break_statement
        : cont_statement
        : inline_block
    }

    inline_block {
        : INLINE formatted_string
    }

    print_statement {
        : PRINT
        : PRINT OPAREN CPAREN
        : PRINT OPAREN formatted_string CPAREN
    }

    break_statement {
        : BREAK
    }

    cont_statement {
        : CONTINUE
    }

    trace_statement {
        : TRACE
        : TRACE OPAREN CPAREN
        : TRACE OPAREN formatted_string CPAREN
    }

    yield_statement {
        : YIELD OPAREN SYMBOL CPAREN
    }

    exit_statement {
        : EXIT OPAREN expression CPAREN
    }

    return_statement {
        : RETURN
        : RETURN OPAREN CPAREN
        : RETURN OPAREN expression CPAREN
    }

    raise_statement {
        : RAISE OPAREN compound_name CPAREN
    }


    if_else_clause {
        : if_clause
        : if_clause else_clause_list
        : if_clause else_clause_list else_clause_final
    }

    if_clause {
        : IF OPAREN expression CPAREN func_block
    }

    else_clause {
        : ELSE OPAREN expression CPAREN func_block
    }

    else_clause_list {
        : else_clause
        : else_clause_list else_clause
    }

    else_clause_final {
        : ELSE OPAREN CPAREN func_block
        : ELSE func_block
    }

    while_clause {
        : WHILE OPAREN expression CPAREN func_block
    }

    do_clause {
        : DO func_block WHILE OPAREN expression CPAREN
    }

    for_clause {
        : FOR OPAREN SYMBOL IN compound_reference CPAREN func_block
        : FOR OPAREN SYMBOL IN expression TO expression CPAREN func_block
    }

    switch_case_clause {
        : switch_clause case_clause_list case_clause_final
    }

    switch_clause {
        : SWITCH OPAREN expression CPAREN OBLOCK
    }

    case_clause {
        : CASE OPAREN const_expression CPAREN func_block
    }

    case_clause_list {
        : case_clause
        : case_clause_list case_clause
    }

    case_clause_final {
        : CBLOCK
        : DEFAULT func_block CBLOCK
    }

    try_except_clause {
        : try_clause except_clause_list except_clause_final
    }

    try_clause {
        : TRY func_block
    }

    except_clause {
        : EXCEPT OPAREN compound_name CPAREN func_block
    }


    except_clause_list {
        : except_clause
        : except_clause_list except_clause
    }

    except_clause_final {
        : EXCEPT OPAREN CPAREN func_block
        : EXCEPT func_block
    }
}

