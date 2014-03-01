/**
  ******************************************************************************
  * @file    udp_echoserver.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   UDP echo server
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

static struct udp_pcb *upcb  = NULL;

static struct { 
	struct ip_addr BagIPaddr;
	u16_t port;
} BagAddr;

static struct b_pool *pb_eth_B, *pb_eth_L;
static struct receiver recE;

/* Private function prototypes -----------------------------------------------*/
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
bool udp_echoserver_init(void)
{
	//create data socket
   upcb = udp_new();   
   if (upcb)
   {
		 udp_bind(upcb, IP_ADDR_ANY, EM_PORT);
		 udp_recv(upcb, udp_echoserver_receive_callback, NULL);
		 IP4_ADDR(&BagAddr.BagIPaddr, DEST_IP_BADDR0, DEST_IP_BADDR1, DEST_IP_BADDR2, DEST_IP_BADDR3);
		 BagAddr.port = BAG_PORT;
   }
   else
		 return false;
	 pb_eth_B = alloc_buf(SIZE_BIG_BUFFER);
	 init_receiver(&recE,  pb_eth_B->pbuf, pb_eth_B->size);
	 pb_eth_L = alloc_buf(SIZE_LITTLE_BUFFER);
	 return true;
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	if(pb_eth_L->size < p->tot_len || port!= BagAddr.port)
		return;
	pbuf_copy_partial(p, pb_eth_L->pbuf, p->tot_len, 0);
	if(generate_message(pb_eth_L->pbuf, &recE) != NOT_FULL) {
		enqueue_buf(pb_eth_B);
		pb_eth_B = alloc_buf(SIZE_BIG_BUFFER);
		init_receiver(&recE,  pb_eth_B->pbuf, pb_eth_B->size);
	}
	pbuf_free(p);
	return;
}

void eth_write(uint8_t* pBuffer, unsigned int size) {
	struct pbuf *p;
	p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_POOL);
	if (p != NULL) {
		pbuf_take(p, pBuffer, size);
    /* send udp data */
		udp_connect(upcb, &BagAddr.BagIPaddr, BagAddr.port);
    udp_send(upcb, p);  
		udp_disconnect(upcb);
    /* free pbuf */
    pbuf_free(p);
  }
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
