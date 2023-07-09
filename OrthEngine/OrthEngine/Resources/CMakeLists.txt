# Get a list of all files in the resources directory
file(GLOB RESOURCE_FILES "${CMAKE_SOURCE_DIR}/OrthEngine/Resources/*")

# Loop through the list of files and copy each file to the binary directory
foreach(file ${RESOURCE_FILES})
  get_filename_component(filename ${file} NAME)
  configure_file(${file} ${CMAKE_BINARY_DIR}/bin COPYONLY)
endforeach()
