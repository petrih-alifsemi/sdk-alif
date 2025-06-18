/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>


// #define UART_NODE   DT_NODELABEL(uart0) //DT_ALIAS(test_uart)
#define UART_NODE DT_ALIAS(test_uart)

/*
 \brief struct UART_Type:- Register map for UART
 */
typedef struct {                                     /*!< UART Register Structure                                                   */

  union {
    volatile const  uint32_t UART_RBR;               /*!< (@ 0x00000000) Receive Buffer Register                                    */
    volatile uint32_t UART_DLL;                      /*!< (@ 0x00000000) Divisor Latch Low Register                                 */
    volatile uint32_t UART_THR;                      /*!< (@ 0x00000000) Transmit Holding Register                                  */
  };

  union {
    volatile uint32_t UART_DLH;                      /*!< (@ 0x00000004) Divisor Latch High Register                                */
    volatile uint32_t UART_IER;                      /*!< (@ 0x00000004) Interrupt Enable Register                                  */
  };

  union {
    volatile uint32_t UART_FCR;                      /*!< (@ 0x00000008) FIFO Control Register                                      */
    volatile const  uint32_t UART_IIR;               /*!< (@ 0x00000008) Interrupt Identification Register                          */
  };

    volatile uint32_t  UART_LCR;                     /*!< (@ 0x0000000C) Line Control Register                                      */
    volatile uint32_t  UART_MCR;                     /*!< (@ 0x00000010) Modem Control Register                                     */
    volatile const  uint32_t  UART_LSR;              /*!< (@ 0x00000014) Line Status Register                                       */
    volatile const  uint32_t  UART_MSR;              /*!< (@ 0x00000018) Modem Status Register                                      */
    volatile uint32_t  UART_SCR;                     /*!< (@ 0x0000001C) Scratchpad Register                                        */
    volatile const  uint32_t  RESERVED[4];

  union {
    volatile const  uint32_t UART_SRBR[16];          /*!< (@ 0x00000030) Shadow Receive Buffer Register (n)                         */
    volatile uint32_t UART_STHR[16];                 /*!< (@ 0x00000030) Shadow Transmit Holding Register (n)                       */
  };

    volatile uint32_t  UART_FAR;                     /*!< (@ 0x00000070) FIFO Access Register                                       */
    volatile const  uint32_t  UART_TFR;              /*!< (@ 0x00000074) Tx FIFO Read Register                                      */
    volatile uint32_t  UART_RFW;                     /*!< (@ 0x00000078) Rx FIFO Write Register                                     */
    volatile const  uint32_t  UART_USR;              /*!< (@ 0x0000007C) UART Status Register                                       */
    volatile const  uint32_t  UART_TFL;              /*!< (@ 0x00000080) Tx FIFO Level Register                                     */
    volatile const  uint32_t  UART_RFL;              /*!< (@ 0x00000084) Rx FIFO Level Register                                     */
    volatile uint32_t  UART_SRR;                     /*!< (@ 0x00000088) Software Reset Register                                    */
    volatile uint32_t  UART_SRTS;                    /*!< (@ 0x0000008C) Shadow Request to Send Register                            */
    volatile uint32_t  UART_SBCR;                    /*!< (@ 0x00000090) Shadow Break Control Register                              */
    volatile uint32_t  UART_SDMAM;                   /*!< (@ 0x00000094) Shadow DMA Mode Register                                   */
    volatile uint32_t  UART_SFE;                     /*!< (@ 0x00000098) Shadow FIFO Enable Register                                */
    volatile uint32_t  UART_SRT;                     /*!< (@ 0x0000009C) Shadow RCVR Trigger Register                               */
    volatile uint32_t  UART_STET;                    /*!< (@ 0x000000A0) Shadow Tx Empty Trigger Register                           */
    volatile uint32_t  UART_HTX;                     /*!< (@ 0x000000A4) Halt Tx Register                                           */
    volatile uint32_t  UART_DMASA;                   /*!< (@ 0x000000A8) DMA Software Acknowledge Register                          */
    volatile uint32_t  UART_TCR;                     /*!< (@ 0x000000AC) Transceiver Control Register                               */
    volatile uint32_t  UART_DE_EN;                   /*!< (@ 0x000000B0) Driver Output Enable Register                              */
    volatile uint32_t  UART_RE_EN;                   /*!< (@ 0x000000B4) Receiver Output Enable Register                            */
    volatile uint32_t  UART_DET;                     /*!< (@ 0x000000B8) Driver Output Enable Timing Register                       */
    volatile uint32_t  UART_TAT;                     /*!< (@ 0x000000BC) Turnaround Timing Register                                 */
    volatile uint32_t  UART_DLF;                     /*!< (@ 0x000000C0) Divisor Latch Fraction Register                            */
    volatile uint32_t  UART_RAR;                     /*!< (@ 0x000000C4) Receive Address Register                                   */
    volatile uint32_t  UART_TAR;                     /*!< (@ 0x000000C8) Transmit Address Register                                  */
    volatile uint32_t  UART_LCR_EXT;                 /*!< (@ 0x000000CC) Line Extended Control Register                             */
    volatile const  uint32_t  RESERVED1;
    volatile uint32_t  UART_REG_TIMEOUT_RST;         /*!< (@ 0x000000D4) Timeout Counter Reset Value Register                       */
    volatile const  uint32_t  RESERVED2[7];
    volatile const  uint32_t  UART_CPR;              /*!< (@ 0x000000F4) Module Configuration Register                              */
    volatile const  uint32_t  UART_UCV;              /*!< (@ 0x000000F8) Reserved                                                   */
    volatile const  uint32_t  UART_CTR;              /*!< (@ 0x000000FC) Reserved                                                   */
} UART_Type;                                         /*!< Size = 256 (0x100)                                                        */

