#ifndef _TIMER_H_
#define _TIMER_H_


typedef struct 
{
	
	unsigned int timer_count;
	unsigned int timer_value;
	unsigned char timer_timeout;
	unsigned char timer_run;
	
}TIMER,*P_TIMER;

#define	TIMER_LEN sizeof(TIMER)


typedef enum {

	TIMER_CLOSE=0,
	TIMER_START=1

}TIMER_STATE;

typedef enum {

	SERVER_LOG_TIMER=0,
	AT_CMD_TIMER,
	AP_MODE_CHECK_TIMER,
	WIFI_CHECK_TIMER,
	HEARTBEAT_TIMER,
	ALARM_RESEND_TIMER,
  SWITCH_SCAN_TIMER,
	TIMER_MAX_NUM

}TIMER_COUNT;


#define	SET_1S		100
#define	SET_10MS		1

extern TIMER timer[TIMER_MAX_NUM];
void start_smartlink_timer2(void);
void TIM2_Int_Init(u16 arr,u16 psc)	;
void timer_signal_set(unsigned int num,unsigned int value);
void timer_all_init(void);
void timer_count_init(u16 arr,u16 psc);
int timer_get_state(unsigned int num);
int timer_get_run(unsigned int num);
void timer_signal_init(unsigned int num);
void timer1_init(void);
void timer2_irq(void);
void timer_clr_state(unsigned int num);
int timer_get_run(unsigned int num);
#endif
