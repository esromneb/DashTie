cmake_minimum_required(VERSION 2.6)

# Set paths, these are relative to the folder which contains CMakeLists.txt, not build/
set(SRC_DIR             src)

#
# Included scripts do automatic cmake_policy PUSH and POP.
# 
# In CMake 2.6.2 and below, CMake Policy settings in scripts loaded by
# the include() and find_package() commands would affect the includer.
# Explicit invocations of cmake_policy(PUSH) and cmake_policy(POP) were
# required to isolate policy changes and protect the includer.  While
# some scripts intend to affect the policies of their includer, most do
# not.  In CMake 2.6.3 and above, include() and find_package() by
# default PUSH and POP an entry on the policy stack around an included
# script, but provide a NO_POLICY_SCOPE option to disable it.  This
# policy determines whether or not to imply NO_POLICY_SCOPE for
# compatibility.  The OLD behavior for this policy is to imply
# NO_POLICY_SCOPE for include() and find_package() commands.  The NEW
# behavior for this policy is to allow the commands to do their default
# cmake_policy PUSH and POP.
#
# see https://github.com/Kitware/CMake/blob/master/Tests/PolicyScope/CMakeLists.txt 
cmake_policy(SET CMP0011 NEW)



################


project(DashTie CXX)



set(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} \
-O3 -Werror=return-type -Wall -Wextra -fPIC -Wno-unused-function -Wno-error=attributes \
-std=gnu++11 -Wvla -Wcast-qual -Wdangling-else -Winit-self -Werror=uninitialized \
-Werror=incompatible-pointer-types -Werror=array-bounds -Wshadow \
-Wduplicated-cond -Wnull-dereference -Wdangling-else -Waddress \
-Wint-in-bool-context -Winit-self \
-Wpointer-arith \
-Werror=uninitialized \
-Werror=strict-prototypes \
-Wlogical-op -Werror=logical-op -Werror=null-dereference \
-Werror=sequence-point \
-Werror=missing-braces -Werror=write-strings -Werror=address -Werror=array-bounds \
-Werror=char-subscripts -Werror=enum-compare -Werror=implicit-int \
-Werror=empty-body -Werror=main -Werror=nonnull -Werror=parentheses \
-Werror=pointer-sign -Werror=ignored-qualifiers \
-Werror=missing-parameter-type -Werror=unused-value \
-Wmissing-declarations \
-Wmissing-field-initializers \
-Wcast-align \
-Wduplicated-branches \
-Wformat \
-Wformat-security \
-Wformat-signedness \
-Woverlength-strings \
-Wredundant-decls \
-Wsynth \
-Wtrampolines \
-Wvariadic-macros \
-Wvirtual-inheritance \
"
)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")


set(MY_INCLUDE_DIRS 
src
)

set(MY_SOURCE_FILES
    ${SRC_DIR}/DashTie.hpp
    ${SRC_DIR}/DashTie.cpp
    ${SRC_DIR}/DashTieManager.hpp
    ${SRC_DIR}/DashTieManager.cpp
)

set(MY_LIBRARIES
    -pthread
)



include_directories(${MY_INCLUDE_DIRS})

add_executable(main ${SRC_DIR}/main.cpp ${MY_SOURCE_FILES})
target_link_libraries(main ${MY_LIBRARIES})

