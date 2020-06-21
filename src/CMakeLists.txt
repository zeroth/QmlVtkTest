CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

find_package(Qt5 COMPONENTS Gui Widgets Core Quick REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
file(GLOB qtresources "*.qrc")

# if you want to remove some file from compilations
# list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/<filename>.cpp)
# list(REMOVE_ITEM headers ${CMAKE_CURRENT_SOURCE_DIR}/<filename>.h)
# list(REMOVE_ITEM qtresources ${CMAKE_CURRENT_SOURCE_DIR}/<filename>.qrc)

#set(VTK_DIR "C:\\Code\\Common\\VTK-8.2.0\\VTK_Debug\\lib\\cmake\\vtk-8.2")
set(VTK_DIR "C:\\Code\\Common\\VTK-7.1.1\\VTK_Debug\\lib\\cmake\\vtk-7.1")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${sources} ${headers} ${qtresources} )

if(MSVC)
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif(MSVC)

# QML debugging
target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

# link libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE Qt5::Quick Qt5::Core Qt5::Quick Qt5::Widgets ${VTK_LIBRARIES}  ${OPENGL_LIBRARIES})

# deploy ready
get_filename_component(QT_BASE_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)

string(TOLOWER ${CMAKE_BUILD_TYPE} WINDEPLOY_TYPE)
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND "${QT_BASE_DIR}/windeployqt.exe" --${WINDEPLOY_TYPE} --compiler-runtime  --json --pdb --qmldir ${QT_BASE_DIR}/../qml "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.exe" > "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/deploy.logs"
)
