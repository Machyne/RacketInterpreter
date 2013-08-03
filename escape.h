/* Interface for un-escaping strings */

/* This method replaces escaped characters with their one-character
   equivalents.

   This method allocates and returns a string, which must be freed by
   the caller.
*/
char *unescape(char *escaped);

/* This method finds the number of non-backslash characters in the input */
int unescape_counter(char *string);
