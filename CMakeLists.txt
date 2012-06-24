cmake_minimum_required(VERSION 2.6)
project(message_board)

include_directories(${CMAKE_SOURCE_DIR})

# Options 

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
	      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

option(STATIC_VIEW "Build Static View instead of dynamically loaded one" OFF)

if(WIN32 OR CYGWIN)
        add_definitions(-DDLL_EXPORT)
endif()


# Dependencies

# For markdown
find_package(Boost 1.45.0 COMPONENTS regex)
find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})

find_library(LIB_BOOSTER booster)
find_library(LIB_CPPCMS cppcms)
find_library(LIB_CPPDB cppdb)
find_program(EXE_TMPL_CC cppcms_tmpl_cc)
find_program(EXE_MSGFMT msgfmt)
find_program(EXE_SQLITE3 sqlite3)

find_path(INC_BOOSTER booster/config.h)
find_path(INC_CPPCMS cppcms/config.h)
find_path(INC_CPPDB cppdb/frontend.h)

# Sources
                                                                                                                                                                                                                 
set(VIEW_NAME simple)

set(VIEW_TEMPLATES 
        view/master.tmpl
        view/forums.tmpl 
        view/thread.tmpl)

set(MB_SRC
		markdown/markdown.cpp
		markdown/markdown-tokens.cpp
        apps/master.cpp
        apps/thread.cpp
        apps/forums.cpp
        apps/mb.cpp
        apps/main.cpp)

set(MB_LANG ru)

# Actual Build

include_directories(${INC_BOOSTER})
include_directories(${INC_CPPCMS})
include_directories(${INC_CPPDB})

set(VIEW_SRC "${CMAKE_CURRENT_BINARY_DIR}/${VIEW_NAME}.cpp")

add_custom_command(
        OUTPUT ${VIEW_SRC}
        COMMAND ${EXE_TMPL_CC}
                -s "${VIEW_NAME}"
                ${TMPL_CC_PARAMS}
                -o ${VIEW_SRC}
                ${VIEW_TEMPLATES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${VIEW_TEMPLATES})


if(STATIC_VIEW)
        set(MB_SRC ${MB_SRC} ${VIEW_SRC})
else()
        add_library(${VIEW_NAME} SHARED ${VIEW_SRC})
        target_link_libraries(${VIEW_NAME} ${LIB_CPPCMS})
        target_link_libraries(${VIEW_NAME} ${LIB_BOOSTER})
endif()


add_executable(mb ${MB_SRC})
target_link_libraries(mb ${Boost_LIBRARIES})
target_link_libraries(mb ${LIB_CPPCMS})
target_link_libraries(mb ${LIB_BOOSTER})
target_link_libraries(mb ${LIB_CPPDB})
target_link_libraries(mb ${ImageMagick_LIBRARIES})

if(EXE_MSGFMT)
        set(MO_FILES)
        foreach(LANG ${MB_LANG}) 
                set(PO locale/${LANG}/LC_MESSAGES/mb.po)
                set(MO ${CMAKE_CURRENT_BINARY_DIR}/locale/${LANG}/LC_MESSAGES/mb.mo)
                file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/locale/${LANG}/LC_MESSAGES")
                set(MO_FILES ${MO_FILES} ${MO})
                add_custom_command(
                        OUTPUT ${MO}
                        COMMAND ${EXE_MSGFMT} ${PO} -o ${MO}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        DEPENDS ${PO})
        endforeach()
        add_custom_target(create-mo ALL DEPENDS ${MO_FILES})
else()
        message("-- msgfmt not found, translations not generatied")
endif()

install(TARGETS mb simple
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib/mb
	ARCHIVE DESTINATION lib/mb)

foreach(IMAGES css csv deb flv gif gz html jpeg jpg js mkv mp3 mp4 ogg pdf png qt rar svg tar txt wav webm wmv xml zip) 
	install(FILES media/images/${IMAGES}.png DESTINATION share/mb/media/images)
endforeach()

foreach(STYLE script.js style.css style-ltr.css style-rtl.css) 
	install(FILES media/${STYLE} DESTINATION share/mb/media)
endforeach()

foreach(SQL sqlite3 mysql) 
	install(FILES model/${SQL}.sql DESTINATION share/mb/sql)
endforeach()

foreach(LOCALE ${MB_LANG})
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/locale/${LOCALE}/LC_MESSAGES/mb.mo 
		DESTINATION share/locale/${LOCALE}/LC_MESSAGES/)
endforeach()

install(FILES config.js.sample DESTINATION share/mb)

if(EXE_SQLITE3)
        add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mb.db
                COMMAND ${EXE_SQLITE3} < ${CMAKE_CURRENT_SOURCE_DIR}/model/sqlite3.sql ${CMAKE_CURRENT_BINARY_DIR}/mb.db)
        add_custom_target(sqlitedb ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mb.db)
endif()
