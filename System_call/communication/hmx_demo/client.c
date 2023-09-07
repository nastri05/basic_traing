#include "inc/hmxcms_lib.h"
int main(){
    struct mq_attr* mq_attr = (struct mq_attr*)malloc(sizeof(mq_attr));
    int mqd_client = cms_client_init(mq_attr,"/Tuan","mq_name",1,"wan");
    printf("value mqd_client %d\n",mqd_client);
    while (1)
    {
    sleep(1);      /* code */
    }
    
    return 0;
}   