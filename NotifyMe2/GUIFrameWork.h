class GUIElement;
class GUIManager
{
public:
	GUIManager(DXOverlay* appinstance);
	void AddElement(GUIElement* Element);
	void DrawElements();
	XMFLOAT2 m_MousePosition;
	DXOverlay* appinstance;
private:
	std::vector<GUIElement*> GUIElements;
};
GUIManager::GUIManager(DXOverlay* app) :appinstance(app){};
void GUIManager::AddElement(GUIElement* Element)
{
	GUIElements.push_back(Element);
}
void GUIManager::DrawElements()
{
	for (GUIElement* Elem : GUIElements)
	{
		Elem->Render();
	}
}

class GUIElement:public GUIManager
{
public:
	GUIElement(DXOverlay* app);
	XMFLOAT2 m_Position;
	XMFLOAT2 m_Bounds;
	bool m_MouseIsOver;
	String m_Name;
	virtual void Render()=0 ;
	virtual bool IsMouseInBounds()=0 ;
};
GUIElement::GUIElement(DXOverlay* app) :GUIElement(app){};

class Button :public GUIElement
{
public:
	Button(XMFLOAT2 position, XMFLOAT2 Size, DXOverlay* app);
	bool IsMouseInBounds();
	void Render();
};
Button::Button(XMFLOAT2 position, XMFLOAT2 Size, DXOverlay* app) :GUIElement(app)
{
	m_Position = position;
	m_Bounds = Size;
}

bool Button::IsMouseInBounds()
{
	//shows the 4 corners of button
	//Pos__________Pos +X
	//1             2
	//
	//3             4
	//Pos__________Pos +X
	//+Y				+Y
	if (m_MousePosition.x > m_Position.x &&
		m_MousePosition.y > m_Position.y &&
		m_MousePosition.x < (m_Position.x + m_Bounds.x) &&
		m_MousePosition.y < (m_Position.y + m_Bounds.y))
		return true;

	return false;
}
void Button::Render()
{
	XMVECTOR Pos1 = XMLoadFloat2(&m_Position);
	XMVECTOR Pos2 = { m_Position.x + m_Bounds.x, m_Position.y };
	XMVECTOR Pos3 = { m_Position.x, m_Position.y + m_Bounds.y };
	XMVECTOR Pos4 = { m_Position.x + m_Bounds.x, m_Position.y + m_Bounds.y };
	appinstance->DrawLine(Pos1, Pos2, Colors::Black);
	appinstance->DrawLine(Pos1, Pos3, Colors::Black);
	appinstance->DrawLine(Pos2, Pos4, Colors::Black);
	appinstance->DrawLine(Pos3, Pos4, Colors::Black);
}