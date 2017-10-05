
find_package(GTK2 REQUIRED gtk)

if(GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIRS})
    add_executable(editor editor.c extra.c file.c find.c menu.c)
    target_link_libraries(editor dictionary ${GTK2_LIBRARIES})
    add_custom_command(TARGET editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/gtk-editor/menu.ui $<TARGET_FILE_DIR:editor>/menu.ui)
    set_target_properties(editor 
            PROPERTIES 
            COMPILE_DEFINITIONS 
            "G_DISABLE_DEPRECATED;GDK_DISABLE_DEPRECATED;GDK_PIXBUF_DISABLE_DEPRECATED;GTK_DISABLE_DEPRECATED;UI_DIR=\"$<TARGET_FILE_DIR:editor>\" "
            )
endif()

