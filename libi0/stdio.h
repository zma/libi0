#ifndef I0STDIO_H
#define I0STDIO_H

#define _DEBUG_I0STDIO_no

// consts
#define C_0 (0)
#define C_n (10)
#define C_r (13)
#define C_CR (13)
#define C_t (9)

// End of file character.
#ifndef EOF
# define EOF (-1)
#endif

// print a char to STDOUT
void output_char(long c)
{
    *(long *)(0x100000208) = (long)c;
    return;
}

#define putchar(c) output_char(c)
#define put1(c) putchar(c)
#define put2(a,b) {put1(a); put1(b);}
#define put4(a,b,c,d) {put2(a,b); put2(c,d);}
#define put8(a,b,c,d,e,f,g,h) {put4(a,b,c,d); put4(e,f,g,h);}
#define writeln4(a,b,c,d) {put4(a,b,c,d); put1(C_n);}
#define writeln6(a,b,c,d,e,f) {put4(a,b,c,d); put2(e,f); put1(C_n);}
#define writelns6(a) {writeln6(a[0],a[1],a[2],a[3],a[4],a[5]);}
#define writeln3(a,b,c) {put2(a,b); put2(c, C_n);}
#define writelns4(a) {put4(a[0], a[1], a[2], a[3]); put1(C_n);}
#define wr5cqn(a,b,c,d,e,q) {put4(a,b,c,d);output_q(q);put1(C_n);}
#define wr4n writeln4
#define wr8n(a,b,c,d,e,f,g,h) {put4(a,b,c,d); put4(e,f,g,h);put1(C_n);}
#define wrln() {putchar(C_n);}

#define putq(a) output_q(a)
#define output_long(a) output_q(a)
#define output_long_hex(a) output_q_hex(a)

// print a 64bit integer to STDOUT
void output_q(long n)
{
    long num;
    long c;
    long num2;
    long count;

    num = n;
    if (n < 0) {
       num = 0 - n;
       output_char((long)'-');
    }

    num2 = 0;

    count = 0;
output_q_j1:
    c = num - (num / 10)*10;
    num = num / 10;
    num2 = num2 * 10;
    num2 = num2 + c;
    count = count + 1;
    if (num > 0) {
        goto output_q_j1;
    }

    num = 0;
output_q_j2:
    // output num by byte
    c = num2 - (num2 / 10)*10;
    num2 = num2 / 10;
    c = c + 48;
    output_char((long)c);
    num = num + 1;
    if (num < count) {
        goto output_q_j2;
    }

    return;
}

// print a 64bit integer of at least d digits (filled 0s for padding)
// to STDOUT
void output_q_digits(long n, long d)
{
    long num;
    long c;
    long num2;
    long count;

    num = n;
    if (n < 0) {
       num = 0 - n;
       output_char((long)'-');
    }

    num2 = 0;

    count = 0;
    while (num > 0) {
        c = num - (num / 10)*10;
        num = num / 10;
        num2 = num2 * 10;
        num2 = num2 + c;
        count = count + 1;
    }

    num = 0;

    while (num + count < d) {
        output_char('0');
        num = num + 1;
    }

    num = 0;
    while (num < count) {
        // output num by byte
        c = num2 - (num2 / 10)*10;
        num2 = num2 / 10;
        c = c + 48;
        output_char((long)c);
        num = num + 1;
    }

    return;
}

// print a 64bit integer in hexdecimal format to STDOUT
// Note: only print 15 bits
void output_q_hex(long n)
{
    long num;
    long c;
    long num2;

    long i;

    num = n;
    num2 = n;

    output_char((long)'0');
    output_char((long)'x');

    num2 = 0;
    i = 0;
output_q_hex_j1:
    c = num & 0xF;

    num = num / 16;
    num2 = num2 * 16;
    num2 = num2 + c;

#ifdef _DEBUG_I0STDIO_
    if (c < 10) {
        c = c + 48;
    } else {
        c = c + 55;
    }
    output_char((long)c);
#endif

    i = i + 1;
    if (i < 15) {
        goto output_q_hex_j1;
    }

#ifdef _DEBUG_I0STDIO_
    output_char(C_n);
    output_q(num2);
    output_char(C_n);
#endif

    i = 0;
output_q_hex_j2:
    // output num by byte
    c = num2 & 0xF;
    num2 = num2 / 16;
    if (c < 10) {
        c = c + 48;
    } else {
        c = c + 55;
    }
    output_char((long)c);
    i = i + 1;
    if (i < 15) {
        goto output_q_hex_j2;
    }

    return;
}

