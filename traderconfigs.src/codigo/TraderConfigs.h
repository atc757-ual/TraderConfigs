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

class Configurations{
public:
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	Component Architecture;
	arrayComponent singleSolution;
	arrayComponent List;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	Configurations();
	~Configurations();
	void	free_configurations();
	int 	load_CandidateCollection(char*);
	int	load_ArchitectureComponent(char*);

	int	show_Solution(arrayComponent&,FILE*);
	int	backSolution(arrayComponent&);
	int	doSolution(int i, arrayComponent &Sol, char*);
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- //
	int	configs(int, arrayComponent&);
};
