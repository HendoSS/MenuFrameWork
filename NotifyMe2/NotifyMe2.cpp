// NotifyMe2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OverlayClasses.h"
#include "GUIFrameWork.h"
#include "CalendarClasses.h"
Calendar* calendar;

void D3DApp::DrawScene()
{
	if (m_Minimized)
		return;

	assert(m_pImmediateDeviceContext);
	assert(m_pSwapChain);

	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float blend[4] = { 0 };

	m_pImmediateDeviceContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&clearColor));

	//calendar->DrawHeader(XMFLOAT2(50.0f,80.0f));
	//calendar->DrawGrid(6,7);

	/*XMVECTOR pos1 = { m_width / 2, m_height / 2 + 1 };
	XMVECTOR pos2 = { m_width / 2, m_height / 2 - 1 };
	DrawLine(pos1, pos2, Colors::Red);

	DrawCircle(pos1, Colors::Green, 70, 30)*/;

	HR(m_pSwapChain->Present(0, 0));
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	
	DXOverlay* app=new D3DApp("Overlay", 600, 600, hInstance, "Untitled - Notepad", 4);
	calendar =new Calendar(app);
	GUIManager* GUIMngr = new GUIManager(app);
	app->MakeWindow();
	app->InitializeDX();
	return app->RunOverlay(true);
}



