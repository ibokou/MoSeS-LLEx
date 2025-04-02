idf_component_register(SRC_DIRS ".")
string(REPLACE "-std=gnu++11" "-std=gnu++23" CXX_OPTIONS
               "${CXX_COMPILE_OPTIONS}")
idf_build_set_property(CXX_COMPILE_OPTIONS "${CXX_OPTIONS}" REPLACE)
component_compile_options(-fexceptions)
