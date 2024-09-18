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
#include <limits.h>

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
		static const char SAMPLE[] = "&#60;&#33;&#45;&#45; i want to inject xss script &#45;&#45;&#62;alert(\"nice\")";
		// do not convert symbols /, >, <, -, and ! to prevent xss
		static const char INPUT[] = "&#60;&#33;&#45;&#45;&#32;&#105;&#32;&#119;&#97;&#110;&#116;&#32;&#116;&#111;&#32;&#105;&#110;&#106;&#101;&#99;&#116;&#32;&#120;&#115;&#115;&#32;&#115;&#99;&#114;&#105;&#112;&#116;&#32;&#45;&#45;&#62;&#97;&#108;&#101;&#114;&#116;&#40;&#34;&#110;&#105;&#99;&#101;&#34;&#41;"; // >
		static char buffer[sizeof INPUT];
		assert(decode_html_entities_utf8_wo_unsafe_symbols(buffer, INPUT, "/\0>\0<\0!\0-\0\0") == sizeof SAMPLE - 1);
		// decode_html_entities_utf8_wo_unsafe(buffer, INPUT); printf("%s", buffer);
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


	{
		static const char SAMPLE[] = "&#33;&#62;&#62;&#60;&#45;&#45;&#60;&#62;&#60;&#62;&#33;&#45;&#47;&#62;&#60;&#47;&#45;&#45;&#62;&#33;&#60;&#33;&#45;&#45; &#45;&#45;&#62;";
		// do not convert symbols /, >, <, -, and ! to prevent xss
		static const char INPUT[] = "&#33;&#62;&#62;&#60;&#45;&#45;&#60;&#62;&#60;&#62;&#33;&#45;&#47;&#62;&#60;&#47;&#45;&#45;&#62;&#33;&#60;&#33;&#45;&#45;&#32;&#45;&#45;&#62;"; // >
		static char buffer[sizeof INPUT];
		assert(decode_html_entities_utf8_wo_unsafe_symbols(buffer, INPUT, "/\0>\0<\0!\0-\0\0") == sizeof SAMPLE - 1);
		// decode_html_entities_utf8_wo_unsafe(buffer, INPUT); printf("%s", buffer);
		assert(strcmp(buffer, SAMPLE) == 0);
	}

	{
		static const char SAMPLE[] = "&#;П";
		// do not convert symbols /, >, <, -, and ! to prevent xss
		static const char INPUT[] = "&#;&#1055;&#62;&#1072;&#1074;&#1077;&#1083;&#62;"; // >
		const size_t buf_len = 10; // "&#;" -- bad string
		char not_null_term_buf[buf_len];

		size_t temp_buf_true_len = decode_html_entities_utf8_wo_unsafe_symbols_n(not_null_term_buf, INPUT, buf_len, "/\0>\0<\0!\0-\0\0");
		// printf("%d %.*s\n", (int)temp_buf_true_len, temp_buf_true_len, not_null_term_buf);
		assert(temp_buf_true_len == sizeof SAMPLE - 1);
		assert(strncmp(not_null_term_buf, SAMPLE, temp_buf_true_len) == 0);
	}

	{
		static const char SAMPLE[] = "&#-2;&#1234567890ABCDEFGHJCLMNOP123456789;";
		// do not convert symbols /, >, <, -, and ! to prevent xss
		static const char INPUT[] = "&#-2;&#1234567890ABCDEFGHJCLMNOP123456789;"; // >
		size_t buf_len = sizeof INPUT - 1; // w/o null terminator
		char not_null_term_buf[buf_len];

		size_t temp_buf_true_len = decode_html_entities_utf8_wo_unsafe_symbols_n(not_null_term_buf, INPUT, buf_len, "/\0>\0<\0!\0-\0\0");
		// printf("%.*s\n", temp_buf_true_len, not_null_term_buf);
		assert(temp_buf_true_len == sizeof SAMPLE - 1);
		assert(strncmp(not_null_term_buf, SAMPLE, temp_buf_true_len) == 0);
	}

	{
		char INPUT[] = "";

		assert(decode_html_entities_utf8_wo_unsafe_symbols_n(INPUT, NULL, 0, "/\0>\0<\0!\0-\0\0") == 0);
	}


	{
		char INPUT[] = "&#;&#1055;&#62;&#1072;&#1074;&#1077;&#1083;&#62;";

		size_t proccessed = decode_html_entities_utf8_wo_unsafe_symbols_n(INPUT, NULL, sizeof INPUT - 1, "/\0>\0<\0!\0-\0\0");
		// printf("%d %.*s", proccessed, proccessed, INPUT);
		// printf("%d", sizeof("&#62;П&#62;авел&#62;") - 1);
		assert(proccessed == sizeof("&#;П&#62;авел&#62;") - 1);
		assert(strncmp(INPUT, "&#;П&#62;авел&#62;", proccessed) == 0);

	}


	fprintf(stdout, "All tests passed :-)\n");
	return EXIT_SUCCESS;
}

