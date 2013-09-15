#ifndef _GLCtrl_GlPicking_h_
#define _GLCtrl_GlPicking_h_

NAMESPACE_UPP

class GlPicking
{
private:
	static int const _bufferSize = 512;
	bool _isPicking;
	Point _pickPoint;
	
public:
	GlPicking()
		: _isPicking(false)
	{}
	
	void InitPickMatrix()
	{
		if (_isPicking)
		{
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			gluPickMatrix(_pickPoint.x, viewport[3] - _pickPoint.y, 3, 3, viewport);
		}
	}
	
	Vector<int> Pick(int x, int y, Callback resizeCallback, Callback paintCallback) 
	{
		GLuint buffer[_bufferSize];
		
		_pickPoint = Point(x, y);
		
		glSelectBuffer(_bufferSize, buffer);
		glRenderMode(GL_SELECT);
		
		_isPicking = true;
		resizeCallback();
		
		glInitNames();
		paintCallback();
		
		_isPicking = false;
		resizeCallback();
		
		// returning to normal rendering mode
		int hits = glRenderMode(GL_RENDER);
		
		if (hits == 0)
			return Vector<int>();
		else 
			return ParseHits(buffer, hits);
	}

private:
	Vector<int> ParseHits(GLuint *buffer, int hits)
	{
		GLuint *minPtr = buffer;
		
		for (int i = 0; i < hits; i++)
		{
			if (*(buffer + 1) < *(minPtr + 1))
				minPtr = buffer;
			
			buffer += *buffer + 3;
		}
		
		Vector<int> result;
		for (GLuint i = 0; i < *minPtr; i++)
			result.Add(*(minPtr + 3 + i));
		
		return result;
	}
};

END_UPP_NAMESPACE

#endif
