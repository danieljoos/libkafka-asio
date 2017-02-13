
#
# A slightly modified version of the amazing function from the blog of
#   Matthieu Brucher:
#   http://blog.audio-tk.com/2015/09/01/sorting-source-files-and-projects-in-folders-with-cmake-and-visual-studioxcode/
#
function(source_group_by_folder target)
    set(SOURCE_GROUP_DELIMITER "/")
    set(last_dir "")
    set(files "")
    foreach(file ${${target}_SOURCE} ${${target}_HEADERS})
        file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})
        get_filename_component(dir "${relative_file}" PATH)
        if (NOT "${dir}" STREQUAL "${last_dir}")
            if (files)
                source_group("${last_dir}" FILES ${files})
            endif ()
            set(files "")
        endif ()
        set(files ${files} ${file})
        set(last_dir "${dir}")
    endforeach()

    if (files)
        source_group("${last_dir}" FILES ${files})
    endif ()
endfunction(source_group_by_folder)
