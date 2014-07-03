class Button :public GUIElement
{
public:
	typedef std::function<void()> CallbackFunc;
	Button(String Text,String Name, XMFLOAT2 position, XMFLOAT2 Size, CallbackFunc callback);
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void Render(DXOverlay* appinst);
	void ChangeText(String txt);
	void HandleMouseDown();
	void HandleMouseUP();
	void SetPosition(XMFLOAT2 Pos);
private:
	String m_Text;
	CallbackFunc m_Callback;
};
Button::Button(String Text,String Name, XMFLOAT2 position, XMFLOAT2 Size, CallbackFunc callback) :m_Callback(std::move(callback))
{
	m_Position = position;
	m_Bounds = Size;
	m_Text = Text;
	m_Name = Name;
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
		XMVECTOR Pos1 = { m_Position.x, m_Position.y };
		XMVECTOR Pos2 = { m_Position.x + m_Bounds.x, m_Position.y };
		XMVECTOR Pos3 = { m_Position.x, m_Position.y + m_Bounds.y };
		XMVECTOR Pos4 = { m_Position.x + m_Bounds.x, m_Position.y + m_Bounds.y };
		appinst->DrawLine(Pos1, Pos2, Colors::Black);
		appinst->DrawLine(Pos1, Pos3, Colors::Black);
		appinst->DrawLine(Pos2, Pos4, Colors::Black);
		appinst->DrawLine(Pos3, Pos4, Colors::Black);
	}
	appinst->DrawString(XMFLOAT2(m_Position.x + (m_Bounds.x / 2), m_Position.y+(m_Bounds.y/2)), 1.0f, true, "%s", m_Text.c_str());
}
void Button::ChangeText(String txt)
{
	m_Text = txt;
}
void Button::HandleMouseDown()
{
	m_Callback();
}
void Button::HandleMouseUP()
{
	
}
void Button::SetPosition(XMFLOAT2 Pos)
{
	m_Position = Pos;
}