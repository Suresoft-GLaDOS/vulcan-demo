#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "example.h"

static int static_total_tests = 0;
static int static_failed_tests = 0;
static int test_index = -1;

#define FAIL(str, line) do {                      \
  printf("Fail on line %d: [%s]\n", line, str);   \
  static_failed_tests++;                          \
} while (0)

#define ASSERT(expr) do {               \
  static_total_tests++;                 \
  if (!(expr)) FAIL(#expr, __LINE__);   \
} while (0)
// // expr을 실행하면 안됨!
// #define ASSERT(expr) do {                         \
//   if (static_total_tests == test_index) {         \
//     if (!(expr)) FAIL(#expr, __LINE__);           \
//     printf("result = %d\n", expr);                \
//     return static_failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE; \
//   }                                               \
//   static_total_tests++;                           \
//   printf("test_index = %d\n", test_index);        \
//   if(static_total_tests < test_index) {           \
//     break;                                        \
//   }                                               \
// } while (0)

/* Regex must have exactly one bracket pair */
static char *slre_replace(const char *regex, const char *buf,
                          const char *sub) {
  char *s = NULL;
  int n, n1, n2, n3, s_len, len = strlen(buf);
  struct slre_cap cap = { NULL, 0 };

  do {
    s_len = s == NULL ? 0 : strlen(s);
    if ((n = slre_match(regex, buf, len, &cap, 1, 0)) > 0) {
      n1 = cap.ptr - buf, n2 = strlen(sub),
         n3 = &buf[n] - &cap.ptr[cap.len];
    } else {
      n1 = len, n2 = 0, n3 = 0;
    }
    s = (char *) realloc(s, s_len + n1 + n2 + n3 + 1);
    memcpy(s + s_len, buf, n1);
    memcpy(s + s_len + n1, sub, n2);
    memcpy(s + s_len + n1 + n2, cap.ptr + cap.len, n3);
    s[s_len + n1 + n2 + n3] = '\0';

    buf += n > 0 ? n : len;
    len -= n > 0 ? n : len;
  } while (len > 0);

  return s;
}

