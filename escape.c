/* Implementation of the escape and unescape functions */

#include <stdlib.h>
#include "escape.h"

int unescape_counter(char *string);

/* This method replaces escaped characters with their one-character
   equivalents. */
char *unescape(char *escaped) {
	int newSize = 1 + unescape_counter(escaped);
	char *unescaped = (char *) malloc(sizeof(char) * newSize);
	int how_much_escaped_is_winning_by = 0;
	for(int i=0; escaped[i]; i++){
		if(escaped[i] == '\\'){
			how_much_escaped_is_winning_by++;
			i++;
			switch(escaped[i]){
				case 'n':
					unescaped[i - how_much_escaped_is_winning_by] = '\n';
					break;
				case 't':
					unescaped[i - how_much_escaped_is_winning_by] = '\t';
					break;
				case '\\':
					unescaped[i - how_much_escaped_is_winning_by] = '\\';
					break;
				case '\'':
					unescaped[i - how_much_escaped_is_winning_by] = '\'';
					break;
				case '\"':
					unescaped[i - how_much_escaped_is_winning_by] = '\"';
					break;
				case 'a':
					unescaped[i - how_much_escaped_is_winning_by] = '\a';
					break;
				case 'b':
					unescaped[i - how_much_escaped_is_winning_by] = '\b';
					break;
				case 'f':
					unescaped[i - how_much_escaped_is_winning_by] = '\f';
					break;
				case 'r':
					unescaped[i - how_much_escaped_is_winning_by] = '\r';
					break;
				case 'v':
					unescaped[i - how_much_escaped_is_winning_by] = '\v';
					break;
				default: /*case '\?': */
					unescaped[i - how_much_escaped_is_winning_by] = '\?';
					break;
			}
		}else{
			unescaped[i - how_much_escaped_is_winning_by] = escaped[i];
		}	
	}
	unescaped[newSize-1]='\0';
	return unescaped;
}

/* This method finds the number of non-backslash characters in the input */
int unescape_counter(char *string){
	int count = 0;
	for(; *string; string++){
		if( ((*string) == '\\') && (string[1] == '\\') ){
			count++;
			string++;
			continue;
		}
		if( (*string) != '\\') count++;
	}
	return count;
}
