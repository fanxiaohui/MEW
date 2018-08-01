#include "stm32f10x.h"

#include "mew_board.h"
#include "mew_stm32.h"
#include "mew_m26.h"

#include "stdio.h"


int main(void)
{
	mew_board_Init();
	
	sprintf(mew_m26.ADDR[0], "www.boryworks.com");
	mew_m26.PORT[0] = 9000;
	
	mew_m26.SocketEnable(0);
	
	mew_board.LED_NS(1);
	mew_board.MC20_PK(0);
	
	while(1)
	{
		mew_m26.Socket_Schedule_NoOS();
	}
}
