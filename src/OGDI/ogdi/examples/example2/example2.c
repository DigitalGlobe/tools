#include <stdio.h>
#include "ecs.h"

int PrintResult();

/* 
   This example will print all the lines objects from a database
   defined by user. The user will give the url and the layer
   selection to the program and, immediately, he will see the
   lines print on screen.
   */

int main()
{
  int ClientID;
  char url[100];
  char layerSelection[100];
  ecs_Result *result;
  ecs_Region selectionRegion;
  ecs_LayerSelection selectionLayer;
  int code;
  int errin;
  code = TRUE;

  printf("Enter the URL: ");
  errin = scanf("%s",url);
    if (errin == 0) {
        printf("URL Input error.\n"); 
        exit(1);
        }
  printf("Enter the layer: ");
  errin = scanf("%s",layerSelection);
    if (errin == 0) {
        printf("Layer Input error.\n"); 
        exit (1); 
	}

  PrintResult("cln_CreateClient", cln_CreateClient(&ClientID,url));
  PrintResult("cln_GetGlobalBound", (result=cln_GetGlobalBound(ClientID)));
  
  /* It's better to allocate locally all the data structure to be sent
     to the API. In this case, ECSREGION(result) is not used directly in
     argument of cln_SelectRegion. The SelectRegion destroy the old 
     ecs_Result and it's argument at the same time. */

  selectionRegion.north = ECSREGION(result).north;
  selectionRegion.south = ECSREGION(result).south;
  selectionRegion.east = ECSREGION(result).east;
  selectionRegion.west = ECSREGION(result).west;
  selectionRegion.ns_res = ECSREGION(result).ns_res;
  selectionRegion.ew_res = ECSREGION(result).ew_res;

  PrintResult("cln_SelectRegion",cln_SelectRegion(ClientID,&selectionRegion));

  selectionLayer.Select = (char *) layerSelection;
  selectionLayer.F = Line;
  PrintResult("cln_SelectLayer",cln_SelectLayer(ClientID,&selectionLayer));

  while (code) {
    code = PrintResult("cln_GetNextObject",cln_GetNextObject(ClientID));
  }

  PrintResult("cln_ReleaseLayer",cln_ReleaseLayer(ClientID,&selectionLayer));

  PrintResult("cln_DestroyClient",cln_DestroyClient(ClientID));

  return 1;
}

/* 
   The following function will make the analysis of the
   structure ecs_Result return by all API functions.
   He will print information of the contain of the
   structure. Of course, this function is incomplete,
   only error messages, region informations, text and
   line info will be print. If an error occur, the function
   will call exit() after the printing the message.
   */

int PrintResult(command,result)
     char *command;
     ecs_Result *result;
{
  unsigned int i;
  int code;

  code = TRUE;

  /* Print the command string */

  printf("\n%s\n\n",command);

  /* Check is a result is pass as an argument */

  if (result == NULL)
    printf("No structure returned\n");

  /* Check is the request is successful */

  if (ECSSUCCESS(result)) {

    /* Check the contain type of result */

    switch(ECSRESULTTYPE(result)) {
    case Object:
      /* result contain an geographical object. Now check
	      the contain if it's a line and print the line
	      geographical object.
         */
      if (ECSGEOMTYPE(result) == Line) {
        printf("Object ID:%s\n",ECSOBJECTID(result));
	     printf("Object Attributes:%s\n",ECSOBJECTATTR(result));
	     for(i=0;i<ECSGEOM(result).line.c.c_len;i++) {
	       printf("%d: (%f , %f)\n",i,
		           ECSGEOM(result).line.c.c_val[i].x,
		           ECSGEOM(result).line.c.c_val[i].y);
        }
      } 
      break;

    case GeoRegion:
      printf("Region: north=%f south=%f east=%f west=%f\n",
	     ECSREGION(result).north,ECSREGION(result).south, 
	     ECSREGION(result).east,ECSREGION(result).west);
      printf("Resolution: ns_resolution=%f ew_resolution=%f\n",
	     ECSREGION(result).ns_res,ECSREGION(result).ew_res);
      break;

    case AText:
      printf("AText: %s\n",ECSTEXT(result));
      break;
    
    case objAttributeFormat:
    case RasterInfo:
      printf("Unsupported feature...\n");
      break;
    
    default:
      break;
    };

  } else {
    /* The ecs_Result contain a error message */
    
    printf("%s\n",result->message);

    if (!ECSEOF(result)) {
      /* The error message is not an eof. Call exit() */
      exit(0);
    }
    code = FALSE;
  }

  printf("***************************************************\n");
  return code;
}

