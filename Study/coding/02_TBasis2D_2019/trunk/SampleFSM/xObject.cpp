#include "xObject.h"
bool		xObject::Init()
{
	return true;
};
void		xObject::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
};
void		xObject::SetHero(bool bHero )
{
	m_bHero = bHero;
}
void		xObject::Left()
{
	m_ObjPos.x -= g_fSecPerFrame * m_fSpeed;
	SetPos(m_ObjPos);
}
void		xObject::Right()
{
	m_ObjPos.x += g_fSecPerFrame * m_fSpeed;
	SetPos(m_ObjPos);
}
void		xObject::Up()
{
	m_ObjPos.y -= g_fSecPerFrame * m_fSpeed;
	SetPos(m_ObjPos);
}
void		xObject::Down()
{
	m_ObjPos.y += g_fSecPerFrame * m_fSpeed;
	SetPos(m_ObjPos);
}
bool		xObject::Frame()
{
	return true;
};
bool		xObject::Release()
{
	return true;
};

xObject::xObject()
{
	m_bHero	 = true;
	m_fSpeed = 70.0f;
	m_iDirection[0] = m_iDirection[1] =1;
	m_vLook = Vector2(0.0f, 0.0f);
}
xObject::~xObject()
{
}
