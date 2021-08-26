# gpu_practice
this repo help you to familiar with some gpu runtimes and gpu kernel code.

Add host code and kernel in repo.

[1] Add your kernel in "kernel" folder, only accept "CM kernel", and "OCL kernel". if you want to change the compile
    flag, please modify it in the "kernel/CMakeLists.txt"
    
[2] Add host code in "host_code" folder, create new host cpp file, you can copy from "host_test.cpp", then just modify 
    "#define CLASSNAME your_case_name", rewrite "class::get_test_name()", this name will be used as program 
    input parameter, which choice test workload to run. finally rewrite "class::run()" function. 
    
[3] offline compile kernel, the name of output bin file should be (kernel_name)_L0.bin or (kernel_name)_OCL.bin,
    e.g kernel file --> empty.cpp , then bin file --> empty_L0.bin , 
        kernel file --> empty.cl ,  then bin file --> empty_OCL.bin.     