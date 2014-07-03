class Panel :public GUIElement
{
public:
	Panel(String Text,String Name, XMFLOAT2 Pos, XMFLOAT2 Size);
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void HandleMouseDown();
	void HandleMouseUP();
	void ToogleVisibility();
	void Render(DXOverlay* appinst);
	void AddElement(GUIElement* Element);
	template<typename T>
	T* GetElement(String name);
	void SetPosition(XMFLOAT2 Pos);
private:
	std::vector<GUIElement*> SubElements;
	String m_Text;
	bool m_Visible;
	bool m_MouseIsDown;
	bool m_PanelTopInBounds; //tells use actual panel is in bounds and not an element
	int m_ElementOfEventIndex; //the index in array of SubElements that is handling the current mouse event
};
Panel::Panel(String Text,String Name, XMFLOAT2 Pos, XMFLOAT2 Size)
{
	m_Text = Text;
	m_Name = Name;
	m_Position = Pos;
	m_Bounds = Size;
	m_Visible = true;
	m_MouseIsDown = false;
}
bool Panel::IsMouseInBounds(XMFLOAT2 MousePos)
{
	/*We have to forward all mouse click logic/Events to the subelements
	this allows for the larger design choice of whether or not we want to use panels 
	for certain elements; Due to the fact that events are handled per GUIElement and 
	dispatched down the chain*/
	if (MousePos.x > m_Position.x &&
		MousePos.y > m_Position.y &&
		MousePos.x < (m_Position.x + m_Bounds.x) &&
		MousePos.y < (m_Position.y + 24))
	{
		m_PanelTopInBounds = true;
		return true;
	}
	m_PanelTopInBounds = false;
	for (int i = 0; i < SubElements.size();i++)
	{
		auto SubElem = SubElements.at(i);
		if (SubElem->IsMouseInBounds(MousePos))
		{
			//Save the index later for mouse event forwarding
			m_ElementOfEventIndex = i;
			return true;
		}
	}
	return false;
}
void Panel::HandleMouseDown()
{
	m_MouseIsDown = true;
	if (m_PanelTopInBounds) //We don't want to send events to subelements if the event should be handles by our panel
		return;
	SubElements.at(m_ElementOfEventIndex)->HandleMouseDown();
	/*Forward the mouse down event to the subelement that
	 was in mouse bounds*/
}
void Panel::HandleMouseUP()
{
	m_MouseIsDown = false;
	if (m_PanelTopInBounds) //We don't want to send events to subelements if the event should be handles by our panel
		return;
	SubElements.at(m_ElementOfEventIndex)->HandleMouseUP();
}
void Panel::ToogleVisibility()
{
	m_Visible = !m_Visible;
}
void Panel::AddElement(GUIElement* Element)
{
	/*Input Coordinates are relative to panel so we need to reconvert to screen space here*/
	Element->SetPosition(XMFLOAT2(m_Position.x + Element->m_Position.x,m_Position.y+Element->m_Position.y));
	SubElements.push_back(Element);
}
/*Performs typesafe downcast with dynamic_cast, please check ptr for null on return*/
template<typename T>
T* Panel::GetElement(String name)
{
	for (auto Element : SubElements)
	{
		if (Element->m_Name == name)
		{
			return dynamic_cast<T*>(Element);
		}
	}
	return nullptr;
}
void Panel::Render(DXOverlay* appinst)
{
	if (!m_Visible)
		return;

	if (m_MouseIsDown && m_PanelTopInBounds)
	{
		POINT mousepos;
		if (GetCursorPos(&mousepos))
		{
			this->SetPosition(XMFLOAT2(mousepos.x-1.0f, mousepos.y-1.0f));
		}
	}

	for (auto Elem : SubElements)
	{
		Elem->Render(appinst);
	}
	appinst->DrawString(XMFLOAT2(m_Position.x + (m_Bounds.x / 2), m_Position.y+12), 1.0f, true, "%s", m_Text.c_str());
	XMVECTOR Pos1 = { m_Position.x, m_Position.y };
	XMVECTOR Pos2 = { m_Position.x +m_Bounds.x, m_Position.y };
	XMVECTOR Pos3 = { m_Position.x, m_Position.y+m_Bounds.y };
	XMVECTOR Pos4 = { m_Position.x + m_Bounds.x, m_Position.y + m_Bounds.y };
	XMVECTOR Banner1 = { m_Position.x, m_Position.y + 24 };
	XMVECTOR Banner2 = { m_Position.x + m_Bounds.x, m_Position.y + 24 };
	appinst->DrawLine(Banner1, Banner2, Colors::Black);
	appinst->DrawLine(Pos1, Pos2, Colors::Black);
	appinst->DrawLine(Pos1, Pos3, Colors::Black);
	appinst->DrawLine(Pos2, Pos4, Colors::Black);
	appinst->DrawLine(Pos3, Pos4, Colors::Black);
}
void Panel::SetPosition(XMFLOAT2 Pos)
{
	XMFLOAT2 Delta = XMFLOAT2(m_Position.x - Pos.x, m_Position.y - Pos.y);
	m_Position = Pos;
	for (auto Elem : SubElements)
	{
		Elem->SetPosition(XMFLOAT2(Elem->m_Position.x - Delta.x, Elem->m_Position.y - Delta.y));
	}
}
