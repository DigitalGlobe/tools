#include "ecs.h"

char url[] = "gltp:/grass/c:/demo/spearfish/PERMANENT";
char layer[] = "roads@PERMANENT(*)";

int main()
{
  int ClientID;
  ecs_Result *result;
  ecs_Region selectionRegion;
  ecs_LayerSelection selectionLayer;

  /* Create a client with ClientID as a reference */
  
  result = cln_CreateClient(&ClientID,url);
    
    /* The user must set a region value in the client geographic projection */
    
  selectionRegion.north = 4928000.0;
  selectionRegion.south = 4914000.0;
  selectionRegion.east  = 609000.0;
  selectionRegion.west  = 590000.0;
  selectionRegion.ns_res = 50.0;
  selectionRegion.ew_res = 50.0;
  result = cln_SelectRegion(ClientID,&selectionRegion);
    
  /* Define the layer to select */
    
  selectionLayer.Select = (char *) layer;
  selectionLayer.F = Line;
  result = cln_SelectLayer(ClientID,&selectionLayer);
    
  /* The application process the result of cln_SelectLayer. */
    
  result = cln_GetNextObject(ClientID);
  while (ECSSUCCESS(result)) {
    result = cln_GetNextObject(ClientID);
  }
    
  result = cln_ReleaseLayer(ClientID,&selectionLayer);
  result = cln_DestroyClient(ClientID);

  return 0;
}
