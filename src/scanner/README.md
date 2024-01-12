# SCANNER
This is the complete scanner for the Simple language, including tests. Look at ``keywords.c`` for a complete list of keywords.

* Keywords are not case-sensitive.
* This scanner does not return single character tokens, it only returns fully developed tokens with a type and the string that created it.
* A newline at the end of the scanned file is highly advised, but not strictly required. If there is no newline at the end of the file, then the wrong line and column number and file name could be returned.
* If the actual keywords are altered, then strict sorting must be observed according to the strcasecmp() library call.
* See ``scanner.h`` for more information about the public interface.

