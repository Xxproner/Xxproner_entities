/*	Copyright 2012 Christoph Gärtner, ooxi/entities
		https://bitbucket.org/cggaertner/cstuff
		https://github.com/ooxi/entities

	Distributed under the Boost Software License, Version 1.0
*/

#include "entities.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> /* LONG_MAX */
#include <ctype.h> /* isspace() */

#define UNICODE_MAX 0x10FFFFul

static const char *const NAMED_ENTITIES[][2] = {
	{ "AElig;", "Æ" },
	{ "Aacute;", "Á" },
	{ "Acirc;", "Â" },
	{ "Agrave;", "À" },
	{ "Alpha;", "Α" },
	{ "Aring;", "Å" },
	{ "Atilde;", "Ã" },
	{ "Auml;", "Ä" },
	{ "Beta;", "Β" },
	{ "Ccedil;", "Ç" },
	{ "Chi;", "Χ" },
	{ "Dagger;", "‡" },
	{ "Delta;", "Δ" },
	{ "ETH;", "Ð" },
	{ "Eacute;", "É" },
	{ "Ecirc;", "Ê" },
	{ "Egrave;", "È" },
	{ "Epsilon;", "Ε" },
	{ "Eta;", "Η" },
	{ "Euml;", "Ë" },
	{ "Gamma;", "Γ" },
	{ "Iacute;", "Í" },
	{ "Icirc;", "Î" },
	{ "Igrave;", "Ì" },
	{ "Iota;", "Ι" },
	{ "Iuml;", "Ï" },
	{ "Kappa;", "Κ" },
	{ "Lambda;", "Λ" },
	{ "Mu;", "Μ" },
	{ "Ntilde;", "Ñ" },
	{ "Nu;", "Ν" },
	{ "OElig;", "Œ" },
	{ "Oacute;", "Ó" },
	{ "Ocirc;", "Ô" },
	{ "Ograve;", "Ò" },
	{ "Omega;", "Ω" },
	{ "Omicron;", "Ο" },
	{ "Oslash;", "Ø" },
	{ "Otilde;", "Õ" },
	{ "Ouml;", "Ö" },
	{ "Phi;", "Φ" },
	{ "Pi;", "Π" },
	{ "Prime;", "″" },
	{ "Psi;", "Ψ" },
	{ "Rho;", "Ρ" },
	{ "Scaron;", "Š" },
	{ "Sigma;", "Σ" },
	{ "THORN;", "Þ" },
	{ "Tau;", "Τ" },
	{ "Theta;", "Θ" },
	{ "Uacute;", "Ú" },
	{ "Ucirc;", "Û" },
	{ "Ugrave;", "Ù" },
	{ "Upsilon;", "Υ" },
	{ "Uuml;", "Ü" },
	{ "Xi;", "Ξ" },
	{ "Yacute;", "Ý" },
	{ "Yuml;", "Ÿ" },
	{ "Zeta;", "Ζ" },
	{ "aacute;", "á" },
	{ "acirc;", "â" },
	{ "acute;", "´" },
	{ "aelig;", "æ" },
	{ "agrave;", "à" },
	{ "alefsym;", "ℵ" },
	{ "alpha;", "α" },
	{ "amp;", "&" },
	{ "and;", "∧" },
	{ "ang;", "∠" },
	{ "apos;", "'" },
	{ "aring;", "å" },
	{ "asymp;", "≈" },
	{ "atilde;", "ã" },
	{ "auml;", "ä" },
	{ "bdquo;", "„" },
	{ "beta;", "β" },
	{ "brvbar;", "¦" },
	{ "bull;", "•" },
	{ "cap;", "∩" },
	{ "ccedil;", "ç" },
	{ "cedil;", "¸" },
	{ "cent;", "¢" },
	{ "chi;", "χ" },
	{ "circ;", "ˆ" },
	{ "clubs;", "♣" },
	{ "colon;", ":" },
	{ "commat;", "@" },
	{ "cong;", "≅" },
	{ "copy;", "©" },
	{ "crarr;", "↵" },
	{ "cup;", "∪" },
	{ "curren;", "¤" },
	{ "dArr;", "⇓" },
	{ "dagger;", "†" },
	{ "darr;", "↓" },
	{ "deg;", "°" },
	{ "delta;", "δ" },
	{ "diams;", "♦" },
	{ "divide;", "÷" },
	{ "eacute;", "é" },
	{ "ecirc;", "ê" },
	{ "egrave;", "è" },
	{ "empty;", "∅" },
	{ "emsp;", " " },
	{ "ensp;", " " },
	{ "epsilon;", "ε" },
	{ "equiv;", "≡" },
	{ "eta;", "η" },
	{ "eth;", "ð" },
	{ "euml;", "ë" },
	{ "euro;", "€" },
	{ "exist;", "∃" },
	{ "excl;", "!" },
	{ "fnof;", "ƒ" },
	{ "forall;", "∀" },
	{ "frac12;", "½" },
	{ "frac14;", "¼" },
	{ "frac34;", "¾" },
	{ "frasl;", "⁄" },
	{ "gamma;", "γ" },
	{ "ge;", "≥" },
	{ "gt;", ">" },
	{ "hArr;", "⇔" },
	{ "harr;", "↔" },
	{ "hearts;", "♥" },
	{ "hellip;", "…" },
	{ "iacute;", "í" },
	{ "icirc;", "î" },
	{ "iexcl;", "¡" },
	{ "igrave;", "ì" },
	{ "image;", "ℑ" },
	{ "infin;", "∞" },
	{ "int;", "∫" },
	{ "iota;", "ι" },
	{ "iquest;", "¿" },
	{ "isin;", "∈" },
	{ "iuml;", "ï" },
	{ "kappa;", "κ" },
	{ "lArr;", "⇐" },
	{ "lambda;", "λ" },
	{ "lang;", "〈" },
	{ "laquo;", "«" },
	{ "larr;", "←" },
	{ "lceil;", "⌈" },
	{ "ldquo;", "“" },
	{ "le;", "≤" },
	{ "lfloor;", "⌊" },
	{ "lowast;", "∗" },
	{ "loz;", "◊" },
	{ "lrm;", "\xE2\x80\x8E" },
	{ "lsaquo;", "‹" },
	{ "lsquo;", "‘" },
	{ "lt;", "<" },
	{ "macr;", "¯" },
	{ "mdash;", "—" },
	{ "micro;", "µ" },
	{ "middot;", "·" },
	{ "minus;", "−" },
	{ "mu;", "μ" },
	{ "nabla;", "∇" },
	{ "nbsp;", " " },
	{ "ndash;", "–" },
	{ "ne;", "≠" },
	{ "ni;", "∋" },
	{ "not;", "¬" },
	{ "notin;", "∉" },
	{ "nsub;", "⊄" },
	{ "ntilde;", "ñ" },
	{ "nu;", "ν" },
	{ "oacute;", "ó" },
	{ "ocirc;", "ô" },
	{ "oelig;", "œ" },
	{ "ograve;", "ò" },
	{ "oline;", "‾" },
	{ "omega;", "ω" },
	{ "omicron;", "ο" },
	{ "oplus;", "⊕" },
	{ "or;", "∨" },
	{ "ordf;", "ª" },
	{ "ordm;", "º" },
	{ "oslash;", "ø" },
	{ "otilde;", "õ" },
	{ "otimes;", "⊗" },
	{ "ouml;", "ö" },
	{ "para;", "¶" },
	{ "part;", "∂" },
	{ "permil;", "‰" },
	{ "perp;", "⊥" },
	{ "phi;", "φ" },
	{ "pi;", "π" },
	{ "piv;", "ϖ" },
	{ "plusmn;", "±" },
	{ "pound;", "£" },
	{ "prime;", "′" },
	{ "prod;", "∏" },
	{ "prop;", "∝" },
	{ "psi;", "ψ" },
	{ "quot;", "\"" },
	{ "rArr;", "⇒" },
	{ "radic;", "√" },
	{ "rang;", "〉" },
	{ "raquo;", "»" },
	{ "rarr;", "→" },
	{ "rceil;", "⌉" },
	{ "rdquo;", "”" },
	{ "real;", "ℜ" },
	{ "reg;", "®" },
	{ "rfloor;", "⌋" },
	{ "rho;", "ρ" },
	{ "rlm;", "\xE2\x80\x8F" },
	{ "rsaquo;", "›" },
	{ "rsquo;", "’" },
	{ "sbquo;", "‚" },
	{ "scaron;", "š" },
	{ "sdot;", "⋅" },
	{ "sect;", "§" },
	{ "shy;", "\xC2\xAD" },
	{ "sigma;", "σ" },
	{ "sigmaf;", "ς" },
	{ "sim;", "∼" },
	{ "spades;", "♠" },
	{ "sub;", "⊂" },
	{ "sube;", "⊆" },
	{ "sum;", "∑" },
	{ "sup1;", "¹" },
	{ "sup2;", "²" },
	{ "sup3;", "³" },
	{ "sup;", "⊃" },
	{ "supe;", "⊇" },
	{ "szlig;", "ß" },
	{ "tau;", "τ" },
	{ "there4;", "∴" },
	{ "theta;", "θ" },
	{ "thetasym;", "ϑ" },
	{ "thinsp;", " " },
	{ "thorn;", "þ" },
	{ "tilde;", "˜" },
	{ "times;", "×" },
	{ "trade;", "™" },
	{ "uArr;", "⇑" },
	{ "uacute;", "ú" },
	{ "uarr;", "↑" },
	{ "ucirc;", "û" },
	{ "ugrave;", "ù" },
	{ "uml;", "¨" },
	{ "upsih;", "ϒ" },
	{ "upsilon;", "υ" },
	{ "uuml;", "ü" },
	{ "weierp;", "℘" },
	{ "xi;", "ξ" },
	{ "yacute;", "ý" },
	{ "yen;", "¥" },
	{ "yuml;", "ÿ" },
	{ "zeta;", "ζ" },
	{ "zwj;", "\xE2\x80\x8D" },
	{ "zwnj;", "\xE2\x80\x8C" }
};

