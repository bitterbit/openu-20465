#ifndef H_TEXT
#define H_TEXT

struct TextImpl {
    int (*readText)();
    int (*printText)();
};

typedef struct TextImpl TextImpl;

#endif
