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

#include <stdio.h>

void errores(int e){
  char *m[]={
		"config <archivo_candidatos> <archivo_arquitectura>",
		"No hay memoria disponible.",
		"Archivo inexistente o error en la lectura.",
		"Error al cargar la coleccion candidata."};
  printf("## ERROR %d: %s\n",e,m[e]);
}

