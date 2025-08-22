#ifndef XLS_WRAPPER_H
#define XLS_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int  rows;
    int  cols;
    char **data;
} XlsText;

XlsText *xls_to_text(const char *filename);
void     xls_text_free(XlsText *t);

#ifdef __cplusplus
}
#endif
#endif