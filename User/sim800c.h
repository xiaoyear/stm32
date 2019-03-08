


//simø®≥ı ºªØ
#define INIT_CHECK_SIM				0
#define INIT_CHECK_SIM_DONE			1
#define INIT_CHECK_GPRS				2
#define INIT_CHECK_GPRS_DONE		3
#define INIT_SET_APN				4
#define INIT_SET_APN_DONE			5
#define INIT_CREATE_NET				6
#define INIT_CREATE_NET_DONE		7
#define INIT_GET_IP					8
#define INIT_GET_IP_DONE			9
#define INIT_SET_SOCKET				10
#define INIT_SET_SOCKET_DONE		11

#define INIT_TIMER					20
#define INIT_TIMER_LONG				21


#define SIM800_INIT					0
#define SIM800_GPRS_REC				1
#define SIM800_GPRS_REC_WAIT		2
#define SIM800_TIMER				3
#define SIM800_TIMER_SHORT			4


extern int sim800_state;

void open_sim800_pwr(void);



