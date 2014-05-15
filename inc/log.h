#ifndef _LOG_H
#define _LOG_H 1

#include <android/log.h>


#define LOG_TAG "Z4ANLIB"
#define DEVELOPMENT
#ifdef DEVELOPMENT
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGW(fmt, args...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#define TRACE __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "TRACE| %s:%d %s(...)\n", __FILE__, __LINE__, __FUNCTION__)
#else
#define LOGI(args...) do{}while(0)
#define LOGD(args...) do{}while(0)
#define LOGW(args...) do{}while(0)
#define LOGE(args...) do{}while(0)
#define TRACE         do{}while(0)
#endif

#endif // _LOG_H
