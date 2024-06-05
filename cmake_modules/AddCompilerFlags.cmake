# ? Different configuration need different flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set(CONFIG_FLAG_g "-g")
		set(CONFIG_FLAG_O "-O0")
		set(CONFIG_FLAG_A "-Wall")
		set(CONFIG_FLAG_Ex "-Wextra")
		set(CONFIG_FLAG_Er "-Werror")
		set(CONFIG_FLAG_p "-Wpedantic")
		set(CONFIG_FLAG_c "-Wconversion")
	else()
		set(CONFIG_FLAG_g "")
		set(CONFIG_FLAG_O "-O3")
		set(CONFIG_FLAG_A "")
		set(CONFIG_FLAG_Ex "")
		set(CONFIG_FLAG_Er "")
		set(CONFIG_FLAG_p "")
		set(CONFIG_FLAG_c "")
	endif()
endif()

# TODO: need same for MSVC

# ? Set specific compiler flags
function(set_compiler_flags TARGET)
	if(CMAKE_GENERATOR MATCHES "Visual Studio .*")
		# ? /O2 and /RTC1 flags can't be combined
		# ? so this is a workaround to remove /RTC1 from resulted command line
		STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
		target_compile_options(
			${TARGET}
			PRIVATE
			/W4
			/Wall
			/O2
			/GA # Optimizes for Windows applications
			/clr
			/LD

			# /clr:safe
			# /O1 # Creates small code.
		)
	else()
		target_compile_options(
			${TARGET}
			PRIVATE
			${CONFIG_FLAG_g}
			${CONFIG_FLAG_O}
			${CONFIG_FLAG_A}
			${CONFIG_FLAG_Ex}
			${CONFIG_FLAG_Er}
			${CONFIG_FLAG_p}
			${CONFIG_FLAG_c}
		)
	endif()
endfunction()

# ? Debugging
# -g: This flag tells the compiler to generate debugging information in the executable file. This information includes symbols, which are used by debuggers to map machine instructions back to the source code.
# -O0: This flag disables optimization. Optimization can rearrange and simplify code, making it harder to debug. Setting the optimization level to 0 ensures that the generated code closely matches the source code.
# -Wall: This flag enables most warning messages. It's a good practice to address warnings as they can indicate potential issues in your code.
# -Wextra: This flag enables additional warning messages not covered by -Wall.
# -Werror: This flag treats all warning messages as errors. It's useful for enforcing strict coding standards and preventing potentially problematic code from compiling.

# ? Release
# -O2 or -O3: These flags enable different levels of optimization. -O2 generally includes a moderate level of optimization, while -O3 enables more aggressive optimizations. You may choose -O3 for maximum performance, but it might increase compilation time and could potentially introduce subtle bugs.
# -DNDEBUG: This flag defines the NDEBUG macro, which disables assertions in your code. Assertions are typically used for debugging purposes and are not necessary in release builds.
# -flto: This flag enables link-time optimization, which allows the compiler to perform optimizations across different source files during the linking phase. This can result in additional performance improvements.
# -march=native: This flag allows the compiler to generate code optimized for the CPU architecture on which the compilation is being performed. It can result in better performance but may reduce portability.
# -s: This flag tells the linker to strip symbols and debug information from the executable, reducing its size.
