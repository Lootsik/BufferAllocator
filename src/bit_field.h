#pragma once
#include <cassert>
#include <cstdint>
#include <cmath>
#include <limits>
#include <type_traits>

template <typename Integer, typename Iteger2>
constexpr Integer div_round_greater(const Integer divisible, const Iteger2 divider)
{
	return divisible / divider + (divisible % divider == 0 ? 0 : 1);
}

//  storage type must be unsigned
template< size_t BitCount,typename StorType = uint32_t >
class bit_field
{
public:
    	
	using storage_type = StorType;

	
	static_assert(std::is_unsigned<storage_type>::value,
					"Storage type is not unsigned intergal");


	// initialize all bits to 0 or 1
	bit_field(bool bits_value = false)
	{

		storage_type init_value = 0;
		// thats how we get all bits to 1
		if (bits_value)
			init_value = std::numeric_limits<storage_type>::max();

		for (size_t i = 0; i < storage_size; ++i)
			storage[i] = init_value;
	}




	bit_field(const bit_field& ) = default;
	bit_field(bit_field&& ) = default;

	bit_field& operator=(const bit_field& ) = default;
	bit_field& operator=(bit_field&& ) = default;




	bool get_first_on(unsigned& first) const
	{
		if( get_on_from( find_pos, first))
			return true;

		if( find_pos + 1 < storage_size) 
		{
			if( get_on_from( find_pos + 1, first) ) 
			{
				++find_pos;
				return true;
			}
		}
		return get_on_full_lookup( first );
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
		auto cell_pos = get_cell_pos(pos);
		auto bit_pos = get_bit_os(pos);
		bit_on(storage[ cell_pos], bit_pos);

		if( storage[find_pos] == 0 )
			find_pos = cell_pos;
	}



	
    void bit_off(unsigned pos)
	{
#ifdef _DEBUG
		assert(pos < BitCount);
#endif
		auto cell_pos = get_cell_pos(pos);
		auto bit_pos = get_bit_os(pos);
		bit_off(storage[cell_pos], bit_pos);
	}
    

    
private:


	bool get_on_full_lookup(unsigned& on_bit) const
	{
		for (unsigned i = 0; i < storage_size; ++i)
		{
			if (get_on_from(i, on_bit))
			{
				find_pos = i;
				return true;
			}
		}
		return false;
	}

	bool get_on_from(unsigned index, unsigned& on_bit) const
	{
		if (storage[index] == 0)
			return false;

		unsigned seted_bit = get_first_on_bit(storage[index]);
		if (index + 1 == storage_size &&
			index * bits_in_cell + seted_bit >= bit_count)
			return false;


		on_bit = seted_bit + index * bits_in_cell;
		return true;
	}




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

    
	static const size_t bit_count = BitCount;

	static const size_t need_bytes = div_round_greater(bit_count, CHAR_BIT);
    
	static const size_t storage_size = div_round_greater(need_bytes, sizeof(storage_type));
    
	static const unsigned bits_in_cell = CHAR_BIT * sizeof(storage_type);
    

	storage_type storage[storage_size] = {};
	mutable size_t find_pos = 0;
};

