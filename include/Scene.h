/*
	2D paint render
*/


#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "LayerManager.h"

class Scene
{
public:
	Scene(int w,int h);

	void Render();
	void RenderInit(int w, int h);
	void OnPaint(int x,int y);


	void AddLayer();
	void ChangeCurrentLayer(int offset);
	void CurrentLayerActive();

	void SetMouse(int x, int y);
	void ChangeCurrentLayerStrokeID(int offset);
	void ChangeColor(PaintColor color);
	void ChangeStrokeID(int newStrokeID);
	PaintColor  currentcolor();
	int currentstroke();

private:
	LayerManager* m_LayerManager;
	StrokeManager* m_StrokeManager;
	int m_MouseX;
	int m_MouseY;
	int m_Width;
	int m_Height;

};


