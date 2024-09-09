/*	Copyright 2012 Christoph Gärtner, ooxi/entities
		https://bitbucket.org/cggaertner/cstuff
		https://github.com/ooxi/entities

	Distributed under the Boost Software License, Version 1.0
*/

#include "entities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef NDEBUG
#include <assert.h>
#include <locale.h>

int main(void)
{
	setlocale(LC_ALL, "");

	{
		static const char SAMPLE[] = "Christoph Gärtner";
		static char buffer[] = "Christoph G&auml;rtner";
		assert(decode_html_entities_utf8(buffer, NULL) == sizeof SAMPLE - 1);
		assert(strcmp(buffer, SAMPLE) == 0);
	}

	{
		static const char SAMPLE[] = "Павел Сергеевич Прокопьев";
		// do not convert symbols /, >, <, -, and ! to prevent xss
		static const char INPUT[] = "&#1055;&#1072;&#1074;&#1077;&#1083; &#1057;&#1077;&#1088;&#1075;&#1077;&#1077;&#1074;&#1080;&#1095; &#1055;&#1088;&#1086;&#1082;&#1086;&#1087;&#1100;&#1077;&#1074;";
		static char buffer[sizeof INPUT];
		assert(decode_html_entities_utf8(buffer, INPUT) == sizeof SAMPLE - 1);
		// decode_html_entities_utf8(buffer, INPUT);
		// printf("%s\n", buffer);
		assert(strcmp(buffer, SAMPLE) == 0);
	}


	{
		static const char SAMPLE[] = "&#62;П&#62;авел&#62;";
		// do not convert symbols /, >, <, -, and ! to prevent xss
		static const char INPUT[] = "&#62;&#1055;&#62;&#1072;&#1074;&#1077;&#1083;&#62;"; // >
		static char buffer[sizeof INPUT];
		assert(decode_html_entities_utf8_wo_unsafe_symbols(buffer, INPUT, "/\0>\0<\0!\0-\0\0") == sizeof SAMPLE - 1);
		// decode_html_entities_utf8_wo_unsafe(buffer, INPUT); printf("%s", buffer);
		assert(strcmp(buffer, SAMPLE) == 0);
	}


	fprintf(stdout, "All tests passed :-)\n");
	return EXIT_SUCCESS;
}

