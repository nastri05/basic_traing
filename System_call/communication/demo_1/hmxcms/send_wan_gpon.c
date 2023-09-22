#include "lib/hmxcms_lib.h"


int main ()
{   
    long long count_error = 0;
    int result = cms_client_init(HMX_CLIENT_1);
    if(result == CMS_ERROR || result == CMS_FAIL){
        LOG_CLIENT_STATE("Can't init client \n");
        return 0;
    }
    cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    while (1)
    {
        int j = 0;
        while (j <600)
        {
            // int i;
            // for(i = 0 ;i <20;i++){
                char str[32];
                sprintf(str,"%d",j);
                result = cms_client_send("wan", str);
                
                if(result == CMS_ERROR || result == CMS_FAIL){
                        count_error++;
                    }
                result = cms_client_send("gpon", str);
                
                if(result == CMS_ERROR || result == CMS_FAIL){
                        count_error++;
                    }
            // }
            j++;
        }
        printf("[HMX_CLIENT_1] number error : %lld\n",count_error);
        sleep(60);
    }
    
    cms_close_client();
    return 0;
}