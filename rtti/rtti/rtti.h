#pragma once

#include <cstdint>
#include <array>


#if !defined(RTTI_CHAIN_MAX_SIZE)
#define RTTI_CHAIN_MAX_SIZE 32
#endif


namespace rtti
{
	class rtti {
	protected:
		std::array<std::intptr_t, RTTI_CHAIN_MAX_SIZE> inheritChainID;
		std::array<void*, RTTI_CHAIN_MAX_SIZE> inheritChainPtr;
		int inheritChainCounter;
		rtti() : inheritChainCounter(-1)
		{
			for (auto& id : inheritChainID) id = -1;
			for (auto& ptr : inheritChainPtr) ptr = 0;
		}
	public:
		virtual ~rtti() {}
		inline void* getPtrKindOf(std::intptr_t type)
		{
			for (int i = 0; i <= inheritChainCounter; i++)
				if (inheritChainID[i] == type)
					return inheritChainPtr[i];
			return nullptr;
		}
		inline bool isKindOf(std::intptr_t type)
		{
			return getPtrKindOf(type) == nullptr ? false : true;
		}
	};

	template<typename T>
	class rtti_port : public virtual rtti {
	private:
		static std::intptr_t id;
	public:
		static std::intptr_t type();
	protected:
		rtti_port()
		{
			inheritChainCounter++;
			inheritChainID[inheritChainCounter] = type();
			inheritChainPtr[inheritChainCounter] = static_cast<T*>(this);
		}
		virtual ~rtti_port() {}
	};
	template<typename T>
	std::intptr_t rtti_port<T>::id(0);
	template<typename T>
	std::intptr_t rtti_port<T>::type()
	{
		return reinterpret_cast<std::intptr_t>(&id);
	}

	template<typename T>
	static bool is(rtti* obj)
	{
		if (obj == nullptr)
			return false;
		return obj->getPtrKindOf(rtti_port<T>::type()) != nullptr;
	}
	template<typename T>
	static T* cast(rtti* to)
	{
		if (to == nullptr)
			return nullptr;
		return static_cast<T*>(to->getPtrKindOf(rtti_port<T>::type()));
	}
}