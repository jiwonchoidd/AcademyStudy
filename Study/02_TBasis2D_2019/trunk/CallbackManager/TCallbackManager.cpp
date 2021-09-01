#include "TCallbackManager.h"

//std::map<Event::EventType, std::vector<Callback>> AppEventManager::listeners = std::map<Event::EventType, std::vector<Callback>>();
//
//// Store callback function for each event type
//void AppEventManager::AddListener(Event::EventType type, Callback c)
//{
//	std::map<Event::EventType, std::vector<Callback>>::iterator it = listeners.find(type);
//
//	if (it != listeners.end())
//	{
//		for (auto& callback : it->second)
//		{
//			// Check if callback exist
//			if (callback.target_type().hash_code() == c.target_type().hash_code())
//			{
//				return;
//			}
//		}
//	}
//
//	listeners[type].emplace_back(c);
//}

//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg)
//	{
//	case WM_CLOSE:
//		DestroyWindow(hwnd);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	case WM_SIZE:
//	{
//		WindowResizedEvent event(LOWORD(lParam), HIWORD(lParam)); < -- -
//			AppEventManager::TriggerEvent<WindowResizedEvent>(event); < -- -
//	}
//	break;
//	default:
//		return DefWindowProc(hwnd, msg, wParam, lParam);
//	}
//	return 0;
//}
//Window::Window()
//{
//	AppEventManager::AddListener(Event::EventType::WindowResize, GF_BIND_FN(Window::WindowResized));
//}
//
//void Window::WindowResized(Event& event)
//{
//	if (event.type == Event::EventType::WindowResize)
//	{
//		WindowResizedEvent e = reinterpret_cast<WindowResizedEvent&>(event);
//		windowWidth = e.width;
//		windowHeight = e.height;
//	}
//}

//Window::Window()
//{
//	AppEventManager::AddListener<WindowResizedEvent>(GF_BIND_FN(Window::WindowResized));
//}
//
//Window::Window()
//{
//	AppEventManager::AddListener<WindowResizedEvent>([&](WindowResizedEvent& event) { WindowResized(event); });
//}