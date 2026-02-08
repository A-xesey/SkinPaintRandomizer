#include "stdafx.h"
#include "SkinPaintRandomizer.h"

using namespace Editors;
using namespace App;

SkinPaintRandomizer::SkinPaintTheme skinpaintCell;
SkinPaintRandomizer::SkinPaintTheme skinpaintC2C;
SkinPaintRandomizer::SkinPaintTheme skinpaintCreature;
SkinPaintRandomizer::SkinPaintTheme skinpaintPlant;

bool isInit = false;

void SkinPaintRandomizer::InitSkinPaints()
{
	if (!isInit)
	{
		/*Resource::StandardFileFilter filter;
		filter.typeID = TypeIDs::prop;*/
		//eastl::vector<ResourceKey> paintPaletteItems;
		uint32_t groupID;
		eastl::vector<uint32_t> paintPaletteItems;

		for (int type = 0; type < 3; type++)
		{
			//cell_paint_palette_items~
			if (type == 0)
				groupID = 0x406B6D06;
			//filter.groupID = 0x406B6D06;

		//creature_paint_palette_items~
			else if (type == 1)
				groupID = 0x406B6D0D;
			//filter.groupID = 0x406B6D0D;

		//flora_paint_palette_items~
			else if (type == 2)
				groupID = 0x406B6D15;
			//filter.groupID = 0x406B6D15;

			PropManager.GetPropertyListIDs(groupID, paintPaletteItems);
			//ResourceManager.GetResourceKeyList(paintPaletteItems, &filter);
			if (paintPaletteItems.size() != 0)
			{
				PropertyListPtr propertyList;
				eastl::vector<ResourceKey> skinPaintList;

				//get skinpaint from all pages
				for (const uint32_t& propID : paintPaletteItems)
				{
					PropManager.GetPropertyList(propID, groupID, propertyList);
					if (propertyList != nullptr)
					{
						//palettePagePartItems
						eastl::vector<ResourceKey> palettePagePartItems;
						Property::GetArrayKey(propertyList.get(), 0xEEEAD734, palettePagePartItems);
						if (palettePagePartItems.size() != 0)
							skinPaintList.insert(skinPaintList.mpBegin, palettePagePartItems.mpBegin, palettePagePartItems.mpEnd);
					}
				}
				if (skinPaintList.size() != 0)
				{
					SkinPaintTheme paintList;

					//then we categorize it by each type (base, coat or detail)
					for (const ResourceKey& skinpaint : skinPaintList)
					{
						PropManager.GetPropertyList(skinpaint.instanceID, skinpaint.groupID, propertyList);
						if (propertyList != nullptr)
						{
							//paintThemePaints
							eastl::vector<ResourceKey> paintThemePaints;
							Property::GetArrayKey(propertyList.get(), 0xB0E066A7, paintThemePaints);

							for (int skinLayout = 0; skinLayout < paintThemePaints.size(); skinLayout++)
							{
								//pass paint themes (dont' ask me why it's inside of this loop)
								if (paintThemePaints[kBase].instanceID != 0
									&& paintThemePaints[kCoat].instanceID != 0
									&& paintThemePaints[kDetail].instanceID != 0) break;
								if (paintThemePaints[skinLayout].instanceID != 0)
								{
									if (skinLayout == kBase)
										paintList.SkinPaintBase.push_back(paintThemePaints[skinLayout].instanceID);
									else if (skinLayout == kCoat)
										paintList.SkinPaintCoat.push_back(paintThemePaints[skinLayout].instanceID);
									else if (skinLayout == kDetail)
										paintList.SkinPaintDetail.push_back(paintThemePaints[skinLayout].instanceID);
								}
							}
						}
					}
					if (type == 0)
						skinpaintCell = paintList;
					else if (type == 1)
						skinpaintCreature = paintList;
					else if (type == 2)
						skinpaintPlant = paintList;
				}
			}
		}
		skinpaintC2C.SkinPaintBase.insert(skinpaintC2C.SkinPaintBase.mpBegin, skinpaintCell.SkinPaintBase.mpBegin, skinpaintCell.SkinPaintBase.mpEnd);
		skinpaintC2C.SkinPaintBase.insert(skinpaintC2C.SkinPaintBase.mpBegin, skinpaintCreature.SkinPaintBase.mpBegin, skinpaintCreature.SkinPaintBase.mpEnd);
		skinpaintC2C.SkinPaintCoat.insert(skinpaintC2C.SkinPaintCoat.mpBegin, skinpaintCreature.SkinPaintCoat.mpBegin, skinpaintCreature.SkinPaintCoat.mpEnd);
		skinpaintC2C.SkinPaintDetail.insert(skinpaintC2C.SkinPaintDetail.mpBegin, skinpaintCreature.SkinPaintDetail.mpBegin, skinpaintCreature.SkinPaintDetail.mpEnd);
		isInit = true;
	}
}

