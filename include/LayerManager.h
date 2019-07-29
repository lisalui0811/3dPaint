/*
	2D paint layers control
*/

#pragma once
#include "Layer.h"
#include "StrokeManager.h"

class LayerManager
{
public:
	LayerManager(int w,int h);
	void AddLayer();
	void Render();
	void SetCurrentLayerActive();
	void ChangeCurrentLayer(int offset);
	void PaintOnCurentCanvas(PaintColor color, Position position);
	void SetStrokeManager(StrokeManager* s);
	Layer* GetCurrentLayer();
	int GetCurrrentLayerID();
private:
	int m_CurrrentLayerID;
	vector<Layer*> m_Layers;
	int m_Width;
	int m_Height;
	StrokeManager* m_StrokeManager;
};


