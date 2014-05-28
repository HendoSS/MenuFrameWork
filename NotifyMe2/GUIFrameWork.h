


class GUIElement
{
public:
	GUIElement();
	XMFLOAT2 m_Position;
	XMFLOAT2 m_Bounds;
	bool m_MouseIsOver;
	String m_Name;
	virtual void Render(DXOverlay* appinst) = 0;
	virtual bool IsMouseInBounds(XMFLOAT2 MousePos) = 0;
};
GUIElement::GUIElement(){};

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
		Elem->Render(appinstance);
	}
}


class Button :public GUIElement
{
public:
	Button(XMFLOAT2 position, XMFLOAT2 Size);
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void Render(DXOverlay* appinst);
};
Button::Button(XMFLOAT2 position, XMFLOAT2 Size)
{
	m_Position = position;
	m_Bounds = Size;
}

bool Button::IsMouseInBounds(XMFLOAT2 MousePos)
{
	//shows the 4 corners of button
	//Pos__________Pos +X
	//1             2
	//
	//3             4
	//Pos__________Pos +X
	//+Y				+Y
	if (MousePos.x > m_Position.x &&
		MousePos.y > m_Position.y &&
		MousePos.x < (m_Position.x + m_Bounds.x) &&
		MousePos.y < (m_Position.y + m_Bounds.y))
		return true;

	return false;
}
void Button::Render(DXOverlay* appinst)
{
	XMVECTOR Pos1 = XMLoadFloat2(&m_Position);
	XMVECTOR Pos2 = { m_Position.x + m_Bounds.x, m_Position.y };
	XMVECTOR Pos3 = { m_Position.x, m_Position.y + m_Bounds.y };
	XMVECTOR Pos4 = { m_Position.x + m_Bounds.x, m_Position.y + m_Bounds.y };
	appinst->DrawLine(Pos1, Pos2, Colors::Black);
	appinst->DrawLine(Pos1, Pos3, Colors::Black);
	appinst->DrawLine(Pos2, Pos4, Colors::Black);
	appinst->DrawLine(Pos3, Pos4, Colors::Black);
}