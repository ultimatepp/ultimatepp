#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
#include <plugin/box2d/Box2D.h>

using namespace Upp;

struct QueryCallback : b2QueryCallback
{
	QueryCallback(const b2Vec2& point)
	{
		this->point = point;
		fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(point);
			if (inside)
			{
				this->fixture = fixture;
				return false;
			}
		}

		return true;
	}

	b2Vec2 point;
	b2Fixture* fixture;
};

struct DebugDraw : b2DebugDraw
{
	Draw* w;
	Sizef sz;
	float cx, cy;
	float aspect;
	float zoom;
	
	DebugDraw()
	{}
	
	void Init(Draw& d, Size s)
	{
		w = &d;
		sz = s;
		zoom = 15.0f;
		cx = float(sz.cx / 2.0f);
		cy = float(sz.cy / 2.0f + 150.0f);
		aspect = float(sz.cx / sz.cy);
		aspect *= zoom;
	}
	
	Point conv(const b2Vec2& v)
	{
		return Point(int(v.x * aspect + cx), int(cy - v.y * aspect));
	}

	b2Vec2 conv(const Point& p)
	{
		b2Vec2 v;

		v.x = (p.x - cx) / aspect;
		v.y = (cy - p.y) / aspect;
		
		return v;
	}

	Color conv(const b2Color& c, double f = 255.0)
	{
		return Color(int(c.r * f), int(c.g * f), int(c.b * f));
	}	
	
	void DrawPolygon(const b2Vec2* v, int vertexCount, const b2Color& color)
	{
		Vector<Point> p;
		p.SetCount(vertexCount + 1);
		for(int i = 0; i < vertexCount; ++i)
			p[i] = conv(v[i]);
		p[vertexCount] = p[0];
		
		w->DrawPolyline(p, 1, conv(color, 150.0));
	}
	
	void DrawSolidPolygon(const b2Vec2* v, int vertexCount, const b2Color& color)
	{
		Vector<Point> p;
		p.SetCount(vertexCount);
		for(int i = 0; i < vertexCount; ++i)
			p[i] = conv(v[i]);
		
		w->DrawPolygon(p, conv(color, 255.0), 1, conv(color, 150));		
	}
	
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		int r = int(aspect * radius * 2.0f);
		Point p = conv(b2Vec2(center.x - radius, center.y + radius));
		w->DrawEllipse(p.x, p.y, r, r, conv(color, 150.0));
	}
	
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		int r = int(aspect * radius * 2.0f);
		Point p = conv(b2Vec2(center.x - radius, center.y + radius));
		w->DrawEllipse(p.x, p.y, r, r, conv(color, 255.0), PEN_SOLID, conv(color, 150.0));
	}
	
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		w->DrawLine(conv(p1), conv(p2), 1, conv(color, 150.0));
	}
	
	void DrawTransform(const b2Transform& xf)
	{
	}
	
	void DrawPoint(const b2Vec2& p0, float32 size, const b2Color& color)
	{
		Point p = conv(p0);
		int s = int(size * aspect);
		w->DrawRect(p.x, p.y, s, s, conv(color, 255.0));
	}
	
	void DrawString(int x, int y, const char* string, ...)
	{
		char buffer[256];
	
		va_list arg;
		va_start(arg, string);
		vsprintf(buffer, string, arg);
		va_end(arg);
		
		w->DrawText(x, y, buffer);
	}
	
	void DrawAABB(b2AABB* aabb, const b2Color& color)
	{
		Point lb = conv(aabb->lowerBound);
		Point ub = conv(aabb->upperBound);
		Color fg = conv(color, 150.0);
				
		w->DrawRect(lb.x, lb.y, ub.x, ub.y, fg);
	}
};

struct App : TopWindow
{
	b2World world;
	b2MouseJoint* mouseJoint;
	b2Vec2 mouseWorld;
	b2Body* groundBody;
	Option showBoxes;
	DropList drawMode;
	DebugDraw debugDraw;
	
	typedef App CLASSNAME;
	
	App() : world(b2Vec2(0.0, -10.0), true)
	{
		Title("Box2D Example");
		SetRectX(0, 640);
		SetRectY(0, 480);
		Sizeable().Zoomable();
		BackPaint();
		SetTimeCallback(-1, THISBACK(Render));
		mouseJoint = NULL;
		b2Vec2 gravity;
		gravity.Set(0.0f, -10.0f);
		world.SetGravity(gravity);
		world.SetDebugDraw(&debugDraw);
		b2BodyDef bodyDef;
		groundBody = world.CreateBody(&bodyDef);
		showBoxes.SetLabel("AABBs");
		
		drawMode
			.Add(0, "Draw")
			.Add(1, "Painter - No aa")
			.Add(2, "Painter - Antialiased")
			.Add(3, "Painter - Subpixel");
			
		drawMode <<= 2;
				
		Add(showBoxes.LeftPosZ(5, 55).TopPosZ(5, 19));
		Add(drawMode.LeftPosZ(5, 130).TopPosZ(28, 19));
		Bridge();
	}
	