static int cmp(const void *key, const void *value)
{
	return strncmp((const char *)key, *(const char *const *)value,
		strlen(*(const char *const *)value)); // strlen?
}

static const char *get_named_entity(const char *name)
{
	const char *const *entity = (const char *const *)bsearch(name,
		NAMED_ENTITIES, sizeof NAMED_ENTITIES / sizeof *NAMED_ENTITIES,
		sizeof *NAMED_ENTITIES, cmp);

	return entity ? entity[1] : NULL;
}

static size_t putc_utf8(unsigned long cp, char *buffer)
{
	unsigned char *bytes = (unsigned char *)buffer;

	if(cp <= 0x007Ful)
	{
		bytes[0] = (unsigned char)cp;
		return 1;
	}

	if(cp <= 0x07FFul)
	{
		bytes[1] = (unsigned char)((2 << 6) | (cp & 0x3F));
		bytes[0] = (unsigned char)((6 << 5) | (cp >> 6));
		return 2;
	}

	if(cp <= 0xFFFFul)
	{
		bytes[2] = (unsigned char)(( 2 << 6) | ( cp       & 0x3F));
		bytes[1] = (unsigned char)(( 2 << 6) | ((cp >> 6) & 0x3F));
		bytes[0] = (unsigned char)((14 << 4) |  (cp >> 12));
		return 3;
	}

	if(cp <= 0x10FFFFul)
	{
		bytes[3] = (unsigned char)(( 2 << 6) | ( cp        & 0x3F));
		bytes[2] = (unsigned char)(( 2 << 6) | ((cp >>  6) & 0x3F));
		bytes[1] = (unsigned char)(( 2 << 6) | ((cp >> 12) & 0x3F));
		bytes[0] = (unsigned char)((30 << 3) |  (cp >> 18));
		return 4;
	}

	return 0;
}

