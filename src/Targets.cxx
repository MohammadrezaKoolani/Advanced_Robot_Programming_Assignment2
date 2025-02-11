// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file Targets.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
}  // namespace
#endif  // _WIN32

#include "Targets.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

Targets::Targets()
{
    // m_targets_x com.eprosima.idl.parser.typecode.SequenceTypeCode@2fd66ad3

    // m_targets_y com.eprosima.idl.parser.typecode.SequenceTypeCode@5d11346a

    // m_targets_number com.eprosima.idl.parser.typecode.PrimitiveTypeCode@7a36aefa
    m_targets_number = 0;

}

Targets::~Targets()
{


}

Targets::Targets(
        const Targets& x)
{
    m_targets_x = x.m_targets_x;
    m_targets_y = x.m_targets_y;
    m_targets_number = x.m_targets_number;
}

Targets::Targets(
        Targets&& x)
{
    m_targets_x = std::move(x.m_targets_x);
    m_targets_y = std::move(x.m_targets_y);
    m_targets_number = x.m_targets_number;
}

Targets& Targets::operator =(
        const Targets& x)
{

    m_targets_x = x.m_targets_x;
    m_targets_y = x.m_targets_y;
    m_targets_number = x.m_targets_number;

    return *this;
}

Targets& Targets::operator =(
        Targets&& x)
{

    m_targets_x = std::move(x.m_targets_x);
    m_targets_y = std::move(x.m_targets_y);
    m_targets_number = x.m_targets_number;

    return *this;
}

bool Targets::operator ==(
        const Targets& x) const
{

    return (m_targets_x == x.m_targets_x && m_targets_y == x.m_targets_y && m_targets_number == x.m_targets_number);
}

bool Targets::operator !=(
        const Targets& x) const
{
    return !(*this == x);
}

size_t Targets::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    current_alignment += (100 * 4) + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    current_alignment += (100 * 4) + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

size_t Targets::getCdrSerializedSize(
        const Targets& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    if (data.targets_x().size() > 0)
    {
        current_alignment += (data.targets_x().size() * 4) + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);
    }



    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);

    if (data.targets_y().size() > 0)
    {
        current_alignment += (data.targets_y().size() * 4) + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);
    }



    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

void Targets::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{

    scdr << m_targets_x;
    scdr << m_targets_y;
    scdr << m_targets_number;

}

void Targets::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_targets_x;
    dcdr >> m_targets_y;
    dcdr >> m_targets_number;
}

/*!
 * @brief This function copies the value in member targets_x
 * @param _targets_x New value to be copied in member targets_x
 */
void Targets::targets_x(
        const std::vector<int32_t>& _targets_x)
{
    m_targets_x = _targets_x;
}

/*!
 * @brief This function moves the value in member targets_x
 * @param _targets_x New value to be moved in member targets_x
 */
void Targets::targets_x(
        std::vector<int32_t>&& _targets_x)
{
    m_targets_x = std::move(_targets_x);
}

/*!
 * @brief This function returns a constant reference to member targets_x
 * @return Constant reference to member targets_x
 */
const std::vector<int32_t>& Targets::targets_x() const
{
    return m_targets_x;
}

/*!
 * @brief This function returns a reference to member targets_x
 * @return Reference to member targets_x
 */
std::vector<int32_t>& Targets::targets_x()
{
    return m_targets_x;
}
/*!
 * @brief This function copies the value in member targets_y
 * @param _targets_y New value to be copied in member targets_y
 */
void Targets::targets_y(
        const std::vector<int32_t>& _targets_y)
{
    m_targets_y = _targets_y;
}

/*!
 * @brief This function moves the value in member targets_y
 * @param _targets_y New value to be moved in member targets_y
 */
void Targets::targets_y(
        std::vector<int32_t>&& _targets_y)
{
    m_targets_y = std::move(_targets_y);
}

/*!
 * @brief This function returns a constant reference to member targets_y
 * @return Constant reference to member targets_y
 */
const std::vector<int32_t>& Targets::targets_y() const
{
    return m_targets_y;
}

/*!
 * @brief This function returns a reference to member targets_y
 * @return Reference to member targets_y
 */
std::vector<int32_t>& Targets::targets_y()
{
    return m_targets_y;
}
/*!
 * @brief This function sets a value in member targets_number
 * @param _targets_number New value for member targets_number
 */
void Targets::targets_number(
        int32_t _targets_number)
{
    m_targets_number = _targets_number;
}

/*!
 * @brief This function returns the value of member targets_number
 * @return Value of member targets_number
 */
int32_t Targets::targets_number() const
{
    return m_targets_number;
}

/*!
 * @brief This function returns a reference to member targets_number
 * @return Reference to member targets_number
 */
int32_t& Targets::targets_number()
{
    return m_targets_number;
}


size_t Targets::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;



    return current_align;
}

bool Targets::isKeyDefined()
{
    return false;
}

void Targets::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
       
}
