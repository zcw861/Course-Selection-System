cmake_minimum_required(VERSION 4.1.1)

#https://github.com/Kitware/CMake/blob/v4.1.1/Help/dev/experimental.rst

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(courseSelection LANGUAGES CXX)

set(PQXX_LIBRARY "/usr/lib/libpqxx.so")
set(PQ_LIBRARY "/usr/lib/libpq.so")
set(PQXX_INCLUDE_DIR "/usr/include")
set(PQ_INCLUDE_DIR "/usr/include/postgresql")

# 验证文件是否存在
if(NOT EXISTS ${PQXX_LIBRARY})
    message(WARNING "libpqxx library not found at: ${PQXX_LIBRARY}")
    # 尝试自动查找
    find_library(PQXX_LIBRARY NAMES pqxx libpqxx)
endif()

if(NOT EXISTS ${PQ_LIBRARY})
    message(WARNING "libpq library not found at: ${PQ_LIBRARY}")
    # 尝试自动查找
    find_library(PQ_LIBRARY NAMES pq libpq)
endif()

if(NOT EXISTS "${PQXX_INCLUDE_DIR}/pqxx/pqxx")
    message(WARNING "pqxx headers not found at: ${PQXX_INCLUDE_DIR}")
    # 尝试自动查找
    find_path(PQXX_INCLUDE_DIR NAMES pqxx/pqxx)
endif()

if(NOT EXISTS "${PQ_INCLUDE_DIR}/libpq-fe.h")
    message(WARNING "libpq headers not found at: ${PQ_INCLUDE_DIR}")
    # 尝试自动查找
    find_path(PQ_INCLUDE_DIR NAMES libpq-fe.h)
endif()

message(STATUS "========================================")
message(STATUS "PQXX_LIBRARY: ${PQXX_LIBRARY}")
message(STATUS "PQXX_INCLUDE_DIR: ${PQXX_INCLUDE_DIR}")
message(STATUS "PQ_LIBRARY: ${PQ_LIBRARY}")
message(STATUS "PQ_INCLUDE_DIR: ${PQ_INCLUDE_DIR}")
message(STATUS "========================================")

# 检查是否找到所有必要组件
if(NOT PQXX_LIBRARY)
    message(FATAL_ERROR "libpqxx library not found! Try: sudo apt-get install libpqxx-dev")
endif()

if(NOT PQ_LIBRARY)
    message(FATAL_ERROR "libpq library not found! Try: sudo apt-get install libpq-dev")
endif()

if(NOT PQXX_INCLUDE_DIR)
    message(FATAL_ERROR "pqxx headers not found!")
endif()

if(NOT PQ_INCLUDE_DIR)
    message(FATAL_ERROR "libpq headers not found!")
endif()

# set(MODULE_LIB_DIR1 /opt/utils/modules)  # 模块库的目录的设置
add_executable(courseSelection
    #math.cpp                              # 模块实现单元
    main.cpp

)


# CS大模块：包含： 交互分区（表现层） 、应用逻辑分区 、数据管理分区、实体类分区（领域层）
set(CS_MODULE
    cs.cppm
)

# 交互层
set(UI_MODULE
    ui.cppm
)

# 领域层/实体类
set(ENTITY_MODULE

    entity/entity.cppm
    entity/teacher.cppm
    entity/score.cppm
    entity/secretary.cppm
    entity/person.cppm
    entity/course.cppm
    entity/student.cppm
)

# 数据管理层
set(DATAMANAGEMENT_MODULE

    dataManagement/database.cppm
    dataManagement/courseDatabase.cppm
    dataManagement/studentDatabase.cppm
    dataManagement/teacherDatabase.cppm
    dataManagement/secretaryDatabase.cppm
    dataManagement/scoreDatabase.cppm
)

# 应用逻辑层
set(APPLICATIONLOGIC_MODULE

    Controller/controller.cppm
    Controller/courseController.cppm
    Controller/enrollmentController.cppm
    Controller/scoreController.cppm
)

set_source_files_properties(
    dataManagement/courseDatabase.cpp
    dataManagement/studentDatabase.cpp
    dataManagement/teacherDatabase.cpp
    dataManagement/secretaryDatabase.cpp
    dataManagement/scoreDatabase.cpp
    PROPERTIES
        CXX_MODULES OFF
)



source_group("entity" FILES ${ENTITY_MODULE})
source_group("dataManage" FILES ${DATAMANAGEMENT_MODULE})
source_group("sc" FILES ${CS_MODULE})
source_group("logic" FILES ${APPLICATIONLOGIC_MODULE})
source_group("ui" FILES ${UI_MODULE})

#source_group("Module Interfaces" FILES ${MODULE_INTERFACE_FILES})


target_sources(courseSelection  PRIVATE FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS                              # 配置BASE_DIRS，用于组织构建输出目录，以便构建不同目录中(同名的)模块文件.
        #${MODULE_LIB_DIR1}
        ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
        ${CS_MODULE}
        ${ENTITY_MODULE}             #实体类模块存放的文件
        ${DATAMANAGEMENT_MODULE}     #数据管理层
        ${APPLICATIONLOGIC_MODULE}
        ${UI_MODULE}

)
target_compile_features(courseSelection PRIVATE cxx_std_23)

# 添加头文件包含路径
target_include_directories(courseSelection PRIVATE
    ${PQXX_INCLUDE_DIR}
    ${PQ_INCLUDE_DIR}
)

# 链接库
target_link_libraries(courseSelection PRIVATE
    ${PQXX_LIBRARY}
    ${PQ_LIBRARY}
)











































# target_link_directories(courseSelection PRIVATE /opt/utils/lib)  # -L /opt/utils/lib 设置链接路径
# target_link_libraries(courseSelection PRIVATE utils)             # -lutils           设置链接库名

# set_target_properties(courseSelection PROPERTIES
#     INSTALL_RPATH_USE_LINK_PATH  True                           # 使用链接路径作为安装文件的RPath，等价于"-Wl,-rpath=/opt/utils/lib"
# )

#add_library(courseSelection SHARED
#  utils.cpp
#)

#set_target_properties( courseSelection PROPERTIES
#    PUBLIC_HEADER utils.cppm
#    VERSION   2.5.1
#    SOVERSION 2
#    LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/lib"
#)

#install(TARGETS courseSelection
#    LIBRARY DESTINATION /opt/utils/lib
#    PUBLIC_HEADER DESTINATION  /opt/utils/modules
#)

#add_custom_command(TARGET courseSelection
#    POST_BUILD
#    COMMAND cmake --install "${CMAKE_CURRENT_BINARY_DIR}"
#)
