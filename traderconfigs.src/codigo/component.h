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

#define SIZE_SERVICE 1024

class Service{
public:
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	char			*service;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	void			init_service();
	int				new_service(int);
	void			free_service();
	char			*get_service();
	int				put_service(char*);
};

class Interface{
public:
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	int				i;
	Service		*interface;
	int		   	copy_interface(Interface&);
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	Interface();
	void			free_interface();
	void			init_interface();
	int				new_interface(int);
	int				pop_service(char*);
	int				push_service();
	char			*get_service(int);
	int 			put_interface(Interface&);
	Interface	get_interface();
};

class Component{
public:
	int 			index;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	Interface	O; // Output: interfaces
	Interface	I; // Input: interfaces
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	void			free_component();
	void			init_component();
	int 			put_interface_O(int,Component&);
	int 			Equal(char*, char*);
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	int 			inclusion_Interfaces(char*);
	int 			intersection_Interfaces(Component&,Component&);
	int				hide_Interfaces(Component&,Component&);
};

class arrayComponent{
protected:
	int			copy_arrayComponent(arrayComponent&);
public:
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	int				n;
	Component *C;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	arrayComponent();
	void			free_arrayComponent();
	int				new_arrayComponent(int);
	void			init_arrayComponent();
	int				pop_component(Component&,int);
	int				push_component();
	int				Composition(Component&);
	int 			inclusion_Interfaces(char*);
	int 			inclusion_Interfaces(Component&);
};

