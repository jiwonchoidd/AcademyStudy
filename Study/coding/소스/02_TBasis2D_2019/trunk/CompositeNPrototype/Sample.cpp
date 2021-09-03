#include <iostream>
#include <list>
#include <map>
using namespace std;
template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T inst;
		return inst;
	}
};
class TPosition
{
public:
	TPosition() {}
	TPosition(int ix, int iy) {
		m_ix = ix;
		m_iy = iy;
	}
	int m_ix;
	int m_iy;
};
class TGraphic {
public:
	virtual TGraphic* Clone() = 0;
	virtual void Draw(TPosition pos) = 0;
};
class TTriangle : public TGraphic {
public:
	TGraphic * Clone() 
	{
		return new TTriangle(*this);
	};
	void Draw(TPosition pos) {
		std::cout << pos.m_ix << pos.m_iy;
	};
};
class TRectangle : public TGraphic {
public:
	TGraphic * Clone()
	{
		return new TRectangle(*this);
	};
	void Draw(TPosition pos) {
		std::cout << pos.m_ix << pos.m_iy;
	};
};
class TGraphicComposite : public TGraphic
{
public:
	void Draw(TPosition pos) {
		std::cout << pos.m_ix << pos.m_iy;
	};
	std::list<TGraphic*> m_Components;
	TGraphic * Clone()
	{
		TGraphicComposite * pGC  = 
			new TGraphicComposite(*this);

		std::list<TGraphic*>::iterator iterSrc;
		for (iterSrc = m_Components.begin();
			iterSrc != m_Components.end();
			iterSrc++)
		{
			TGraphic* pNewGraphic = (*iterSrc)->Clone();
			pGC->m_Components.push_back(pNewGraphic);
		}
		return pGC;
	};
};

class TMouse
{
public:
	TPosition m_pos;
	bool     m_bButton[3];
public:
	bool IsLeftButtonPushed()
	{
		return m_bButton[0];
	}
	TPosition GetPosition()
	{
		return m_pos;
	}
};

TMouse g_Mouse;

class TDocument
{
public:
	list<TGraphic*> m_docList;

public:
	void Add(TGraphic* pObj) 
	{
		m_docList.push_back(pObj);
	}
};
class TGraphicEditor {
public:	
	TDocument		m_doc;
public:
	void AddNewGraphics(TGraphic* pSelected)
	{
		TGraphic* pObj = pSelected->Clone();
		while (g_Mouse.IsLeftButtonPushed())
		{			
			pObj->Draw(g_Mouse.GetPosition());
		}
		m_doc.Add(pObj);
	}
private:

};

class TPalette : public TSingleton<TPalette>
{
private:
	friend class TSingleton<TPalette>;
	std::map<int, TGraphic*>  m_items;
public:
	void Draw()	{}
	/*static TPalette* g_pPlette;
	static TPalette* GetInstance()
	{
		if (g_pPlette == NULL)
		{
			g_pPlette = new TPalette;
		}
		return g_pPlette;
	}*/
	// ¿ø½Ã ¸ðµ¨ Ãß°¡ µî·Ï
	void RegisterNewGraphic(TGraphic* pGraphic)
	{
		m_items[m_items.size() + 1] = pGraphic;
	}

	TGraphic* GetSelectedObj()
	{
		return m_items[GetItemOrder()];
	}
	int GetItemOrder()
	{
		int iItem = -1;
		//g_Mouse.GetPosition();
		return iItem;
	}
protected:
	TPalette()
	{
		// ¿ø½Ã ¸ðµ¨
		TGraphic* pGraphicA = new TTriangle;
		m_items[0] = pGraphicA;
		TGraphic* pGraphicB = new TRectangle;
		m_items[1] = pGraphicB;
	}
	
};
#define I_Palette TPalette::GetInstance()

void main()
{
	//'g_pPlette': 'TPalette'ÀÇ ¸â¹ö°¡ ¾Æ´Õ´Ï´Ù.
	TPalette palette = TPalette::GetInstance();
	TPalette paletteA = TPalette::GetInstance();
	palette.Draw();

	TGraphicEditor editer;
	editer.AddNewGraphics(palette.GetSelectedObj());
	
	TGraphicComposite etc;
	list<TGraphic*>::iterator iter;
	for ( iter= editer.m_doc.m_docList.begin();
		iter != editer.m_doc.m_docList.end();
		iter++ )
	{
		TGraphic* pData = (*iter);
		etc.m_Components.push_back(pData);
	}
	palette.RegisterNewGraphic(&etc);
}
