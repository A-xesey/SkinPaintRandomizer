#pragma once
#include <Spore\BasicIncludes.h>
#include "SkinPaintRandomizer.h"

using namespace Editors;

UILayoutPtr SPaintUI;
SkinPaintRandomizerPtr SPRandom;

class SPRandomEditor
{
public:

	static void Initialize() {
	}

	static void Dispose() {
		SPaintUI = nullptr;
		SPRandom = nullptr;
	}

	static void AttachDetours();
};

member_detour(EditorUI_Load, EditorUI, bool(cEditor*, uint32_t, uint32_t, bool))
{
	bool detoured(cEditor * pEditor, uint32_t instanceID, uint32_t groupID, bool editorModelForceSaveover)
	{
		bool res = original_function(this, pEditor, instanceID, groupID, editorModelForceSaveover);
		if (res && (Editor.mSaveExtension == TypeIDs::flr || Editor.mSaveExtension == TypeIDs::crt || Editor.mSaveExtension == TypeIDs::cll))
		{
			SPaintUI = new UTFWin::UILayout();
			SPaintUI->LoadByID(id("SkinPaintRandomizer"));
			SPaintUI->SetParentWindow(Editor.mpEditorUI->field_6C);//mWinPartPaintPaletteRoot

			//SPaintUI->FindWindowByID(id("SPRandomWindow"))->SetArea({ 400.0f, 10.0f, 740.0f, 240.0f });
			//SPaintUI->FindWindowByID(id("SPRandomWindow"))->SetArea({ 525.0f, 70.0f, 865.0f, 300.0f });
			//SPaintUI->FindWindowByID(id("SPRandomWindow"))->SetArea({ 460.0f, 70.0f, 800.0f, 300.0f });

			//SPaintUI->FindWindowByID(id("SPRandomWindow"))->SetArea({ 550.0f, 75.0f, 890.0f, 305.0f });

			SPRandom = new SkinPaintRandomizer();
			SPRandom->InitSkinPaints();
			SPaintUI->FindWindowByID(id("SPRandomShowPanel"))->AddWinProc(SPRandom.get());
			SPaintUI->FindWindowByID(id("SkinPaintRandom"))->AddWinProc(SPRandom.get());
			SPaintUI->FindWindowByID(id("SkinPaintType"))->AddWinProc(SPRandom.get());
		}
		return res;
	}
};

//virtual_detour(Editor_OnExit, cEditor, App::IGameMode, bool())
//{
//	bool detoured()
//	{
//		Editor.mpEditorUI->mWinPartPaintPaletteRoot->RemoveWindow(SPaintUI->FindWindowByID(id("SPRandomWindow")));
//		SPaintUI = nullptr;
//		SPRandom = nullptr;
//		return original_function(this);
//	}
//};

void SPRandomEditor::AttachDetours()
{
	EditorUI_Load::attach(GetAddress(EditorUI, Load));
	//Editor_OnExit::attach(GetAddress(cEditor, OnExit));
}