#ifndef XMODEM_H
#define XMODEM_H

#include <stdint.h>

typedef enum {
	xm_soh_e = 0x01,
	xm_eot_e = 0x04,
	xm_ack_e = 0x06,
	xm_nack_e = 0x15,
	xm_can_e = 0x18,
} xm_frame_char_t;

typedef enum {
	xm_st_ok_e = 0,
	xm_st_nok_e,
	xm_st_not_init_e,
	xm_st_inv,
} xm_st_t;

typedef void (*serial_rx_t)(uint8_t*, uint16_t);

typedef void (*serial_tx_t)(uint8_t*, uint16_t);

typedef struct xmodem_config {
	serial_rx_t serial_rx;
	serial_tx_t serial_tx;
} xm_config_t;

void xmodem_init(xm_config_t config);

#endif
