#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "xmodem.h"

#define XM_RETRY_CNT_MAX (10)
#define XM_DATA_SZ (128)

typedef enum {
  xm_handle_soh_e = 0,
  xm_handle_pkt_sz_e,
  xm_handle_data_e,
  xm_handle_chksum_e,
  xm_handle_eot_e,
} xm_state_t;

typedef struct {
  xm_config_t config;
  bool is_init;
  xm_state_t state;
  uint8_t pkt_idx;
  uint8_t retry_cnt;
  uint8_t buffer[XM_DATA_SZ];
  uint8_t blk_num;
  uint8_t chksum;
} xm_t;

typedef struct {
  uint8_t head;
  uint8_t blk_cnt[2];
  uint8_t data[XM_DATA_SZ];
  uint8_t chksum;
} xm_pkt_t;

static xm_t xm_data = {0};

void xmodem_init(xm_config_t config)
{
  if(config.serial_rx == NULL || config.serial_tx == NULL) {
    memset((void*)&xm_data, 0, sizeof(xm_data));
    return;
  }
  xm_data.config = config;
  xm_data.is_init = true;
  xm_data.state = xm_handle_soh_e;

  /* Starts from #1 block and wraps to #0. */
  xm_data.blk_num = 1;
}

xm_st_t xmodem_run(void)
{
  uint8_t tx_rsp = (uint8_t)xm_nack_e;
  xm_st_t ret = xm_st_nok_e;
  xm_pkt_t pkt = {0};

  if(!xm_data.is_init) {
    return (uint8_t)xm_st_not_init_e;
  }

  switch(xm_data.state) {
    case xm_handle_soh_e:
      xm_data.config.serial_rx(&pkt.head, 1);
      if(pkt.head == xm_soh_e) {
        tx_rsp = (uint8_t)xm_ack_e;
        ret = xm_st_ok_e;
        xm_data.state = xm_handle_pkt_sz_e;
        xm_data.blk_num++;
      } else {
        tx_rsp = (uint8_t)xm_nack_e;
        ret = xm_st_nok_e;
        xm_data.config.serial_tx(&tx_rsp, 1);
        xm_data.state = xm_handle_soh_e;
      }
    break;


    case xm_handle_pkt_sz_e:

    break;

    case xm_handle_eot_e:
      tx_rsp = (uint8_t)xm_ack_e;
      xm_data.config.serial_tx(&tx_rsp, 1);
      xmodem_init(xm_data.config);
    break;

    default:
    break;
  }

  return ret;
}
