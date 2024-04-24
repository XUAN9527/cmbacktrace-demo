#ifndef __SOFT_TIMER_H
#define __SOFT_TIMER_H

#include <stdint.h>

#define STIMER_OPT_SINGLE  1
#define STIMER_OPT_REPEAT  2

typedef struct _stimer
{
	const char* name;
	uint32_t timeout_ticks;
	uint32_t current_ticks;
	void (*callback)(void* arg);
	int opt;
	void* arg;
	struct _stimer* next;
}stimer_t;

typedef struct _stimer stimer_t;

int stimer_init(stimer_t* timer, const char* name, void (*timeout_callback)(void* arg), void* arg, uint32_t ticks, int opt);
int stimer_start(stimer_t* timer);
int stimer_stop(stimer_t* timer);
int stimer_restart(stimer_t* timer);
uint32_t stimer_getticks(const stimer_t* timer);
void stimer_set_optmode(stimer_t* timer,int mode);
void stimer_set_timeout(stimer_t* timer,uint32_t ticks);
void stimer_set_indicate(stimer_t* timer,void (*rx_ind)(void* arg));

void stimer_poll(void);

void stimer_ticks(void);

#endif
