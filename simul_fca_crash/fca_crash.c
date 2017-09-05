#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fca/fca_api.h>
#include <fca/config/fca_parse_specfile.h>
#include <errno.h>

#define error(fmt,arg...)  fprintf(stderr,"CHECK: "fmt"\n", ## arg)

int g_progress_counter = 0;

void my_progress(void *arg)
{
  int *counter=arg;
  ++(*counter);
}

struct fca_context *init_context(fca_init_spec_t *fca_spec,int job_id)
{
  fca_t *ctx;
  int ret;

  fca_spec->element_type = FCA_ELEMENT_RANK;
  fca_spec->progress.func = my_progress;
  fca_spec->progress.arg = &g_progress_counter;
  fca_spec->job_id = job_id;
  ret = fca_init(fca_spec,&ctx);

  if (ret<0){
    printf("Error");
    return NULL;
  }
  return ctx;
}


int main(int argc, char ** argv, char **envp)
{ 
  struct fca_context *ctx,*ctx2;
  fca_comm_desc_t *comm_desc;
  fca_comm_t *comm;
  fca_comm_caps_t comm_caps;

  fca_init_spec_t *fca_spec;
  fca_comm_new_spec_t spec;
  int my_info_len,max_info_len,ret = -1;
  void *my_info;
  char *all_info;
  int i;
  
  fca_spec = fca_parse_spec_file(NULL);
  ctx = init_context(fca_spec,0);

  if (!ctx)
    return -2;
  fca_free_init_spec(fca_spec);

  my_info = fca_get_rank_info(ctx,&my_info_len);

  printf("My info length is %d \n",my_info_len);
  printf("Printing information\n");
  for (i=0;i<my_info_len;i++)
    {
      printf("%02x", ((unsigned char *)my_info)[i]);
    }
  printf("\n");
  
  max_info_len = my_info_len*2; 
  all_info = malloc(max_info_len);
  memcpy(all_info, my_info,my_info_len);
  fca_free_rank_info(my_info);
  /* Create new communicator */
  spec.rank_info = all_info;
  spec.rank_count = 2;
  spec.is_comm_world = 0;
  printf("OK \n");  
  ret = fca_comm_new(ctx,&spec,comm_desc);
  printf("Connected ===>");
  if (ret < 0)
    {
      error("fca_comm_new() failed: %s", fca_strerror(ret));
    }
  printf("Finished\n");
  return 0;

}
