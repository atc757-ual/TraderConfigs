/***************************************************************************
    copyright            : (C) by Luis Iribarne
    email                : luis.iribarne@ual.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#include 	<string.h>
#include 	"component.h"
#include 	"error.h"
#include 	"TraderConfigs.h"

char 			*single_configurations	="single_configurations.out";
char			*long_configurations	="long_configurations.out";
int 			error, indice=0;
FILE 			*file1, *file2;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description : Carga en List la coleccion candidata desde archivo
// Return		   : 0 exito, 2 otro caso
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
void Configurations::free_configurations(){
	singleSolution.free_arrayComponent();
	Architecture.free_component();
	List.free_arrayComponent() ;
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description : Carga en List la coleccion candidata desde archivo
// Return		   : 0 exito, 2 otro caso
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Configurations::load_CandidateCollection(char *file){
char a[SIZE_SERVICE];
Component aux_component;
int index=0;
FILE *f;
	if((f=fopen(file,"r"))==NULL) return 2;
	fscanf(f,"%s",a);
	while(((aux_component.Equal(a,"O:"))!=1)&&(!feof(f)))
	  fscanf(f,"%s",a);
	if(aux_component.Equal(a,"O:")!=1) return 2;
	printf("\nO: ");
	for(;;){
	  fscanf(f,"%s",a);
	  while(((aux_component.Equal(a,"I:"))!=1)&&((aux_component.Equal(a,"O:"))!=1)){
		 		printf("%s ",a);
		 		if((aux_component.O.pop_service(a))==1) return 1;
		 		if(fscanf(f,"%s",a)==EOF) goto END_FILE;
	  }
	  if((aux_component.Equal(a,"O:"))!=1){
		 	if(fscanf(f,"%s",a)==EOF) goto END_FILE;
		 	printf("I: ");
		 	while((aux_component.Equal(a,"O:"))!=1){
			 		printf("%s ",a);
			 		if((aux_component.I.pop_service(a))==1) return 1;
			 		if(fscanf(f,"%s",a)==EOF) break;
		 	}
	  }
END_FILE:
    index++;
	  if((List.pop_component(aux_component, index))==1) return 1;
	  aux_component.init_component();
	  if(feof(f)) break;
	  printf("\nO: ");
	} // endfor
	return 0;
} // end _load_Component()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description : Carga en Architecture la arquitectura desde archivo
// Return		   : 0 exito, 2 otro caso
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Configurations::load_ArchitectureComponent(char *file){
char a[SIZE_SERVICE];
FILE *f;
	if((f=fopen(file,"r"))==NULL) return 2;
	fscanf(f,"%s",a);
	while(((Architecture.Equal(a,"O:"))!=1)&&(!feof(f)))
	  fscanf(f,"%s",a);
	if(Architecture.Equal(a,"O:")!=1) return 2;
	for(;;){
	  fscanf(f,"%s",a);
	  while(((Architecture.Equal(a,"I:"))!=1)&&((Architecture.Equal(a,"O:"))!=1)){
		 if((Architecture.O.pop_service(a))==1) return 1;
		 if(fscanf(f,"%s",a)==EOF) break;
	  }
	  if((Architecture.Equal(a,"O:"))!=1){
		 fscanf(f,"%s",a);
		 while(((Architecture.Equal(a,"O:"))!=1)&&(!feof(f))){
			 if((Architecture.I.pop_service(a))==1) return 1;
			 fscanf(f,"%s",a);
		 }
	  }
	  if(feof(f)) break;
	} // endfor
	return 0;
} // end _load_Component()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Muestra en pantalla una configuracion valida (y en disco)
// Return       : ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

int Configurations::show_Solution(arrayComponent &c, FILE *file){
  fprintf(file,"%d: ",++indice);
  for(int i=0;i<c.n;i++){
	 for(int j=0;j<c.C[i].O.i;j++){
	   fprintf(file,"C%d.%s ",c.C[i].index,c.C[i].O.get_service(j));
	 }
  }
  fprintf(file,"\n");
	return 0;
} // end _show_Solution()

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description : Constructor de la clase Configurations
// Return		   : ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
Configurations::Configurations(){
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description : Destructor de la clase Configurations
// Return		   : ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
Configurations::~Configurations(){
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description : Backtraking para la version 1 del algoritmo configs
// Return	     : ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Configurations::backSolution(arrayComponent &Sol){
  if(Sol.push_component()!=0) return 1;
  return 0;
} // end backsolution()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description : Do in Backtraking para la version 1 del algoritmo configs
// Return	     : ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Configurations::doSolution(int i, arrayComponent &Sol, char *service){
Component aux;
  aux.init_component();
  aux.O.pop_service(service);
  Sol.pop_component(aux,i);
  return 0;
} // end arrayComponent::doSolution()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //


/***************************************************************************
 *                                                                         *
 *    Algoritmo buscaConfiguraciones() presentado en JISBD2000             *
 *        V Jornadas de Ingenieria del Software y Bases de Datos           *
 *                       Valladolid, noviembre 2000                        *
 *                                                                         *
 ***************************************************************************/

int Configurations::configs(int i, arrayComponent &Sol){
  if(i<List.n){
		for(int j=0; j<List.C[i].O.i; j++){
			if(Sol.inclusion_Interfaces(List.C[i].O.get_service(j))==0){
				doSolution(i+1,Sol,List.C[i].O.get_service(j));
				if(Sol.inclusion_Interfaces(Architecture)==1){
			 		 show_Solution(Sol,file1);
				}
				else{
					 show_Solution(Sol,file2);
					 configs(i,Sol);
				}
				backSolution(Sol);
			} // endif
	 	} // endfor
	 	configs(i+1,Sol);
  } // endif
  return 0;
} // end _configs()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

/***************************************************************************
 *                                                                         *
 *   Para su funcionamiento, este programa requiere dos paramentros de     *
 *   entrada: 1 el nombre del archivo que contiene la especificacion de    *
 *   la coleccion de componentes candidatos; 2 el nombre del archivo que   *
 *   contiene la especificacion de la arquitectura. Los dos archivos de    *
 *   entrada deben ser de tipo texto. Vease archivo README.                *
 *                                                                         *
 ***************************************************************************/

//--------------------------------------------------------
//---- main program
//--------------------------------------------------------

int main(int argv, char *argc[]){
// -------------------------------------------------------
  Configurations cfg;
  arrayComponent Sol;
// -------------------------------------------------------
  if(argv!=3){
		 error=0;
		 goto EXIT;
  }
  if((file1=fopen(single_configurations,"w+"))==NULL) return 2;
  if((file2=fopen(long_configurations,"w+"))==NULL) return 2;
  Sol.n=0;
  printf("\n   Cargando lista de candidatos......");
  if((error=cfg.load_CandidateCollection(argc[1]))!=0) goto EXIT;
  printf("\n-- Lista de candidatos cargada.");
  printf("\n   Cargando arquitectura......");
  if((error=cfg.load_ArchitectureComponent(argc[2]))!=0) goto EXIT;
  printf("\n-- Arquitectura cargada.");
  printf("\n   Calculando las configuraciones..... (espere)");
  if((error=cfg.configs(0,Sol))!=0) goto EXIT;
  printf("\n-- Configuraciones validas en: single_configurations.out");
  printf("\n-- Configuraciones generadas : long_configurations.out");
  goto END;
EXIT:
  errores(error);
END:
  cfg.free_configurations();
  Sol.free_arrayComponent();
  fclose(file1); fclose(file2);
  printf("\nEl programa ha finalizado !!");
}


// (c) Luis Iribarne, 2000.