// print the current runner's to STDOUT
void output_id()
{
    long id;
    id = *(long*)0x100000120;
    output_q_hex(id);
    return;
}

// print @sl charactores in @s to STDOUT
void output_char_str(char* s, long sl)
{
    long n;
    n = 0;
    while (n < sl) {
        output_char((long)s[n]);
        n = n + 1;
    }

    return;
}

// print string @s to STDOUT
void output_str(char* s)
{
    long n;
    n = 0;
output_str_j1:
    if (s[n] != 0) {
        output_char((long)s[n]);
        n = n + 1;
        goto output_str_j1;
    }

    /*
    if (n == 0) {
        output_char((long)'N');
        output_char((long)'U');
        output_char((long)'L');
        output_char((long)'L');
    }
    */

    return;
}

// input a char from STDIN
#define __input_char() *(long*)0x100000200

// // for debug only
// long __input_char() {
//     long q;
//     q = *(long*)0x100000200;
//     output_char(':');
//     output_q(q);
//     output_char(':');
//     output_char(q);
//     output_char(C_n);
//     return q;
// }

#define getc1 input_char

// input a char from STDIN
char input_char()
{
    long n;

    n = *(long*)0x100000200;

    return (char)n;
}

// input a long from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
long input_q()
{
    long c;
    long is_neg;
    long is_checking;
    long n;

    n = 0;
    is_neg = 0;
    is_checking = 1;

    // try to get -
    do {
        c = __input_char();
        if (c == EOF) goto input_q_exit;
        if (c == (long)'-') {
            c = __input_char();
            if (c <= (long)'9') {
                if (c >= (long)'0') {
                    is_neg = 1;
                    is_checking = 0;
                }
            }
        } else if (c <= (long)'9') {
            if (c >= (long)'0') {
                is_checking = 0;
            }
        }
    } while (is_checking == 1);

    is_checking = 1;
    // already one digit in c
    do {
        n = n * 10 + (long)c - (long)'0';
        // read next char
        c = __input_char();
        if (c == EOF) goto input_q_exit;
        if (c > (long)'9') {
            is_checking = 0;
        }
        if (c < (long)'0') {
            is_checking = 0;
        }
    } while (is_checking == 1);

input_q_exit:
    if (is_neg == 1) {
        n = 0 - n;
    }

    return n;
}

// input a long from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// result is in *pq
// return 0 on success
// -1 on end of file
long input_long(long *result)
{
    long c;
    long is_neg;
    long is_checking;
    long n;
    long ret;

    n = 0;
    is_neg = 0;
    is_checking = 1;
    ret = -1;

    // try to get -
    do {
        c = __input_char();
        if (c == EOF) goto input_long_exit;
        if (c == (long)'-') {
            c = __input_char();
            if (c <= (long)'9') {
                if (c >= (long)'0') {
                    is_neg = 1;
                    is_checking = 0;
                    ret = 0;
                }
            }
        } else if (c <= (long)'9') {
            if (c >= (long)'0') {
                is_checking = 0;
                ret = 0;
            }
        }
    } while (is_checking == 1);

    is_checking = 1;
    // already one digit in c
    do {
        n = n * 10 + (long)c - (long)'0';
        // read next char
        c = __input_char();
        if (c == EOF) goto input_long_exit;
        if (c > (long)'9') {
            is_checking = 0;
        }
        if (c < (long)'0') {
            is_checking = 0;
        }
    } while (is_checking == 1);

input_long_exit:
    if (is_neg == 1) {
        n = 0 - n;
    }

    *result = n;
    return ret;
}

