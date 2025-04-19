#pragma once

class __declspec(align(8)) GameSettings
{
public:
	struct __declspec(align(8)) GraphicsSettings
	{
		int32_t GraphicsSet;
		int32_t qualityLevel;
		bool vsyncValue;
		int32_t resolutionValue;
		int32_t fullscreenType;
		int32_t aaType;
		int32_t msaaValue;
		int32_t smaaQuality;
		int32_t msaaAmount;
		int32_t skyLight;
		int32_t fog;
		int32_t shadowType;
		int32_t shadowRes;
		int32_t textureRes;
		int32_t anisotropic;
		int32_t ambientOcclusion;
		bool chromaticValue;
		bool filmGrainValue;
		int32_t bloom;
		int32_t eyeAdaption;
		int32_t radiantGIValue;
		float brightnessValue;
		bool reflectionValue;
		int32_t fsr;
		int32_t fsrSharpness;
		float fsrSharpnessAmount;
	};
	VALIDATE_SIZE(GraphicsSettings, 0x68);

	void* gameplay;
	void* pc;
	void* vr;
	void* network;
	GraphicsSettings* graphics;
	void* audio;
	void* other;
};
VALIDATE_SIZE(GameSettings, 0x38);