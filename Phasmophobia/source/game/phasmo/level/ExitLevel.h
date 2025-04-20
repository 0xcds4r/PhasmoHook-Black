#pragma once

class ExitLevel : public II::MonoBehaviourPun
{
public:
	bool isExiting;
	void* trigger;
	void* source;
	void* startExitSound;
	void* stopExitSound;
	void* invisibleWall;
};
VALIDATE_SIZE(ExitLevel, 0x48 + STRUCT_STUCK);