	void Bridge()
	{
		const int ecount = 30;
		
		b2BodyDef bd;
		b2Body* ground = world.CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsEdge(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ground->CreateFixture(&shape, 0.0f);

		shape.SetAsBox(0.5f, 0.125f);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 20.0f;
		fd.friction = 0.2f;

		b2RevoluteJointDef jd;

		b2Body* prevBody = ground;
		for(int i = 0; i < ecount; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-14.5f + 1.0f * i, 5.0f);
			b2Body* body = world.CreateBody(&bd);
			body->CreateFixture(&fd);

			b2Vec2 anchor(-15.0f + 1.0f * i, 5.0f);
			jd.Initialize(prevBody, body, anchor);
			world.CreateJoint(&jd);

			prevBody = body;
		}

		b2Vec2 anchor(-15.0f + 1.0f * ecount, 5.0f);
		jd.Initialize(prevBody, ground, anchor);
		world.CreateJoint(&jd);

		for(int i = 0; i < 2; ++i)
		{
			b2Vec2 vertices[3];
			vertices[0].Set(-0.5f, 0.0f);
			vertices[1].Set(0.5f, 0.0f);
			vertices[2].Set(0.0f, 1.5f);

			b2PolygonShape shape;
			shape.Set(vertices, 3);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-8.0f + 8.0f * i, 12.0f);
			b2Body* body = world.CreateBody(&bd);
			body->CreateFixture(&fd);
		}

		for(int i = 0; i < 3; ++i)
		{
			b2CircleShape shape;
			shape.m_radius = 0.5f;

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-6.0f + 6.0f * i, 10.0f);
			b2Body* body = world.CreateBody(&bd);
			body->CreateFixture(&fd);
		}		
	}
	
	void Render() { Refresh(); }
	
	virtual void Paint(Draw& w)
	{
		int m = ~drawMode;

		float hz = 60;
		int velocityIterations = 8;
		int positionIterations = 10;

		float32 timeStep = 1.0f / hz;
		
		int flags = b2DebugDraw::e_shapeBit | b2DebugDraw::e_jointBit;
		if(showBoxes)
			flags |= b2DebugDraw::e_aabbBit;
		
		debugDraw.SetFlags(flags);
	
		world.SetWarmStarting(1);
		world.SetContinuousPhysics(1);	
		world.Step(timeStep, velocityIterations, positionIterations);	

		Point p1, p2;
		if(mouseJoint)
		{
			p1 = debugDraw.conv(mouseJoint->GetAnchorB());
			p2 = debugDraw.conv(mouseJoint->GetTarget());
		}

		Size sz = GetSize();
						
		if(m > 0)
		{
			ImageBuffer ib(sz);
			BufferPainter bp(ib, m == 1 ? MODE_NOAA : m == 2 ? MODE_ANTIALIASED : MODE_SUBPIXEL);
			RGBA bg;
			bg.r = bg.g = bg.b = bg.a = 255;
			bp.Clear(bg);
			debugDraw.Init(bp, sz);
	
			world.DrawDebugData();	
			if(mouseJoint)
			{
				bp.DrawLine(p1, p2, 2, LtGreen);
				bp.DrawEllipse(p2.x - 3, p2.y - 3, 6, 6, Green, PEN_SOLID, Black);
			}
	
			w.DrawImage(0, 0, ib);
		}
		else
		{
			w.DrawRect(sz, White);
			debugDraw.Init(w, sz);
			world.DrawDebugData();	
			if(mouseJoint)
			{
				w.DrawLine(p1, p2, 2, LtGreen);
				w.DrawEllipse(p2.x - 3, p2.y - 3, 6, 6, Green, PEN_SOLID, Black);
			}
		}
	}
	
	virtual void LeftDown(Point p0, dword keyflags)
	{
		b2Vec2 p = debugDraw.conv(p0);
		mouseWorld = p;
		
		if(mouseJoint != NULL)
			return;
	
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = p - d;
		aabb.upperBound = p + d;
	
		QueryCallback callback(p);
		world.QueryAABB(&callback, aabb);
	
		if (callback.fixture)
		{
			b2Body* body = callback.fixture->GetBody();
			b2MouseJointDef md;
			md.bodyA = groundBody;
			md.bodyB = body;
			md.target = p;
			md.maxForce = 1000.0f * body->GetMass();
			mouseJoint = (b2MouseJoint*) world.CreateJoint(&md);
			body->SetAwake(true);
		}		
	}

	virtual void LeftUp(Point p0, dword keyflags)
	{
		if (mouseJoint)
		{
			world.DestroyJoint(mouseJoint);
			mouseJoint = NULL;
		}		
	}

	virtual void MouseMove(Point p, dword keyflags)
	{
		mouseWorld = debugDraw.conv(p);
		
		if(mouseJoint)
			mouseJoint->SetTarget(mouseWorld);
	}
	
	virtual void MouseWheel(Point p, int zdelta, dword keyflags)
	{
		debugDraw.zoom += zdelta / 80.0f;
	}
};

GUI_APP_MAIN
{
	App().Run();
}

