#include <CtrlLib/CtrlLib.h>
#include <plugin/box2d/Box2D.h>

using namespace Upp;

struct QueryCallback : b2QueryCallback
{
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;
				return false;
			}
		}

		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

struct DebugDraw : b2DebugDraw
{
	Draw* w;
	Sizef sz;
	float cx, cy;
	float aspect;
	
	void DrawPolygon(const b2Vec2* v, int vertexCount, const b2Color& color)
	{
		Vector<Point> p;
		p.SetCount(vertexCount + 1);
		for(int i = 0; i < vertexCount; ++i)
		{
			p[i].x = int(v[i].x * aspect + cx);
			p[i].y = int(cy - v[i].y * aspect);
		}
		p[vertexCount - 1].x = int(v[0].x);
		p[vertexCount - 1].y = int(v[0].x);
		
		Color bg(int(color.r * 255.0), int(color.g * 255.0), int(color.b * 255.0));
		Color fg(int(color.r * 150.0), int(color.g * 150.0), int(color.b * 150.0));
		
		w->DrawPolyline(p, vertexCount, 1, fg);
	}
	
	void DrawSolidPolygon(const b2Vec2* v, int vertexCount, const b2Color& color)
	{
		Vector<Point> p;
		p.SetCount(vertexCount);
		for(int i = 0; i < vertexCount; ++i)
		{
			p[i].x = int(v[i].x * aspect + cx);
			p[i].y = int(cy - v[i].y * aspect);
		}
		
		Color bg(int(color.r * 255.0), int(color.g * 255.0), int(color.b * 255.0));
		Color fg(int(color.r * 150.0), int(color.g * 150.0), int(color.b * 150.0));
		
		w->DrawPolygon(p, bg, 1, fg);		
	}
	
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		float r = aspect * radius * 2.0f;
		int x = int((center.x - radius) * aspect + cx);
		int y = int(cy - (center.y + radius) * aspect);
		Color fg(int(color.r * 150.0), int(color.g * 150.0), int(color.b * 150.0));
		w->DrawEllipse(x, y, int(r), int(r), fg);
	}
	
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		float r = aspect * radius * 2.0f;
		int x = int((center.x - radius) * aspect + cx);
		int y = int(cy - (center.y + radius) * aspect);
		Color bg(int(color.r * 255.0), int(color.g * 255.0), int(color.b * 255.0));
		Color fg(int(color.r * 150.0), int(color.g * 150.0), int(color.b * 150.0));
		w->DrawEllipse(x, y, int(r), int(r), bg, 1, fg);
	}
	
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		Color fg(int(color.r * 150.0), int(color.g * 150.0), int(color.b * 150.0));
		int x0 = int(p1.x * aspect + cx);
		int y0 = int(cy - p1.y * aspect);
		int x1 = int(p2.x * aspect + cx);
		int y1 = int(cy - p2.y * aspect);
		w->DrawLine(x0, y0, x1, y1, 1, fg);
	}
	
	void DrawTransform(const b2Transform& xf)
	{
	}
	
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{
	}
	
	void DrawString(int x, int y, const char* string, ...)
	{
	}
	
	void DrawAABB(b2AABB* aabb, const b2Color& color)
	{
	}
};

struct App : TopWindow
{
	b2Body* m_middle;
	b2World* m_world;
	b2MouseJoint* m_mouseJoint;
	DebugDraw m_debugDraw;
	b2Vec2 m_mouseWorld;
	b2Body* m_groundBody;
	b2AABB m_worldAABB;
	
	typedef App CLASSNAME;
	
	App()
	{
		Sizeable().Zoomable();
		Bridge();
		SetTimeCallback(-10, THISBACK(Render0));
		BackPaint();
		m_mouseJoint = NULL;
	}
	~App()
	{
		delete m_world;
	}
	
