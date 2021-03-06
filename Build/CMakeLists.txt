# Minimum version required.
CMake_Minimum_Required(VERSION 3.1)

# Include following modules.
Include(ExternalProject)

#
# Config
#

# Build settings.
Set(ProjectName "Project")
Set(TargetName "Application")

# Application settings.
Set(ShowConsole "Yes")

#
# Source
#

# Source files.
Set(SourceDir "../Source")
Set(PrecompiledHeader "Precompiled.hpp")
Set(PrecompiledSource "Precompiled.cpp")

Set(SourceFiles
    "${PrecompiledHeader}"
    "${PrecompiledSource}"

	"Context.hpp"
    "Main.cpp"

    "Common/Debug.hpp"
    "Common/Build.hpp"
    "Common/Build.cpp"
    "Common/Utility.hpp"
    "Common/Utility.cpp"
    "Common/NonCopyable.hpp"
    "Common/ScopeGuard.hpp"
    "Common/Delegate.hpp"
    "Common/Collector.hpp"
    "Common/Dispatcher.hpp"
    "Common/Receiver.hpp"

    "Logger/Logger.hpp"
    "Logger/Logger.cpp"
    "Logger/Output.hpp"
    "Logger/Message.hpp"
    "Logger/Message.cpp"
    "Logger/Sink.hpp"
    "Logger/Sink.cpp"
    "Logger/FileOutput.hpp"
    "Logger/FileOutput.cpp"
    "Logger/ConsoleOutput.hpp"
    "Logger/ConsoleOutput.cpp"
    "Logger/DebuggerOutput.hpp"
    "Logger/DebuggerOutput.cpp"

    "Lua/Lua.hpp"
    "Lua/State.hpp"
    "Lua/State.cpp"
    "Lua/StackGuard.hpp"
    "Lua/StackGuard.cpp"
    "Lua/Reference.hpp"
    "Lua/Reference.cpp"
    "Lua/Helpers.hpp"
    "Lua/Bindings.hpp"
    "Lua/Bindings.cpp"
    "Lua/Bindings/Math.hpp"
    "Lua/Bindings/Math.cpp"
    "Lua/Bindings/System.hpp"
    "Lua/Bindings/System.cpp"
    "Lua/Bindings/Game.hpp"
    "Lua/Bindings/Game.cpp"

    "System/Config.hpp"
    "System/Config.cpp"
    "System/Timer.hpp"
    "System/Timer.cpp"
    "System/Window.hpp"
    "System/Window.cpp"
    "System/InputState.hpp"
    "System/InputState.cpp"
    "System/Resource.hpp"
    "System/ResourcePool.hpp"
    "System/ResourceManager.hpp"
    "System/ResourceManager.cpp"

    "Graphics/ScreenSpace.hpp"
    "Graphics/ScreenSpace.cpp"
    "Graphics/Buffer.hpp"
    "Graphics/Buffer.cpp"
    "Graphics/VertexInput.hpp"
    "Graphics/VertexInput.cpp"
    "Graphics/Sampler.hpp"
    "Graphics/Sampler.cpp"
    "Graphics/Texture.hpp"
    "Graphics/Texture.cpp"
    "Graphics/Shader.hpp"
    "Graphics/Shader.cpp"
    "Graphics/SpriteSheet.hpp"
    "Graphics/SpriteSheet.cpp"
    "Graphics/BasicRenderer.hpp"
    "Graphics/BasicRenderer.cpp"

    "Game/EntityHandle.hpp"
    "Game/EntitySystem.hpp"
    "Game/EntitySystem.cpp"
    "Game/Component.hpp"
    "Game/ComponentPool.hpp"
    "Game/ComponentSystem.hpp"
    "Game/ComponentSystem.cpp"
    "Game/IdentitySystem.hpp"
    "Game/IdentitySystem.cpp"
    "Game/ScriptSystem.hpp"
    "Game/ScriptSystem.cpp"
    "Game/RenderSystem.hpp"
    "Game/RenderSystem.cpp"

    "Game/Components/Transform.hpp"
    "Game/Components/Transform.cpp"
    "Game/Components/Script.hpp"
    "Game/Components/Script.cpp"
    "Game/Components/Render.hpp"
    "Game/Components/Render.cpp"
)

# Append source directory path to each source file.
Set(SourceFilesTemp)

