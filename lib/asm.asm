section .text
bits 32

global get_eax:function
get_eax:
	ret

global get_ebx:function
get_ebx:
	mov eax, ebx
	ret

global get_ecx:function
get_ecx:
	mov eax, ecx
	ret

global get_edx:function
get_edx:
	mov eax, edx
	ret

global get_esi:function
get_esi:
	mov eax, esi
	ret

global get_edi:function
get_edi:
	mov eax, edi
	ret

global get_ebp:function
get_ebp:
	mov eax, ebp
	ret

global get_esp:function
get_esp:
	mov eax, esp
	ret

global get_flags:function
get_flags:
	pushfd
	pop eax
	ret

global get_ss:function
get_ss:
	mov eax, ss
	ret

global get_cs:function
get_cs:
	mov eax, cs
	ret

global get_ds:function
get_ds:
	mov eax, ds
	ret

global get_es:function
get_es:
	mov eax, es
	ret

global get_fs:function
get_fs:
	mov eax, fs
	ret

global get_gs:function
get_gs:
	mov eax, gs
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
