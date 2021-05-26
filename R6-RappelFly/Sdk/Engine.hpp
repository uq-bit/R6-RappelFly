// ReSharper disable CppClangTidyClangDiagnosticUnusedPrivateField
#pragma once

/*++

	Single-header this shit because I can't
	be assed to make 90 different files for
	each class lol.

--*/

#include "Havok.hpp"
#include "../Utilities.hpp"

// C++ Headers
#include <memory>

namespace engine
{

	/// All [engine] classes that have virtual functions
	/// will derive from this base, it just makes things
	/// simpler and I am sure that this kind of thing is
	/// implemented in the actual engine.
	struct BaseClass
	{
			/// Pointer to this class' virtual function
			/// table.
		void **vft {};

			/// Less writing later...
		template<typename ReturnTy = void, typename ...ArgumentsTy>
		__forceinline decltype(auto) callVirtual(const std::size_t index, ArgumentsTy ...arguments)
		{
			return static_cast<ReturnTy(*)(decltype(this), ArgumentsTy...)>(this->vft[index])(this, arguments...);
		}

		template<typename ReturnTy = void, typename ...ArgumentsTy>
		__forceinline decltype(auto) callVirtual(const std::size_t index, ArgumentsTy ...arguments) const
		{
			return static_cast<ReturnTy(*)(decltype(this), ArgumentsTy...)>(this->vft[index])(this, arguments...);
		}
	};

	//
	// Forward declarations of our engine classes.
	//
	
	struct Actor: BaseClass
	{
		[[nodiscard]] havok::Vector4f origin() const
		{
			return *reinterpret_cast<havok::Vector4f*>(reinterpret_cast<std::uintptr_t>(this) + 0x50);
		}
	};
	struct Component: BaseClass
	{
		[[nodiscard]] Actor *actor() const
		{
			return *reinterpret_cast<Actor**>(reinterpret_cast<std::uintptr_t>(this) + 0x18);
		}
	};

	//
	// [[Components]]
	//

	struct RappelComponent: Component
	{
		
	};
	struct PawnComponent: Component
	{
		struct Interactive: BaseClass
		{
			struct Rappeller: BaseClass
			{
				void rappel(RappelComponent *rappelComponent, const havok::Vector4f *position, const std::uint32_t type)
				{
					this->callVirtual(0x1, rappelComponent, position, type);
				}
			};

			[[nodiscard]] Rappeller *getRappeller() const
			{
				return this->callVirtual<Rappeller*>(0x1B, 4u);
			}
		};
		struct InteractiveProperties
		{
			void setRappelVelocity(const float newVelocity)
			{
				*reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(this) + 0x218) = newVelocity;
			}
			void setRappelPosition(const havok::Vector4f &newPosition)
			{
				*reinterpret_cast<havok::Vector4f*>(reinterpret_cast<std::uintptr_t>(this) + 0x1F0) = newPosition;
			}
		};

		[[nodiscard]] Interactive *interactive() const
		{
			return reinterpret_cast<Interactive*>(
				*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x130) ^ 0x75AFCF43FFD1AEE2
				);
		}
		[[nodiscard]] InteractiveProperties *interactiveProperties() const
		{
			return reinterpret_cast<InteractiveProperties*>(
				_rotr64((*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(this) + 0xCB0) ^ 0x5B76D0EEBADA170C) + 0xC9362EACF202E0D, 0x33)
				);
		}
	};

	//
	// [[I DON'T KNOW]]
	//

	struct Controller: BaseClass
	{
	};
	struct PlayerController: Controller
	{
		[[nodiscard]] PawnComponent *getPawnComponent() const
		{
			return this->callVirtual<PawnComponent*>(0x25);
		}
	};

	//
	// [[Managers]]
	//

	struct GameManager: BaseClass
	{
		[[nodiscard]] static GameManager *global()
		{
			return reinterpret_cast<GameManager*>(
				(_rotl64(*reinterpret_cast<std::uintptr_t*>(utilities::imageBase() + 0x7102990), 0x24) - 0x6F) ^ 0x69C7993AD4EEF7BA
				);
		}
		[[nodiscard]] PlayerController *getLocalController() const
		{
			return this->callVirtual<PlayerController*>(0x28);
		}
	};
	struct RoundManager
	{
		[[nodiscard]] static RoundManager *global()
		{
			return reinterpret_cast<RoundManager*>(
				_rotl64(*reinterpret_cast<std::uintptr_t*>(utilities::imageBase() + 0x72E0E30) + 0x30DD5317745959C8, 0x32) - 0x13
				);
		}

		[[nodiscard]] std::uint32_t roundState() const noexcept
		{
			return ((_rotl(*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x90), 0x14) - 0x5D) ^ 0x4D0F07BE);
		}
		[[nodiscard]] bool isInGame() const noexcept
		{
			return this->roundState() == 2 || this->roundState() == 3;
		}
	};
}
