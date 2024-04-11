# SCANNER
This is the complete scanner for the Kata language, including tests. Look at ``keywords.c`` for a complete list of keywords.

* Keywords are not case-sensitive.
* This scanner does not return single character tokens, it only returns fully developed tokens with a type and the string that created it.
* A newline at the end of the scanned file is highly advised, but not strictly required. If there is no newline at the end of the file, then the wrong line and column number and file name could be returned.
* If the actual keywords are altered, then strict sorting must be observed according to the strcasecmp() library call.
* See ``scanner.h`` for more information about the public interface.

Note that there is a python script called mk_keywords.py. This script reads the file keywords.txt and creates keywords.c that has an actual data structure that could be used to scan the keywords. However, that is not in use at this moment. There there are no major changes with the keywords at this time.
