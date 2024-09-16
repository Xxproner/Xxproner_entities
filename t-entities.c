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
		// static const char SAMPLE[] = "123123123123123123123123123123";
		// static const char SAMPLE_h[] = "AAB123";
		// assert(strtoul_n(SAMPLE, 0, NULL, 10) == 0);
		// assert(strtoul_n(SAMPLE, 3, NULL, 10) == 123);
		// assert(strtoul_n(SAMPLE, 4, NULL, 10) == 1231);
		// assert(strtoul_n(SAMPLE, 5, NULL, 10) == 12312);
		// assert(strtoul_n(SAMPLE, 6, NULL, 10) == 123123);
		// assert(strtoul_n(SAMPLE, 20, NULL, 10) == LONG_MAX); // overflow
		// assert(strncmp(buffer, SAMPLE, sizeof SAMPLE) == 0);
	}

	{
		static const char SAMPLE[] = "&#62;П";
		// do not convert symbols /, >, <, -, and ! to prevent xss
		static const char INPUT[] = "&#;&#1055;&#62;&#1072;&#1074;&#1077;&#1083;&#62;"; // >
		const size_t buf_len = 10; // "&#;" -- bad string
		char not_null_term_buf[buf_len];
		// assert(decode_html_entities_utf8_wo_unsafe_symbols_n(buffer, INPUT, 17, "/\0>\0<\0!\0-\0\0") == sizeof SAMPLE - 1);
		size_t temp_buf_true_len = decode_html_entities_utf8_wo_unsafe_symbols_n(not_null_term_buf, INPUT, buf_len, "/\0>\0<\0!\0-\0\0");
		printf("len = %d %.*s\n", temp_buf_true_len, temp_buf_true_len, not_null_term_buf);
		not_null_term_buf[buf_len - 1] = 0;
		// assert(strncmp(buffer, SAMPLE, sizeof SAMPLE) == 0);
	}

	fprintf(stdout, "All tests passed :-)\n");
	return EXIT_SUCCESS;
}