#define IEEE754_SIGN   0x8000000000000000
#define IEEE754_EXP    0x7FF0000000000000
#define IEEE754_FRAC   0x000FFFFFFFFFFFFF
#define IEEE754_2POW52 0x0010000000000000

// N x 2 ^ M
void output_double_binary(double d)
{
    long exp;
    long exp_raw;
    long frac;
    long frac_raw;
    long sign;
    long lv;

    lv = *(long*)(&d);

    sign = IEEE754_SIGN & lv;
    frac_raw = (IEEE754_FRAC & lv);
    frac = frac_raw + IEEE754_2POW52;

    exp_raw = IEEE754_EXP & lv;
    exp_raw = exp_raw / IEEE754_2POW52; // better sith shift; compiler limitation
    exp = exp_raw - 1023 - 52;

    // handle 0
    if (exp_raw == 0) {
        if (frac_raw == 0) {
            putchar('0');
            return;
        }
    }

    // hanlde Inf and NaNs
    if (exp_raw == 0x7FF) {
        if (frac_raw == 0) {
            putchar('I'); putchar('n'); putchar('f');
            return;
        } else {
            putchar('N'); putchar('a'); putchar('N'); putchar('s');
            return;
        }
    }

    if (sign == IEEE754_SIGN) {
        putchar('-');
    }

    output_q(frac);

    putchar('L'); putchar('*'); putchar('2'); putchar('*'); putchar('*');

    output_q(exp);

    return;
}

void output_double(double d)
{
    long n;
    long i;

    if (d < 0.0) {
        d = 0.0 - d;
        putchar('-');
    }

    n = (long)d;
    putq(n);
    d = d - (double)n;

    putchar('.');

    for (i = 0; i < 16; i = i + 1) {
        d = d * 10.0;
        n = (long)d;
        putq(n);
        d = d - (double)n;
    }

    return;
}

// input a double from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// must leading with 0.xxx or nnn.xxx
// that is, .xxx is not supported
// result is in *result
// return 0 on success
// -1 on end of file
long input_double(double *result)
{
    long c;
    long is_neg;
    long is_checking;
    long n;
    double d;
    double scale;

    long ret;

    n = 0;
    d = 0.0;
    is_neg = 0;
    is_checking = 1;
    ret = -1;

    // try to get -
    do {
        c = __input_char();
        if (c == EOF) goto input_double_exit;
        if (c == (long)'-') {
            c = __input_char();
            if (c <= (long)'9') {
                if (c >= (long)'0') {
                    is_neg = 1;
                    is_checking = 0;
                    ret = 0;
                }
            }
        } else if (c <= (long)'9') {
            if (c >= (long)'0') {
                is_checking = 0;
                ret = 0;
            }
        }
    } while (is_checking == 1);

    is_checking = 1;
    // already one digit in c
    // read the integer part
    do {
        n = n * 10 + (long)c - (long)'0';
        // read next char
        c = __input_char();
        if (c == EOF) goto input_double_exit;
        if (c == (long)'.') goto input_double_after_dot;
        if (c > (long)'9') {
            is_checking = 0;
        }
        if (c < (long)'0') {
            is_checking = 0;
        }
    } while (is_checking == 1);

input_double_after_dot:
    is_checking = 1;
    scale = 0.1;
    do {
        // read next char
        c = __input_char();
        if (c == EOF) goto input_double_exit;
        if (c <= (long)'9') {
            if (c >= (long)'0') {
                c = (long)c - (long)'0';
                d = d + scale * (double)c;
                scale = scale / 10.0;
            } else {
                is_checking = 0;
            }
        } else {
            is_checking = 0;
        }
        n = n * 10 + (long)c - (long)'0';
    } while (is_checking == 1);

input_double_exit:
    d = (double)n + d;
    if (is_neg == 1) {
        d = 0.0 - d;
    }

    *result = d;
    return ret;
}

#endif // I0STDIO_H
