//
// Created by James on 21/09/2015.
//

#ifndef BB_SERIALIZE_H
#define BB_SERIALIZE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int BERR;

#define BERR_SUCCESS 0
#define BERR_OUTOFMEMORY 14

struct bbuf {
    size_t location;
    size_t size;
    char *buf;
};

BERR bb_deserialize_buf(char **out, struct bbuf *const buf, size_t *const size);
BERR bb_serialize_buf(const char *const value, struct bbuf *const buf, size_t size);

int bb_deserialize_int(struct bbuf *const buf);
BERR bb_serialize_int(const int value, struct bbuf *const buf);

size_t bb_deserialize_size_t(struct bbuf *const buf);
BERR bb_serialize_size_t(const size_t value, struct bbuf *const buf);

BERR bb_deserialize_str(char **out, struct bbuf *const buf);
BERR bb_serialize_str(const char *const value, struct bbuf *const buf);

unsigned int bb_deserialize_uint(struct bbuf *const buf);
BERR bb_serialize_uint(const unsigned int value, struct bbuf *const buf);

BERR bb_deserialize_wcs(wchar_t **out, struct bbuf *const buf);
BERR bb_serialize_wcs(wchar_t *const value, struct bbuf *const buf);

void bb_free(struct bbuf * buf);
void bb_head(struct bbuf *const buf);
struct bbuf * bb_init(size_t size);

BERR bb_reserve_space(struct bbuf *const buf, size_t new_data_len);

#ifdef __cplusplus
}
#endif

#endif //BB_SERIALIZE_H
