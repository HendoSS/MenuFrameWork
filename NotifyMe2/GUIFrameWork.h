/*!!!!!!!!!!!! When adding elements to a panel coordinates are relative to the panel, when adding elements
strait to the GUIManager w/o a panel they are relative to screen/game window !!!!!!!!!!!!!! */

//^^^^^ READ THAT !!!!!!!!!
class GUIManager;
#include <chrono>
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
	virtual void SetPosition(XMFLOAT2 Pos) = 0;
	bool m_ShouldHandleMouseUP;
};
GUIElement::GUIElement(){};

class GUIManager
{
public:
	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::milliseconds milliseconds;
	GUIManager(DXOverlay* appinstance);
	~GUIManager();
	template<typename T> 
	T* GetElement(int index);
	template<typename T>
	T* GetElement(String name);
	int AddElement(GUIElement* Element);
	void DrawElements();
	void UpdateMouse();
	XMFLOAT2 m_MousePosition;
	DXOverlay* appinstance;
	virtual LRESULT MsgProc(int nCode, WPARAM wParam, LPARAM lParam);
private:
	/*The clock stuff prevents events from being sent in the wrong order it prevents
	the series: Mouseclick then Mouseover then SendMouseEvent, it now only allows
	MouseOver then MouseDown then SendMouseEvent*/
	Clock::time_point m_StartTime;
	Clock::time_point m_EndTime;
	milliseconds m_ElapsedTime;
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
	m_StartTime = Clock::now();
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
template<typename T>
T* GUIManager::GetElement(String name)
{
	for (auto Elem : GUIElements)
	{
		if (Elem->m_Name == name)
		{
			return dynamic_cast<T*>(Elem); 
		}
	}
	return nullptr;
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
	//End time is when we compare the time the mouse has been down
	m_EndTime = Clock::now();
	m_ElapsedTime=std::chrono::duration_cast<milliseconds>(m_EndTime-m_StartTime);
	if (m_ElapsedTime.count() >= 100 &&(m_SendMouseDown || m_MouseisDown))
	{
		//only ignore event if elapsed time is over 100ms and send button flags
		//are set
		std::cout << "Mouse Event Ignored \n";
		m_SendMouseDown = false;
		m_MouseisDown = false;
	}
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
			//Set start time to time the button was pressed
			m_StartTime = Clock::now();
			m_MouseisDown = true;
			m_SendMouseDown = true;
			m_SendMouseUp = false;
			printf("left mouse down\n");
		}
		if (wParam == WM_LBUTTONUP) 
		{
			m_MouseisDown = false;
			m_SendMouseDown = false;
			m_SendMouseUp = true; 
			printf("left mouse up\n");
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}


#include "Button.h"
#include "Slider.h"
#include "Panel.h"
#include "CheckBox.h"