ForEach(SourceFile ${SourceFiles})
    List(APPEND SourceFilesTemp "${SourceDir}/${SourceFile}")
EndForEach()

Set(SourceFiles ${SourceFilesTemp})

# Organize source files based on their directory structure.
ForEach(SourceFile ${SourceFiles})
    # Get the path to the source file's directory.
    Get_Filename_Component(SourceFilePath ${SourceFile} PATH)
    
    # Remove the path to the base source directory.
    String(REPLACE "${SourceDir}" "" SourceFilePath ${SourceFilePath})

    # Add source file to a group.
    If(NOT ${SourceFilePath} STREQUAL "")
        # Normalize folder separators.
        String(REPLACE "/" "\\" SourceFilePath ${SourceFilePath})
        
        # Remove leading folder separator.
        String(SUBSTRING ${SourceFilePath} 0 1 FirstCharacter)

        If(FirstCharacter STREQUAL "\\")
            String(LENGTH ${SourceFilePath} SourceFilePathLength)
            Math(EXPR SourceFilePathLength "${SourceFilePathLength} - 1")
            String(SUBSTRING ${SourceFilePath} 1 ${SourceFilePathLength} SourceFilePath)
        EndIf()
    
        # Add a source file to a corresponding group.
        Source_Group("Source\\${SourceFilePath}" FILES ${SourceFile})
    Else()
        # Add source file to the default group.
        Source_Group("Source" FILES ${SourceFile})
    EndIf()
EndForEach()

#
# Build
#

# Create the project.
Project(${ProjectName})

# Create an executable target.
Add_Executable(${TargetName} ${SourceFiles})

# Add the source directory as an include directory.
Include_Directories(${SourceDir})

# Enable unicode support.
Add_Definitions(-DUNICODE -D_UNICODE)

# Enable target folders.
Set_Property(GLOBAL PROPERTY USE_FOLDERS ON)

# Move CMake predefined targets to a separate folder.
Set_Property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

#
# Debug
#

# Working directory path.
Set(WorkingDir "../Deploy")

# Write a file in the build directory that will point to the working directory.
Get_Filename_Component(WorkingDirPath "${CMAKE_CURRENT_LIST_DIR}/${WorkingDir}" ABSOLUTE)
File(WRITE "${CMAKE_BINARY_DIR}/WorkingDir.txt" "${WorkingDirPath}/")

# Write a file in the build directory that will point to the source directory.
Get_Filename_Component(SourceDirPath "${CMAKE_CURRENT_LIST_DIR}/${SourceDir}" ABSOLUTE)
File(WRITE "${CMAKE_BINARY_DIR}/SourceDir.txt" "${SourceDirPath}/")

#
# Windows
#

