/***********************************************************************\
|                                                                       |
| IMigrator.h                                                           |
|                                                                       |
| This file is part of MicroGP v3 (ugp3)                                |
| http://ugp3.sourceforge.net/                                          |
|                                                                       |
| Copyright (c) 2006-2016 Giovanni Squillero                            |
|                                                                       |
|-----------------------------------------------------------------------|
|                                                                       |
| This program is free software; you can redistribute it and/or modify  |
| it under the terms of the GNU General Public License as published by  |
| the Free Software Foundation, either version 3 of the License, or (at |
| your option) any later version.                                       |
|                                                                       |
| This program is distributed in the hope that it will be useful, but   |
| WITHOUT ANY WARRANTY; without even the implied warranty of            |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      |
| General Public License for more details                               |
|                                                                       |
|***********************************************************************'
| $Revision: 644 $
| $Date: 2015-02-23 14:50:30 +0100 (Mon, 23 Feb 2015) $
\***********************************************************************/

#include "ugp3_config.h"
#include "IMigrator.h"
#include "EvolutionaryCore.h"
// ADD HERE: header files for each new type of migrator

using namespace ugp3::core;
using namespace std;

const string IMigrator::XML_NAME = "migration";

IMigrator::~IMigrator()
{ }

IMigrator* IMigrator::instantiate(	const EvolutionaryAlgorithm& parent,
			const string& type)
{
	IMigrator* migrator = nullptr;

	if( type == ClassicalMigrator::XML_SCHEMA_TYPE )
	{
		migrator = new ClassicalMigrator();
	}
	// ADD HERE: a new 'if' block for each new type of migrator

	if( migrator == nullptr)
	{
        	throw Exception("Migrator type '" + type + "' unknown.", LOCATION);
	}

	return migrator;
}
