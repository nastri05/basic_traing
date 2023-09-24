check different topic
hmxcms_client_list.c:
cms_get_index_by_name()-> free(cl_name);
cms_get_data_by_name() -> free(cl_name);
hmxcms_log.c
cms_print_monitor_list()->free(client_name);
