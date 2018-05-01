/**
 * This file is part of OpenAtomic.
 *
 * OpenAtomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * OpenAtomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenAtomic. If not, see <http://www.gnu.org/licenses/>.
 */

#include "tokenizer.h"

const string CTokenizer::DELIMITERS(" \t\n\r");

CTokenizer::CTokenizer(const string& s) :
	m_string(s),
	m_offset(0),
	m_delimiters(DELIMITERS) {}

CTokenizer::CTokenizer(const string& s, const string& delimiters) :
	m_string(s),
	m_offset(0),
	m_delimiters(delimiters) {}

bool CTokenizer::NextToken()
{
	return NextToken(m_delimiters);
}

bool CTokenizer::NextToken(const string& delimiters)
{
	size_t i = m_string.find_first_not_of(delimiters, m_offset);
	if (string::npos == i)
	{
		m_offset = m_string.length();
		return false;
	}

	size_t j = m_string.find_first_of(delimiters, i);
	if (string::npos == j)
	{
		m_token = m_string.substr(i);
		m_offset = m_string.length();
		return true;
	}

	m_token = m_string.substr(i, j - i);
	m_offset = j;
	return true;
}

const string CTokenizer::GetToken() const
{
	return m_token;
}


