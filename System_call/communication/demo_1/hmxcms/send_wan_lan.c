#include "lib/hmxcms_lib.h"


int main ()
{   
    long long count_error = 0;
    struct mq_attr *mq_attr = cms_create_attr(0,10,sizeof(cms_msg_t));
    cms_client_infor * my_mq = cms_client_init(mq_attr,HMX_CLIENT_1);
    if(my_mq == NULL){
        LOG_CLIENT_STATE("Can't init client \n");
        free(mq_attr);
        return 0;
    }

    cms_msg_t * message = (cms_msg_t*) malloc(sizeof(cms_msg_t));
    while (1)
    {
        int j = 0;
        while (j <600)
        {
                char str[32];
                sprintf(str,"%d",j);
                int result = cms_client_send(my_mq, "HMX2130X_WAN", str);
                
                if(result == CMS_ERROR || result == CMS_FAIL){
                        count_error++;
                    }
                result = cms_client_send_to(my_mq, HMX_CLIENT_4, str);
                
                if(result == CMS_ERROR || result == CMS_FAIL){
                        count_error++;
                    }
            j++;
        }
        printf("[HMX_CLIENT_1] number error when send : %lld\n",count_error);
        sleep(60);
    }
    
    cms_close_client(my_mq);
    free(mq_attr);
    free(my_mq);
    return 0;
}



