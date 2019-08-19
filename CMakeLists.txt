cmake_minimum_required(VERSION 3.13)
project(chk VERSION 1.0.0 LANGUAGES CXX)


add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

#set(include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include/chk")
set(include_dir "include/chk")

target_compile_features(${PROJECT_NAME}
	INTERFACE
		cxx_std_14  # FIXME: is it correct? can actually be >=14...
)
set(headers
	cassert.hpp
	cterminate.hpp
	unreachable.hpp
	detail/source_location.hpp
	detail/utils.hpp
)
list(TRANSFORM headers PREPEND "${include_dir}/")

target_sources(${PROJECT_NAME}
	INTERFACE
		$<BUILD_INTERFACE:${headers}>
)

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# FIXME: reuse headers added in target_sources, this flattens folder structure
#foreach(file ${headers})
#    get_filename_component(dir ${file} DIRECTORY)
#    install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${dir})
#endforeach()

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-target
)
install(EXPORT ${PROJECT_NAME}-target
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CTest REQUIRED)
add_subdirectory(tests)

# Packaging support, for deb there are still some warnings
set(CPACK_PACKAGE_VENDOR "Vendor name")
set(CPACK_PACKAGE_CONTACT "Federico Kircheis <federico.kircheis@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ library for compile-time tests and assertions")
set(CPACK_PACKAGE_DESCRIPTION "C++ library for compile-time tests and assertions")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/readme.adoc")
include(CPack)
