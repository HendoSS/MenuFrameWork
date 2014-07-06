class CheckBox :public GUIElement
{
public:
	typedef std::function<void()> CallbackFunc;
	CheckBox(String Text, String Name,bool Value, XMFLOAT2 position, XMFLOAT2 size, CallbackFunc callback);
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void Render(DXOverlay* appinst);
	void HandleMouseDown();
	void HandleMouseUP();
	void SetPosition(XMFLOAT2 Pos);
	bool GetValue();
private:
	CallbackFunc m_Callback;
	String m_Text;
	bool m_Value;
};

CheckBox::CheckBox(String Text, String Name, bool Value, XMFLOAT2 position, XMFLOAT2 size, CallbackFunc callback) :m_Callback(std::move(callback))
{
	m_Value = Value;
	m_Position = position;
	m_Bounds = size;
	m_Text = Text;
	m_Name = Name;
}
bool CheckBox::IsMouseInBounds(XMFLOAT2 MousePos)
{
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
void CheckBox::Render(DXOverlay* appinst)
{
	XMVECTOR Pos1 = { m_Position.x, m_Position.y };
	XMVECTOR Pos2 = { m_Position.x + m_Bounds.x, m_Position.y };
	XMVECTOR Pos3 = { m_Position.x, m_Position.y + m_Bounds.y };
	XMVECTOR Pos4 = { m_Position.x + m_Bounds.x, m_Position.y + m_Bounds.y };
	appinst->DrawLine(Pos1, Pos2, Colors::Black);
	appinst->DrawLine(Pos1, Pos3, Colors::Black);
	appinst->DrawLine(Pos2, Pos4, Colors::Black);
	appinst->DrawLine(Pos3, Pos4, Colors::Black);
	if (m_Value)
	{
		XMVECTOR p1 = { m_Position.x+5.0f, m_Position.y + (m_Bounds.y*.6f) };
		XMVECTOR p2 = { m_Position.x + (m_Bounds.x /2), m_Position.y + (m_Bounds.y * .85f) };
		XMVECTOR p3 = { m_Position.x + m_Bounds.x - 7.0f, m_Position.y + (m_Bounds.y*.15f) };
		appinst->DrawLine(p1, p2, Colors::Red);
		appinst->DrawLine(p2, p3, Colors::Red);
	}
	appinst->DrawString(XMFLOAT2(m_Position.x + m_Bounds.x + 3.0f, m_Position.y + (m_Bounds.y / 2)), 1.0f, false, "%s", m_Text.c_str());
}
void CheckBox::HandleMouseDown()
{
	m_Value = !m_Value;
	m_Callback();
}
void CheckBox::HandleMouseUP()
{

}
void CheckBox::SetPosition(XMFLOAT2 Pos)
{
	m_Position = Pos;
}
bool CheckBox::GetValue()
{
	return m_Value;
}