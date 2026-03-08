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

//--------------------------------------------------------
// ---- OPERATIONS
//--------------------------------------------------------

int Equal(char s1[MAX_INTERFACE], char s2[MAX_INTERFACE]);
int inclusion_Interfaz(char i[MAX_INTERFACE], Component *I);
int inclusion_Interfaces(Component *c1, Component *c2);
int intersection_Interfaces(Component *c1, Component *c2, Component *s);
void hide_Interfaces(Component *c1, Component *c2, Component *s);
void Composition(arrayComponent *a, Component *b);
