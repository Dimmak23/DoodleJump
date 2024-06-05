function(set_specific_definitions TARGET)
	if(CMAKE_GENERATOR MATCHES "Visual Studio .*")
		target_compile_definitions(
			${TARGET}
			PRIVATE _CRT_SECURE_NO_WARNINGS
			PRIVATE __STDC_WANT_SECURE_LIB__
		)
	endif()
endfunction()
