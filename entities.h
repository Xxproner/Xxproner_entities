/*	Copyright 2012 Christoph Gärtner, ooxi/entities
		https://bitbucket.org/cggaertner/cstuff
		https://github.com/ooxi/entities

	Distributed under the Boost Software License, Version 1.0
*/

#ifndef DECODE_HTML_ENTITIES_UTF8_
#define DECODE_HTML_ENTITIES_UTF8_

#include <stddef.h>

extern size_t decode_html_entities_utf8(char *dest, const char *src);
/*	Takes input from <src> and decodes into <dest>, which should be a buffer
	large enough to hold <strlen(src) + 1> characters.

	If <src> is <NULL>, input will be taken from <dest>, decoding
	the entities in-place.

	The function returns the size of the decoded string.
*/

extern size_t decode_html_entities_utf8_wo_unsafe_symbols(char *dest, const char *src, 
	const char* unsafe_symbs);
/*	Takes one more params <unsafe_symbs> string delimited '\0' of anscii characters 
  	that prevented to decode. Ends by double '\0'

*/

extern size_t decode_html_entities_utf8_wo_unsafe_symbols_n(char *dest, const char *src, 
	size_t src_size, const char* unsafe_symbs);
/*	Takes one more params <src_size> that indicated 
	how many characters must be decode i.e. size of <src>

	<src> may be not null terminated!
*/

#endif // DECODE_HTML_ENTITIES_UTF8_

