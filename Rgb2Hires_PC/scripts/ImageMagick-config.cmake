set(ImageMagick_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/ImageMagick ${CMAKE_CURRENT_LIST_DIR}/ImageMagick/Magick++/lib)

set(ImageMagickLibDir ${CMAKE_CURRENT_LIST_DIR}/VisualMagick/lib)
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
	set(ImageMagick_LIBRARIES ${ImageMagickLibDir}/CORE_DB_Magick++_.lib ${ImageMagickLibDir}/CORE_DB_Magick_.lib ${ImageMagickLibDir}/CORE_DB_wand_.lib)
else()
	set(ImageMagick_LIBRARIES ${ImageMagickLibDir}/CORE_RL_Magick++_.lib ${ImageMagickLibDir}/CORE_RL_Magick_.lib ${ImageMagickLibDir}/CORE_RL_wand_.lib)
endif()

