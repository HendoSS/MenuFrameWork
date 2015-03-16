/*!!!!!!!!!!!! When adding elements to a panel coordinates are relative to the panel, when adding elements
strait to the GUIManager w/o a panel they are relative to screen/game window !!!!!!!!!!!!!! */

//^^^^^ READ THAT !!!!!!!!!
class GUIManager;
#include <chrono>

//This class it to make it very easy to override the drawing functiosn for different
//Engines, simple Define in the function how each function is handles.
class DrawingAbstractor
{
public:
	DrawingAbstractor(DXOverlay* pOverlay)
	{
		m_Overlay = pOverlay;
	}
	void DrawString(XMFLOAT2 position, float scale, bool center, const char* Format, ...)
	{
		char Buffer[1024] = { '\0' };
		va_list va_alist;
		va_start(va_alist, Format);
		vsprintf_s(Buffer, Format, va_alist);
		va_end(va_alist);

		m_Overlay->DrawString(position, 1.0f, false, "%s", Buffer);
	}
	void DrawLine(FXMVECTOR pos1, FXMVECTOR pos2, FXMVECTOR color)
	{
		m_Overlay->DrawLine(pos1, pos2, color);
	}
private:
	DXOverlay* m_Overlay;
};

class GUIElement
{
public:
	GUIElement();
	XMFLOAT2 m_Position;
	XMFLOAT2 m_Bounds;
	bool m_MouseIsOver;
	String m_Name;
	virtual void Render(DrawingAbstractor* appinst) = 0;
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
	GUIManager(DrawingAbstractor* appinstance);
	~GUIManager();
	template<typename T> 
	T* GetElement(int index);
	template<typename T>
	T* GetElement(String name);
	int AddElement(GUIElement* Element);
	void DrawElements();
	void UpdateMouse();
	XMFLOAT2 m_MousePosition;
	DrawingAbstractor* appinstance;
private:
	/*The clock stuff prevents events from being sent in the wrong order it prevents
	the series: Mouseclick then Mouseover then SendMouseEvent, it now only allows
	MouseOver then MouseDown then SendMouseEvent*/
	Clock::time_point m_StartTime;
	Clock::time_point m_EndTime;
	milliseconds m_ElapsedTime;
	std::vector<GUIElement*> GUIElements;
	bool m_MouseisDown;
	bool m_SendMouseDown;
	bool m_SendMouseUp;
	SHORT m_OldMouseState;
};

GUIManager::GUIManager(DrawingAbstractor* app) :appinstance(app)
{ 
	m_MouseisDown = false;
	m_SendMouseDown = true;
	m_StartTime = Clock::now();
};
GUIManager::~GUIManager()
{
	
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
		std::cout << Elem->m_Name << "\n";
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
	SHORT m_MouseState = GetAsyncKeyState(VK_LBUTTON);
	if ((m_MouseState & 0x8000) && !(m_OldMouseState & 0x8000))
	{
		m_StartTime = Clock::now();
		m_MouseisDown = true;
		m_SendMouseDown = true;
		m_SendMouseUp = false;
		printf("left mouse down\n");
	}else if ((m_OldMouseState & 0x8000) && !(m_MouseState & 0x8000)){
		m_MouseisDown = false;
		m_SendMouseDown = false;
		m_SendMouseUp = true;
		printf("left mouse up\n");
	}
	m_OldMouseState = m_MouseState;

	m_EndTime = Clock::now();
	m_ElapsedTime = std::chrono::duration_cast<milliseconds>(m_EndTime - m_StartTime);
	if (m_ElapsedTime.count() >= 100 && (m_SendMouseDown || m_MouseisDown))
	{
		//only ignore event if elapsed time is over 100ms and send button flags
		//are set
		std::cout << "Mouse Event Ignored \n";
		m_SendMouseDown = false;
		m_MouseisDown = false;
	}

	//End time is when we compare the time the mouse has been down
	m_EndTime = Clock::now();
	m_ElapsedTime=std::chrono::duration_cast<milliseconds>(m_EndTime-m_StartTime);
	if (m_ElapsedTime.count() >= 100 &&(m_SendMouseDown || m_MouseisDown))
	{
		//only ignore event if elapsed time is over 100ms and send button flags
		//are set
		//std::cout << "Mouse Event Ignored \n";
		m_SendMouseDown = false;
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
	appinstance->DrawString(XMFLOAT2(50.0f, 50.0f), 1.0f, false, "%f %f",m_MousePosition.x, m_MousePosition.y);
}


#include "Button.h"
#include "Slider.h"
#include "Panel.h"
#include "CheckBox.h"
#include "Label.h"