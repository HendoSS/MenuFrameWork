class Label:public GUIElement
{
public:
	Label(String Text, String Name, XMFLOAT2 position, XMFLOAT2 Size);
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void Render(DrawingAbstractor* appinst);
	void ChangeText(String txt);
	void HandleMouseDown();
	void HandleMouseUP();
	void SetPosition(XMFLOAT2 Pos);
private:
	String m_Text;
};
Label::Label(String Text, String Name, XMFLOAT2 position, XMFLOAT2 Size) 
{
	m_Position = position;
	m_Bounds = Size;
	m_Text = Text;
	m_Name = Name;
}
bool Label::IsMouseInBounds(XMFLOAT2 MousePos)
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
void Label::Render(DrawingAbstractor* appinst)
{
	appinst->DrawString(XMFLOAT2(m_Position.x /*+ (m_Bounds.x / 2)*/, m_Position.y/*+(m_Bounds.y/2)*/ - 3.0f), 1.0f, true, "%s", m_Text.c_str());
}
void Label::ChangeText(String txt)
{
	m_Text = txt;
}
void Label::HandleMouseDown()
{
}
void Label::HandleMouseUP()
{

}
void Label::SetPosition(XMFLOAT2 Pos)
{
	m_Position = Pos;
}