void SkinPaintRandomizer::SetRandomSkinEffect(int skinLayout, uint32_t modelType)
{
	SkinPaintTheme skinPaintList;
	if (Editor.mEditorName == id("CellToCreatureEditor") || Editor.mEditorName == id("CellToCreatureEditorInGame"))
	{
		skinPaintList = skinpaintC2C;
	}
	else
	{
		if (modelType == kCell)
			skinPaintList = skinpaintCell;

		else if (modelType == kCreature || modelType == kTribeCreature || modelType == kCivCreature
			|| modelType == kSpaceCreature || modelType == kAdventurerCreature)
			skinPaintList = skinpaintCreature;

		else if (modelType == kPlantLarge || modelType == kPlantMedium || modelType == kPlantSmall)
			skinPaintList = skinpaintPlant;
	}
	uint32_t rando;
	uint32_t effectID = 0;
	if (skinLayout == kBase)
	{
		rando = rand(skinPaintList.SkinPaintBase.size() - 1);
		effectID = skinPaintList.SkinPaintBase[rando];
	}
	if (modelType != kCell)
	{
		if (skinLayout == kCoat)
		{
			rando = rand(skinPaintList.SkinPaintCoat.size() - 1);
			effectID = skinPaintList.SkinPaintCoat[rando];
		}
		else if (skinLayout == kDetail)
		{
			rando = rand(skinPaintList.SkinPaintDetail.size() - 1);
			effectID = skinPaintList.SkinPaintDetail[rando];
		}
	}
	if (effectID != 0)
		Editor.mpEditorModel->mSkinEffectIDs[skinLayout] = effectID;
		/*Editor.mpEditorModel->SetSkinEffect(skinLayout, effectID);*/
}

void SkinPaintRandomizer::SetRandomSkinEffectSeed(int skinLayout)
{
	uint32_t seed = rand(0xFFFFFFFF);
	Editor.mpEditorModel->mSkinEffectSeeds[skinLayout] = seed;
	//Editor.mpEditorModel->SetSkinEffectSeed(skinLayout, seed);
}

void SkinPaintRandomizer::SetRandomColor(int skinLayout)
{
	Editor.mpEditorModel->SetColor(skinLayout, Math::ColorRGB(randf(), randf(), randf()));
}

void SkinPaintRandomizer::SetRandomColorAll()
{
	for (int skinLayout = 0; skinLayout < 3; skinLayout++)
		SetRandomColor(skinLayout);
}

void SkinPaintRandomizer::SetRandomSkinEffectAll()
{
	for (int skinLayout = 0; skinLayout < 3; skinLayout++)
		SetRandomSkinEffect(skinLayout, Editor.mpEditorModel->mModelType);
}

void SkinPaintRandomizer::SetRandomSkinEffectSeedAll()
{
	for (int skinLayout = 0; skinLayout < 3; skinLayout++)
		SetRandomSkinEffectSeed(skinLayout);
}

void SkinPaintRandomizer::RandomizeByType(int skinLayout)
{
	SetRandomColor(skinLayout);
	SetRandomSkinEffectSeed(skinLayout);
	SetRandomSkinEffect(skinLayout, Editor.mpEditorModel->mModelType);
}