	void Bridge()
	{
		const int e_count = 30;
		b2Vec2 gravity;
		gravity.Set(0.0f, -10.0f);

		bool doSleep = true;

		m_world = new b2World(gravity, doSleep);
		m_world->SetDebugDraw(&m_debugDraw);

		
		b2BodyDef bd;
		b2Body* ground = m_world->CreateBody(&bd);

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
		for(int i = 0; i < e_count; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-14.5f + 1.0f * i, 5.0f);
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&fd);

			b2Vec2 anchor(-15.0f + 1.0f * i, 5.0f);
			jd.Initialize(prevBody, body, anchor);
			m_world->CreateJoint(&jd);

			if (i == (e_count >> 1))
			{
				m_middle = body;
			}
			prevBody = body;
		}

		b2Vec2 anchor(-15.0f + 1.0f * e_count, 5.0f);
		jd.Initialize(prevBody, ground, anchor);
		m_world->CreateJoint(&jd);

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
			b2Body* body = m_world->CreateBody(&bd);
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
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&fd);
		}		
	}

	void Step(float hz = 60, int velocityIterations = 8, int positionIterations = 3)
	{
		float32 timeStep = 1.0f / hz;
		
		int flags = 0;
		flags += b2DebugDraw::e_shapeBit;
		m_debugDraw.SetFlags(flags);
	
		m_world->SetWarmStarting(1);
		m_world->SetContinuousPhysics(1);
		m_world->Step(timeStep, velocityIterations, positionIterations);	
		m_world->DrawDebugData();	
	}
	
	b2Vec2 ConvertScreenToWorld(int x, int y)
	{
		Size sz = GetSize();
		x += sz.cx / 2;
		y = sz.cy / 2 - y;
		float32 u = x / float32(sz.cx);
		float32 v = (sz.cy - y) / float32(sz.cy);
	
		float32 ratio = float32(sz.cx) / float32(sz.cy);
		b2Vec2 extents(ratio * 25.0f, 25.0f);
	
		b2Vec2 viewCenter(0.0f, 0.0f);
		b2Vec2 lower = viewCenter - extents;
		b2Vec2 upper = viewCenter + extents;
	
		b2Vec2 p;
		p.x = (1.0f - u) * lower.x + u * upper.x;
		p.y = (1.0f - v) * lower.y + v * upper.y;
		return p;
	}
	
	void Render0()
	{
		Refresh();
	}
	
	void Render(Draw& w)
	{
		Size sz = GetSize();
		m_debugDraw.w = &w;
		m_debugDraw.sz = sz;
		m_debugDraw.cx = sz.cx / 2.0f;
		m_debugDraw.cy = sz.cy / 2.0f;
		m_debugDraw.aspect = 18.0f;

		Step();
	}
	
	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, White);
		Render(w);
	}
	
	virtual void LeftDown(Point p0, dword keyflags)
	{
		b2Vec2 p(float32(p0.x), float32(p0.y));
		m_mouseWorld = ConvertScreenToWorld(p0.x, p0.y);
		
		if(m_mouseJoint != NULL)
			return;
	
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = p - d;
		aabb.upperBound = p + d;
	
		QueryCallback callback(p);
		m_world->QueryAABB(&callback, aabb);
	
		if (callback.m_fixture)
		{
			b2Body* body = callback.m_fixture->GetBody();
			b2MouseJointDef md;
			md.bodyA = m_groundBody;
			md.bodyB = body;
			md.target = p;
			md.maxForce = 1000.0f * body->GetMass();
			m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
			body->SetAwake(true);
		}		
	}

	virtual void LeftUp(Point p0, dword keyflags)
	{
		b2Vec2 p = ConvertScreenToWorld(p0.x, p0.y);
		if (m_mouseJoint)
		{
			m_world->DestroyJoint(m_mouseJoint);
			m_mouseJoint = NULL;
		}		
	}

	virtual void MouseMove(Point p0, dword keyflags)
	{
		b2Vec2 p = ConvertScreenToWorld(p0.x, p0.y);
		
		m_mouseWorld = p;
		
		if (m_mouseJoint)
		{
			m_mouseJoint->SetTarget(p);
		}
	}
};

GUI_APP_MAIN
{
	App().Run();
}

