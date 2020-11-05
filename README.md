    git clone
    git submodule update --init --recursive


Can load matrix patch:

need root with sudo? Need to give some specific parameters?

No 32-64 bit incompatibility. Both are elf32-littlearm. Checked with

    file ...
    objdump -f /usr/bin/pd-l2ork


Start gdb

	sudo gdb /usr/bin/pd-l2ork
	add-symbol-file ltbl.pd_linux
	break ltbl_init

start pd

	sudo pd-l2ork ltbl-help.pd




or something

...

sudo gdb pd-l2ork
attach-symbol file ltbl.pd_linux
break ltbl_setup

run sudo pd-l2ork ltbl-help 


Try:

- https://stackoverflow.com/questions/36692315/what-exactly-does-rdynamic-do-and-when-exactly-is-it-needed
- simple liblib
