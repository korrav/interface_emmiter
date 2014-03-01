#include "main.h"

static void transPackage(struct b_pool* pbuf, void (*pfunc)(uint8_t* pBuffer, unsigned int size));

int main(void) {
	struct b_pool* pbuf;
	struct head* h;
	init_pools_buffers();
	//configures the priority grouping
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	//start time
	start_timer();
	//configure spi
	SPI_Config();
	//configure i2c
	I2C_Config();
	//ETHERNET INITIALIZATION
	ETH_BSP_Config();
	/* Initilaize the LwIP stack */
	LwIP_Init();
	//udp initialization
	if (!udp_echoserver_init())
		return 1;
  while (1)
  {  
		//PROCESSING OF PEREODIC TIMERS FOR LWIP
    LwIP_Periodic_Handle(gettime());
		//PROCESSING OF INCOMING PACKET
    if (ETH_CheckFrameReceived())
    { 
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
		//PROCESSING OF QUEUING BUFFERS
		pbuf = pull_out_queue();
		if(pbuf != NULL) {
			h = (struct head*)pbuf->pbuf;
			if(h->dst & INTERFACE)
				h->dst &= ~INTERFACE;
			if(h->dst & BAG)
				transPackage(pbuf, eth_write);
			if((h->dst & MASTER) || (h->dst & SLAVE))
				transPackage(pbuf, i2c_write);
			free_buf(pbuf);
		}
  }   
}

void transPackage(struct b_pool* pbuf, void (*pfunc)(uint8_t* pBuffer, unsigned int size)) {
	struct head *htr, *h = (struct head*)pbuf->pbuf;
	struct b_pool* ptr = alloc_buf(SIZE_LITTLE_BUFFER);
	unsigned int fullSize = h->size;
	unsigned int trans =0, fillSize = 0, count = 0;
	htr = (struct head*)ptr->pbuf;
	while(fullSize > 0) {
		memcpy(htr, h, sizeof(struct head));
		if(fullSize + sizeof(struct head) <= SIZE_LITTLE_BUFFER) {
			trans = fullSize;
			htr->count = LAST;
		}
		else {
			trans = SIZE_LITTLE_BUFFER - sizeof(struct head);
			htr->count = count++;
		}
		htr->size = trans;
		memcpy(ptr->pbuf + sizeof(struct head), pbuf->pbuf + sizeof(struct head) + fillSize, trans);
		pfunc((uint8_t*)htr, trans + sizeof(struct head));
		fillSize += trans;
		fullSize -= trans;
	}
	free_buf(ptr);
	return;
}