void SkinPaintRandomizer::RandomizeAll()
{
	SetRandomColorAll();
	SetRandomSkinEffectAll();
	SetRandomSkinEffectSeedAll();
}

//void FETest::ParseLine(const ArgScript::Line& line)
//{
//	if (!isInit)
//		InitSkinPaints();
//	if (Editor.IsMode(Mode::PaintMode))
//		Editor.mpEditorSkin->PaintSkin(Editor.mpEditorModel);
//
//	//if (Editor.mIsActive && Editor.mpEditorModel != nullptr)
//	//{
//	//	Editors::EditorModel* model = Editor.mpEditorModel;
//	//	uint32_t type = model->mModelType;
//	//	//get props from _paint_palette_items~ folder for each creation type
//	//	Resource::StandardFileFilter filter;
//	//	filter.typeID = TypeIDs::prop;
//	//	eastl::vector<ResourceKey> paintPaletteItems;
//	//	//completecell_paint_palette_items~
//	//	if (Editor.mEditorName == id("CellToCreatureEditor") || Editor.mEditorName == id("CellToCreatureEditorInGame"))
//	//	{
//	//		filter.groupID = 0x406B6D07;
//	//	}
//	//	else
//	//	{
//	//		//cell_paint_palette_items~
//	//		if (type == kCell)
//	//			filter.groupID = 0x406B6D06;
//	//		//creature_paint_palette_items~
//	//		else if (type == kCreature || type == kTribeCreature || type == kCivCreature
//	//			|| type == kSpaceCreature || type == kAdventurerCreature)
//	//			filter.groupID = 0x406B6D0D;
//	//		//flora_paint_palette_items~
//	//		else if (type == kPlantLarge || type == kPlantMedium || type == kPlantSmall)
//	//			filter.groupID = 0x406B6D15;
//	//	}
//	//	ResourceManager.GetResourceKeyList(paintPaletteItems, &filter);
//	//	if (paintPaletteItems.size() != 0)
//	//	{
//	//		PropertyListPtr propertyList;
//	//		eastl::vector<ResourceKey> skinPaintList;
//	//		//get skinpaint from all pages
//	//		for (const ResourceKey& prop : paintPaletteItems)
//	//		{
//	//			PropManager.GetPropertyList(prop.instanceID, prop.groupID, propertyList);
//	//			if (propertyList != nullptr)
//	//			{
//	//				//palettePagePartItems
//	//				eastl::vector<ResourceKey> palettePagePartItems;
//	//				Property::GetArrayKey(propertyList.get(), 0xEEEAD734, palettePagePartItems);
//	//				if (palettePagePartItems.size() != 0)
//	//					skinPaintList.insert(skinPaintList.mpBegin, palettePagePartItems.mpBegin, palettePagePartItems.mpEnd);
//	//			}
//	//		}
//	//		if (skinPaintList.size() != 0)
//	//		{
//	//			eastl::vector<ResourceKey> PaintBaseList;
//	//			eastl::vector<ResourceKey> PaintCoatList;
//	//			eastl::vector<ResourceKey> PaintDetailList;
//	//			//then we categorize it by each type (base, coat or detail)
//	//			for (const ResourceKey& skinpaint : skinPaintList)
//	//			{
//	//				PropManager.GetPropertyList(skinpaint.instanceID, skinpaint.groupID, propertyList);
//	//				if (propertyList != nullptr)
//	//				{
//	//					//paintThemePaints
//	//					eastl::vector<ResourceKey> paintThemePaints;
//	//					Property::GetArrayKey(propertyList.get(), 0xB0E066A7, paintThemePaints);
//	//					for (int skinLayout = 0; skinLayout < paintThemePaints.size(); skinLayout++)
//	//					{
//	//						//pass paint themes (dont' ask me why it's inside of this loop)
//	//						if (paintThemePaints[kBase].instanceID != 0
//	//							&& paintThemePaints[kCoat].instanceID != 0
//	//							&& paintThemePaints[kDetail].instanceID != 0) break;
//	//						if (paintThemePaints[skinLayout].instanceID != 0)
//	//						{
//	//							if (skinLayout == kBase)
//	//								PaintBaseList.push_back(paintThemePaints[skinLayout]);
//	//							else if (skinLayout == kCoat)
//	//								PaintCoatList.push_back(paintThemePaints[skinLayout]);
//	//							else if (skinLayout == kDetail)
//	//								PaintDetailList.push_back(paintThemePaints[skinLayout]);
//	//						}
//	//					}
//	//				}
//	//			}
//	//			//randomize skinpaint theme!
//	//			if (PaintBaseList.size() != 0 || PaintCoatList.size() != 0 || PaintDetailList.size() != 0)
//	//			{
//	//				uint32_t rando;
//	//				for (int skinLayout = 0; skinLayout <= 2; skinLayout++)
//	//				{
//	//					if (type == kCell && skinLayout > 0) break;
//	//					if (skinLayout == kBase && PaintBaseList.size() != 0)
//	//					{
//	//						rando = rand(PaintBaseList.size() - 1);
//	//						model->SetSkinEffect(skinLayout, PaintBaseList[rando].instanceID);
//	//					}
//	//					else if (skinLayout == kCoat && PaintCoatList.size() != 0)
//	//					{
//	//						rando = rand(PaintCoatList.size() - 1);
//	//						model->SetSkinEffect(skinLayout, PaintCoatList[rando].instanceID);
//	//					}
//	//					else if (skinLayout == kDetail && PaintDetailList.size() != 0)
//	//					{
//	//						rando = rand(PaintDetailList.size() - 1);
//	//						model->SetSkinEffect(skinLayout, PaintDetailList[rando].instanceID);
//	//					}
//	//					model->SetColor(skinLayout, Math::ColorRGB(randf(), randf(), randf()));
//	//					rando = rand(0xFFFFFFFF);
//	//					model->SetSkinEffectSeed(skinLayout, rando);
//	//				}
//	//				if (Editor.IsMode(Mode::PaintMode))
//	//					Editor.mpEditorSkin->PaintSkin(model);
//	//			}
//	//		}
//	//	}
//	//	model = nullptr;
//	//}
//
////	if (Editor.mIsActive && Editor.mpEditorModel != nullptr)
////	{
////		EditorModel* model = Editor.mpEditorModel;
////
////		uint32_t type = model->mModelType;
////
////		//get props from _paints_EditorKeys~ folder for each creation type
////		Resource::StandardFileFilter filter;
////		filter.typeID = TypeIDs::prop;
////		eastl::vector<ResourceKey> validEditorProps;
////
////		//c2c_paints_EditorKeys~
////		if (Editor.mEditorName == id("CellToCreatureEditor") || Editor.mEditorName == id("CellToCreatureEditorInGame"))
////		{
////			filter.groupID = 0x406B7B07;
////		}
////		else 
////		{
////			//cl_paints_EditorKeys~
////			if (type == kCell)
////				filter.groupID = 0x406B7B06;
////
////			//skinpaints_EditorKeys~
////			else if (type == kCreature || type == kTribeCreature || type == kCivCreature
////				|| type == kSpaceCreature || type == kAdventurerCreature)
////				filter.groupID = 0x406B7B0D;
////
////			//fe_paints_EditorKeys~
////			else if (type == kPlantLarge || type == kPlantMedium || type == kPlantSmall)
////				filter.groupID = 0x406B7B15;
////		}
////
////		ResourceManager.GetResourceKeyList(validEditorProps, &filter);
////
////		if (validEditorProps.size() != 0)
////		{
////			PropertyListPtr propertyList;
////			eastl::vector<ResourceKey> skinPaintList;
////			//eastl::vector<ResourceKey> validAdditionalManifestGroupIDs;
////			//eastl::vector<ResourceKey> AdditionalGroups;
////
////			//get skinpaints from every props
//////skinPaintListAdding:
////			for (const ResourceKey& prop : validEditorProps)
////			{
////				PropManager.GetPropertyList(prop.instanceID, prop.groupID, propertyList);
////				if (propertyList != nullptr)
////				{
////					//validEditorKeys
////					eastl::vector<ResourceKey> validEditorKeys;
////					Property::GetArrayKey(propertyList.get(), 0x053878D0, validEditorKeys);
////					if (validEditorKeys.size() != 0)
////						skinPaintList.insert(skinPaintList.mpBegin,validEditorKeys.mpBegin, validEditorKeys.mpEnd);
////
////
////					//validAdditionalManifestGroupIDs
////					//Property::GetArrayKey(propertyList.get(), 0x0769EEEE, validAdditionalManifestGroupIDs);
////					/*if (validAdditionalManifestGroupIDs.size() != 0)
////						AdditionalGroups.insert(AdditionalGroups.mpBegin, validAdditionalManifestGroupIDs.mpBegin, validAdditionalManifestGroupIDs.mpEnd);*/
////				}
////			}
////			//if (validAdditionalManifestGroupIDs.size() != 0)
////			//{
////			//	validEditorProps.clear();
////			//	for (const ResourceKey& prop : validAdditionalManifestGroupIDs)
////			//	{
////			//		App::ConsolePrintF("prop.groupID is 0x%x (validAdditionalManifestGroupIDs)", prop.groupID);
////			//		filter.groupID = prop.groupID;
////			//		ResourceManager.GetResourceKeyList(validEditorProps, &filter);
////			//		ConsolePrintF("validEditorProps size is %i", validEditorProps.size());
////			//	}
////			//	if (validEditorProps.size() != 0)
////			//		ConsolePrintF("validEditorProps size is %i (outsiode of loop)", validEditorProps.size());
////			//		//goto skinPaintListAdding;
////			//}
////
////			eastl::vector<ResourceKey> PaintBaseList;
////			eastl::vector<ResourceKey> PaintCoatList;
////			eastl::vector<ResourceKey> PaintDetailList;
////
////			//then we categorize it by each type (base, coat or detail)
////			if (skinPaintList.size() != 0)
////			{
////				for (const ResourceKey& skinpaint : skinPaintList)
////				{
////					if (skinpaint.groupID != 0)
////					{
////						PropManager.GetPropertyList(skinpaint.instanceID, skinpaint.groupID, propertyList);
////						if (propertyList != nullptr)
////						{
////							//paintThemePaints
////							eastl::vector<ResourceKey> paintThemePaints;
////							Property::GetArrayKey(propertyList.get(), 0xB0E066A7, paintThemePaints);
////
////							for (int skinLayout = 0; skinLayout < paintThemePaints.size(); skinLayout++)
////							{
////								if (paintThemePaints[skinLayout].instanceID != 0)
////								{
////									if (skinLayout == 0)
////										PaintBaseList.push_back(paintThemePaints[skinLayout]);
////									else if (skinLayout == 1)
////										PaintCoatList.push_back(paintThemePaints[skinLayout]);
////									else if (skinLayout == 2)
////										PaintDetailList.push_back(paintThemePaints[skinLayout]);
////								}
////							}
////						}
////					}
////				}
////			}
////			//randomize skinpaint theme!
////			if (PaintBaseList.size() != 0 && PaintCoatList.size() != 0 && PaintDetailList.size() != 0)
////			{
////				uint32_t rando;
////				for (int skinLayout = 0; skinLayout <= 2; skinLayout++)
////				{
////					if (skinLayout == 0)
////					{
////						rando = rand(PaintBaseList.size() - 1);
////						model->SetSkinEffect(skinLayout, PaintBaseList[rando].instanceID);
////					}
////					else if (skinLayout == 1)
////					{
////						rando = rand(PaintCoatList.size() - 1);
////						model->SetSkinEffect(skinLayout, PaintCoatList[rando].instanceID);
////					}
////					else if (skinLayout == 2)
////					{
////						rando = rand(PaintDetailList.size() - 1);
////						model->SetSkinEffect(skinLayout, PaintDetailList[rando].instanceID);
////					}
////
////					rando = rand(0xFFFFFFFF);
////					model->SetSkinEffectSeed(skinLayout, rando);
////				}
////				if (Editor.IsMode(Mode::PaintMode))
////					Editor.mpEditorSkin->PaintSkin(model);
////			}
////		}
////		model = nullptr;
////	}
//}

