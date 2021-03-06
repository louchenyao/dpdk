/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 1982, 1986, 1990, 1993
 *      The Regents of the University of California.
 * Copyright(c) 2010-2014 Intel Corporation.
 * All rights reserved.
 */

#ifndef _RTE_TCP_H_
#define _RTE_TCP_H_

/**
 * @file
 *
 * TCP-related defines
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * TCP Header
 */
struct rte_tcp_hdr {
	uint16_t src_port;  /**< TCP source port. */
	uint16_t dst_port;  /**< TCP destination port. */
	uint32_t sent_seq;  /**< TX data sequence number. */
	uint32_t recv_ack;  /**< RX data acknowledgement sequence number. */
	uint8_t  data_off;  /**< Data offset. */
	uint8_t  tcp_flags; /**< TCP flags */
	uint16_t rx_win;    /**< RX flow control window. */
	uint16_t cksum;     /**< TCP checksum. */
	uint16_t tcp_urp;   /**< TCP urgent pointer, if any. */
} __attribute__((__packed__));

/**
 * TCP Flags
 */
#define RTE_TCP_CWR_FLAG 0x80 /**< Congestion Window Reduced */
#define RTE_TCP_ECE_FLAG 0x40 /**< ECN-Echo */
#define RTE_TCP_URG_FLAG 0x20 /**< Urgent Pointer field significant */
#define RTE_TCP_ACK_FLAG 0x10 /**< Acknowledgment field significant */
#define RTE_TCP_PSH_FLAG 0x08 /**< Push Function */
#define RTE_TCP_RST_FLAG 0x04 /**< Reset the connection */
#define RTE_TCP_SYN_FLAG 0x02 /**< Synchronize sequence numbers */
#define RTE_TCP_FIN_FLAG 0x01 /**< No more data from sender */

#ifdef __cplusplus
}
#endif

#endif /* RTE_TCP_H_ */
