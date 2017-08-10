/*
 * opencog/nlp/lg-dict/LGDictNode.cc
 *
 * Copyright (C) 2017 Linas Vepstas
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the
 * exceptions at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <opencog/nlp/types/atom_types.h>

#include "LGDictNode.h"

using namespace opencog;

LgDictNode::LgDictNode(const std::string& name)
	: Node(LG_DICT_NODE, name), _dict(nullptr)
{
}

LgDictNode::LgDictNode(const Node& n)
	: Node(n), _dict(nullptr)
{
	// Type must be as expected
	Type tdict = n.getType();
	if (not classserver().isA(tdict, LG_DICT_NODE))
	{
		const std::string& tname = classserver().getTypeName(tdict);
		throw InvalidParamException(TRACE_INFO,
			"Expecting an LgDictNode, got %s", tname.c_str());
	}
}

LgDictNode::~LgDictNode()
{
	if (_dict)
		dictionary_delete(_dict);

	_dict = nullptr;
}

/// Get the dictionary associated with the node.  This performs a
/// delayed open, because we don't really want the open to happen
/// in the constructor (since the constructor might run mmultiple
/// times!?)
Dictionary LgDictNode::get_dictionary()
{
	if (_dict) return _dict;

	const char * lang = getName().c_str();
	_dict = dictionary_create_lang(lang);
	return _dict;
}

Handle LgDictNode::factory(const Handle& base)
{
	if (LgDictNodeCast(base)) return base;
	Handle h(createLgDictNode(base->getName()));
	return h;
}

/* This runs when the shared lib is loaded. */
static __attribute__ ((constructor)) void init(void)
{
   classserver().addFactory(LG_DICT_NODE, &LgDictNode::factory);
}
