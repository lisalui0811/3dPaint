#include "LayerManager.h"

LayerManager::LayerManager(int w, int h)
{
	m_Width = w;
	m_Height = h;
	m_CurrrentLayerID = 0;
	AddLayer();
}

void LayerManager::AddLayer()
{
	Layer* newLayer = new Layer();
	newLayer->Init(m_Width,m_Height);
	newLayer->SetActive(true);
	m_Layers.push_back(newLayer);
}

void LayerManager::Render()
{
	for (int i = 0; i < m_Layers.size(); i++)
		m_Layers[i]->Render();
}

void LayerManager::SetCurrentLayerActive()
{
	GetCurrentLayer()->SetActive(!GetCurrentLayer()->GetActive());
}

void LayerManager::ChangeCurrentLayer(int offset)
{
	int newCurrrentLayerID = m_CurrrentLayerID + offset;
	if (newCurrrentLayerID < m_Layers.size() && newCurrrentLayerID >= 0)
		m_CurrrentLayerID = newCurrrentLayerID;
}
void LayerManager::PaintOnCurentCanvas(PaintColor color, Position position)
{
	IplImage* currentCanvasImage = m_Layers[m_CurrrentLayerID]->GetCanvasImage();

	int LayerStrokeID = m_Layers[m_CurrrentLayerID]->GetStrokeID();
	Stroke* currentLayerStroke = m_StrokeManager->GetStroke(LayerStrokeID);
	IplImage* currentLayerStrokeImage = currentLayerStroke->GetStrokeImage();

	for (int y = 0; y < currentLayerStrokeImage->height; y++)
		for (int x = 0; x < currentLayerStrokeImage->width; x++)
		{
			uchar*  strokeImagePtr = &CV_IMAGE_ELEM(currentLayerStrokeImage, uchar, y, x * 3);

			double intensity = (1 - ((double)strokeImagePtr[0] / 255));
			int fx = position.x + (x - currentLayerStrokeImage->width / 2);
			int fy = m_Height - position.y + (y - currentLayerStrokeImage->height / 2);
			if (fx < 0 || fx >= currentCanvasImage->width || fy < 0 || fy >= currentCanvasImage->height)
				continue;
			uchar*  CanvasImagePtr = &CV_IMAGE_ELEM(currentCanvasImage, uchar, fy, fx * 4);
			if ((int)strokeImagePtr[0] != 255 &&
				    !(CanvasImagePtr[0] == 0 && CanvasImagePtr[1] == 0 && CanvasImagePtr[2] == 0))
			{
				if (intensity > 0.6)
				{
					uchar TempAlpha;
					TempAlpha = CanvasImagePtr[3];
					CanvasImagePtr[0] = (unsigned int)(color.r * 255);
					CanvasImagePtr[1] = (unsigned int)(color.g * 255);
					CanvasImagePtr[2] = (unsigned int)(color.b * 255);

					unsigned int  alpha = intensity * 255;
					if ((unsigned int)TempAlpha + alpha >255)
						TempAlpha = 255;
					else
						TempAlpha += alpha;
					CanvasImagePtr[3] = TempAlpha;
				}
			}
		}
	m_Layers[m_CurrrentLayerID]->ResetGLTexture();

}

void LayerManager::SetStrokeManager(StrokeManager * s)
{
	m_StrokeManager = s;
}

Layer * LayerManager::GetCurrentLayer()
{
	return m_Layers[m_CurrrentLayerID];
}

int LayerManager::GetCurrrentLayerID()
{
	return m_CurrrentLayerID;
}

