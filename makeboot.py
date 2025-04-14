#! /usr/bin/python3

# makeboot

import sys

def usage():
	print(f"Makeboot - usage: {sys.argv[0]} [-ibm|-atari|-amiga|-msx] <image file name>")
	exit()

def ibm_makeboot(filename):
	with open(filename, 'r+b') as fp:
		fp.seek(0x1FE)
		fp.write(b'\x55\xAA')

def atari_makeboot(filename):
	with open(filename, "r+b") as fp:
		checksum = 0x1234
		for i in range(510):
			if (i & 1) == 0:
				checksum -= (fp.read(1)[0] & 0xFF) << 8
			else:
				checksum -= fp.read(1)[0] & 0xFF
			checksum &= 0xFFFF
		fp.write(bytes([(checksum >> 8) & 0xFF, checksum & 0xFF]))

def msx_makeboot(filename):
	with open(filename, "r+b") as fp:
		fp.write(b'\xEB\xFE\x90')
		# entry at offset 0x1E

def amiga_makeboot(filename, block_size = 1024):
	with open(filename, "r+b") as fp:
		fp.write(b"DOS\0\0\0\0\0\0\0\x03\x70")
		fp.seek(0)
		checksum = 0
		for i in range(block_size):
			if (i & 3) == 0:
				checksum += (fp.read(1)[0] & 0xFF) << 24
			elif (i & 3) == 1:
				checksum += (fp.read(1)[0] & 0xFF) << 16
			elif (i & 3) == 2:
				checksum += (fp.read(1)[0] & 0xFF) << 8
			elif (i & 3) == 3:
				checksum += fp.read(1)[0] & 0xFF
				if checksum > 0xFFFFFFFF:
					checksum += 1
				checksum &= 0xFFFFFFFF
		fp.seek(4)
		checksum = ~checksum
		fp.write(bytes([(checksum >> 24) & 0xFF, (checksum >> 16) & 0xFF, (checksum >> 8) & 0xFF, checksum & 0xFF]))
		# entry at offset 0x0C

def main():
	if len(sys.argv) <= 1:
		usage()
		exit()
	if sys.argv[1] == '-ibm':
		if len(sys.argv) <= 2:
			usage()
			exit()
		ibm_makeboot(sys.argv[2])
	elif sys.argv[1] == '-atari':
		if len(sys.argv) <= 2:
			usage()
			exit()
		atari_makeboot(sys.argv[2])
	elif sys.argv[1] == '-msx':
		if len(sys.argv) <= 2:
			usage()
			exit()
		msx_makeboot(sys.argv[2])
	elif sys.argv[1] == '-amiga':
		if len(sys.argv) <= 2:
			usage()
			exit()
		amiga_makeboot(sys.argv[2])
	else:
		ibm_makeboot(sys.argv[1])

if __name__ == '__main__':
	main()