volatile UART_Type *uart_config = (UART_Type *)DT_REG_ADDR(UART_NODE);



/**
  * @brief DMA_DMA_CHANNEL_RT_INFO_Type [DMA_CHANNEL_RT_INFO] ([0..7])
  */
typedef struct {
  volatile const  uint32_t  DMA_CSR;            /*!< (@ 0x00000000) Channel Status for DMA Channel (n) Register                */
  volatile const  uint32_t  DMA_CPC;            /*!< (@ 0x00000004) Channel PC for DMA Channel (n) Register                    */
} DMA_DMA_CHANNEL_RT_INFO_Type;                 /*!< Size = 8 (0x8)                                                            */


/**
  * @brief DMA_DMA_RT_CHANNEL_CFG_Type [DMA_RT_CHANNEL_CFG] ([0..7])
  */
typedef struct {
  volatile const  uint32_t  DMA_SAR;            /*!< (@ 0x00000000) Source Address for DMA Channel (n) Register                */
  volatile const  uint32_t  DMA_DAR;            /*!< (@ 0x00000004) Destination Address for DMA Channel (n) Register           */
  volatile const  uint32_t  DMA_CCR;            /*!< (@ 0x00000008) Channel Control for DMA Channel (n) Register               */
  volatile const  uint32_t  DMA_LC0;            /*!< (@ 0x0000000C) Loop Counter 0 for DMA Channel (n) Register                */
  volatile const  uint32_t  DMA_LC1;            /*!< (@ 0x00000010) Loop Counter 1 for DMA Channel (n) Register                */
  volatile const  uint32_t  RESERVED[3];
} DMA_DMA_RT_CHANNEL_CFG_Type;                  /*!< Size = 32 (0x20)                                                          */


/**
  * @brief DMA (DMA)
  */

