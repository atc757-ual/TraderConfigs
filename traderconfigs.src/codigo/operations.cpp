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

//----------------------------------------------------------------------
// ---- OPERATIONS
//----------------------------------------------------------------------

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Comprueba si dos servicios son iguales
// Return		: 1 si s1=s2, 0 otro caso
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Component::Equal(char *service1, char *service2){
int i=0;
  for(;;){
	 if(service1[i]!=service2[i]) return 0;
	 if((service1[i]=='\x0')&&(service2[i]=='\x0')) break;
	 i++;
  } return 1;
} // end Component::Equal()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: �servicio i est� incluido en Interface O de la clase?
// Return		: 1 si, 0 en otro caso
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Component::inclusion_Interfaces(char *service) {
  for(int j=0;j<O.i;j++)
	 if(Equal(service,O.get_service(j))==1) return 1;
  return 0;
} // end Component::inclusion_Interfaces()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Interseccion Interface O de c y Interface O de la clase
// Return		: Componente interseccion: s.O.i es -1 si problemas memoria
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Component::intersection_Interfaces(Component &c, Component &s){
int aux1=0, aux2=0;
  if((O.i==0)||(c.O.i==0)) return 0;
  while(aux1<O.i){
	 while(aux2<c.O.i){
		if(Equal(O.get_service(aux1),c.O.get_service(aux2))==1){
		  if((s.O.pop_service(O.get_service(aux1)))==1) return 1;
		  break;
		} else aux2++;
	 } // end while j
	 aux1++; aux2=0;
  } // end while i
  return 0;
} // end Component::intersection_Interfaces()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: (Wrapper) Oculta Interface O de la clase con Interface O de c
// Return		: Componente envuelto con c: s.O.i es -1 si problemas memoria
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int Component::hide_Interfaces(Component &c, Component &s){
int i;
  if(c.O.i==0) {
	 for(i=0;i<O.i;i++)
		if(s.O.pop_service(O.get_service(i))==1) return 1;
  }
  else {
	 for(i=0;i<O.i;i++){
		if (c.inclusion_Interfaces(O.get_service(i))!=1)
			if(s.O.pop_service(O.get_service(i))==1) return 1;
	 }
  } return 0;
} // end Component::hide_Interfaces()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: Composicion de los componentes de la lista
// Return		: Componente resultado de la composicion: s.O.i es -1 si problemas memoria
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int arrayComponent::Composition(Component &aux){
  for(int i=0;i<n;i++)
	 for(int j=0;j<C[i].O.i;j++){
		if(aux.inclusion_Interfaces(C[i].O.get_service(j))==0)
		  if((aux.O.pop_service(C[i].O.get_service(j)))==1) return 1;
	 }
  return 0;
} // end arrayComponent::Composition()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
// Description	: �Interface O de la clase est� incluido en c?
// Return		: 1 Si, -1 error de memoria, 0 en otro caso
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
int arrayComponent::inclusion_Interfaces(Component &c){
int i=0, j=0, level=0, total=0;
char *service1, *service2;
for(level=0;level<n;level++){
  if(c.O.i<C[level].O.i) return 0; // c1 isn't included in c2;
  else {
	 while(i<C[level].O.i){
		while(j<c.O.i){
			service1=C[level].O.get_service(i);
			service2=c.O.get_service(j);
			if(C[level].Equal(service1,service2)==1){
				 total++;
				 break;
			}
			j++;
		} // end while j
		i++; j=0;
	 } // end while i
  } // end else
	j=0; i=0;
}
if(total==c.O.i) return 1;
else return 0;
} // end Component::inclusion_Interfaces()
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //


int arrayComponent::inclusion_Interfaces(char *service){
int i;
  for(i=0;i<n;i++)
    if(C[i].inclusion_Interfaces(service)==1) return 1;
	return 0;
}
