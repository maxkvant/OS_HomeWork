#include<lib.h>
#include <stdarg.h>
    
static void (*putc_f)(char c);


static int64_t toBytesS(int64_t x, int bytes) {
    if (bytes == 1) {
        x = (int64_t)(int8_t)x;
    } else if (bytes == 2) {
        x = (int64_t)(int16_t)x;
    } else if (bytes == 4) {
        x = (int64_t)(int32_t)x;
    }
    return x;
}
static uint64_t toBytesU(uint64_t x, int bytes) {
    if (bytes == 1) {
        x = (uint64_t)(uint8_t)x;
    } else if (bytes == 2) {
        x = (uint64_t)(uint16_t)x;
    } else if (bytes == 4) {
        x = (uint64_t)(uint32_t)x;
    }
    return x;
}

static void putD(int64_t x, int bytes) {
    x = toBytesS(x, bytes);
    int len = 0;
    char ans[20];
    int64_t y = x;
    if (x < 0) {
        x = -x;
    }
    do {
        ans[len++] = x % 10 + '0';
        x /= 10;
    } while(x);
    if (y < 0) {
        putc_f('-');
    }
    while (len--) {
        (*putc_f)(ans[len]);
    }
}

static void putU(uint64_t x, int bytes) {
    x = toBytesU(x, bytes);
    int len = 0;
    char ans[20];
    do {
        ans[len++] = x % 10 + '0';
        x /= 10;
    } while(x);
    while (len--) {
        (*putc_f)(ans[len]);
    }
}

static void putS(char *s) {
    for (; *s; s++) {
        putc_f(*s);
    }
}

static void putX(uint64_t x, int bytes) {
    x = toBytesU(x, bytes);
    putc_f('0');
    putc_f('x');
    for (int i = 64 - 4, b = 0; i >= 0; i -= 4) {
        char c = ((x >> i) & 0xf);
        b |= (i == 0 || c); 
        if (b) {
            if (c < 10) {
                c += '0';
            } else {
                c += 'a' - 10;
            }
            putc_f(c);
        }
    }
}

static void putO(uint64_t x, int bytes) {
    x = toBytesU(x, bytes);
    putc_f('0');
    for (int i = 63, b = 0; i >= 0; i -= 3) {
        char c = ((x >> i) & 0x7);
        b |= (i == 0 || c); 
        if (b) {
            putc_f(c + '0');
        }
    }
}

static int64_t cur_len;

static int output_used = 0;

static void __vprintf(char* s, va_list ap, void(*__putc_f)(char)) {
    lock(&output_used);
    
    cur_len = 0;
    putc_f = __putc_f;
    for (; *s; s++) {
        if (*s == '%') {
            s++;
            int cntl = 0, cnth = 0;
            while (*s == 'l') {
                cntl++;
                s++;
            }
            while (*s == 'h') {
                cnth++;
                s++;
            }
            int bytes = 4;
            if (cntl || cnth) {
                if (cntl) {
                    cntl--;
                }
                bytes = (4 >> cnth) << cntl;
            }
            if (*s == '%') {
                putc_f('%');
            } if (*s == 's') { 
                putS(va_arg(ap, char*));
            } else if (*s == 'c') {
                putc_f((char)va_arg(ap, int));
            } else if (*s == 'x') {
                putX(va_arg(ap, uint64_t), bytes);
            } else if (*s == 'o') {
                putO(va_arg(ap, uint64_t), bytes);
            } else if (*s == 'd' || *s == 'i') {
                putD(va_arg(ap,  int64_t), bytes);
            } else if (*s == 'u') {
                putU(va_arg(ap, uint64_t), bytes);
            }
        } else {
            putc_f(*s);
        }
    }
    output_used = 0;
}

static char* buf;
static int64_t buf_n;

static void putc_n(char c) {
    if (cur_len + 1 < buf_n) {
        buf[cur_len] = c;
        buf[cur_len + 1] = '\0';
        cur_len++;
    }
}

static void putc2(char c) {
    putc(c);
    cur_len++;
}

int64_t vsnprintf(char *__buf, int64_t n, char* s, va_list ap) {
    buf = __buf;
    buf_n = n;
    if (n) {
        buf[0] = '\0';
    }
    __vprintf(s, ap, putc_n);
    return cur_len;
}

int64_t sprintf(char *__buf, int64_t n, char* s, ...) {
    va_list ap;
    va_start(ap, s);
    int64_t res = vsnprintf(__buf, n, s, ap);
    va_end(ap);
    return res;
}

int64_t vprintf(char* s, va_list ap) {
    __vprintf(s, ap, putc2);
    return cur_len;
}

int64_t printf(char* s, ...) {
    va_list ap;
    va_start(ap, s);
    int64_t res =  vprintf(s, ap);
    va_end(ap);
    return res;
}