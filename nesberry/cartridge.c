#include "cartridge.h"
#include "win_io.h"

#define INES_HEADER_MAGIC_NUMBER 0x4e45531a
#define INES_HEADER_SIZE 16
#define PRG_PAGE_SIZE (16 * 1024)

int cart_init(char* filename)
{
	for (uint32 i = 0; i < CART_MAX_ROM_SIZE; i++)
	{
		cartridge.data[i] = 0;
	}

	int size = io_load_rom(filename, cartridge.data);
	if (size < 0)
	{
		return CART_INIT_INVALID_FILE;
	}

	uint32 header_magic_number = ((cartridge.data[0] * 256 + cartridge.data[1]) * 256 + cartridge.data[2]) * 256 + cartridge.data[3];
	if (header_magic_number != INES_HEADER_MAGIC_NUMBER)
	{
		return CART_INIT_INVALID_ROM;
	}
	
	cartridge.data_size = size;
	cartridge.prg_num_pages = cartridge.data[4];
	cartridge.prg_start = INES_HEADER_SIZE;
	cartridge.chr_start = INES_HEADER_SIZE + cartridge.prg_num_pages * PRG_PAGE_SIZE;

	return CART_INIT_SUCCESS;
}

inline uint8 cart_get_prg(uint16 address)
{
	uint16 prg_address = address - 0x8000;
	if (cartridge.prg_num_pages == 1)
	{
		prg_address %= PRG_PAGE_SIZE;
	}
	return cartridge.data[cartridge.prg_start + prg_address];
}
