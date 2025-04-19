#pragma once

class LoadingController : public II::MonoBehaviour
{
public:
	II::GameObject* pcLoadingCanvas;
	II::GameObject* pcLoadingFadeToBlack;
	bool canLoadScene;
};
VALIDATE_SIZE(LoadingController, 0x28 + STRUCT_STUCK);