#pragma once
#include <cstdio>
#include <functional>
#include <map>
#include <vector>
#include <memory>

#define GF_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)

struct Event
{
public:
	enum EventType
	{
		AppQuit,
		AppBackground,

		WindowResize,
		WindowGainFocus,
		WindowLostFocus

	};

	EventType type = EventType::AppQuit;

	Event(EventType type) : type(type) {}
};

struct WindowResizedEvent : public Event
{
public:
	int width = 0;
	int height = 0;

	WindowResizedEvent(int width, int height) : width(width), height(height), Event(Event::EventType::WindowResize) {}
};

typedef std::function<void(Event&)> Callback;


//class AppEventManager
//{
//public:
//
//	static void AddListener(Event::EventType type, Callback c);
//
//	template <typename T>
//	static void TriggerEvent(Event& event);
//
//private:
//
//	static std::map<Event::EventType, std::vector<Callback>> listeners;
//};
//
//template<typename T>
//inline void AppEventManager::TriggerEvent(Event& event)
//{
//	std::map<Event::EventType, std::vector<Callback>>::iterator it = listeners.find(event.type);
//
//	if (it != listeners.end())
//	{
//		for (auto& callback : it->second)
//		{
//			callback(static_cast<T&>(event));
//		}
//	}
//}

class AppEventManager
{
public:

	template <typename T>
	static void AddListener(std::function<void(T&)> callback) 
	{
		get_listeners<T>().push_back(std::move(callback));
	}

	template <typename T>
	static void TriggerEvent(T& event) 
	{
		for (auto& listener : get_listeners<T>()) 
		{
			listener(event);
		}
	}

private:

	template <typename T>
	static std::vector<std::function<void(T&)>>& get_listeners()
	{
		static std::vector<std::function<void(T&)>> listeners;
		return listeners;
	}
};


