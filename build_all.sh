(
cd pc/ && make mbrtool && cd ../;
cmake ./ && make;
rm -rf CMakeFiles && rm cmake_install.cmake && rm CMakeCache.txt && rm Makefile;
rm -rf iuxulkernel_stubs && rm iuxulkernel_stubs.yml && rm compile_*;
echo "";
echo "DONE! [ IMCUNLOCK.vpk | MBRTOOL ]";
echo "";
)
