#include "../main.h"

PlayerPool* pPlayerPool = nullptr;

void Network::Init()
{
#ifdef __USE__PLAYER_POOLS
	if (!pPlayerPool) {
		pPlayerPool = new PlayerPool();
	}
#endif
}
