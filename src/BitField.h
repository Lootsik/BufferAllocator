#pragma once
#include <cassert>
#include <cstdint>
#include <cmath>

#include <type_traits>

template <typename Integer, typename Iteger2>
constexpr Integer round_greater(const Integer divisible, const Iteger2 divider)
{
	return divisible / divider + (divisible % divider == 0 ? 0 : 1);
}

//  storage type must be unsigned
template< size_t BitCount,typename StorType = uint32_t >
class BitField
{
public:
    	
	using storage_type = StorType;

	
	static_assert(std::is_unsigned<storage_type>::value,
					"Storage type is not unsigned intergal");


	// initialize all bits to 0 or 1
	BitField(bool bits_value = false)
	{

		storage_type init_value = 0;
		// thats how we get all bits to 1
		if (bits_value)
			--init_value;

		for (size_t i = 0; i < storage_size; ++i)
			storage[i] = init_value;
	}




	BitField(const BitField& ) = default;
	BitField(BitField&& ) = default;

	BitField& operator=(const BitField& ) = default;
	BitField& operator=(BitField&& ) = default;



	
	bool get_first_on(unsigned& first) const
	{
		for (unsigned i = 0; i < storage_size ; ++i)
		{
			// empty cell, no on bist
			if (storage[i] == 0)
				continue;

			unsigned seted_bit = get_first_on_bit(storage[i]);
			if (i + 1 == storage_size &&
				i * bits_in_cell + seted_bit >= bit_count)
				return false;

			first = seted_bit + i * bits_in_cell;
			return true;
		}
		return false;
	}
     




    bool get_bit(unsigned pos) const
	{
#ifdef _DEBUG
		assert(pos < BitCount);
#endif

		return is_setted_bit(storage[get_cell_pos(pos)], get_bit_os(pos));
	}






    void bit_on(unsigned pos)
	{
#ifdef _DEBUG
		assert(pos < BitCount);
#endif

		bit_on(storage[get_cell_pos(pos)], get_bit_os(pos));
	}



	
    void bit_off(unsigned pos)
	{
#ifdef _DEBUG
		assert(pos < BitCount);
#endif

		bit_off(storage[get_cell_pos(pos)], get_bit_os(pos));
	}
    

    
private:

	/*
				Bits operations 
	*/


	static void bit_on(storage_type& num, unsigned i)
	{
		num |= static_cast<storage_type>(1) << i;
	}
    

	
	static void bit_off(storage_type& num, unsigned i)
	{
		storage_type save_mask = ~((storage_type)1 << i);
		num &= save_mask;
	}



	static bool is_setted_bit(storage_type num, unsigned i)
	{
		return (num >> i) & 1;
	}
    



    static unsigned get_first_on_bit(storage_type num)
    {
        return static_cast<unsigned>(std::log2(num & (~num + 1)));
    }
    
    
	
	/*
			Helper function to find bit position
	 */



	size_t get_cell_pos(const size_t bit_pos) const
	{
		return bit_pos / bits_in_cell;
	}




	unsigned get_bit_os(const size_t bit_pos) const
	{
		return bit_pos % bits_in_cell;
	}





    static const unsigned bits_in_byte = 8;
    
	static const size_t bit_count = BitCount;

	static const size_t needed_bytes = round_greater(bit_count, bits_in_byte);
    
	static const size_t storage_size = round_greater(needed_bytes, sizeof(storage_type));
    
	static const unsigned bits_in_cell = bits_in_byte * sizeof(storage_type);
    



	storage_type storage[storage_size] = {};
};


