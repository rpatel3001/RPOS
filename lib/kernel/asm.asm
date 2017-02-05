section .text

global get_eax:function
get_eax:
	ret

global get_ebx:function
get_ebx:
	push ebx
	pop eax
	ret

global get_ecx:function
get_ecx:
	push ecx
	pop eax
	ret

global get_edx:function
get_edx:
	push edx
	pop eax
	ret

global get_esi:function
get_esi:
	push esi
	pop eax
	ret

global get_edi:function
get_edi:
	push edi
	pop eax
	ret

global get_ebp:function
get_ebp:
	push ebp
	pop eax
	ret

global get_esp:function
get_esp:
	push esp
	pop eax
	ret

global get_flags:function
get_flags:
	pushfd
	pop eax
	ret

global get_ss:function
get_ss:
	push ss
	pop eax
	ret

global get_cs:function
get_cs:
	push cs
	pop eax
	ret

global get_ds:function
get_ds:
	push ds
	pop eax
	ret

global get_es:function
get_es:
	push es
	pop eax
	ret

global get_fs:function
get_fs:
	push fs
	pop eax
	ret

global get_gs:function
get_gs:
	push gs
	pop eax
	ret

global read_port:function
read_port:
	mov edx, [esp + 4]
	in al, dx
	ret

global write_port:function
write_port:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret