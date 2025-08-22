/* xls_wrapper.c */
#define _GNU_SOURCE        /* strdup */
#include <string.h>       /* strlen, memcpy, strcmp, strdup */
#include <stdlib.h>
#include "xls_wrapper.h"
#include <xls.h>

static char *strdup_or_null(const char *s)
{
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *p = malloc(len);
    if (p) memcpy(p, s, len);
    return p;
}

/* 对外唯一接口：把 XLS 读成纯文本二维数组 */
XlsText *xls_to_text(const char *filename)
{
    xls_error_t err = LIBXLS_OK;
    xlsWorkBook *wb = xls_open_file(filename, "UTF-8", &err);
    if (!wb) return NULL;

    int total_rows = 0, total_cols = 0;
    for (int i = 0; i < wb->sheets.count; ++i) {
        xlsWorkSheet *ws = xls_getWorkSheet(wb, i);
        if (xls_parseWorkSheet(ws) != LIBXLS_OK) continue;
        total_rows += ws->rows.lastrow + 1;
        if (ws->rows.lastcol + 1 > total_cols)
            total_cols = ws->rows.lastcol + 1;
    }
    if (total_rows == 0 || total_cols == 0) {
        xls_close_WB(wb);
        return NULL;
    }

    char **buf = calloc(total_rows * total_cols, sizeof(char *));
    int row_base = 0;
    for (int i = 0; i < wb->sheets.count; ++i) {
        xlsWorkSheet *ws = xls_getWorkSheet(wb, i);
        if (xls_parseWorkSheet(ws) != LIBXLS_OK) continue;
        for (WORD r = 0; r <= ws->rows.lastrow; ++r) {
            xlsRow *row = xls_row(ws, r);
            for (WORD c = 0; c <= ws->rows.lastcol; ++c) {
                const xlsCell *cell = &row->cells.cell[c];
                char *text = NULL;
                switch (cell->id) {
                case 0x0203: case 0x027E:
                    text = malloc(32);
                    snprintf(text, 32, "%.15g", cell->d);
                    break;
                case 0x00FD: case 0x0204: case 0x00D6:
                    text = strdup_or_null(cell->str);
                    break;
                case 0x0006:
                    if (cell->str && strcmp(cell->str, "bool") == 0)
                        text = strdup(cell->d > 0.0 ? "true" : "false");
                    else if (cell->str && strcmp(cell->str, "error") == 0)
                        text = strdup("#ERROR!");
                    else
                        text = strdup_or_null(cell->str);
                    if (!text) { text = malloc(32); snprintf(text, 32, "%.15g", cell->d); }
                    break;
                case 0x0201:
                default:
                    text = strdup("");
                    break;
                }
                buf[(row_base + r) * total_cols + c] = text;
            }
        }
        row_base += ws->rows.lastrow + 1;
    }

    xls_close_WB(wb);

    XlsText *res = malloc(sizeof *res);
    res->rows = total_rows;
    res->cols = total_cols;
    res->data = buf;
    return res;
}

void xls_text_free(XlsText *t)
{
    if (!t) return;
    for (int i = 0; i < t->rows * t->cols; ++i)
        free(t->data[i]);
    free(t->data);
    free(t);
}