# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(AFTER SYSTEM
             ../../LithUSS/)

add_library( # Sets the name of the library.
             LithUSS

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ../../LithUSS/fv2id.cpp
             ../../LithUSS/ilgiai.cpp
             ../../LithUSS/LithUSS.cpp
             ../../LithUSS/strtokf.cpp
             ../../LithUSS/raw2wav.cpp
             ../../LithUSS/getLUSSErrorMessages.cpp
             src/main/cpp/LithUSS/AndroidLoader.cpp )

add_library( # Sets the name of the library.
             RateChange

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ../../RateChange/Euristika.cpp
             ../../RateChange/greitis.cpp
             ../../RateChange/RateChange.cpp
             ../../RateChange/VeiksmaiSuFailais.cpp )

add_library( # Sets the name of the library.
             TextNormalization

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

             ../../TextNormalization/stringWithLetterPosition.cpp
             ../../TextNormalization/TextNormalization.cpp )

add_library( # Sets the name of the library.
             transcrLUSS

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

             ../../transcrLUSS/ArKirciuoti.cpp
             ../../transcrLUSS/fv2id.cpp
             ../../transcrLUSS/Kircdb.cpp
             ../../transcrLUSS/Skiemen.cpp
             ../../transcrLUSS/strtokf.cpp
             ../../transcrLUSS/transcrLUSS.cpp
             ../../transcrLUSS/Transkr.cpp )

add_library( # Sets the name of the library.
             UnitSelection

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

             ../../UnitSelection/LithUSS_Error.h
             ../../UnitSelection/strtokf.cpp
             ../../UnitSelection/UnitSelection.cpp )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       LithUSS
					   
                       RateChange
                       TextNormalization
                       transcrLUSS
                       UnitSelection


                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )