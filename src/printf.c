#include<lib.h>

//va_list -- fake
//printf, snprintf -- not fake

static int64_t va_i;

static void __norm_va_i() {
    if (8 <= va_i && va_i <= 10) {
        va_i = 10;
    } else if (va_i <= 3) {
        va_i = 3;
    }
}

uint64_t __va_arg(uint64_t* va_list) {
    int i = va_i;
    __norm_va_i(va_i);
    va_i++;
    return *(va_list + i);
}

void __set_va_i(int64_t i) {
    va_i = i;
}

#define va_start(va_list, first_arg) \
    va_list = (uint64_t*)&first_arg; \
    __set_va_i(0); \
    __va_arg(va_list); 
    
#define va_arg(list, type) ((type)__va_arg(list))

    
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

static void __vprintf(char* s, uint64_t* va_list, void(*__putc_f)(char)) {
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
                putS(va_arg(va_list, char*));
            } else if (*s == 'c') {
                putc_f(va_arg(va_list, char));
            } else if (*s == 'x') {
                putX(va_arg(va_list, uint64_t), bytes);
            } else if (*s == 'o') {
                putO(va_arg(va_list, uint64_t), bytes);
            } else if (*s == 'd' || *s == 'i') {
                putD(va_arg(va_list,  int64_t), bytes);
            } else if (*s == 'u') {
                putU(va_arg(va_list, uint64_t), bytes);
            }
        } else {
            putc_f(*s);
        }
    }
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

int64_t vsnprintf(char *__buf, int64_t n, char* s, uint64_t* va_list) {
    buf = __buf;
    buf_n = n;
    if (n) {
        buf[0] = '\0';
    }
    __vprintf(s, va_list, putc_n);
    return cur_len;
}

int64_t sprintf(char *__buf, int64_t n, char* s, ...) {
    uint64_t *va_list;
    va_start(va_list, __buf);
    __va_arg(va_list);
    __va_arg(va_list);
    return vsnprintf(__buf, n, s, va_list);
}

int64_t vprintf(char* s, uint64_t* va_list) {
    __vprintf(s, va_list, putc2);
    return cur_len;
}

int64_t printf(char* s, ...) {
    uint64_t *va_list;
    va_start(va_list, s);
    return vprintf(s, va_list);
}