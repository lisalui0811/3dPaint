#include "Layer.h"
Layer::Layer()
{
	
}

void Layer::Init(int w, int h)
{
	m_isActive = false;
	m_StrokeID = 0;
	InitLayer(w,h);
	InitCanvas(w,h);
}

void Layer::InitLayer(int w, int h)
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER,		"./Shader/Canvas.vert" },
		{ GL_FRAGMENT_SHADER,	"./Shader/Canvas.frag" },
		{ GL_NONE, "" }
	};
	m_Program = LoadShaders(shaders);
	
    m_Vertices[0] = -1.0;
    m_Vertices[1] = -1.0;
    m_Vertices[2] = 1.0;
    m_Vertices[3] = -1.0;
    m_Vertices[4] = 1.0;
    m_Vertices[5] = 1.0;
    m_Vertices[6] = -1.0;
    m_Vertices[7] = 1.0;
    m_TextureCoordinate[0] = 0.0f;
    m_TextureCoordinate[1] = 0.0f;
    m_TextureCoordinate[2] = 1.0f;
    m_TextureCoordinate[3] = 0.0f;
    m_TextureCoordinate[4] = 1.0f;
    m_TextureCoordinate[5] = 1.0f;
    m_TextureCoordinate[6] = 0.0f;
    m_TextureCoordinate[7] = 1.0f;

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_vVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &m_Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_uVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &m_TextureCoordinate[0], GL_STATIC_DRAW);
}

void Layer::InitCanvas(int w, int h)
{
	IplImage* tex;
	tex = cvLoadImage("./monster123.jpg");
	cvCvtColor(tex, tex, CV_BGR2RGB);
	CvScalar p1, p2;
	int width = tex->width;
	int height = tex->height;
	m_Canvas = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 4);
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			p1 = cvGet2D(tex, j, i);
			p2.val[0] = p1.val[0];
			p2.val[1] = p1.val[1];
			p2.val[2] = p1.val[2];
			p2.val[3] = 255;
			cvSet2D(m_Canvas, j, i, p2);
		}
	}

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Canvas->width, m_Canvas->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)m_Canvas->imageData);

}

void Layer::Render()
{
	
	if (!m_isActive)
		return;
	
	glUseProgram(m_Program);
	glBindVertexArray(m_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glUniform1i(glGetUniformLocation(m_Program, "Texture"), 0);


	glBindBuffer(GL_ARRAY_BUFFER, m_vVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &m_Vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &m_TextureCoordinate[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glUseProgram(0);
}

void Layer::SetActive(bool a)
{
	m_isActive = a;
}

bool Layer::GetActive()
{
	return m_isActive;
}

IplImage * Layer::GetCanvasImage()
{
	return m_Canvas;
}

int Layer::GetStrokeID()
{
	return m_StrokeID;
}

void Layer::ResetGLTexture()
{
	glDeleteTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Canvas->width, m_Canvas->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)m_Canvas->imageData);
}

void Layer::ChangeStrokeID(int newStrokeID)
{
	if (newStrokeID < 18 && newStrokeID >= 0)
		m_StrokeID = newStrokeID;
}
