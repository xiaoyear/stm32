#ifndef _WIFI_H_
#define _WIFI_H_


#include "stm32f10x.h"

void net_usart2(u32 Boudrate);
void net_val_init(void);






#define	NET_BUFFER_LEN sizeof(NET_BUFFER)
#define	NETBUF_SIZE	256


typedef struct
{

	unsigned char *rxbuffer;
	unsigned char *txbuffer;

	unsigned int rxcounter;
	unsigned int txcounter;

	unsigned int r_pos;
	unsigned int t_pos;

}NET_BUFFER,*P_NET_BUFFER;




typedef enum {

	NET_ERR_MODE=0,
	NET_NOLINK_ROUTE=1,
	NET_NOLINK_SERVER=2,
	NET_ROUTE_LINK_OK=3,
	NET_SERVER_LINK_NOW=4,
	NET_SERVER_LINK_OK=5,
}NET_SERVER_LINK_STATE;


extern unsigned char net_link;

void com_init(void);
void com_send(unsigned char *p,u32 num);
#endif