# Visual C++ compiler.
If("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Show or hide the console window.
    If(ShowConsole)
        Set_Property(TARGET ${TargetName} APPEND_STRING PROPERTY LINK_FLAGS "/SUBSYSTEM:Console ")
    Else()
        Set_Property(TARGET ${TargetName} APPEND_STRING PROPERTY LINK_FLAGS "/SUBSYSTEM:Windows ")
    EndIf()
    
    # Restore default main() entry instead of WinMain().
    Set_Property(TARGET ${TargetName} APPEND_STRING PROPERTY LINK_FLAGS "/ENTRY:mainCRTStartup ")

    # Disable Standard C++ Library warnings.
    Set_Property(TARGET ${TargetName} APPEND_STRING PROPERTY COMPILE_DEFINITIONS "_CRT_SECURE_NO_WARNINGS")
    Set_Property(TARGET ${TargetName} APPEND_STRING PROPERTY COMPILE_DEFINITIONS "_SCL_SECURE_NO_WARNINGS")
    
    # Use the precompiled header.
    Get_Filename_Component(PrecompiledName ${PrecompiledHeader} NAME_WE)
    
    Set(PrecompiledBinary "$(IntDir)/${PrecompiledName}.pch")
    
    Set_Source_Files_Properties(${SourceFiles} PROPERTIES 
        COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
        OBJECT_DEPENDS "${PrecompiledBinary}"
    )
    
    Set_Source_Files_Properties("${SourceDir}/${PrecompiledSource}" PROPERTIES 
        COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
        OBJECT_OUTPUTS "${PrecompiledBinary}"
    )
EndIf()

#
# Linux
#

# GCC compiler.
If("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # Enable C++11 support.
    List(APPEND CMAKE_CXX_FLAGS "-std=c++11")
EndIf()

#
# GLM
#

# Add include directory.
Include_Directories("../External/GLM-0.9.7.2")

#
# OpenGL
#

# Find library.
Find_Package(OpenGL REQUIRED)

# Link library.
Target_Link_Libraries(${TargetName} ${OPENGL_gl_LIBRARY})

#
# GLEW
#

# Add library directory.
Add_Subdirectory("../External/GLEW-1.13.0" "GLEW" EXCLUDE_FROM_ALL)

# Add include directory.
Include_Directories("../External/GLEW-1.13.0/include")

# Move library target to dependencies.
Set_Property(TARGET "GLEW" PROPERTY FOLDER "External")

# Link library target.
Add_Dependencies(${TargetName} "GLEW")
Target_Link_Libraries(${TargetName} "GLEW")

#
# GLFW
#

# Set build options.
Set(GLFW_BUILD_EXAMPLES OFF)
Set(GLFW_BUILD_TESTS OFF)
Set(GLFW_BUILD_DOCS OFF)
Set(GLFW_INSTALL OFF)

# Add library directory.
Add_Subdirectory("../External/GLFW-3.1.2" "GLFW" EXCLUDE_FROM_ALL)

# Add include directory.
Include_Directories("../External/GLFW-3.1.2/include")

# Set target properties.
Set_Property(TARGET "glfw" PROPERTY PROJECT_LABEL "GLFW")
Set_Property(TARGET "glfw" PROPERTY FOLDER "External")

# Link library target.
Add_Dependencies(${TargetName} "glfw")
Target_Link_Libraries(${TargetName} "glfw")

#
# LuaJIT
#

# Add library directory.
Add_Subdirectory("../External/LuaJIT-2.0.4" "LuaJIT" EXCLUDE_FROM_ALL)

# Add include directory.
Include_Directories("../External/LuaJIT-2.0.4/src")

# Move library targets to dependencies.
Set_Property(TARGET "LuaJIT" PROPERTY FOLDER "External")
Set_Property(TARGET "LuaJIT-BuildVM" PROPERTY FOLDER "External")
Set_Property(TARGET "LuaJIT-MiniLua" PROPERTY FOLDER "External")

# Link library target.
Add_Dependencies(${TargetName} "LuaJIT")
Target_Link_Libraries(${TargetName} "LuaJIT")

#
# ZLib
#

# Set build options.
Set(SKIP_INSTALL_ALL ON)

# Add library directory.
Add_Subdirectory("../External/ZLib-1.2.8" "ZLib" EXCLUDE_FROM_ALL)

# Add include directory.
Include_Directories("../External/ZLib-1.2.8")
Include_Directories("${CMAKE_BINARY_DIR}/ZLib")

# Move library target to dependencies.
Set_Property(TARGET "zlibstatic" PROPERTY PROJECT_LABEL "ZLib")
Set_Property(TARGET "zlibstatic" PROPERTY FOLDER "External")

# Link library target.
Add_Dependencies(${TargetName} "zlibstatic")
Target_Link_Libraries(${TargetName} "zlibstatic")

# Help dependencies find this library.
Set(ZLIB_ROOT "../External/ZLib-1.2.8")
Set(ZLIB_INCLUDE_DIR "../External/ZLib-1.2.8")
Set(ZLIB_LIBRARY "zlibstatic")

#
# PNG
#

# Set build options.
Set(PNG_SHARED OFF)
Set(PNG_STATIC ON)
Set(PNG_TESTS OFF)
Set(SKIP_INSTALL_ALL ON)

# Add library directory.
Add_Subdirectory("../External/PNG-1.6.21" "PNG" EXCLUDE_FROM_ALL)

# Add include directory.
Include_Directories("../External/PNG-1.6.21")
Include_Directories("${CMAKE_BINARY_DIR}/PNG")

# Move library target to dependencies.
Set_Property(TARGET "png16_static" PROPERTY PROJECT_LABEL "PNG")
Set_Property(TARGET "png16_static" PROPERTY FOLDER "External")

# Link library target.
Add_Dependencies(${TargetName} "png16_static")
Target_Link_Libraries(${TargetName} "png16_static")
