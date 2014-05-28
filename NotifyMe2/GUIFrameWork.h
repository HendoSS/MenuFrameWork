


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
	void UpdateMouse();
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
void GUIManager::UpdateMouse()
{
	POINT mouse;
	if (GetCursorPos(&mouse))
	{
		m_MousePosition.x = mouse.x;
		m_MousePosition.y = mouse.y;
	}else{
		m_MousePosition = XMFLOAT2(0.0f, 0.0f);
	}
	for (GUIElement* Elem : GUIElements)
	{
		Elem->IsMouseInBounds(m_MousePosition);
	}
	appinstance->DrawString(XMFLOAT2(50.0f, 50.0f), 1.0f, false, "%f %f", m_MousePosition.x, m_MousePosition.y);
}

class Button :public GUIElement
{
public:
	Button(String Text,XMFLOAT2 position, XMFLOAT2 Size);
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void Render(DXOverlay* appinst);
private:
	String m_Text;
};
Button::Button(String Text,XMFLOAT2 position, XMFLOAT2 Size)
{
	m_Position = position;
	m_Bounds = Size;
	m_Text = Text;
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
	{
		m_MouseIsOver = true;
		return true;
	}
	m_MouseIsOver = false;
	return false;
}
void Button::Render(DXOverlay* appinst)
{
	if (m_MouseIsOver)
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
		appinst->DrawString(XMFLOAT2(m_Position.x+(m_Bounds.x/2),m_Position.y+(m_Bounds.y/2)), 1.0f, true, "%s",m_Text.c_str());
}