typedef struct {                                /*!< (@ 0x00000000) DMA Structure                                         */
  volatile const  uint32_t  DMA_DSR;            /*!< (@ 0x00000000) DMA Manager Status Register                                */
  volatile const  uint32_t  DMA_DPC;            /*!< (@ 0x00000004) DMA Program Counter Register                               */
  volatile const  uint32_t  RESERVED[6];
  volatile        uint32_t  DMA_INTEN;          /*!< (@ 0x00000020) Interrupt Enable Register                                  */
  volatile const  uint32_t  DMA_INT_EVENT_RIS;  /*!< (@ 0x00000024) Event-Interrupt Raw Status Register                        */
  volatile const  uint32_t  DMA_INTMIS;         /*!< (@ 0x00000028) Interrupt Status Register                                  */
  volatile        uint32_t  DMA_INTCLR;         /*!< (@ 0x0000002C) Interrupt Clear Register                                   */
  volatile const  uint32_t  DMA_FSRD;           /*!< (@ 0x00000030) Fault Status DMA Manager Register                          */
  volatile const  uint32_t  DMA_FSRC;           /*!< (@ 0x00000034) Fault Status DMA Channel Register                          */
  volatile const  uint32_t  DMA_FTRD;           /*!< (@ 0x00000038) Fault Type DMA Manager Register                            */
  volatile const  uint32_t  RESERVED1;
  volatile const  uint32_t  DMA_FTR[8];         /*!< (@ 0x00000040) Fault Type for DMA Channel (n) Register                    */
  volatile const  uint32_t  RESERVED2[40];
  volatile DMA_DMA_CHANNEL_RT_INFO_Type DMA_CHANNEL_RT_INFO[8];/*!< (@ 0x00000100) [0..7]                                    */
  volatile const  uint32_t  RESERVED3[176];
  volatile DMA_DMA_RT_CHANNEL_CFG_Type DMA_RT_CHANNEL_CFG[8];/*!< (@ 0x00000400) [0..7]                                      */
  volatile const  uint32_t  RESERVED4[512];
  volatile const  uint32_t  DMA_DBGSTATUS;      /*!< (@ 0x00000D00) Debug Status Register                                      */
  volatile        uint32_t  DMA_DBGCMD;         /*!< (@ 0x00000D04) Debug Command Register                                     */
  volatile        uint32_t  DMA_DBGINST0;       /*!< (@ 0x00000D08) Debug Instruction Register 0                               */
  volatile        uint32_t  DMA_DBGINST1;       /*!< (@ 0x00000D0C) Debug Instruction Register 1                               */
  volatile const  uint32_t  RESERVED5[60];
  volatile const  uint32_t  DMA_CR0;            /*!< (@ 0x00000E00) Configuration Register 0                                   */
  volatile const  uint32_t  DMA_CR1;            /*!< (@ 0x00000E04) Configuration Register 1                                   */
  volatile const  uint32_t  DMA_CR2;            /*!< (@ 0x00000E08) Configuration Register 2                                   */
  volatile const  uint32_t  DMA_CR3;            /*!< (@ 0x00000E0C) Configuration Register 3                                   */
  volatile const  uint32_t  DMA_CR4;            /*!< (@ 0x00000E10) Configuration Register 4                                   */
  volatile const  uint32_t  DMA_CRD;            /*!< (@ 0x00000E14) DMA Configuration Register                                 */
  volatile const  uint32_t  RESERVED6[26];
  volatile        uint32_t  DMA_WD;             /*!< (@ 0x00000E80) Watchdog Register                                          */
} DMA_Type;                                     /*!< Size = 3716 (0xe84)                                                       */

// volatile DMA_Type * dma_config = (DMA_Type*)0x400c0000;
volatile DMA_Type *dma_config = (DMA_Type *)DT_REG_ADDR(DT_NODELABEL(dma2));

K_SEM_DEFINE(wait_sem, 0, 1);


static const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);

#define BUFF_SIZE 32

static bool pingpong;
static uint8_t rx_buffer[2][BUFF_SIZE];

#define RX_SIZE BUFF_SIZE // 4

void send_error(const char * err)
{
	size_t const len = strnlen(err, 32);
	for (size_t iter = 0; iter < len; iter++)
		uart_poll_out(uart_dev, err[iter]);
}

void async_send(const char * str)
{
	size_t const len = strlen(str);
	if (uart_tx(uart_dev, str, len, 10000000/*SYS_FOREVER_US*/)) {
		send_error("!!tx\r\n");
	}
}

char debug_str[128];

void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type) {
	case UART_TX_DONE:
		//send_error("-txrdy\r\n");
		//k_sem_give(&wait_sem);
		break;
	case UART_TX_ABORTED:
		//send_error("txabrd\r\n");
		//k_sem_give(&wait_sem);
		break;
	case UART_RX_RDY: /* rx ready */
		char * p_start = evt->data.rx.buf + evt->data.rx.offset;
		snprintf(debug_str, sizeof(debug_str), "len: %u - %s\r\n", evt->data.rx.len, (char*)p_start);
		async_send(debug_str);
		// buf->len = evt->data.rx.len;
		//if ((uintptr_t)evt->data.rx.buf != (uintptr_t)rx_buffer)
		//	uart_poll_out(uart_dev, '!');
		//k_sem_give(&wait_sem);
		break;
	case UART_RX_BUF_REQUEST: /* new buffer */
		pingpong ^= true;
		memset(rx_buffer[pingpong], 0, sizeof(rx_buffer[pingpong]));
		if (uart_rx_buf_rsp(uart_dev, rx_buffer[pingpong], RX_SIZE)) {
			send_error("!!buf\r\n");
		}
		//send_error("rxbuf\r\n");
		break;
	case UART_RX_BUF_RELEASED:
		//send_error("rxrel\r\n");
		break;
	case UART_RX_DISABLED:
		// uart_rx_enable(uart, buf->data, sizeof(buf->data), UART_WAIT_FOR_RX_US);
		//send_error("rxdis\r\n");
		break;
	case UART_RX_STOPPED:
		//send_error("rxstp\r\n");
		break;
	}
}

