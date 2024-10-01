#include "../main.h"

NetworkPlayer::NetworkPlayer(Player* player)
{
	this->player = player;
	memset(szUsername, 0, sizeof(szUsername));
	isLocal = false;
}

NetworkPlayer::~NetworkPlayer()
{
	this->player = nullptr;
	memset(szUsername, 0, sizeof(szUsername));
	isLocal = false;
}
