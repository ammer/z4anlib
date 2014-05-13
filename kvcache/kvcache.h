
#ifdef __cplusplus
extern "C" {
#endif

  typedef void *kvcache;

  kvcache kvc_create(void);
  void kvc_destroy(kvcache kvc);
  int kvc_set(kvcache kvc, const char *key, const char *value);
  char *kvc_get(kvcache kvc, const char *key);


#ifdef __cplusplus
}
#endif
