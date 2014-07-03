template<typename T>
T Re_Range(T x, T inmin, T inmax, T outmin, T outmax)
{
	//Make sure the precision isn't truncated until the the final result, so keep it as double until then
	T newval =static_cast<T>(static_cast<double>((x - inmin) * (outmax - outmin)) / static_cast<double>((inmax - inmin) + outmin));
	return newval;
}

template<typename T>
class Slider:public GUIElement
{
public:
	typedef std::function<void()> CallbackFunc;
	/*TODO add templated member function, with some sort of callback for user defined
	modification of templated member (because we won't know how to modify it as the type is unknown*/
	Slider(String Text, XMFLOAT2 position, XMFLOAT2 Size,T MinVal,T MaxVal, XMVECTOR Color,CallbackFunc callback);
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void Render(DXOverlay* appinst);
	void HandleMouseDown();
	void HandleMouseUP();
	T GetValue();
	void SetPosition(XMFLOAT2 Pos);
private:
	CallbackFunc m_Callback;
	T m_Min, m_Max;
	XMFLOAT2 m_SliderPosition;
	XMVECTOR m_Color;
	bool m_MouseIsDown;
	T m_Value;
};
template<typename T>
Slider<T>::Slider(String Text, XMFLOAT2 position, XMFLOAT2 Size,T MinVal, T MaxVal,XMVECTOR Color, CallbackFunc callback) :m_Callback(std::move(callback))
{
	m_Name = Text;
	m_Position = position;
	m_Bounds = Size;
	m_Min = MinVal;
	m_Max = MaxVal;
	m_SliderPosition = XMFLOAT2(m_Position.x+(m_Bounds.x/2),m_Position.y);
	m_Color = Color;
	m_MouseIsDown = false;
}
template<typename T>
bool Slider<T>::IsMouseInBounds(XMFLOAT2 MousePos)
{
	/*We only have the position of the slider, so we need to create a box around that point, because you can't do a bounds check on a single point
	I went with a fixed amount. Graphical layout is:
	
	1) x-2,y-4              2)x+2,y-4
	
	                
	               x,y 

				   3) x-2,y+4              4)x+2,y+4*/
	if (MousePos.x > (m_SliderPosition.x-4) &&
		MousePos.y > (m_SliderPosition.y-(m_Bounds.y/2)) &&
		MousePos.x < (m_SliderPosition.x + 4) &&
		MousePos.y < (m_SliderPosition.y +(m_Bounds.y/2)))
	{
		m_MouseIsOver = true;
		return true;
	}
	m_MouseIsOver = false;
	return false;
}
template<typename T>
void Slider<T>::Render(DXOverlay* appinst)
{
	if (m_MouseIsDown)
	{
		POINT mousepos;
		if (GetCursorPos(&mousepos))
		{
			if (mousepos.x>m_Position.x && mousepos.x<(m_Position.x+m_Bounds.x))
				m_SliderPosition.x= mousepos.x;
		}
	}
	
	m_Value = Re_Range<T>(m_SliderPosition.x, m_Position.x, (m_Position.x + m_Bounds.x), m_Min, m_Max);

	//We need to provide the proper input identifier to the text print func
	String identifier;
	if (typeid(T) == typeid(int))
		identifier = "%i";

	if (typeid(T) == typeid(float))
		identifier = "%f";

	if (typeid(T) == typeid(double))
		identifier = "%f";

	//we print the min value, with -2 on the x just for formatting and then we draw the main slider bar
	appinst->DrawString(XMFLOAT2(m_Position.x-5,m_Position.y), 1.0f, true, identifier.c_str(), m_Min);

	XMVECTOR Pos1 = { m_Position.x, m_Position.y - (m_Bounds.y / 2) };
	XMVECTOR Pos2 = { m_Position.x + m_Bounds.x, m_Position.y - (m_Bounds.y / 2) };
	XMVECTOR Pos3 = { m_Position.x, m_Position.y + (m_Bounds.y/2) };
	XMVECTOR Pos4 = { m_Position.x + m_Bounds.x, m_Position.y +(m_Bounds.y / 2) };
	appinst->DrawLine(Pos1, Pos2, Colors::Black);
	appinst->DrawLine(Pos1, Pos3, Colors::Black);
	appinst->DrawLine(Pos2, Pos4, Colors::Black);
	appinst->DrawLine(Pos3, Pos4, Colors::Black);

	//these are the 4 points extracted from the position of the slider to form a box
	XMVECTOR sPos1 = { m_SliderPosition.x, m_SliderPosition.y -(m_Bounds.y/2)};
	XMVECTOR sPos2 = { m_SliderPosition.x, m_SliderPosition.y+(m_Bounds.y/2)};
	
	//Draw the slider
	appinst->DrawLine(sPos1, sPos2, m_Color);

	//Draw Max Value, again +2 cuz of formatting
	appinst->DrawString(XMFLOAT2(m_Position.x + m_Bounds.x+5, m_Position.y), 1.0f, true, identifier.c_str(), m_Max);
	appinst->DrawString(XMFLOAT2(m_Position.x + (m_Bounds.x / 2), m_Position.y), 1.0f, true, identifier.c_str(), m_Value);
}
template<typename T>
void Slider<T>::HandleMouseDown()
{
	m_MouseIsDown = true;
}
template<typename T>
void Slider<T>::HandleMouseUP()
{
	m_Callback();
	m_MouseIsDown = false;
}
template<typename T>
T Slider<T>::GetValue()
{
	return m_Value;
}
template<typename T>
void Slider<T>::SetPosition(XMFLOAT2 Pos)
{
	/*We have to move the slider, the second line here keeps the slider at the same amount
	away from the new X position as it was away from the old x position*/
	m_SliderPosition.y = Pos.y;
	m_SliderPosition.x = Pos.x + (m_Position.x - m_SliderPosition.x);
	m_Position = Pos;
}