static bool parse_entity(
	const char *current, char **to, const char **from)
{
	const char *end = strchr(current, ';');
	if(!end) return 0;

	if(current[1] == '#')
	{
		char *tail = NULL;
		int errno_save = errno;
		bool hex = current[2] == 'x' || current[2] == 'X';

		errno = 0;
		unsigned long cp = strtoul(
			current + (hex ? 3 : 2), &tail, hex ? 16 : 10);

		bool fail = errno || tail != end || cp > UNICODE_MAX;
		errno = errno_save;
		if(fail) return 0;

		*to += putc_utf8(cp, *to);
		*from = end + 1;

		return 1;
	}

	const char *entity = get_named_entity(&current[1]);
	if(!entity) return 0;

	size_t len = strlen(entity);
	memcpy(*to, entity, len);

	*to += len;
	*from = end + 1;

	return 1;
}


size_t decode_html_entities_utf8(char *dest, const char *src)
{
	if(!src) src = dest;

	char *to = dest;
	const char *from = src;

	for(const char *current; (current = strchr(from, '&'));)
	{
		memmove(to, from, (size_t)(current - from));
		to += current - from;

		if(parse_entity(current, &to, &from))
			continue;

		from = current;
		*to++ = *from++;
	}

	size_t remaining = strlen(from);

	memmove(to, from, remaining);
	to += remaining;
	*to = 0;

	return (size_t)(to - dest);
}


