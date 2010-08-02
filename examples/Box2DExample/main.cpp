#include <CtrlLib/CtrlLib.h>
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
	b2World world;
	b2MouseJoint* mouseJoint;
	DebugDraw debugDraw;
	b2Vec2 mouseWorld;
	b2Body* groundBody;
	
	typedef App CLASSNAME;
	
	App() : world(b2Vec2(0.0, -10.0), true)
	{
		Title("Box2D Example");
		SetRectX(0, 640);
		SetRectY(0, 480);
		Sizeable().Zoomable();
		BackPaint();
		SetTimeCallback(-10, THISBACK(Render));
		mouseJoint = NULL;
		b2Vec2 gravity;
		gravity.Set(0.0f, -10.0f);
		world.SetGravity(gravity);
		world.SetDebugDraw(&debugDraw);
		b2BodyDef bodyDef;
		groundBody = world.CreateBody(&bodyDef);
		debugDraw.zoom = 15.0f;
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

	b2Vec2 ConvertScreenToWorld(int x, int y)
	{
		b2Vec2 p;
		p.x = (x - debugDraw.cx) / debugDraw.aspect;
		p.y = (debugDraw.cy - y) / debugDraw.aspect;		
		return p;
	}
	
	void Render() { Refresh(); }
	
	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, White);

		debugDraw.w = &w;
		debugDraw.sz = sz;
		debugDraw.cx = sz.cx / 2.0f;
		debugDraw.cy = sz.cy / 2.0f + 150.0f;
		debugDraw.aspect = sz.cx / (float) sz.cy;
		debugDraw.aspect *= debugDraw.zoom;

		float hz = 60;
		int velocityIterations = 8;
		int positionIterations = 3;

		float32 timeStep = 1.0f / hz;
		
		debugDraw.SetFlags(b2DebugDraw::e_shapeBit);
	
		world.SetWarmStarting(1);
		world.SetContinuousPhysics(1);	
		world.Step(timeStep, velocityIterations, positionIterations);	
		world.DrawDebugData();	

		int px = int(mouseWorld.x * debugDraw.aspect + debugDraw.cx);
		int py = int(debugDraw.cy - mouseWorld.y * debugDraw.aspect);
		w.DrawEllipse(px - 3, py - 3, 6, 6, Green);
	}
	
	virtual void LeftDown(Point p0, dword keyflags)
	{
		b2Vec2 p = ConvertScreenToWorld(p0.x, p0.y);
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
		mouseWorld = ConvertScreenToWorld(p.x, p.y);
		
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

