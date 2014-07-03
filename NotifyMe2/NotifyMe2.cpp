// NotifyMe2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Helpers.h"
#include "OverlayClasses.h"
#include "GUIFrameWork.h"
#include "CalendarClasses.h"
Calendar *calendar;
GUIManager *GUIMngr;

void D3DApp::DrawScene() {
  if (m_Minimized)
    return;

  assert(m_pImmediateDeviceContext);
  assert(m_pSwapChain);

  float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  float blend[4] = {0};

  m_pImmediateDeviceContext->ClearRenderTargetView(
      m_pRenderTargetView, reinterpret_cast<const float *>(&clearColor));

  GUIMngr->DrawElements();
  GUIMngr->UpdateMouse();

  calendar->DrawHeader(XMFLOAT2(50.0f, 80.0f));
  // calendar->DrawGrid(6,7);

  /* XMVECTOR pos1 = { m_width / 2, m_height / 2 + 1 };
 XMVECTOR pos2 = { m_width / 2, m_height / 2 - 1 };
 DrawLine(pos1, pos2, Colors::Red);

 DrawCircle(pos1, Colors::Green, 70, 30)*/

  HR(m_pSwapChain->Present(0, 0));
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine,
                       _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  
  Tools::CreateConsole();

  DXOverlay *app =
		new D3DApp("Overlay", 600, 600, hInstance, "Untitled - Notepad", 4);

  calendar = new Calendar(app);
  GUIMngr = new GUIManager(app);

  GUIMngr->AddElement(
		new Button("Test","Button 1", XMFLOAT2(150.0f, 150.f), XMFLOAT2(50.0f, 20.0f),
		[]()->void
		{
			std::cout << "Button Pressed \n";
		}));

  GUIMngr->AddElement(
		new Button("Test 2","Button 2", XMFLOAT2(500.0f, 200.0f), XMFLOAT2(50.0f,20.0f),
		[]()->void
		{
			std::cout << "Button 2 Pressed \n";
		}));

  GUIMngr->AddElement(
		new Slider<int>("Slider 1", XMFLOAT2(500.0f, 400.0f), XMFLOAT2(50.0f, 20.0f), 0, 5, Colors::Black,
		[]()->void
		{
			 std::cout << "Slider Released \n";
		}));

  Panel* pPanel = new Panel("Test Panel", "Panel 1", XMFLOAT2(600.0f,300.0f), XMFLOAT2(300.0f, 300.0f));
  pPanel->AddElement(
	 new Button("Test 3", "Button 3", XMFLOAT2(50.0f,50.0f), XMFLOAT2(50.0f, 20.0f),
	 []()->void
	 {
		std::cout << "Button 3 Pressed \n";
	 }));

  GUIMngr->AddElement(pPanel);


  //GUIMngr->GetElement<Slider<int>>("Slider 1");

  app->MakeWindow();
  app->InitializeDX();
  return app->RunOverlay(true);
}
