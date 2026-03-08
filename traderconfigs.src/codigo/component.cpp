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

#include "component.h"
#include <string.h>
#include <stdlib.h>

//---------------------------------------------------------------------
// class: Service
//---------------------------------------------------------------------

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Reserva memoria para un nuevo servicio
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Service::new_service(int size){
  if((service=new char [size+1])==NULL) return 1;
  return 0;
} // end Service::init_service()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: inicializa un servicio
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
void Service::init_service(){
  strcpy(service,"");
} // end Service::init_service()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: libera la memoria de un servicio
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
void Service::free_service(){
  delete [] service;
} // end Service::init_service()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Retorna el servicio
// Return		: servicio
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
char *Service::get_service(){
  return service;
} // end Service::get_service()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Escribe un servicio en el campo service de Service
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Service::put_service(char *_service){
  if(new_service(strlen(_service))==1) return 1;
  strcpy(service,_service);
  return 0;
} // end Service::put_service()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

//---------------------------------------------------------------------
// class: Interface
//---------------------------------------------------------------------

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Constructor de la clase Interface
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
Interface::Interface(){
  i=0;
} // end Interface::Interface()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Copia interface de la clase en exit: exit <- Interface
// Return		: 0 exito, 1 error
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Interface::copy_interface(Interface &exit){
  exit.i=i;
  for(int j=0;j<i;j++)
	  if (exit.interface[j].put_service(get_service(j))==1) return 1;
  return 0;
} // end Component::copy_interface()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: libera memoria para una interface
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
void Interface::free_interface(){
  if(i>=1) {
	  for(int aux=0;aux<i;aux++)
		 interface[aux].free_service();
	  delete [] interface;
  } i=0;
} // end Interface::free_interface()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: libera memoria para una interface
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
void Interface::init_interface(){
  i=0;
} // end Interface::init_interface()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Reserva memoria para una nueva interface
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Interface::new_interface(int size){
  if(size<0) return 1;
  if(size>=1){
	 if((interface=new Service [size])==NULL) return 1;
	 init_interface();
  }
  return 0;
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Incorpora un servicio _service a Interface
// Return		: 1 fallo por problemas de memoria, 0 exito
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Interface::pop_service(char *_service){
Interface aux_interface;
  if (aux_interface.new_interface(i)==1) return 1;
  if (copy_interface(aux_interface)==1) return 1;
  free_interface();
  if (new_interface(aux_interface.i+1)==1) return 1;
  if (aux_interface.copy_interface(*this)==1) return 1;
  if (interface[i].put_service(_service)==1) return 1;
  aux_interface.free_interface();
  i++; return 0;
} // end Interface::pop_service()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Elimina el ultimo servicio de Interface
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Interface::push_service(){
Interface aux_interface;
  if(i>=1){
	 if (aux_interface.new_interface(i)==1) return 1;
	 if (copy_interface(aux_interface)==1) return 1;
	 aux_interface.i--;
	 free_interface();
	 if (new_interface(aux_interface.i)==1) return 1;
	 if (aux_interface.copy_interface(*this)==1) return 1;
	 aux_interface.i++;
	 aux_interface.free_interface();
  }
  return 0;
} // end Interface::push_service()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Obtiene el servicio que ocupa la posicion index en Interface
// Return		: servicio
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
char *Interface::get_service(int index){
  return interface[index].get_service();
} // end Interface::get_service()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Escribe una Interface inter en las Output (O)
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Interface::put_interface(Interface &inter){
  if(inter.i>=1){
	 for(int aux=0;aux<inter.i;aux++)
		if(pop_service(inter.get_service(aux))==1) return 1;
  } return 0;
} // end Component::put_interface_O()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	:
// Return		:
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
Interface Interface::get_interface(){
  return *this;
} // end Interface::get_interface()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

//---------------------------------------------------------------------
// class: Component
//---------------------------------------------------------------------

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: libera la memoria de un componente
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
void Component::free_component(){
  O.free_interface();
  I.free_interface();
} // end Component::free_component()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: inicializa un componente
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
void Component::init_component(){
  O.init_interface();
  I.init_interface();
} // end Component::init_component()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Envuelve el componente 1 con inter
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Component::put_interface_O(int i, Component &wrapper){
  char number[2];
  char aux[SIZE_SERVICE];
  int j;
//  strcpy(aux,"C"); itoa(i+1,number,10); strcat(aux,number);
  if(wrapper.O.i>0){
	 strcat(aux,"-{");
	 for(j=0;j<wrapper.O.i-1;j++){
		strcat(aux,wrapper.O.get_service(j)); strcat(aux,",");
	 }
	 strcat(aux,wrapper.O.get_service(j)); strcat(aux,"}\x0");
  } // end
  if(O.pop_service(aux)==1) return 1; else return 0;
} // end Component::add_Solution()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

//---------------------------------------------------------------------
// class: arrayComponent
//---------------------------------------------------------------------

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Constructor de la clase arrayComponent
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
arrayComponent::arrayComponent(){
  n=0;
} // end arrayComponent::arrayComponent()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Inicializa una instancia de la clase arrayComponent
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
void arrayComponent::free_arrayComponent(){
	for(int aux=0;aux<n;aux++){
	  C[aux].O.free_interface();
	  C[aux].I.free_interface();
	}
	if(n>=1) delete [] C;
	n=0;
} // end arrayComponent::init_arrayComponent()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Crea size elementos Component para el campo *C
// Return		: 0 en caso de exito, 1 problemas de memoria
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int arrayComponent::new_arrayComponent(int size){
int aux;
  if(size>=1){
	 if((C=new Component [size])==NULL) return 1;
	 for(aux=0; aux<size;aux++)
		 C[aux].init_component();
  }
  n=0;
  return 0;
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Copia arrayComponent de la clase en exit
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int arrayComponent::copy_arrayComponent(arrayComponent &exit){
  exit.n=n;
  for(int aux=0; aux<n; aux++){
	 exit.C[aux].index=C[aux].index;
	 if (exit.C[aux].O.new_interface(C[aux].O.i)==1) return 1;
	 if (exit.C[aux].I.new_interface(C[aux].I.i)==1) return 1;
	 if (C[aux].O.copy_interface(exit.C[aux].O)==1) return 1;
	 if (C[aux].I.copy_interface(exit.C[aux].I)==1) return 1;
  } return 0;
} // end arrayComponent::copy_arrayComponent
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Incorpora un componente b en la lista de componentes
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int arrayComponent::pop_component(Component &b, int index){
arrayComponent aux_arrayComponent;
Interface inter;
  if (aux_arrayComponent.new_arrayComponent(n)==1) return 1;
  if (copy_arrayComponent(aux_arrayComponent)==1) return 1;
  free_arrayComponent();
  if (new_arrayComponent(aux_arrayComponent.n+1)==1) return 1;
  if (aux_arrayComponent.copy_arrayComponent(*this)==1) return 1;
  inter=b.O.get_interface();
  C[n].index=index;
  if (C[n].O.put_interface(inter)==1) return 1;
  inter=b.I.get_interface();
  if (C[n].I.put_interface(inter)==1) return 1;
  aux_arrayComponent.free_arrayComponent();
  n++; return 0;
} // end arrayComponent::pop_component(
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Elimina el ultimo componente de la lista de componentes
// Return		: ninguno
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int arrayComponent::push_component(){
arrayComponent aux_arrayComponent;
  if(n>=1){
	 if (aux_arrayComponent.new_arrayComponent(n)==1) return 1;
	 if (copy_arrayComponent(aux_arrayComponent)==1) return 1;
	 aux_arrayComponent.n--;
	 free_arrayComponent();
	 if (new_arrayComponent(aux_arrayComponent.n)==1) return 1;
	 if (aux_arrayComponent.copy_arrayComponent(*this)==1) return 1;
	 aux_arrayComponent.n++;
	 aux_arrayComponent.free_arrayComponent();
  }
  return 0;
} // end arrayComponent::push_component()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

