
class GUIManager;
namespace
{
	GUIManager* GUIInst = 0;
}

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
	virtual void HandleClick()=0;
};
GUIElement::GUIElement(){};

class GUIManager
{
public:
	GUIManager(DXOverlay* appinstance);
	~GUIManager();
	template<typename T> 
	T* GetElement(int index);
	int AddElement(GUIElement* Element);
	void DrawElements();
	void UpdateMouse();
	XMFLOAT2 m_MousePosition;
	DXOverlay* appinstance;
	virtual LRESULT MsgProc(int nCode, WPARAM wParam, LPARAM lParam);
private:
	HHOOK m_MouseHook;
	std::vector<GUIElement*> GUIElements;
	bool m_MouseisDown;
	bool m_SendMouseClick;
};
/*Function for our MouseHook*/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return GUIInst->MsgProc(nCode, wParam, lParam);
}
GUIManager::GUIManager(DXOverlay* app) :appinstance(app)
{ 
	GUIInst = this;
	m_MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
	m_MouseisDown = false;
	m_SendMouseClick = true;
};
GUIManager::~GUIManager()
{
	UnhookWindowsHookEx(m_MouseHook);
}
/*Returns Element index so you can reference it later*/
int GUIManager::AddElement(GUIElement* Element)
{
	GUIElements.push_back(Element);
	return GUIElements.size()-1;
}
/*Performs typesafe downcast with dynamic_cast, please check ptr for null on return*/
template<typename T>
T* GUIManager::GetElement(int index)
{
	return dynamic_cast<T*>(GUIElements.at(index));
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
		if (Elem->IsMouseInBounds(m_MousePosition) && m_MouseisDown && m_SendMouseClick)
		{
			Elem->HandleClick();
			m_SendMouseClick = false;
			//Only send one mousedown event to a button at a time
		}
	}
	appinstance->DrawString(XMFLOAT2(50.0f, 50.0f), 1.0f, false, "%f %f", m_MousePosition.x, m_MousePosition.y);
}
LRESULT GUIManager::MsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0) {
		if (wParam == WM_LBUTTONDOWN)
		{
			m_MouseisDown = true;
			printf("left mouse down\n");
		}
		if (wParam == WM_LBUTTONUP) 
		{
			m_MouseisDown = false;
			m_SendMouseClick = true;
			printf("left mouse up\n");
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}


class Button :public GUIElement
{
public:
	typedef std::function<void()> CallbackFunc;
	Button(String Text, XMFLOAT2 position, XMFLOAT2 Size, CallbackFunc callback);
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void Render(DXOverlay* appinst);
	void ChangeText(String txt);
	void HandleClick();
private:
	String m_Text;
	CallbackFunc m_Callback;
};
Button::Button(String Text, XMFLOAT2 position, XMFLOAT2 Size, CallbackFunc callback) :m_Callback(std::move(callback))
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
void Button::ChangeText(String txt)
{
	m_Text = txt;
}
void Button::HandleClick()
{
	m_Callback();
}