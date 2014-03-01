#ifndef __EMMITER
#define __EMMITER
#include "stm32f4xx.h"
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "stm32f4x7_eth_conf.h"
#include "udp_echoserver.h"
#include "lwipopts.h"
#include "netconf.h"
#include "time.h"
#include "i2cExchange.h"
#include "spiExchange.h"
#include "generate_message.h"
#include "allocator.h"
#include <stdio.h>

//configure Ethernet
/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   0x06
#define MAC_ADDR1   0x02
#define MAC_ADDR2   0
#define MAC_ADDR3   0xFE
#define MAC_ADDR4   0xFF
#define MAC_ADDR5   0x1
 
/*Static IP ADDRESS MAD: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   100
#define IP_ADDR3   21

/*Static IP ADDRESS BAG: IP_BADDR0.IP_BADDR1.IP_BADDR2.IP_BADDR3 */
#define DEST_IP_BADDR0   192
#define DEST_IP_BADDR1   168
#define DEST_IP_BADDR2   100
#define DEST_IP_BADDR3   25
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   100
#define GW_ADDR3   1  

#define EM_PORT    32000   
#define BAG_PORT   32000    

#define MII_MODE
#define CHECKSUM_BY_HARDWARE  //checksum payoland ip4

//src and dst flags
enum { BAG = 1, INTERFACE = 2, MASTER = 4, SLAVE = 8};

//packet header format
#define LAST -1
struct head{
	int size; //buffer size
	int count; //serial number of buffer
	int dst;	//destination
	int src;	//source
	int type;	//buffer type
};

#endif /* __EMMITER */
