#include <time.h>
#include "../gtest-1.7.0/include/gtest/gtest.h"
#include "../bbserialize.h"

GTEST_TEST(SerializeTest, bb_serialize_str)
{
    char *str = "james";

    struct bbuf * buf = bb_init(10);

    BERR err = bb_serialize_str(str, buf);
    ASSERT_EQ((BERR)BERR_SUCCESS, err);

    char *out = (char*)malloc(6);
    buf->location = 0;

    err = bb_deserialize_str(&out, buf);
    EXPECT_STREQ("james", out);

    bb_free(buf);
}

GTEST_TEST(SerializeTest, bb_serialize_wcs)
{
    wchar_t *str1 = L"james";
    wchar_t *str2 = L"baron";

    struct bbuf *buf = bb_init(1);

    BERR err = bb_serialize_wcs(str1, buf);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);

    err = bb_serialize_wcs(str2, buf);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);

    wchar_t *out1 = (wchar_t *)malloc(12);
    wchar_t *out2 = (wchar_t *)malloc(12);
    bb_head(buf);

    err = bb_deserialize_wcs(&out1, buf);
    err = bb_deserialize_wcs(&out2, buf);
    EXPECT_STREQ(L"james", out1);
    EXPECT_STREQ(L"baron", out2);

    free(out1);
    free(out2);
    bb_free(buf);
}

GTEST_TEST(SerializeTest, bb_serialize_uint)
{
    unsigned int int_out;

    struct bbuf *buf = bb_init(1);

    BERR err = bb_serialize_uint(5, buf);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);
    bb_head(buf);
    int_out = bb_deserialize_uint(buf);
    EXPECT_EQ((unsigned int)5, int_out);
    bb_free(buf);
}

GTEST_TEST(SerializeTest, bb_serialize_size_t)
{
    struct bbuf *buf = bb_init(1);

    BERR err = bb_serialize_size_t(5, buf);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);
    bb_head(buf);
    size_t out = bb_deserialize_size_t(buf);
    EXPECT_EQ((size_t)5, out);
    bb_free(buf);
}

GTEST_TEST(SerializeTest, bb_serialize_int)
{
    int out;
    struct bbuf *buf = bb_init(sizeof out);

    BERR err = bb_serialize_int(5, buf);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);

    bb_head(buf);

    out = bb_deserialize_int(buf);

    bb_free(buf);
}

GTEST_TEST(SerializeTest, bb_serialize_deserialize_buf) {

    srand(time(NULL));

    char random_buf_1[512];
    char random_buf_2[512];

    struct bbuf *buf = bb_init(1);

    for (size_t x = 0; x < 512; ++x)
    {
        random_buf_1[x] = (char)(rand() % SCHAR_MAX);
        random_buf_2[x] = (char)(rand() % SCHAR_MAX);
    }

    BERR err = bb_serialize_buf(random_buf_1, buf, 512);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);
    err = bb_serialize_buf(random_buf_2, buf, 512);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);

    char *random_buf_chk_1;
    char *random_buf_chk_2;

    bb_head(buf);

    size_t out_size;

    err = bb_deserialize_buf(&random_buf_chk_1, buf, &out_size);
    EXPECT_EQ((size_t)512, out_size);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);

    err = bb_deserialize_buf(&random_buf_chk_2, buf, &out_size);
    EXPECT_EQ((size_t)512, out_size);
    EXPECT_EQ((BERR)BERR_SUCCESS, err);

    for (size_t x = 0; x < 512; ++x)
    {
        EXPECT_EQ(random_buf_1[x], random_buf_chk_1[x]);
        EXPECT_EQ(random_buf_2[x], random_buf_chk_2[x]);
    }

    free(random_buf_chk_1);
    free(random_buf_chk_2);
    bb_free(buf);
}

