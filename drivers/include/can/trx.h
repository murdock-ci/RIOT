/*
 * Copyright (C) 2016  OTA keys S.A.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    trx_can CAN transceiver
 * @ingroup     can
 * @ingroup     drivers
 * @brief       generic transceiver interface
 *
 * @{
 *
 * @file
 * @brief       generic transceiver interface
 *
 * @author      Aurelien Gonce <aurelien.gonce@altran.com>
 * @author      Vincent Dupont <vincent@otakeys.com>
 */
#ifndef CAN_TRX_H
#define CAN_TRX_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * trx transceiver mode
 */
typedef enum {
    TRX_NORMAL_MODE = 0,
    TRX_SILENT_MODE,
    TRX_SLEEP_MODE,
    /* single wire can modes */
    TRX_HIGH_SPEED_MODE,
    TRX_HIGH_VOLTAGE_WAKE_UP_MODE
} trx_mode_t;

/**
 * @brief forward declaration of trx_driver
 */
typedef struct trx_driver trx_driver_t;

/**
 * @brief Generic transceiver descriptor
 */
typedef struct trx {
    const trx_driver_t *driver; /**< driver */
    trx_mode_t mode;            /**< current mode */
} trx_t;

/**
 * @brief Generic transceiver driver
 */
struct trx_driver {
    /**
     * @brief initialize the trx device
     *
     * @param[in] dev     Transceiver to initialize
     *
     * @return 0 on success
     * @return < 0 on error
     */
    int (*init)(trx_t *dev);

    /**
     * @brief set mode interface
     *
     * @param[in] dev      Transceiver to set
     * @param[in] mode     Mode to set
     *
     * @return  0 on success
     * @return  < 0 on error
     */
    int (*set_mode)(trx_t *dev, trx_mode_t mode);
};

/**
 * @brief initialize a transceiver
 *
 * @param[in] dev      Transceiver to initialize
 *
 * @return  0 on success
 * @return  < 0 on error
 */
int trx_init(trx_t *dev);

/**
 * @brief transceiver set mode
 *
 * @param[in] dev      Transceiver to set
 * @param[in] mode     Mode to set
 *
 * @return  0 on success
 * @return  < 0 on error
 */
int trx_set_mode(trx_t *dev, trx_mode_t mode);


#ifdef __cplusplus
}
#endif

#endif /* CAN_TRX_H */
/** @} */
