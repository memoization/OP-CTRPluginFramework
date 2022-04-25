#include <3ds.h>

void hidExitFake();

void    __appExit(void)
{
	// Exit services
	hidExitFake();
    cfguExit();
    fsExit();
    amExit();
    acExit();
    srvExit();
}
