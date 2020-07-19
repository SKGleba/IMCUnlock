cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(imcunlock)
set(TITLE_ID "SKGIMCULK")
set(TITLE_NAME "IMCUnlock")

include("${VITASDK}/share/vita.cmake" REQUIRED)

enable_language(C ASM)

set(CMAKE_C_FLAGS "-Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_BINARY_DIR}/iuxulkernel_stubs
)

add_executable(compile_app.elf
	app/main.c
	app/graphics.c
	app/font.c
)

target_link_libraries(compile_app.elf
	taihen_stub
	SceDisplay_stub
	SceCtrl_stub
	iuxulkernel_stub
	SceAppMgr_stub
	ScePower_stub 
	SceVshBridge_stub
)

add_custom_target(compile_app.velf ALL
	COMMAND vita-elf-create compile_app.elf compile_app.velf
)

add_custom_target(compile_eboot.bin ALL
	COMMAND vita-make-fself -c compile_app.velf compile_eboot.bin
)

add_custom_target(${PROJECT_NAME}.vpk ALL
	COMMAND vita-mksfoex -s TITLE_ID=${TITLE_ID} "${TITLE_NAME}" compile_param.sfo
	COMMAND vita-pack-vpk -s compile_param.sfo -b compile_eboot.bin -a compile_imcunlock.skprx=imcunlock.skprx
	-a res/icon0.png=sce_sys/icon0.png ${PROJECT_NAME}.vpk
	-a res/template.xml=sce_sys/livearea/contents/template.xml
	-a res/bg.png=sce_sys/livearea/contents/bg.png ${PROJECT_NAME}.vpk
)

add_dependencies(compile_app.velf compile_app.elf)
add_dependencies(compile_eboot.bin compile_app.velf)
add_dependencies(${PROJECT_NAME}.vpk compile_eboot.bin)

vita_create_stubs(iuxulkernel_stubs compile_imcunlock.elf ${CMAKE_SOURCE_DIR}/plugin/imcunlock.yml KERNEL)

add_executable(compile_imcunlock.elf
	plugin/imcunlock.c
)

target_link_libraries(compile_imcunlock.elf
	SceSblAIMgrForDriver_stub
	SceSblSmCommForKernel_stub
	SceThreadmgrForDriver_stub
	SceSysconForDriver_stub
	SceIofilemgrForDriver_stub
	SceSdifForDriver_stub
	SceSysclibForDriver_stub
	SceCpuForDriver_stub
	taihenModuleUtils_stub
	taihenForKernel_stub
	ScePervasiveForDriver_stub
	SceSysmemForDriver_stub
	SceSblSsMgrForKernel_stub
	SceSysrootForDriver_stub
	SceDisplayForDriver_stub
	SceKernelUtilsForDriver_stub
	ScePowerForDriver_stub
	SceSysrootForKernel_stub
)

set_target_properties(compile_imcunlock.elf
	PROPERTIES LINK_FLAGS "-nostdlib"
	COMPILE_FLAGS "-D__VITA_KERNEL__"
)

add_custom_target(compile_imcunlock.skprx ALL
	COMMAND vita-elf-create -e ${CMAKE_SOURCE_DIR}/plugin/imcunlock.yml compile_imcunlock.elf compile_imcunlock.velf
	COMMAND vita-make-fself -c compile_imcunlock.velf compile_imcunlock.skprx
)
add_dependencies(compile_imcunlock.skprx compile_imcunlock.elf)
