cmake_minimum_required(VERSION 3.4.1)

set(DIR_OS ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
set(DIR_FFMPEG_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/ffmpeg/include)
set(DIR_COMMON_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/common)
set(DIR_DEMUX_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/demux)
set(DIR_BEAN_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/bean)
set(DIR_SERVICE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/service)
set(DIR_DECODEC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/decodec)
set(DIR_PLAY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/play)
set(DIR_VIDEO_INCLUDE ${DIR_PLAY_INCLUDE}/video)
set(DIR_AUDIO_INCLUDE ${DIR_PLAY_INCLUDE}/audio)

# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DIR_OS})

include_directories(
        ${DIR_FFMPEG_INCLUDE}
        ${DIR_COMMON_INCLUDE}
        ${DIR_DEMUX_INCLUDE}
        ${DIR_BEAN_INCLUDE}
        ${DIR_SERVICE_INCLUDE}
        ${DIR_DECODEC_INCLUDE}
        ${DIR_AUDIO_INCLUDE}
        ${DIR_VIDEO_INCLUDE})

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${DIR_OS}/libavcodec.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${DIR_OS}/libavfilter.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${DIR_OS}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${DIR_OS}/libavutil.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${DIR_OS}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${DIR_OS}/libswscale.so)

add_library(
             native-lib
             SHARED
             src/main/cpp/native-lib.cpp

             ${DIR_DEMUX_INCLUDE}/FFDemux.cpp
             ${DIR_DEMUX_INCLUDE}/IDemux.cpp

             ${DIR_BEAN_INCLUDE}/XData.cpp
             ${DIR_BEAN_INCLUDE}/XParameter.cpp

             ${DIR_SERVICE_INCLUDE}/XThread.cpp
             ${DIR_SERVICE_INCLUDE}/IObserver.cpp

             ${DIR_DECODEC_INCLUDE}/FFDecode.cpp
             ${DIR_DECODEC_INCLUDE}/IDecode.cpp

             ${DIR_VIDEO_INCLUDE}/GLVideoView.cpp
             ${DIR_VIDEO_INCLUDE}/IVideoView.cpp
             ${DIR_VIDEO_INCLUDE}/XEGL.cpp
             ${DIR_VIDEO_INCLUDE}/XTexture.cpp
             ${DIR_VIDEO_INCLUDE}/XShader.cpp

             ${DIR_AUDIO_INCLUDE}/IResample.cpp
             ${DIR_AUDIO_INCLUDE}/FFResample.cpp
             ${DIR_AUDIO_INCLUDE}/IAudioPlay.cpp
             ${DIR_AUDIO_INCLUDE}/SLAudioPlay.cpp)

target_link_libraries(
                       native-lib
                       android
                       OpenSLES
                       GLESv2
                       EGL
                       avcodec avfilter avformat avutil swresample swscale
                       log )