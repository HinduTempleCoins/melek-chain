file(GLOB HEADERS "include/steem/plugins/evm/*.hpp")

add_library( evm_plugin
             evm_plugin.cpp
             ${HEADERS}
           )

target_link_libraries( evm_plugin chain_plugin )
target_include_directories( evm_plugin
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if( CLANG_TIDY_EXE )
   set_target_properties(
      evm_plugin PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

install( TARGETS
   evm_plugin

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
