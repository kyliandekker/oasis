#pragma once

#include <vector>
#include <functional>

namespace oasis
{
	template<typename... Args>
	class Event {
	public:
		void operator+=(const std::function<void(Args...)>& listener)
		{
			auto it = std::find_if(listeners.begin(), listeners.end(),
				[&](const std::function<void(Args...)>& other)
				{
					return listener.target_type() == other.target_type();
				});

			if (it != listeners.end())
			{
				return;
			}
			listeners.push_back(listener);
		}

		// Remove a listener (unsubscribe)
		void operator-=(const std::function<void(Args...)>& listener)
		{
			auto it = std::find_if(listeners.begin(), listeners.end(),
				[&](const std::function<void(Args...)>& other)
				{
					return listener.target_type() == other.target_type();
				});

			if (it != listeners.end())
			{
				listeners.erase(it);
			}
		}

		// Notify all listeners (invoke event)
		void operator()(Args... args) const
		{
			for (auto& listener : listeners)
			{
				listener(args...);
			}
		}

		void invoke(Args... args) const
		{
			for (auto& listener : listeners)
			{
				listener(args...);
			}
		}

		void clear()
		{
			listeners.clear();
		}

	private:
		std::vector<std::function<void(Args...)>> listeners;
	};
}