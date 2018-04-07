#pragma once
#include "bit_field.h"


template< size_t BuffSize, size_t NumBuff >
class buffer_manager
{
public:
	buffer_manager()
	{
		alloc_space = (uint8_t*) malloc( sizeof(uint8_t) * alloc_size);
		if (!alloc_space)
			throw std::bad_alloc{};
	}


	~buffer_manager()
	{
		free(alloc_space);
	}


	buffer_manager(const buffer_manager& right) = delete;
	
	buffer_manager(buffer_manager&& right)
	{
		move_from(std::move(right));
	}



	buffer_manager& operator=(const buffer_manager& right) = delete;

	buffer_manager& operator=(buffer_manager&& right)
	{
		move_from(std::move(right));
		return *this;
	}







	uint8_t * get_buffer()
	{
		unsigned bit_on;

		// no space left
		if (!alloc_map.get_first_on(bit_on))
			throw std::bad_alloc{};
		
#ifdef _DEBUG
		if ( alloc_map.get_bit(bit_on) == false)
			throw bit_on;
#endif

		alloc_map.bit_off(bit_on);

		return alloc_space + buffer_size * bit_on;
	}





	void release_buffer(uint8_t * Buffer)
	{
		unsigned Bit = static_cast<unsigned>((Buffer - alloc_space) / buffer_size);

#ifdef _DEBUG
		if (Buffer < alloc_space)
			throw Buffer;
		if (alloc_map.get_bit(Bit) == true)
			throw Bit;
#endif

		alloc_map.bit_on(Bit);
	}




	static const size_t buffer_size = BuffSize;
	static const size_t buffers = NumBuff;

	static const size_t alloc_size = buffer_size * buffers;

private:


	void move_from(buffer_manager&& right)
	{
		if (alloc_space)
			free(alloc_space);

		alloc_space = right.alloc_space;
		alloc_map = std::move(right.alloc_map);

		right.alloc_space = nullptr;
		right.alloc_map = -1;
	}



	uint8_t* alloc_space = nullptr;

	// if bit setted, block is free
	bit_field<buffers,uint64_t> alloc_map{ true };
};


