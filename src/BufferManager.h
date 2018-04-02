#pragma once
#include "BitField.h"


template< size_t BuffSize, size_t NumBuff >
class BufferManager
{
public:
	BufferManager()
	{
		AllocSpace = (uint8_t*) malloc( sizeof(uint8_t) * alloc_size);
		if (!AllocSpace)
			throw std::bad_alloc{};
	}


	~BufferManager()
	{
		free(AllocSpace);
	}


	BufferManager(const BufferManager& right) = delete;
	
	BufferManager(BufferManager&& right)
	{
		MoveFrom(std::move(right));
	}



	BufferManager& operator=(const BufferManager& right) = delete;

	BufferManager& operator=(BufferManager&& right)
	{
		MoveFrom(std::move(right));
		return *this;
	}







	uint8_t * GetBuffer()
	{
		unsigned bit_on;

		// no space left
		if (!AllocMap.get_first_on(bit_on))
			throw std::bad_alloc{};
		
#ifdef _DEBUG
		if ( AllocMap.get_bit(bit_on) == false)
			throw bit_on;
#endif

		AllocMap.bit_off(bit_on);

		return AllocSpace + buffer_size * bit_on;
	}





	void ReleaseBuffer(uint8_t * Buffer)
	{
		unsigned Bit = static_cast<unsigned>((Buffer - AllocSpace) / buffer_size);

#ifdef _DEBUG
		if (Buffer < AllocSpace)
			throw Buffer;
		if (AllocMap.get_bit(Bit) == true)
			throw Bit;
#endif

		AllocMap.bit_on(Bit);
	}




	static const size_t buffer_size = BuffSize;
	static const size_t buffers = NumBuff;

	static const size_t alloc_size = buffer_size * buffers;

private:


	void MoveFrom(BufferManager&& right)
	{
		if (AllocSpace)
			free(AllocSpace);

		AllocSpace = right.AllocSpace;
		AllocMap = std::move(right.AllocMap);

		right.AllocSpace = nullptr;
		right.AllocMap = -1;
	}



	uint8_t* AllocSpace = nullptr;

	// if bit setted, block is free
	BitField<buffers,uint64_t> AllocMap{ true };
};


