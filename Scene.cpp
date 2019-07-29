#include "Scene.h"

Scene::Scene(int w, int h)
{
	m_StrokeManager = new StrokeManager();
	m_LayerManager = new LayerManager(w,h);
	m_LayerManager->SetStrokeManager(m_StrokeManager);
	m_Width = w;
	m_Height = h;
}

void Scene::Render()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_LayerManager->Render();
	glDisable(GL_BLEND);


	glEnable(GL_POINT_SMOOTH);
	glPointSize(1.0f);
	glBegin(GL_POINTS);

	int LayerStrokeID = m_LayerManager->GetCurrentLayer()->GetStrokeID();
	Stroke* currentLayerStroke = m_StrokeManager->GetStroke(LayerStrokeID);
	IplImage* currentLayerStrokeImage = currentLayerStroke->GetStrokeImage();
	PaintColor strokeColor = m_StrokeManager->GetCurrentStrokeColor();

	for (int y = 0; y < currentLayerStrokeImage->height; y++)
		for (int x = 0; x < currentLayerStrokeImage->width; x++)
		{
			uchar*  strokeImagePtr = &CV_IMAGE_ELEM(currentLayerStrokeImage, uchar, y, x*3);
			double intensity = (1 - ((double)strokeImagePtr[0] / 255));
			if (intensity>0.94)
			{
				glColor4d(strokeColor.r, strokeColor.g, strokeColor.b, intensity);
				int fx = m_MouseX + (x - currentLayerStrokeImage->width / 2);
				int fy = m_Height - m_MouseY + (y - currentLayerStrokeImage->height / 2);
				glVertex2d(fx, fy);
			}
		}
	glEnd();
}

void Scene::OnPaint(int x, int y)
{
	m_LayerManager->PaintOnCurentCanvas(m_StrokeManager->GetCurrentStrokeColor(),Position(x,y));
}

void Scene::AddLayer()
{
	m_LayerManager->AddLayer();
}

void Scene::ChangeCurrentLayer(int offset)
{
	m_LayerManager->ChangeCurrentLayer(offset);
}

void Scene::CurrentLayerActive()
{
	m_LayerManager->SetCurrentLayerActive();
}



void Scene::SetMouse(int x, int y)
{
	m_MouseX = x;
	m_MouseY = y;
}



void Scene::ChangeCurrentLayerStrokeID(int offset)
{
	m_LayerManager->GetCurrentLayer()->ChangeStrokeID(offset);
}

void Scene::RenderInit(int w,int h) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void Scene::ChangeColor(PaintColor color) {
	m_StrokeManager->SetCurrentStrokeColor(color);
}
void Scene::ChangeStrokeID(int newStrokeID) {
	m_LayerManager->GetCurrentLayer()->ChangeStrokeID(newStrokeID);
}

PaintColor  Scene::currentcolor() {
	return m_StrokeManager->GetCurrentStrokeColor();
}

int Scene::currentstroke(){
	return  m_LayerManager->GetCurrentLayer()->GetStrokeID();
}

	
	

