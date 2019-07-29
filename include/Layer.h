/*
	2D paint one layer information
*/


#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "LoadShaders.h"
#include"../include/opencv2/opencv.hpp"
class Layer
{
public:
	Layer();
	void Init(int w, int h);
	void InitLayer(int w, int h);
	void InitCanvas(int w, int h);

	void Render();
	void SetActive(bool a);
	bool GetActive();
	IplImage* GetCanvasImage();
	int GetStrokeID();
	void ResetGLTexture();
	void ChangeStrokeID(int newStrokeID);
	
private:
	GLuint m_Program;
	GLuint m_vVBO;
	GLuint m_uVBO;
	GLuint m_VAO;
	
	GLuint m_TextureID;
	IplImage* m_Canvas;

	GLfloat m_Vertices[8];
	GLfloat m_TextureCoordinate[8];

	bool m_isActive;
	int m_StrokeID;

};



