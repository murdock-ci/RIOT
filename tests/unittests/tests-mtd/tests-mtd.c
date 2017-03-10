/*
 * Copyright (C) 2016 OTA keys S.A.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 */
#include <string.h>
#include <errno.h>

#include "embUnit.h"

#include "mtd.h"
#include "board.h"

#ifdef MTD_0
#define _dev MTD_0
#else
#ifndef SECTOR_COUNT
#define SECTOR_COUNT 8
#endif
#ifndef PAGE_PER_SECTOR
#define PAGE_PER_SECTOR 4
#endif
#ifndef PAGE_SIZE
#define PAGE_SIZE 255
#endif

static uint8_t dummy_sector[PAGE_PER_SECTOR * PAGE_SIZE];

static int init(mtd_dev_t *dev)
{
    memset(dummy_sector, 0xff, sizeof(dummy_sector));
    return 0;
}

static int read(mtd_dev_t *dev, void *buff, uint32_t addr, uint32_t size)
{
    (void)dev;
    (void)addr;
    memcpy(buff, dummy_sector, size);

    return size;
}

static int write(mtd_dev_t *dev, const void *buff, uint32_t addr, uint32_t size)
{
    (void)dev;
    (void)addr;
    memcpy(dummy_sector, buff, size);

    return size;
}

static int erase(mtd_dev_t *dev, uint32_t addr, uint32_t size)
{
    (void)dev;
    (void)addr;

    if (size % (PAGE_PER_SECTOR * PAGE_SIZE) != 0) {
        return -EOVERFLOW;
    }

    return 0;
}

static int power(mtd_dev_t *dev, enum mtd_power_state power)
{
    (void)dev;
    (void)power;
    return 0;
}

const mtd_desc_t driver = {
    .init = init,
    .read = read,
    .write = write,
    .erase = erase,
    .power = power,
};

static mtd_dev_t _dev = {
    .driver = &driver,
    .sector_count = SECTOR_COUNT,
    .pages_per_sector = PAGE_PER_SECTOR,
    .page_size = PAGE_SIZE,
};
#endif

static void setup_teardown(void)
{
    mtd_dev_t *dev = (mtd_dev_t*) &_dev;

    mtd_erase(dev, 0, dev->pages_per_sector * dev->page_size);
}

static void test_mtd_init(void)
{
    mtd_dev_t *dev = (mtd_dev_t*) &_dev;

    int ret = mtd_init(dev);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

static void test_mtd_erase(void)
{
    mtd_dev_t *dev = (mtd_dev_t*) &_dev;

    int ret = mtd_erase(dev, 0, dev->pages_per_sector * dev->page_size);
    TEST_ASSERT_EQUAL_INT(0, ret);

    ret = mtd_erase(dev, 0, dev->pages_per_sector);
    TEST_ASSERT_EQUAL_INT(-EOVERFLOW, ret);
}

static void test_mtd_write_read(void)
{
    mtd_dev_t *dev = (mtd_dev_t*) &_dev;
    const char buf[] = "ABCDEFGH";
    uint8_t buf_empty[] = {0xff, 0xff, 0xff};
    char buf_read[sizeof(buf) + sizeof(buf_empty)];

    int ret = mtd_write(dev, buf, 0, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(sizeof(buf), ret);

    ret = mtd_read(dev, buf_read, 0, sizeof(buf_read));
    TEST_ASSERT_EQUAL_INT(sizeof(buf_read), ret);
    TEST_ASSERT_EQUAL_INT(0, memcmp(buf, buf_read, sizeof(buf)));
    TEST_ASSERT_EQUAL_INT(0, memcmp(buf_empty, buf_read + sizeof(buf), sizeof(buf_empty)));
}


Test *tests_mtd_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_mtd_init),
        new_TestFixture(test_mtd_erase),
        new_TestFixture(test_mtd_write_read),
    };

    EMB_UNIT_TESTCALLER(mtd_tests, setup_teardown, setup_teardown, fixtures);

    return (Test *)&mtd_tests;
}

void tests_mtd(void)
{
    TESTS_RUN(tests_mtd_tests());
}
/** @} */
