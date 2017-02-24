/*
 * Copyright (C) 2015 Daniel Krebs
 *               2016 INRIA
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    net_gnrc_mac Common MAC module
 * @ingroup     net_gnrc
 * @brief       A MAC module for providing common MAC parameters and helper functions.
 *
 * @{
 *
 * @file
 * @brief       Definitions of GNRC_MAC
 *
 * @author      Daniel Krebs <github@daniel-krebs.net>
 * @author      Shuguo Zhuo  <shuguo.zhuo@inria.fr>
 */

#ifndef GNRC_MAC_H
#define GNRC_MAC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   The default rx queue size for incoming packets
 */
#ifndef GNRC_MAC_RX_QUEUE_SIZE
#define GNRC_MAC_RX_QUEUE_SIZE             (8U)
#endif

/**
 * @brief   The default buffer size for storing dispatching packets
 */
#ifndef GNRC_MAC_DISPATCH_BUFFER_SIZE
#define GNRC_MAC_DISPATCH_BUFFER_SIZE      (8U)
#endif

/**
 * @brief   Count of neighbor nodes in one-hop distance
 */
#ifndef GNRC_MAC_NEIGHBOR_COUNT
#define GNRC_MAC_NEIGHBOR_COUNT            (8U)
#endif

/**
 * @brief   The default queue size for transmission packets coming from higher layers
 */
#ifndef GNRC_MAC_TX_QUEUE_SIZE
#define GNRC_MAC_TX_QUEUE_SIZE             (8U)
#endif

/**
 * @brief   The default largest number of running timeouts in lwmac
 */
#ifndef LWMAC_TIMEOUT_COUNT
#define LWMAC_TIMEOUT_COUNT             (3U)
#endif

#ifdef __cplusplus
}
#endif

#endif /* GNRC_MAC_H */
/** @} */