SkinPaintRandomizer::SkinPaintRandomizer()
{
}


SkinPaintRandomizer::~SkinPaintRandomizer()
{
}

// For internal use, do not modify.
int SkinPaintRandomizer::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int SkinPaintRandomizer::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* SkinPaintRandomizer::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(SkinPaintRandomizer);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int SkinPaintRandomizer::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
int skintype = -1;
bool SkinPaintRandomizer::HandleUIMessage(UTFWin::IWindow* pWindow, const UTFWin::Message& message)
{
	if (message.IsType(UTFWin::kMsgButtonSelect)) {
		if (message.source->GetControlID() == id("SkinTypeAll")) skintype = kAll;
		if (message.source->GetControlID() == id("SkinTypeBase")) skintype = kBase;
		if (message.source->GetControlID() == id("SkinTypeCoat")) skintype = kCoat;
		if (message.source->GetControlID() == id("SkinTypeDetail")) skintype = kDetail;

		IWindowPtr parent = message.source->GetParent()->GetParent();
		if (Editor.mpEditorModel->mModelType == kCell && skintype != kBase)
			parent->FindWindowByID(id("RandomSkin"))->SetEnabled(false);
		else
			parent->FindWindowByID(id("RandomSkin"))->SetEnabled(true);
		parent->FindWindowByID(id("RandomAll"))->SetEnabled(true);
		parent->FindWindowByID(id("RandomColor"))->SetEnabled(true);
		parent->FindWindowByID(id("RandomSeed"))->SetEnabled(true);
		Audio::PlayAudio(id("editor_handle_select"));
	}
	if (message.IsType(UTFWin::kMsgButtonClick))
	{
		if (message.source->GetControlID() == id("SPRandomShowPanel"))
		{
			IWindowPtr panel = message.source->GetParent()->FindWindowByID(id("SPRandomPanel"));
			if (panel->IsVisible())
				panel->SetVisible(false);
			else
				panel->SetVisible(true);
			panel->FindWindowByID(id("SkinTypeAll"))->SetState(WindowStates::kStateEnabled);
			panel->FindWindowByID(id("SkinTypeBase"))->SetState(WindowStates::kStateEnabled);
			panel->FindWindowByID(id("SkinTypeCoat"))->SetState(WindowStates::kStateEnabled);
			panel->FindWindowByID(id("SkinTypeDetail"))->SetState(WindowStates::kStateEnabled);
			panel->FindWindowByID(id("RandomAll"))->SetEnabled(false);
			panel->FindWindowByID(id("RandomSkin"))->SetEnabled(false);
			panel->FindWindowByID(id("RandomColor"))->SetEnabled(false);
			panel->FindWindowByID(id("RandomSeed"))->SetEnabled(false);
			Audio::PlayAudio(id("editor_general_click"));
		}
		else
		{
			if (message.source->GetControlID() == id("RandomAll"))
			{
				if (skintype == kAll)
					RandomizeAll();
				else
					RandomizeByType(skintype);
			}
			if (message.source->GetControlID() == id("RandomSkin"))
			{
				if (skintype == kAll)
					SetRandomSkinEffectAll();
				else
					SetRandomSkinEffect(skintype, Editor.mpEditorModel->mModelType);
			}
			if (message.source->GetControlID() == id("RandomColor"))
			{
				if (skintype == kAll)
					SetRandomColorAll();
				else
					SetRandomColor(skintype);
			}
			if (message.source->GetControlID() == id("RandomSeed"))
			{
				if (skintype == kAll)
					SetRandomSkinEffectSeedAll();
				else
					SetRandomSkinEffectSeed(skintype);
			}
			if (Editor.IsMode(Mode::PaintMode))
				Editor.mpEditorSkin->PaintSkin(Editor.mpEditorModel);
			Editor.CommitEditHistory(true);
			Audio::PlayAudio(id("editor_click"));
		}
	}
	// By default, just return false since we didn't handle the message
	return false;
}
