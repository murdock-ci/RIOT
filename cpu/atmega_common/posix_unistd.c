/*
 * Copyright (C) 2017 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 * @brief   Implements various POSIX syscalls
 * @author  Martine Lenders <mlenders@inf.fu-berlin.de>
 */

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef MODULE_VFS
#include "vfs.h"
#endif

int close(int fd)
{
#ifdef MODULE_VFS
    int res = vfs_close(fd);
    if (res < 0) {
        errno = -res;
        return -1;
    }
    return res;
#else
    (void)fd;
    errno = ENODEV;
    return -1;
#endif
}

off_t lseek(int fd, off_t off, int whence)
{
#ifdef MODULE_VFS
    off_t res = vfs_lseek(fd, off, whence);
    if (res < 0) {
        errno = -res;
        return -1;
    }
    return res;
#else
    (void)fd;
    (void)off;
    (void)whence;
    errno = ENODEV;
    return -1;
#endif
}

int fcntl(int fd, int cmd, int arg)
{
#ifdef MODULE_VFS
    int res = vfs_fcntl(fd, cmd, arg);
    if (res < 0) {
        errno = -res;
        return -1;
    }
    return res;
#else
    (void)fd;
    (void)cmd;
    (void)arg;
    errno = ENODEV;
    return -1;
#endif
}

int fstat(int fd, struct stat *buf)
{
#ifdef MODULE_VFS
    int res = vfs_fstat(fd, buf);
    if (res < 0) {
        errno = -res;
        return -1;
    }
    return res;
#else
    (void)fd;
    (void)buf;
    errno = ENODEV;
    return -1;
#endif
}

ssize_t read(int fd, void *dest, size_t count)
{
#ifdef MODULE_VFS
    ssize_t res = vfs_read(fd, dest, count);
    if (res < 0) {
        errno = -res;
        return -1;
    }
    return res;
#else
    (void)fd;
    (void)dest;
    (void)count;
    errno = ENODEV;
    return -1;
#endif
}

ssize_t write(int fd, const void *src, size_t count)
{
#ifdef MODULE_VFS
    ssize_t res = vfs_write(fd, src, count);
    if (res < 0) {
        errno = -res;
        return -1;
    }
    return res;
#else
    (void)fd;
    (void)src;
    (void)count;
    errno = ENODEV;
    return -1;
#endif
}

/** @} */