int main(int argc, char *argv[]) {
  struct slre_cap caps[10];
  char* p =0;
  test_index = (int)strtol(argv[1], &p, 10);
  
  switch(test_index) {
  /* Metacharacters */
  case 0:
  ASSERT(slre_match("$", "abcd", 4, NULL, 0, 0) == 4);
  break;
  case 1:
  ASSERT(slre_match("^", "abcd", 4, NULL, 0, 0) == 0);
  break;
  case 2:
  ASSERT(slre_match("x|^", "abcd", 4, NULL, 0, 0) == 0);
  break;
  case 3:
  ASSERT(slre_match("x|$", "abcd", 4, NULL, 0, 0) == 4);
  break;
  case 4:
  ASSERT(slre_match("x", "abcd", 4, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 5:
  ASSERT(slre_match(".", "abcd", 4, NULL, 0, 0) == 1);
  break;
  case 6:
  ASSERT(slre_match("^.*\\\\.*$", "c:\\Tools", 8, NULL, 0, SLRE_IGNORE_CASE) == 8);
  break;
  case 7:
  ASSERT(slre_match("\\", "a", 1, NULL, 0, 0) == SLRE_INVALID_METACHARACTER);
  break;
  case 8:
  ASSERT(slre_match("\\x", "a", 1, NULL, 0, 0) == SLRE_INVALID_METACHARACTER);
  break;
  case 9:
  ASSERT(slre_match("\\x1", "a", 1, NULL, 0, 0) == SLRE_INVALID_METACHARACTER);
  break;
  case 10:
  ASSERT(slre_match("\\x20", " ", 1, NULL, 0, 0) == 1);
  break;
  case 11:
  ASSERT(slre_match("^.+$", "", 0, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 12:
  ASSERT(slre_match("^(.+)$", "", 0, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 13:
  ASSERT(slre_match("^([\\+-]?)([\\d]+)$", "+", 1,
                    caps, 10, SLRE_IGNORE_CASE) == SLRE_NO_MATCH);
  break;
  case 14:
  ASSERT(slre_match("^([\\+-]?)([\\d]+)$", "+27", 3,
                    caps, 10, SLRE_IGNORE_CASE) == 3);
  ASSERT(caps[0].len == 1);
  ASSERT(caps[0].ptr[0] == '+');
  ASSERT(caps[1].len == 2);
  ASSERT(memcmp(caps[1].ptr, "27", 2) == 0);
  break;
  case 15:
  ASSERT(slre_match("tel:\\+(\\d+[\\d-]+\\d)",
                    "tel:+1-201-555-0123;a=b", 23, caps, 10, 0) == 19);
  ASSERT(caps[0].len == 14);
  ASSERT(memcmp(caps[0].ptr, "1-201-555-0123", 14) == 0);
  break;
  /* Character sets */
  case 16:
  ASSERT(slre_match("[abc]", "1c2", 3, NULL, 0, 0) == 2);
  break;
  case 17:
  ASSERT(slre_match("[abc]", "1C2", 3, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 18:
  ASSERT(slre_match("[abc]", "1C2", 3, NULL, 0, SLRE_IGNORE_CASE) == 2);
  break;
  case 19:
  ASSERT(slre_match("[.2]", "1C2", 3, NULL, 0, 0) == 1);
  break;
  case 20:
  ASSERT(slre_match("[\\S]+", "ab cd", 5, NULL, 0, 0) == 2);
  break;
  case 21:
  ASSERT(slre_match("[\\S]+\\s+[tyc]*", "ab cd", 5, NULL, 0, 0) == 4);
  break;
  case 22:
  ASSERT(slre_match("[\\d]", "ab cd", 5, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 23:
  ASSERT(slre_match("[^\\d]", "ab cd", 5, NULL, 0, 0) == 1);
  break;
  case 24:
  ASSERT(slre_match("[^\\d]+", "abc123", 6, NULL, 0, 0) == 3);
  break;
  case 25:
  ASSERT(slre_match("[1-5]+", "123456789", 9, NULL, 0, 0) == 5);
  break;
  case 26:
  ASSERT(slre_match("[1-5a-c]+", "123abcdef", 9, NULL, 0, 0) == 6);
  break;
  case 27:
  ASSERT(slre_match("[1-5a-]+", "123abcdef", 9, NULL, 0, 0) == 4);
  break;
  case 28:
  ASSERT(slre_match("[1-5a-]+", "123a--2oo", 9, NULL, 0, 0) == 7);
  break;
  case 29:
  ASSERT(slre_match("[htps]+://", "https://", 8, NULL, 0, 0) == 8);
  break;
  case 30:
  ASSERT(slre_match("[^\\s]+", "abc def", 7, NULL, 0, 0) == 3);
  break;
  case 31:
  ASSERT(slre_match("[^fc]+", "abc def", 7, NULL, 0, 0) == 2);
  break;
  case 32:
  ASSERT(slre_match("[^d\\sf]+", "abc def", 7, NULL, 0, 0) == 3);
  break;
  /* Flags - case sensitivity */
  case 33:
  ASSERT(slre_match("FO", "foo", 3, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 34:
  ASSERT(slre_match("FO", "foo", 3, NULL, 0, SLRE_IGNORE_CASE) == 2);
  break;
  case 35:
  ASSERT(slre_match("(?m)FO", "foo", 3, NULL, 0, 0) ==
    SLRE_UNEXPECTED_QUANTIFIER);
  break;
  case 36:
  ASSERT(slre_match("(?m)x", "foo", 3, NULL, 0, 0) ==
    SLRE_UNEXPECTED_QUANTIFIER);
  break;
  case 37:
  ASSERT(slre_match("fo", "foo", 3, NULL, 0, 0) == 2);
  break;
  case 38:
  ASSERT(slre_match(".+", "foo", 3, NULL, 0, 0) == 3);
  break;
  case 39:
  ASSERT(slre_match(".+k", "fooklmn", 7, NULL, 0, 0) == 4);
  break;
  case 40:
  ASSERT(slre_match(".+k.", "fooklmn", 7, NULL, 0, 0) == 5);
  break;
  case 41:
  ASSERT(slre_match("p+", "fooklmn", 7, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 42:
  ASSERT(slre_match("ok", "fooklmn", 7, NULL, 0, 0) == 4);
  break;
  case 43:
  ASSERT(slre_match("lmno", "fooklmn", 7, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 44:
  ASSERT(slre_match("mn.", "fooklmn", 7, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 45:
  ASSERT(slre_match("o", "fooklmn", 7, NULL, 0, 0) == 2);
  break;
  case 46:
  ASSERT(slre_match("^o", "fooklmn", 7, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 47:
  ASSERT(slre_match("^", "fooklmn", 7, NULL, 0, 0) == 0);
  break;
  case 48:
  ASSERT(slre_match("n$", "fooklmn", 7, NULL, 0, 0) == 7);
  break;
  case 49:
  ASSERT(slre_match("n$k", "fooklmn", 7, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 50:
  ASSERT(slre_match("l$", "fooklmn", 7, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 51:
  ASSERT(slre_match(".$", "fooklmn", 7, NULL, 0, 0) == 7);
  break;
  case 52:
  ASSERT(slre_match("a?", "fooklmn", 7, NULL, 0, 0) == 0);
  break;
  case 53:
  ASSERT(slre_match("^a*CONTROL", "CONTROL", 7, NULL, 0, 0) == 7);
  break;
  case 54:
  ASSERT(slre_match("^[a]*CONTROL", "CONTROL", 7, NULL, 0, 0) == 7);
  break;
  case 55:
  ASSERT(slre_match("^(a*)CONTROL", "CONTROL", 7, NULL, 0, 0) == 7);
  break;
  case 56:
  ASSERT(slre_match("^(a*)?CONTROL", "CONTROL", 7, NULL, 0, 0) == 7);
  break;
  case 57:
  ASSERT(slre_match("\\_", "abc", 3, NULL, 0, 0) == SLRE_INVALID_METACHARACTER);
  break;
  case 58:
  ASSERT(slre_match("+", "fooklmn", 7, NULL, 0, 0) == SLRE_UNEXPECTED_QUANTIFIER);
  break;
  case 59:
  ASSERT(slre_match("()+", "fooklmn", 7, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 60:
  ASSERT(slre_match("\\x", "12", 2, NULL, 0, 0) == SLRE_INVALID_METACHARACTER);
  break;
  case 61:
  ASSERT(slre_match("\\xhi", "12", 2, NULL, 0, 0) == SLRE_INVALID_METACHARACTER);
  break;
  case 62:
  ASSERT(slre_match("\\x20", "_ J", 3, NULL, 0, 0) == 2);
  break;
  case 63:
  ASSERT(slre_match("\\x4A", "_ J", 3, NULL, 0, 0) == 3);
  break;
  case 64:
  ASSERT(slre_match("\\d+", "abc123def", 9, NULL, 0, 0) == 6);
  break;
  /* Balancing brackets */
  case 65:
  ASSERT(slre_match("(x))", "fooklmn", 7, NULL, 0, 0) == SLRE_UNBALANCED_BRACKETS);
  break;
  case 66:
  ASSERT(slre_match("(", "fooklmn", 7, NULL, 0, 0) == SLRE_UNBALANCED_BRACKETS);
  break;
  case 67:
  ASSERT(slre_match("klz?mn", "fooklmn", 7, NULL, 0, 0) == 7);
  break;
  case 68:
  ASSERT(slre_match("fa?b", "fooklmn", 7, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  /* Brackets & capturing */
  case 69:
  ASSERT(slre_match("^(te)", "tenacity subdues all", 20, caps, 10, 0) == 2);
  break;
  case 70:
  ASSERT(slre_match("(bc)", "abcdef", 6, caps, 10, 0) == 3);
  break;
  case 71:
  ASSERT(slre_match(".(d.)", "abcdef", 6, caps, 10, 0) == 5);
  break;
  case 72:
  ASSERT(slre_match(".(d.)\\)?", "abcdef", 6, caps, 10, 0) == 5);
  ASSERT(caps[0].len == 2);
  ASSERT(memcmp(caps[0].ptr, "de", 2) == 0);
  break;
  case 73:
  ASSERT(slre_match("(.+)", "123", 3, caps, 10, 0) == 3);
  break;
  case 74:
  ASSERT(slre_match("(2.+)", "123", 3, caps, 10, 0) == 3);
  ASSERT(caps[0].len == 2);
  ASSERT(memcmp(caps[0].ptr, "23", 2) == 0);
  break;
  case 75:
  ASSERT(slre_match("(.+2)", "123", 3, caps, 10, 0) == 2);
  ASSERT(caps[0].len == 2);
  ASSERT(memcmp(caps[0].ptr, "12", 2) == 0);
  break;
  case 76:
  ASSERT(slre_match("(.*(2.))", "123", 3, caps, 10, 0) == 3);
  break;
  case 77:
  ASSERT(slre_match("(.)(.)", "123", 3, caps, 10, 0) == 2);
  break;
  case 78:
  ASSERT(slre_match("(\\d+)\\s+(\\S+)", "12 hi", 5, caps, 10, 0) == 5);
  break;
  case 79:
  ASSERT(slre_match("ab(cd)+ef", "abcdcdef", 8, NULL, 0, 0) == 8);
  break;
  case 80:
  ASSERT(slre_match("ab(cd)*ef", "abcdcdef", 8, NULL, 0, 0) == 8);
  break;
  case 81:
  ASSERT(slre_match("ab(cd)+?ef", "abcdcdef", 8, NULL, 0, 0) == 8);
  break;
  case 82:
  ASSERT(slre_match("ab(cd)+?.", "abcdcdef", 8, NULL, 0, 0) == 5);
  break;
  case 83:
  ASSERT(slre_match("ab(cd)?", "abcdcdef", 8, NULL, 0, 0) == 4);
  break;
  case 84:
  ASSERT(slre_match("a(b)(cd)", "abcdcdef", 8, caps, 1, 0) ==
      SLRE_CAPS_ARRAY_TOO_SMALL);
  break;
  case 85:
  ASSERT(slre_match("(.+/\\d+\\.\\d+)\\.jpg$", "/foo/bar/12.34.jpg", 18,
                    caps, 1, 0) == 18);
  break;
  case 86:
  ASSERT(slre_match("(ab|cd).*\\.(xx|yy)", "ab.yy", 5, NULL, 0, 0) == 5);
  break;
  case 87:
  ASSERT(slre_match(".*a", "abcdef", 6, NULL, 0, 0) == 1);
  break;
  case 88:
  ASSERT(slre_match("(.+)c", "abcdef", 6, NULL, 0, 0) == 3);
  break;
  case 89:
  ASSERT(slre_match("\\n", "abc\ndef", 7, NULL, 0, 0) == 4);
  break;
    /* Greedy vs non-greedy */
  case 90:
  ASSERT(slre_match(".+c", "abcabc", 6, NULL, 0, 0) == 6);
  break;
  case 91:
  ASSERT(slre_match(".+?c", "abcabc", 6, NULL, 0, 0) == 3);
  break;
  case 92:
  ASSERT(slre_match(".*?c", "abcabc", 6, NULL, 0, 0) == 3);
  break;
  case 93:
  ASSERT(slre_match(".*c", "abcabc", 6, NULL, 0, 0) == 6);
  break;
  case 94:
  ASSERT(slre_match("bc.d?k?b+", "abcabc", 6, NULL, 0, 0) == 5);
  break;
  case 95:
  ASSERT(slre_match("b.\\s*\\n", "aa\r\nbb\r\ncc\r\n\r\n", 14,
                  caps, 10, 0) == 8);
  break;
  /* Branching */
  case 96:
  ASSERT(slre_match("|", "abc", 3, NULL, 0, 0) == 0);
  break;
  case 97:
  ASSERT(slre_match("|.", "abc", 3, NULL, 0, 0) == 1);
  break;
  case 98:
  ASSERT(slre_match("x|y|b", "abc", 3, NULL, 0, 0) == 2);
  break;
  case 99:
  ASSERT(slre_match("k(xx|yy)|ca", "abcabc", 6, NULL, 0, 0) == 4);
  break;
  case 100:
  ASSERT(slre_match("k(xx|yy)|ca|bc", "abcabc", 6, NULL, 0, 0) == 3);
  break;
  case 101:
  ASSERT(slre_match("(|.c)", "abc", 3, caps, 10, 0) == 3);
  ASSERT(caps[0].len == 2);
  ASSERT(memcmp(caps[0].ptr, "bc", 2) == 0);
  break;
  case 102:
  ASSERT(slre_match("a|b|c", "a", 1, NULL, 0, 0) == 1);
  break;
  case 103:
  ASSERT(slre_match("a|b|c", "b", 1, NULL, 0, 0) == 1);
  break;
  case 104:
  ASSERT(slre_match("a|b|c", "c", 1, NULL, 0, 0) == 1);
  break;
  case 105:
  ASSERT(slre_match("a|b|c", "d", 1, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  /* Optional match at the end of the string */
  case 106:
  ASSERT(slre_match("^.*c.?$", "abc", 3, NULL, 0, 0) == 3);
  break;
  case 107:
  ASSERT(slre_match("^.*C.?$", "abc", 3, NULL, 0, SLRE_IGNORE_CASE) == 3);
  break;
  case 108:
  ASSERT(slre_match("bk?", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 109:
  ASSERT(slre_match("b(k?)", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 110:
  ASSERT(slre_match("b[k-z]*", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 111:
  ASSERT(slre_match("ab(k|z|y)*", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 112:
  ASSERT(slre_match("[b-z].*", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 113:
  ASSERT(slre_match("(b|z|u).*", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 114:
  ASSERT(slre_match("ab(k|z|y)?", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 115:
  ASSERT(slre_match(".*", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 116:
  ASSERT(slre_match(".*$", "ab", 2, NULL, 0, 0) == 2);
  break;
  case 117:
  ASSERT(slre_match("a+$", "aa", 2, NULL, 0, 0) == 2);
  break;
  case 118:
  ASSERT(slre_match("a*$", "aa", 2, NULL, 0, 0) == 2);
  break;
  case 119:
  ASSERT(slre_match( "a+$" ,"Xaa", 3, NULL, 0, 0) == 3);
  break;
  case 120:
  ASSERT(slre_match( "a*$" ,"Xaa", 3, NULL, 0, 0) == 3);
  break;
   /* Ignore case flag */
  case 121:
  ASSERT(slre_match("[a-h]+", "abcdefghxxx", 11, NULL, 0, 0) == 8);
  break;
  case 122:
  ASSERT(slre_match("[A-H]+", "ABCDEFGHyyy", 11, NULL, 0, 0) == 8);
  break;
  case 123:
  ASSERT(slre_match("[a-h]+", "ABCDEFGHyyy", 11, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 124:
  ASSERT(slre_match("[A-H]+", "abcdefghyyy", 11, NULL, 0, 0) == SLRE_NO_MATCH);
  break;
  case 125:
  ASSERT(slre_match("[a-h]+", "ABCDEFGHyyy", 11, NULL, 0, SLRE_IGNORE_CASE) == 8);
  break;
  case 126:
  ASSERT(slre_match("[A-H]+", "abcdefghyyy", 11, NULL, 0, SLRE_IGNORE_CASE) == 8);
  break;
  case 127:
  {
    /* Example: HTTP request */
    const char *request = " GET /index.html HTTP/1.0\r\n\r\n";
    struct slre_cap caps[4];

    if (slre_match("^\\s*(\\S+)\\s+(\\S+)\\s+HTTP/(\\d)\\.(\\d)",
                   request, strlen(request), caps, 4, 0) > 0) {
      printf("Method: [%.*s], URI: [%.*s]\n",
             caps[0].len, caps[0].ptr,
             caps[1].len, caps[1].ptr);
    } else {
      printf("Error parsing [%s]\n", request);
    }

    ASSERT(caps[1].len == 11);
    ASSERT(memcmp(caps[1].ptr, "/index.html", caps[1].len) == 0);
  }
  break;
  case 128:
  {
    /* Example: string replacement */
    char *s = slre_replace("({{.+?}})",
                           "Good morning, {{foo}}. How are you, {{bar}}?",
                           "Bob");
    printf("%s\n", s);
    ASSERT(strcmp(s, "Good morning, Bob. How are you, Bob?") == 0);
    free(s);
  }
  break;
  case 129:
  {
    /* Example: find all URLs in a string */
    static const char *str =
      "<img src=\"HTTPS://FOO.COM/x?b#c=tab1\"/> "
      "  <a href=\"http://cesanta.com\">some link</a>";

    static const char *regex = "((https?://)[^\\s/'\"<>]+/?[^\\s'\"<>]*)";
    struct slre_cap caps[2];
    int i, j = 0, str_len = (int) strlen(str);    
    while (j < str_len &&
           (i = slre_match(regex, str + j, str_len - j, caps, 2, SLRE_IGNORE_CASE)) > 0) {
      printf("Found URL: [%.*s]\n", caps[0].len, caps[0].ptr);
      j += i;
    }
  }
  break;
  case 130:
  {
    /* Example more complex regular expression */
    static const char * str = "aa 1234 xy\nxyz";
    static const char * regex = "aa ([0-9]*) *([x-z]*)\\s+xy([yz])";
    struct slre_cap caps[3];
    ASSERT(slre_match(regex, str, strlen(str), caps, 3, 0) > 0);
    ASSERT(caps[0].len == 4);
    ASSERT(caps[1].len == 2);
    ASSERT(caps[2].len == 1);
    ASSERT(caps[2].ptr[0] == 'z');
  }
  break;
  default:
  printf("Test number range: 0 to 130\n");
  static_failed_tests = -1;
  }
  printf("Unit test %s (total test: %d, failed tests: %d)\n",
         static_failed_tests > 0 ? "FAILED" : "PASSED",
         static_total_tests, static_failed_tests);

  return static_failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}