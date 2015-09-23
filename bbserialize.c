#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "bbserialize.h"

BERR bb_deserialize_buf(char **out, struct bbuf *const buf, size_t *const size)
{
    size_t new_buf_len = bb_deserialize_size_t(buf);

    *out = malloc(new_buf_len);
    memcpy(*out, buf->buf + buf->location, new_buf_len);
    buf->location += new_buf_len;
    *size = new_buf_len;

    return BERR_SUCCESS;
}

BERR bb_serialize_buf(const char *const value, struct bbuf *const buf, size_t size)
{
    BERR err = bb_reserve_space(buf, size + sizeof size);

    if(err != BERR_SUCCESS)
        return err;

    err = bb_serialize_size_t(size, buf);

    if(err != BERR_SUCCESS)
        return err;

    memcpy(buf->buf + buf->location, value, size);
    buf->location += size;
    return BERR_SUCCESS;
}

int bb_deserialize_int(struct bbuf *const buf)
{
    int out;
    memcpy(&out, buf->buf + buf->location, sizeof out);
    buf->location += sizeof out;
    return out;
}

BERR bb_serialize_int(const int value, struct bbuf *const buf)
{
    BERR err = bb_reserve_space(buf, sizeof value);

    if(err != BERR_SUCCESS)
        return err;

    memcpy(buf->buf + buf->location, &value, sizeof value);
    buf->location += sizeof value;
    return BERR_SUCCESS;
}

size_t bb_deserialize_size_t(struct bbuf *const buf)
{
    size_t out;
    memcpy(&out, buf->buf + buf->location, sizeof out);
    buf->location += sizeof out;
    return out;
}

BERR bb_serialize_size_t(const size_t value, struct bbuf *const buf)
{
    BERR err = bb_reserve_space(buf, sizeof value);

    if(err != BERR_SUCCESS)
        return err;

    memcpy(buf->buf + buf->location, &value, sizeof value);
    buf->location += sizeof value;
    return BERR_SUCCESS;
}

BERR bb_deserialize_str(char **out, struct bbuf *const buf)
{
    size_t len = strlen(buf->buf + buf->location) + 1;

    char *new_str_out = (char*)malloc(len);

    if(new_str_out == NULL)
        return BERR_OUTOFMEMORY;

    memcpy(new_str_out, buf->buf + buf->location, len);
    buf->location += len;

    *out = new_str_out;

    return BERR_SUCCESS;
}

BERR bb_serialize_str(const char *const value, struct bbuf *const buf)
{
    size_t length = strlen(value);
    BERR err = bb_reserve_space(buf, length);

    if(err != BERR_SUCCESS)
        return err;

    for(int x = 0; x < length; ++x)
    {
        (buf->buf)[(buf->location)++] = value[x];
    }

    (buf->buf)[(buf->location)++] = '\0';

    return BERR_SUCCESS;
}

unsigned int bb_deserialize_uint(struct bbuf *const buf)
{
    unsigned int out;
    memcpy(&out, buf->buf + buf->location, sizeof out);
    buf->location += sizeof out;
    return out;
}

BERR bb_serialize_uint(const unsigned int value, struct bbuf *const buf)
{
    BERR err = bb_reserve_space(buf, sizeof value);

    if(err != BERR_SUCCESS)
        return err;

    memcpy(buf->buf + buf->location, &value, sizeof(unsigned int));
    buf->location += sizeof(int);
    return BERR_SUCCESS;
}

BERR bb_deserialize_wcs(wchar_t **out, struct bbuf *const buf)
{
    size_t len = (wcslen((wchar_t *)buf->buf + buf->location) + 1) * sizeof **out;

    char *new_str_out = (char*)malloc(len);

    if(new_str_out == NULL)
        return BERR_OUTOFMEMORY;

    memcpy(new_str_out, buf->buf + buf->location, len);
    buf->location += len;

    *out = (wchar_t*)new_str_out;

    return BERR_SUCCESS;
}

BERR bb_serialize_wcs(wchar_t *const value, struct bbuf *const buf)
{
    size_t length = (wcslen(value) + 1) * sizeof *value;
    char *buf_in = (char *)value;

    BERR err = bb_reserve_space(buf, length);

    if(err != BERR_SUCCESS)
        return err;

    for(int x = 0; x < length; ++x)
    {
        (buf->buf)[(buf->location)++] = buf_in[x];
    }

    return BERR_SUCCESS;
}

struct bbuf * bb_init(size_t size)
{
    struct bbuf *buf = malloc(sizeof * buf);

    if(buf == NULL)
        return NULL;

    buf->location = 0;
    buf->buf = malloc(size);
    if(buf->buf == NULL) {
        free(buf);
        return NULL;
    }
    buf->size = size;

    return buf;
}

void bb_head(struct bbuf *const buf)
{
    buf->location = 0;
}

void bb_free(struct bbuf * buf)
{
    free(buf->buf);
    free(buf);
}

BERR bb_reserve_space(struct bbuf *const buf, size_t new_data_len)
{
    if(buf->location + new_data_len < buf->size)
        return BERR_SUCCESS;

    size_t new_buf_size = buf->size;

    while(new_buf_size < buf->location + new_data_len)
        new_buf_size *= 2;

    char* new_buf = (char*)realloc(buf->buf, new_buf_size);

    if (new_buf == NULL)
        return BERR_OUTOFMEMORY;

    buf->buf = new_buf;
    buf->size = new_buf_size;

    return BERR_SUCCESS;
}




