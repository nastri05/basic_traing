#include "hmxcms_log.h"

static cms_log_level_t main_cms_log_level = CMS_LOG_LEVEL_NONE;

void cms_set_log_level(cms_log_level_t logLevel){
    main_cms_log_level = logLevel;
}

cms_log_level_t cms_get_log_level(){
    return main_cms_log_level;
}