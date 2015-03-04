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

#include "aigameareafield.h"

/**
 * @brief CAIGameAreaField::CAIGameAreaField
 */
CAIGameAreaField::CAIGameAreaField()
{
    reset();
}

/**
 * @brief CAIGameAreaField::reset Reseting field
 */
void CAIGameAreaField::reset()
{
    rating = -1;
    mark = false;
}

/**
 * @brief CAIGameAreaField::setRating Setting fields rating.
 * @param value New rating of field. Value -1 is solid, 0-Inf is rating
 */
void CAIGameAreaField::setRating(Sint32 value)
{
    rating = value;
}

/**
 * @brief CAIGameAreaField::getRating
 * @return Returning rating of field.
 */
Sint32 CAIGameAreaField::getRating()
{
    return rating;
}

/**
 * @brief CAIGameAreaField::setMark
 * @param value
 */
void CAIGameAreaField::setMark(bool value)
{
    mark = value;
}

/**
 * @brief CAIGameAreaField::getMark
 * @return
 */
bool CAIGameAreaField::getMark()
{
    return mark;
}
