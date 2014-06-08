
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
	virtual void HandleMouseDown()=0;
	virtual void HandleMouseUP() = 0;
	bool m_ShouldHandleMouseUP;
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
	bool m_SendMouseDown;
	bool m_SendMouseUp;
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
	m_SendMouseDown = true;
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
		if (Elem->IsMouseInBounds(m_MousePosition) && m_MouseisDown && m_SendMouseDown)
		{
			Elem->HandleMouseDown();
			Elem->m_ShouldHandleMouseUP = true;
			m_SendMouseDown = false;
			//Only send one mousedown event to a button at a time
		}
		if (m_SendMouseUp && Elem->m_ShouldHandleMouseUP) 
		{
			/*shouldhandlemouse up is our way of knowing which element was clicked
			we can't do a bounds check like mousedown because the cursor could have moved off of the 
			element that was originally clicked*/
			Elem->HandleMouseUP();
			Elem->m_ShouldHandleMouseUP = false;
			m_SendMouseUp = false;
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
			m_SendMouseDown = true;
			printf("left mouse down\n");
		}
		if (wParam == WM_LBUTTONUP) 
		{
			m_MouseisDown = false;
			m_SendMouseUp = true; 
			printf("left mouse up\n");
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

#include "Button.h"
#include "Slider.h"