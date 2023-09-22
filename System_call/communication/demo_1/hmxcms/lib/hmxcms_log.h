#ifndef HMX_CMS_LOG_H
#define HMX_CMS_LOG_H

#include <stdio.h>

typedef enum {
    CMS_LOG_LEVEL_NONE = 0,
    CMS_LOG_LEVEL_ERROR,
    CMS_LOG_LEVEL_WARNING,
    CMS_LOG_LEVEL_INFO,
    CMS_LOG_LEVEL_DEBUG,
} cms_log_level_t;


// cms_log_level_t cms_get_log_level_from_cfg();
void cms_set_log_level(cms_log_level_t logLevel);
cms_log_level_t cms_get_log_level();




#define CMS_ERROR_LOG(fmt, ...) \
            do { if (cms_get_log_level() >= CMS_LOG_LEVEL_ERROR)  printf((char*)"CMS_ERR_LOG  [%s:%d] "  fmt, \
                                __FUNCTION__, __LINE__, ## __VA_ARGS__); } while (0)

#define CMS_WARNING_LOG(fmt, ...) \
            do { if (cms_get_log_level() >= CMS_LOG_LEVEL_WARNING) printf((char*)"CMS_WAR_LOG  [%s:%d] "  fmt, \
                                __FUNCTION__, __LINE__,  ## __VA_ARGS__); } while (0)

#define CMS_INFO_LOG(fmt, ...) \
            do { if (cms_get_log_level() >= CMS_LOG_LEVEL_INFO)   printf((char*)"CMS_INF_LOG  [%s:%d] "  fmt, \
                                __FUNCTION__, __LINE__,  ## __VA_ARGS__); } while (0)

#define CMS_DEBUG_LOG(fmt, ...) \
            do { if (cms_get_log_level() >= CMS_LOG_LEVEL_DEBUG)   printf((char*)"CMS_DEBUG_LOG  [%s:%d] "  fmt, \
                                __FUNCTION__, __LINE__,  ## __VA_ARGS__); } while (0)



#endif