#define UART_DMA_RX_GROUP 0x0
#define UART_DMA_TX_GROUP 0x0
#define UART_DMA_RX_REQ 10
#define UART_DMA_TX_REQ 18

#if CONFIG_SOC_FAMILY_ENSEMBLE
#define REG_DMA_CTRL0 EVTRTR0_DMA_CTRL0
#define REG_DMA_ACK_TYPE0 EVTRTR0_DMA_ACK_TYPE0
#elif CONFIG_SOC_FAMILY_BALLETTO
#define REG_DMA_CTRL0 EVTRTRLOCAL_DMA_CTRL0
#define REG_DMA_ACK_TYPE0 EVTRTRLOCAL_DMA_ACK_TYPE0
#else
#error "Invalid SOC family"
#endif

#define EVTRTR_DMA_CTRL_ENA        (1U << 4)
#define EVTRTR_DMA_CTRL_ACK_PERIPH (0x0 << 16)
#define EVTRTR_DMA_CTRL_ACK_ROUTER (0x1 << 16)

static int configure_dma_event_router(const uint32_t dma_group, const uint32_t dma_request)
{
	uint32_t regdata;

	if (dma_group > 3) {
		return -EINVAL;
	}

	if (dma_request > 31) {
		return -EINVAL;
	}

	/* Enable event router channel */
	regdata = EVTRTR_DMA_CTRL_ENA + EVTRTR_DMA_CTRL_ACK_PERIPH + dma_group;
	sys_write32(regdata, REG_DMA_CTRL0 + (dma_request * 0x4));

	/* DMA Handshake enable */
	regdata = sys_read32(REG_DMA_ACK_TYPE0 + (dma_group * 0x4));
	regdata |= (0x1 << dma_request);
	sys_write32(regdata, REG_DMA_ACK_TYPE0 + (dma_group * 0x4));

	return 0;
}

int main(void)
{
#if 0
	volatile bool start_wait = true;
	while (start_wait)
		;
#endif
	if (!device_is_ready(uart_dev)) {
		char err[] = "!dev\r\n";
		send_error(err);
		return -1;
	}

	configure_dma_event_router(UART_DMA_RX_GROUP, UART_DMA_RX_REQ);
	configure_dma_event_router(UART_DMA_TX_GROUP, UART_DMA_TX_REQ);

	send_error("------\r\n");

	if (uart_config->UART_FCR || dma_config->DMA_DSR) {
		send_error("\r\n");
	}

	if (uart_callback_set(uart_dev, uart_callback, NULL) != 0) {
		send_error("!cb\r\n");
	}

	if (uart_rx_disable(uart_dev) != 0) {
		send_error("!dis\r\n");
	}

	//async_send("fuuuuu");
	//k_sem_take(&wait_sem, K_FOREVER);

	rx_buffer[0][0] = 'F';
	rx_buffer[0][1] = 'U';
	rx_buffer[0][2] = 'U';
	rx_buffer[0][3] = 'U';
	rx_buffer[0][4] = 'U';

	rx_buffer[1][0] = 'B';
	rx_buffer[1][1] = 'A';
	rx_buffer[1][2] = 'A';
	rx_buffer[1][3] = 'A';
	rx_buffer[1][4] = 'A';

#define TIMEOUT 500000
//#define TIMEOUT SYS_FOREVER_US

	if (uart_rx_enable(uart_dev, rx_buffer[pingpong], RX_SIZE, TIMEOUT) != 0) {
		send_error("!rx\r\n");
	}

	while(1) {
		k_sem_take(&wait_sem, K_FOREVER /*K_MSEC(1000)*/);
		send_error("RX: ");
		//send_error(evt->data.rx.buf[0]);
		for (size_t jter = 0; jter < 2; jter++) {
			uart_poll_out(uart_dev, '0' + jter);
			uart_poll_out(uart_dev, '=');
			for (size_t iter = 0; iter < 4; iter++) {
				char test = rx_buffer[jter][iter];
				uart_poll_out(uart_dev, test ? test : '_');
			}
			uart_poll_out(uart_dev, ',');
		}
		send_error("\r\n");
	}

	return 0;
}
