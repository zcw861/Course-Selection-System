cmake_minimum_required(VERSION 4.1.1)

#https://github.com/Kitware/CMake/blob/v4.1.1/Help/dev/experimental.rst

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(courseSelection LANGUAGES CXX)

# set(MODULE_LIB_DIR1 /opt/utils/modules)  # 模块库的目录的设置
add_executable(courseSelection
    #math.cpp                              # 模块实现单元
    main.cpp

)


# CS大模块：包含： 交互分区（表现层） 、应用逻辑分区 、数据管理分区、实体类分区（领域层）
set(CS_MODULE
    cs.cppm
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

source_group("entity" FILES ${ENTITY_MODULE})
source_group("dataManage" FILES ${DATAMANAGEMENT_MODULE})
source_group("sc" FILES ${CS_MODULE})

#source_group("Module Interfaces" FILES ${MODULE_INTERFACE_FILES})

target_sources(courseSelection  PRIVATE FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS                              # 配置BASE_DIRS，用于组织构建输出目录，以便构建不同目录中(同名的)模块文件.
        #${MODULE_LIB_DIR1}
        ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
        ${CS_MODULE}
        ${ENTITY_MODULE}             #实体类模块存放的文件
        ${DATAMANAGEMENT_MODULE}     #数据管理层

)
target_compile_features(courseSelection PRIVATE cxx_std_23)












































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