static bool parse_entity_wo_unsafe_symbols(
	const char *current, char **to, const char **from,
	const char* unsafe_symbs)
{
	const char *end = strchr(current, ';');
	if(!end) return 0;

	if(current[1] == '#')
	{
		char *tail = NULL;
		int errno_save = errno;
		bool hex = current[2] == 'x' || current[2] == 'X';

		errno = 0;
		unsigned long cp = strtoul(
			current + (hex ? 3 : 2), &tail, hex ? 16 : 10);

		bool fail = errno || tail != end || cp > UNICODE_MAX;
		errno = errno_save;
		if(fail) return 0;


		// *to += putc_utf8(cp, *to);
		size_t utf8_symb_len = putc_utf8(cp, *to);

		size_t unsafe_symbs_len;
		for (const char* unsafe_symb = unsafe_symbs; (unsafe_symbs_len = strlen(unsafe_symb)) != 0; unsafe_symb += (unsafe_symbs_len + 1))
		{
			if (utf8_symb_len == unsafe_symbs_len && strncmp(*to, unsafe_symb, utf8_symb_len) == 0)
			{
				// rollback
				size_t html_entities_len = (size_t)(end - current) + 1;
				strncpy(*to, current, html_entities_len);
				utf8_symb_len = html_entities_len;
				break;
			}
		}

		*to += utf8_symb_len;
		*from = end + 1;

		return 1;
	}

	const char *entity = get_named_entity(&current[1]);
	if(!entity) return 0;

	size_t len = strlen(entity);
	memcpy(*to, entity, len);

	*to += len;
	*from = end + 1;

	return 1;
}

/**
 * @param unsafe_symbs strings of unwanted symbs 
 * delimited '\0'. Ends by double '\0' 
 * */
size_t decode_html_entities_utf8_wo_unsafe_symbols(char *dest, const char *src, 
	const char* unsafe_symbs) 
{
	if(!src) src = dest;

	char *to = dest;
	const char *from = src;

	for(const char *current; (current = strchr(from, '&'));)
	{
		memmove(to, from, (size_t)(current - from));
		to += current - from;

		if(parse_entity_wo_unsafe_symbols(current, &to, &from, unsafe_symbs))
			continue;

		from = current;
		*to++ = *from++;
	}

	size_t remaining = strlen(from);

	memmove(to, from, remaining);
	to += remaining;
	*to = 0;

	return (size_t)(to - dest);
}



static int strcmp_n(const char *lhs, size_t lhs_len, const char *rhs)
{
	size_t rhs_len = strlen(rhs);

	if (rhs_len > lhs_len)
		return rhs[lhs_len];
	else if (rhs_len < lhs_len)
		return lhs[rhs_len];

	// equal len
	int i = 0;
	while (lhs[i] == rhs[i] && lhs_len > 0){
		++i;
		--lhs_len;
	}

	return lhs_len == 0 ? 0 : lhs[i] - rhs[i];
}

static size_t c_is_shit = 0;

static int cmp_n(const void *key, const void *value)
{
	return strcmp_n((const char *)key, c_is_shit, 
		(const char *const *)value);
}

static const char *get_named_entity_n(const char *name, size_t name_len)
{
	c_is_shit = name_len;
	const char *const *entity = (const char *const *)bsearch(name,
		NAMED_ENTITIES, sizeof NAMED_ENTITIES / sizeof *NAMED_ENTITIES,
		sizeof *NAMED_ENTITIES, cmp_n);

	return entity ? entity[1] : NULL;
}

static const char* strchr_n(const char* src, size_t src_size, int chr)
{
	size_t i;
	for (i = 0; i < src_size && src[i] != '\0'; ++i)
	{
		if ((int)src[i] == chr)
		{
			break;
		}
	}

	return i < src_size ? &src[i] : NULL;
}

