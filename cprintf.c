/* cprintf(): Take input printf() syntax and colorize
 *
 * Copyright (c) 2017  Joachim Nilsson <troglobit@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

int colorize = 1;

int cprintf(const char *fmt, ...)
{
	int i = 0, ret;
	char *format, *num, *ptr, ansi[10];
	va_list ap;

	format = calloc(strlen(fmt) + 10, sizeof(char));
	if (!format)
		return 0;

	while (*fmt) {
		if (*fmt == '%') {
			num = ptr = (char *)&fmt[1];
			while (*ptr && isdigit(*ptr))
				ptr++;

			if (*ptr == 'C') {
				int code = 0;

				if (!colorize)
					goto skip;

				if (num != ptr)
					code = atoi(num);

				snprintf(ansi, sizeof(ansi), "\e[%dm", code);
				strcat(format, ansi);
				i += strlen(ansi);
			skip:
				fmt = ++ptr;
				continue;
			}
		}
		format[i++] = *fmt++;
	}

	va_start(ap, fmt);
	ret = vprintf(format, ap);
	va_end(ap);

	free(format);
	return ret;
}

/**
 * Local Variables:
 *  indent-tabs-mode: t
 *  c-file-style: "linux"
 * End:
 */