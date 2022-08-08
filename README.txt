COMPILE:
	on LINUX: make command
	on WINDOWS:
		gcc affinehill.c modulo.c -o affinehill
		gcc keyGen.c modulo.c -o keyGen
		gcc crack.c modulo.c -o crack

AFFINEHILL:
	Usage: ./affinehill [OPTION]... keyfile.txt SOURCE DECT
                -e		for encryption
                -d		for decryption
	example: ./affinehill -e keyfile.txt kern.log kern.log => output: kern.log.enc
		 ./affinehill -d keyfile.txt kern.log.enc kern.log

	<<!b vector is constant!>>

keyGen: (generate key)
	Usage: ./keyGen or ./keyGen -d (for more details)

crack: (based on known plaintext attack)
	Usage: ./crack plaintext(file) ciphertxt(file)

