#pragma once

#include <Spore\BasicIncludes.h>

#define SkinPaintRandomizerPtr intrusive_ptr<SkinPaintRandomizer>

using namespace UTFWin;

class SkinPaintRandomizer 
	: public IWinProc
	, public DefaultRefCounted
{
public:
	struct SkinPaintTheme
	{
		eastl::vector<uint32_t> SkinPaintBase;
		eastl::vector<uint32_t> SkinPaintCoat;
		eastl::vector<uint32_t> SkinPaintDetail;
	};

	enum eSkinPaintType
	{
		kBase = 0,
		kCoat = 1,
		kDetail = 2,
		kAll = 3,
	};

	static const uint32_t TYPE = id("SkinPaintRandomizer");
	
	SkinPaintRandomizer();
	~SkinPaintRandomizer();

	virtual void InitSkinPaints();

	virtual void SetRandomSkinEffect(int skinLayout, uint32_t modelType);
	virtual void SetRandomSkinEffectSeed(int skinLayout);
	virtual void SetRandomColor(int skinLayout);

	virtual void SetRandomColorAll();
	virtual void SetRandomSkinEffectAll();
	virtual void SetRandomSkinEffectSeedAll();

	virtual void RandomizeByType(int skinLayout);
	virtual void RandomizeAll();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;
};
