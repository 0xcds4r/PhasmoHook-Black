#pragma once

class NetworkPlayer
{
private:
	Player* player;
	char szUsername[128 + 1];
	bool isLocal;
public:
	NetworkPlayer(Player* player);
	~NetworkPlayer();

	void SetName(const char* name) {
		if (name && strlen(name) > 0) {
			strncpy(szUsername, name, sizeof(szUsername) - 1);
			szUsername[sizeof(szUsername) - 1] = '\0';
		}
	}

	void setLocal(bool bLocal) {
		isLocal = bLocal;
	}

	bool IsLocalPlayer() {
		return isLocal;
	}

	const char* GetName() {
		return static_cast<const char*>(szUsername);
	}

	Player* GetPlayer() {
		return player;
	}
};

