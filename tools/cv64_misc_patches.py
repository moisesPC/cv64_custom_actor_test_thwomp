import sys
import os

ROM = sys.argv[1]

def writeBytesToROM(value, address, size, ROM_file):
	ROM_file.seek(address)
	ROM_file.write(value.to_bytes(size, 'big'))

def patch_assignActorListToTestGrid(ROM_file):
	test_grid_map_actor_ptr = 0x10D9EC
	custom_map_actor_list_addr = 0x06001E30		# TODO: Unhardcode this

	writeBytesToROM(custom_map_actor_list_addr, test_grid_map_actor_ptr, 4, ROM_file)

def patch_updateMapFileSizeInMapFileInfo(ROM_file):
	map_file_info_field_end_ptr = 0x104CC0
	map_file_size = os.stat("build/map/map.bin").st_size	# Get map's file size
	map_file_size |= 0x06000000

	writeBytesToROM(map_file_size, map_file_info_field_end_ptr, 4, ROM_file)

if __name__ == "__main__":
	with open(ROM, "r+b") as ROM_file:
		patch_assignActorListToTestGrid(ROM_file)
		patch_updateMapFileSizeInMapFileInfo(ROM_file)
