
extern void gcoap_register_handlers(void);
extern void gcoap_for_resources(void(*function)(const coap_resource_t*, void*), void *param);

#define COAP_RESOURCE(path_, methods_, handler_, extra_) \
    (coap_resource_t *) & (elli_coap_resource_t) { \
	.path = (path_), .methods = (methods_), .handler = (coap_handler_t)(handler_), extra_ }
