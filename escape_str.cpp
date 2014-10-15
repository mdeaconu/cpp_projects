#include <string.h>
#include <malloc.h>
#include <iostream>

char *replace_str(const char *str, const char *old, const char *new_)
{
	char *ret, *r;
	const char *p, *q;
	size_t oldlen = strlen(old);
	size_t count, retlen, newlen = strlen(new_);
	int samesize = (oldlen == newlen);

	if (!samesize) {
		for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
			count++;
		/* This is undefined if p - str > PTRDIFF_MAX */
		retlen = p - str + strlen(p) + count * (newlen - oldlen);
	} else
		retlen = strlen(str);

	if ((ret = (char *)malloc(retlen + 1)) == NULL)
		return NULL;

	r = ret, p = str;
	while (1) {
		/* If the old and new strings are different lengths - in other
		 * words we have already iterated through with strstr above,
		 * and thus we know how many times we need to call it - then we
		 * can avoid the final (potentially lengthy) call to strstr,
		 * which we already know is going to return NULL, by
		 * decrementing and checking count.
		 */
		if (!samesize && !count--)
			break;
		/* Otherwise i.e. when the old and new strings are the same
		 * length, and we don't know how many times to call strstr,
		 * we must check for a NULL return here (we check it in any
		 * event, to avoid further conditions, and because there's
		 * no harm done with the check even when the old and new
		 * strings are different lengths).
		 */
		if ((q = strstr(p, old)) == NULL)
			break;
		/* This is undefined if q - p > PTRDIFF_MAX */
		ptrdiff_t l = q - p;
		memcpy(r, p, l);
		r += l;
		memcpy(r, new_, newlen);
		r += newlen;
		p = q + oldlen;
	}
	strcpy(r, p);

	return ret;
}

char *escape_str(const char *str)
{
	char *pStrLt = NULL, *pStrGt = NULL;
	char *pStrAmp = NULL;
	pStrAmp = replace_str(str, "&", "&amp;");
	pStrLt = replace_str(pStrAmp, "<", "&lt;");
	pStrGt = replace_str(pStrLt, ">", "&gt;");
	free(pStrAmp);
	free(pStrLt);
	
	return pStrGt;
}

int main()
{
	char a[] = "ion & maria geo>";
	char *pStr = escape_str(a);
	std::cout << strlen(a) << " " << pStr << " " << strlen(pStr) << std::endl;
	return 0;
}
