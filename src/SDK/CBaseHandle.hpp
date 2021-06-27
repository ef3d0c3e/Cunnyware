#ifndef SDK_CBASEHANDLE_HPP
#define SDK_CBASEHANDLE_HPP

class CBaseHandle
{
public:
	i32 index;

	[[nodiscard]] inline friend bool operator==(const CBaseHandle& h1, const CBaseHandle& h2) noexcept
	{
		return h1.index == h2.index;
	}
};

#endif // SDK_CBASEHANDLE_HPP
