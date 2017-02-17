target remote localhost:1234
add-symbol-file build/rpos.bin 0xE0101000  
break kernel_main
