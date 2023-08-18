/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Hartmut Holzgraefe <hholzgra@php.net>                        |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_ctype.h"
#include "SAPI.h"
#include "ext/standard/info.h"

#include <ctype.h>

#if HAVE_CTYPE

static PHP_MINFO_FUNCTION(ctype);

static PHP_FUNCTION(ctype_alnum);
static PHP_FUNCTION(ctype_alpha);
static PHP_FUNCTION(ctype_cntrl);
static PHP_FUNCTION(ctype_digit);
static PHP_FUNCTION(ctype_lower);
static PHP_FUNCTION(ctype_graph);
static PHP_FUNCTION(ctype_print);
static PHP_FUNCTION(ctype_punct);
static PHP_FUNCTION(ctype_space);
static PHP_FUNCTION(ctype_upper);
static PHP_FUNCTION(ctype_xdigit);

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_ctype_alnum, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_alpha, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_cntrl, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_digit, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_lower, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_graph, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_print, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_punct, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_space, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_upper, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ctype_xdigit, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ ctype_functions[]
 * Every user visible function must have an entry in ctype_functions[].
 */
static const zend_function_entry ctype_functions[] = {
	PHP_FE(ctype_alnum,	arginfo_ctype_alnum)
	PHP_FE(ctype_alpha,	arginfo_ctype_alpha)
	PHP_FE(ctype_cntrl,	arginfo_ctype_cntrl)
	PHP_FE(ctype_digit,	arginfo_ctype_digit)
	PHP_FE(ctype_lower,	arginfo_ctype_lower)
	PHP_FE(ctype_graph,	arginfo_ctype_graph)
	PHP_FE(ctype_print,	arginfo_ctype_print)
	PHP_FE(ctype_punct,	arginfo_ctype_punct)
	PHP_FE(ctype_space,	arginfo_ctype_space)
	PHP_FE(ctype_upper,	arginfo_ctype_upper)
	PHP_FE(ctype_xdigit,	arginfo_ctype_xdigit)
	PHP_FE_END
};
/* }}} */

/* {{{ ctype_module_entry
 */
zend_module_entry ctype_module_entry = {
	STANDARD_MODULE_HEADER,
	"ctype",
	ctype_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	PHP_MINFO(ctype),
    PHP_CTYPE_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CTYPE
ZEND_GET_MODULE(ctype)
#endif

/* {{{ PHP_MINFO_FUNCTION
 */
static PHP_MINFO_FUNCTION(ctype)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "ctype functions", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ ctype
 */
#define CTYPE(iswhat, allow_digits, allow_minus) \
	zval *c; \
	ZEND_PARSE_PARAMETERS_START(1, 1); \
		Z_PARAM_ZVAL(c) \
	ZEND_PARSE_PARAMETERS_END(); \
	if (Z_TYPE_P(c) == IS_LONG) { \
		if (Z_LVAL_P(c) <= 255 && Z_LVAL_P(c) >= 0) { \
			RETURN_BOOL(iswhat((int)Z_LVAL_P(c))); \
		} else if (Z_LVAL_P(c) >= -128 && Z_LVAL_P(c) < 0) { \
			RETURN_BOOL(iswhat((int)Z_LVAL_P(c) + 256)); \
		} else if (Z_LVAL_P(c) >= 0) { \
			RETURN_BOOL(allow_digits); \
		} else { \
			RETURN_BOOL(allow_minus); \
		} \
	} else if (Z_TYPE_P(c) == IS_STRING) { \
		char *p = Z_STRVAL_P(c), *e = Z_STRVAL_P(c) + Z_STRLEN_P(c); \
		if (e == p) {	\
			RETURN_FALSE;	\
		}	\
		while (p < e) { \
			if(!iswhat((int)*(unsigned char *)(p++))) { \
				RETURN_FALSE; \
			} \
		} \
		RETURN_TRUE; \
	} else { \
		RETURN_FALSE; \
	} \

/* }}} */

/* {{{ proto bool ctype_alnum(mixed c)
   Checks for alphanumeric character(s) */
static PHP_FUNCTION(ctype_alnum)
{
	CTYPE(isalnum, 1, 0);
}
/* }}} */

/* {{{ proto bool ctype_alpha(mixed c)
   Checks for alphabetic character(s) */
static PHP_FUNCTION(ctype_alpha)
{
	CTYPE(isalpha, 0, 0);
}
/* }}} */

/* {{{ proto bool ctype_cntrl(mixed c)
   Checks for control character(s) */
static PHP_FUNCTION(ctype_cntrl)
{
	CTYPE(iscntrl, 0, 0);
}
/* }}} */

/* {{{ proto bool ctype_digit(mixed c)
   Checks for numeric character(s) */
static PHP_FUNCTION(ctype_digit)
{
	CTYPE(isdigit, 1, 0);
}
/* }}} */

/* {{{ proto bool ctype_lower(mixed c)
   Checks for lowercase character(s)  */
static PHP_FUNCTION(ctype_lower)
{
	CTYPE(islower, 0, 0);
}
/* }}} */

/* {{{ proto bool ctype_graph(mixed c)
   Checks for any printable character(s) except space */
static PHP_FUNCTION(ctype_graph)
{
	CTYPE(isgraph, 1, 1);
}
/* }}} */

/* {{{ proto bool ctype_print(mixed c)
   Checks for printable character(s) */
static PHP_FUNCTION(ctype_print)
{
	CTYPE(isprint, 1, 1);
}
/* }}} */

/* {{{ proto bool ctype_punct(mixed c)
   Checks for any printable character which is not whitespace or an alphanumeric character */
static PHP_FUNCTION(ctype_punct)
{
	CTYPE(ispunct, 0, 0);
}
/* }}} */

/* {{{ proto bool ctype_space(mixed c)
   Checks for whitespace character(s)*/
static PHP_FUNCTION(ctype_space)
{
	CTYPE(isspace, 0, 0);
}
/* }}} */

/* {{{ proto bool ctype_upper(mixed c)
   Checks for uppercase character(s) */
static PHP_FUNCTION(ctype_upper)
{
	CTYPE(isupper, 0, 0);
}
/* }}} */

/* {{{ proto bool ctype_xdigit(mixed c)
   Checks for character(s) representing a hexadecimal digit */
static PHP_FUNCTION(ctype_xdigit)
{
	CTYPE(isxdigit, 1, 0);
}
/* }}} */

#endif	/* HAVE_CTYPE */