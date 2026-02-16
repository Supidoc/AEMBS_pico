/*
 * Copyright (c) 2025-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CDC_LOCK_H_
#define CDC_LOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Locks the CDC channel. Release it afterwards again with ::I2CBus_ReleaseBus()
 */
void CdcLock_Take(void);

/*!
 * \brief Releases the bus againg after a ::CdcLock_Take()
 */
void CdcLock_Give(void);

/*!
 * \brief Module de-initialization
 */
void CdcLock_Deinit(void);

/*!
 * \brief Module initialization
 */
void CdcLock_Init(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* CDC_LOCK_H_ */
