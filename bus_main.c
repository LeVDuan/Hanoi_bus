
#include "cgraph/cgraph.h"
#include "cgen/s2i.h"
#include "cgen/svector.h"
#include "libfdr/jrb.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>

typedef struct {
  char *station;
  char *route;
} BStop;

JRB key2id = NULL; //cay chua ten ben xe dua tren key la id dang int
JRB id2key = NULL; // vector chu cac ten dinh do thi theo chi so
cgraph_ivec_t edges = NULL; //vector chua cac canh do thi
cgraph_rvec_t weights = NULL; //vector chua cac canh do thi
const int w_change_station = 1000;
const int w_change_route = 1;
int vertices_id = 0;         // id dem so luong dinh


cgraph_ivec_t from_id = NULL;
cgraph_ivec_t to_id = NULL;


BStop *make_bstop(char *station, char *route) {
  BStop *bstop = (BStop*)malloc(sizeof(BStop));
  bstop->station = strdup(station);
  bstop->route = strdup(route);
  return bstop;
}

int get_save_id(char *station, char * route) 
{
  JRB node = jrb_find_str(key2id, station);
  JRB route_of_station = NULL;
  if (node) 
  {
    route_of_station = node->val.v;
  } else {
    route_of_station = make_jrb();
    jrb_insert_str(key2id, strdup(station),
            (Jval){.v = route_of_station});
  }

  if(jrb_find_str(route_of_station, route))
  {
    printf("Điểm dừng %s bị trùng trên tuyến %s\n",station, route );
    return 0;
  }
  else
  {
    jrb_insert_str(route_of_station, strdup(route), (Jval){.i = vertices_id});
    jrb_insert_int(id2key, vertices_id,(Jval){.v = make_bstop(station,route)});
  }
  printf("vertices: %s-%s, id: %d\n",station, route, vertices_id );
}

void remove_trailing(char *s) {
  int len = strlen(s);
  while (len > 0 && isspace(s[len - 1])) {
    s[len - 1] = '\0';
    --len;
  }
}


void store_edge(int f, int t, int w) {
  cgraph_ivec_push_back(&edges, f);
  cgraph_ivec_push_back(&edges, t);
  cgraph_rvec_push_back(&weights, w);
  //printf("Edge(%d, %d) %d\n", f, t, w_edge);
}

int read_input() {
  	
  key2id = make_jrb();
  id2key = make_jrb();
  edges = cgraph_ivec_create();
  weights = cgraph_rvec_create();
  char station[100];
  char route[100];
  FILE *f = fopen("../hanoi-bus.txt", "r");
  while(!feof(f)) {
    fscanf(f, "%[^:]%*[:]", route);
    int prev = -1;
    while (fscanf(f, " %[^>\n]", station) > 0) {
      remove_trailing(station);
      get_save_id(station, route); 
      if (prev >= 0) {
        store_edge(prev, vertices_id, w_change_station);
      }
      prev = vertices_id++;
      fscanf(f, "%*[>]");
      if (fscanf(f, "%[\n]", station) > 0) {
        break;
      }
    }
  }
  fclose(f);
  return 0;
}


int connect_station_other_route() 
{
  JRB i;
  jrb_traverse(i,key2id)
  {
    JRB routes = ((Jval*)&i->val)->v;
    JRB i1, i2;
    jrb_traverse(i1, routes) 
    {
      jrb_traverse(i2, routes) 
      {
        if (i1 != i2) {
          store_edge(i1->val.i, i2->val.i, w_change_route);
        }
      }
    }
  }
  
  return 0;
}

int station_not_found(cgraph_ivec_t *v, char *station) 
{
  JRB node = jrb_find_str(key2id, station);
  JRB i;
  if (node) 
  {
    JRB routes = ((Jval*)&node->val)->v;
    jrb_traverse(i,routes)
    {
      cgraph_ivec_push_back(v, ((Jval*)&i->val)->i);
    }
    return 0;
  }
  printf("Không tìm thấy bến %s!\n", station);
  return 1;
}

long path_cost(cgraph_ivec_t epath) {
  long cost = 0;
  for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(epath); ++i) {
      cost = cost + weights[epath[i]];
  }
  return cost;
}
void printf_path(cgraph_ivec_t vpath)
{

  for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(vpath); ++i) 
  {
      JRB node = jrb_find_int(id2key,vpath[i]);
      BStop* data = (BStop*)node->val.v;
      if( i == 0)
      {
              printf("Bắt đầu từ bến: '%s' lên '%s' > ",data->station, data->route);
      }
      if( i > 0 && i < cgraph_ivec_size(vpath) - 1)
      {
          JRB node_prev = jrb_find_int(id2key,vpath[i-1]);
          BStop* data_prev = (BStop*)node_prev->val.v;
          if( strcmp(data->station, data_prev->station) == 0)
          {
            printf("tại bến '%s' chuyển sang '%s' > ",data->station, data->route);
          }
          else{
        printf("'%s' tiếp tục '%s' > ",data->station, data->route);
        }
      }
      if(i == cgraph_ivec_size(vpath) - 1)
      {
        printf("'%s' Kết thúc!\n\n\n",data->station);
      }
      

  }
}
void find_shortest_paths(cgraph_t *g)
{
  long min_cost = -1;
  cgraph_ivec_t vpath = cgraph_ivec_create(),
                epath = cgraph_ivec_create();
  for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(from_id); ++i) 
  {
      for (CGRAPH_INTEGER j = 0; j < cgraph_ivec_size(to_id); ++j) 
      {
        cgraph_get_shortest_path_dijkstra(*g,
        &vpath,
        &epath,
        from_id[i],
        to_id[j],
        weights,
        CGRAPH_OUT);

        long cost = path_cost(epath);

        if(cost < min_cost || min_cost < 0)
        {
          min_cost = cost;
        }
      }
  }
  for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(from_id); ++i) 
  {
      for (CGRAPH_INTEGER j = 0; j < cgraph_ivec_size(to_id); ++j) 
      {
        cgraph_get_shortest_path(*g,
                                &vpath,
                                &epath,
                                from_id[i],
                                to_id[j],
                                CGRAPH_OUT);

        long cost = path_cost(epath);
        if(cost == min_cost)
        {
          printf_path(vpath);
        }
      }
  }
  
}

int main(int argc, char const *argv[])
{
  from_id = cgraph_ivec_create();
  to_id = cgraph_ivec_create();
  read_input();
  connect_station_other_route();
	cgraph_t g = cgraph_create(edges, 0, CGRAPH_DIRECTED);


  char from[100];
  char to[100];
  printf("Nhập điểm đi: ");
  gets(from);
  __fpurge(stdin);
  printf("Nhập điểm đến: ");
  gets(to);
  remove_trailing(from);
  remove_trailing(to);
  while(station_not_found(&from_id, from) || station_not_found(&to_id, to)) 
  {
    printf("Nhập lại\n");
    printf("Nhập điểm đi: ");
    gets(from);
    __fpurge(stdin);
    printf("Nhập điểm đến: ");
    gets(to);
    remove_trailing(from);
    remove_trailing(to);
  }
  find_shortest_paths(&g);
	return 0;
}