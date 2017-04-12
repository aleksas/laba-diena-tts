# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative gpaths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(AFTER SYSTEM
             ../../../native-modules/LithUSS/
             ../../../native-modules/include/)

add_library( # Sets the name of the library.
             laba-diena-tts

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/laba-diena-tts/AndroidLoader.cpp )

add_library( # Sets the name of the library.
             LithUSS

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ../../../native-modules/LithUSS/fv2id.cpp
             ../../../native-modules/LithUSS/ilgiai.cpp
             ../../../native-modules/LithUSS/LithUSS.cpp
             ../../../native-modules/LithUSS/strtokf.cpp
             ../../../native-modules/LithUSS/raw2wav.cpp
             ../../../native-modules/LithUSS/getLUSSErrorMessages.cpp )

add_library( # Sets the name of the library.
             RateChange

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ../../../native-modules/RateChange/Euristika.cpp
             ../../../native-modules/RateChange/greitis.cpp
             ../../../native-modules/RateChange/RateChange.cpp
             ../../../native-modules/RateChange/VeiksmaiSuFailais.cpp )

add_library( # Sets the name of the library.
             TextNormalization

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

             ../../../native-modules/TextNormalization/stringWithLetterPosition.cpp
             ../../../native-modules/TextNormalization/TextNormalization.cpp )

add_library( # Sets the name of the library.
             transcrLUSS

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

             ../../../native-modules/transcrLUSS/ArKirciuoti.cpp
             ../../../native-modules/transcrLUSS/fv2id.cpp
             ../../../native-modules/transcrLUSS/Kircdb.cpp
             ../../../native-modules/transcrLUSS/Skiemen.cpp
             ../../../native-modules/transcrLUSS/strtokf.cpp
             ../../../native-modules/transcrLUSS/transcrLUSS.cpp
             ../../../native-modules/transcrLUSS/Transkr.cpp )

add_library( # Sets the name of the library.
             UnitSelection

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

             ../../../native-modules/UnitSelection/strtokf.cpp
             ../../../native-modules/UnitSelection/UnitSelection.cpp )


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
                       laba-diena-tts

                       LithUSS

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )

target_link_libraries( # Specifies the target library.
                      LithUSS

                      RateChange
                      TextNormalization
                      transcrLUSS
                      UnitSelection )