/*https://stackoverflow.com/questions/7457163/what-is-the-implementation-of-strtol*/
static unsigned long 
strtoul_n(const char *restrict nptr, size_t nptr_len, char **restrict endptr, int base) {
    const char *p = nptr, *endp;
    _Bool overflow = 0;
    /* Need long long so (LONG_MAX) can fit in these: */
    long long n = 0UL, cutoff;
    int cutlim;
    if (base < 0 || base == 1 || base > 36) {
#ifdef EINVAL /* errno value defined by POSIX */
        errno = EINVAL;
#endif
        return 0L;
    }
    endp = nptr;
    while (nptr_len > 0 && isspace(*p)){
        p++;
    	--nptr_len;
    }

    if (base == 0)
        base = 10;

    cutoff = LONG_MAX / base;
    cutlim = LONG_MAX % base;
    while (nptr_len > 0) {
        int c;
        if (*p >= 'A')
            c = ((*p - 'A') & (~('a' ^ 'A'))) + 10;
        else if (*p <= '9')
            c = *p - '0';
        else
            break;
        if (c < 0 || c >= base) break;
        endp = ++p;
        if (overflow) {
            /* endptr should go forward and point to the non-digit character
             * (of the given base); required by ANSI standard. */
            if (endptr) continue;
            break;
        }
        if (n > cutoff || (n == cutoff && c > cutlim)) {
            overflow = 1; continue;
        }
        n = n * base + c;
        --nptr_len;
    }

    if (endptr) *endptr = (char *)endp;
    if (overflow) {
        errno = ERANGE; 
        return LONG_MAX;
    }

    return (unsigned long)n;
}

static bool parse_entity_wo_unsafe_symbols_n(
	const char *current, size_t* curr_size,
	char **to, const char **from,
	const char* unsafe_symbs)
{
	const char *end = strchr_n(current, *curr_size, ';');
	if(!end) return 0;

	// *curr_size should be more than 3 (start symb, # and)
	size_t entity_len = (end - current);
	if(entity_len > 3 && current[1] == '#')
	{
		char *tail = NULL;
		int errno_save = errno;
		bool hex = entity_len > 4 && (current[2] == 'x' || current[2] == 'X');

		errno = 0;
		unsigned long cp = strtoul_n(
			current + (hex ? 3 : 2), *curr_size - (hex ? 3 : 2), &tail, hex ? 16 : 10);

		bool fail = errno || tail != end || cp > UNICODE_MAX;
		errno = errno_save;

		// maybe error with tail
		*curr_size -= (tail + 1) - current; // start `symb', `#' and `value'

		if(fail) return 0;

		size_t utf8_symb_len = putc_utf8(cp, *to);

		size_t unsafe_symbs_len;
		for (const char* unsafe_symb = unsafe_symbs; (unsafe_symbs_len = strlen(unsafe_symb)) != 0; unsafe_symb += (unsafe_symbs_len + 1))
		{
			if (utf8_symb_len == unsafe_symbs_len && strncmp(*to, unsafe_symb, utf8_symb_len) == 0)
			{
				// rollback
				size_t html_entities_len = (size_t)(end - current) + 1;
				strncpy(*to, current, html_entities_len);
				utf8_symb_len = html_entities_len;
				break;
			}
		}

		*to += utf8_symb_len;
		*from = end + 1;
		--curr_size;

		return 1;
	}

	if (*curr_size < 2)	
		return 0;

	const char *entity = get_named_entity_n(&current[1], *curr_size - 1); // dangerous
	if(!entity) return 0;

	size_t len = strlen(entity);
	memcpy(*to, entity, len);

	*to += len;

	*curr_size -= (end + 1) - *from;
	*from = end + 1;

	return 1;
}

size_t decode_html_entities_utf8_wo_unsafe_symbols_n(char *dest, const char *src, 
	size_t src_size, const char* unsafe_symbs)
{
	if(!src) src = dest;

	char *to = dest;
	const char *from = src;

	for(const char *current; (current = strchr_n(from, src_size, '&'));)
	{
		memmove(to, from, (size_t)(current - from));
		to += current - from;
		src_size -= current - from;

		if(parse_entity_wo_unsafe_symbols_n(current, &src_size, &to, &from, unsafe_symbs))
			continue;

		from = current;
		*to++ = *from++;
		src_size -= 1;
	}

	memmove(to, from, src_size);
	to += src_size;
	*to = 0;

	return (size_t)(to - dest);
}