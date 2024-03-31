/**
 * @file command_line.h
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-29-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __COMMAND_LINE_H__
#define __COMMAND_LINE_H__

#include "sstrings.h"

// Flags are a bitmask. For example, OR them together, such as
// (CMD_LIST|CMD_REQD) to specify a required list parameter.
typedef enum {
    CMD_NONE  = 0x00,
    CMD_REQD  = 0x01,
    CMD_LIST  = 0x02,
    CMD_STR   = 0x04,
    CMD_BOOL  = 0x08,
    CMD_FLOAT = 0x10,
    CMD_INT   = 0x20,
    CMD_SEEN  = 0x40,
    CMD_HELP  = 0x80,
} CmdFlag;

// Opaque handle for command line.
typedef void* CmdLine;

// Create the command line data structure.
CmdLine create_cmd_line(const char* description);

// Destroy the command line data structure.
void destroy_cmd_line(CmdLine cl);

// Add a command line parameter to the parser.
//    parm = the name to recognize on the command line.
//    name = the name to use when retrieving a parameter.
//    dvalue = the default value of the parameter.
//    flags = controls the specify the behavior of the
//            parameter on the command line.
// If this is called with a NULL param and CMD_LIST as a
// flag, then random strings (like file names) will be
// stored under the name.
void add_cmd(CmdLine cl,
             const char* parm,
             const char* name,
             const char* help,
             const char* dvalue,
             unsigned char flags);

// Read the actual command line into the data structure and abort
// the program if there is an error.
void parse_cmd_line(CmdLine cl, int argc, char** argv);

// retrieve the value stored by the name, or publish an error.
String* get_cmd_str(CmdLine cl, const char* name);
const char* get_cmd_raw(CmdLine cl, const char* name);
long int get_cmd_int(CmdLine cl, const char* name);
unsigned long int get_cmd_unsigned(CmdLine cl, const char* name);
double get_cmd_float(CmdLine cl, const char* name);
StrList* get_cmd_list(CmdLine cl, const char* name);
bool get_cmd_bool(CmdLine cl, const char* name);
CmdFlag get_cmd_flag(CmdLine cl, const char* name);

// Print out the current state of the data structures for debugging.
void dump_cmd_line(CmdLine cl);


#endif /* __COMMAND_LINE_H__ */
