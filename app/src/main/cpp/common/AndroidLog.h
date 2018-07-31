//
// Created by BMW on 2018/7/17.
//

#ifndef FFMPEG_STUDY_CSDN_LOG_H
#define FFMPEG_STUDY_CSDN_LOG_H

#include <android/log.h>

#define LOG_TAG "weiwei"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


#endif //FFMPEG_STUDY_CSDN_LOG_H
