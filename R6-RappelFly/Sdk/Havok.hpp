#pragma once

/*++

	Siege uses Havok Physics, so maybe
	they also use havok structures within
	their internal structures? Whatever,
	this is merely a "Math.h".

--*/

namespace havok
{
	template<typename Ty>
	struct alignas(16) /* 16-byte aligned, siege functions get unhappy
						  if not */ Vector4
	{
		using ValueType = Ty;

		//
		// [[Members]]
		//

		ValueType
			x {},
			y {},
			z {},
			w {};
	};

	//
	// [[Type aliases]]
	//

	using Vector4f = Vector4<float>;
}