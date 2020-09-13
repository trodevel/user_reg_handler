/*

Permission checker.

Copyright (C) 2020 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Revision: 13806 $ $Date:: 2020-09-13 #$ $Author: serge $

#include "perm_checker_thunk.h"         // self

#include <typeindex>                    // std::type_index
#include <unordered_map>

#include "generic_protocol/protocol.h"      // generic_protocol::
#include "user_reg_protocol/protocol.h"     // user_reg_protocol::


#include "utils/dummy_logger.h"      // dummy_log
#include "utils/utils_assert.h"      // ASSERT

#include "perm_checker.h"               // PermChecker

#define MODULENAME      "user_reg_handler::PermCheckerThunk"

namespace user_reg_handler
{

PermCheckerThunk::PermCheckerThunk()
{
}

bool PermCheckerThunk::init()
{
    return true;
}

bool PermCheckerThunk::is_authenticated( session_manager::user_id_t * session_user_id, const basic_parser::Object * req )
{
    return true;
}

bool PermCheckerThunk::is_allowed( session_manager::user_id_t session_user_id, const basic_parser::Object * req )
{
    typedef PermCheckerThunk Type;

    typedef bool (Type::*PPMF)( session_manager::user_id_t session_user_id, const basic_parser::Object * rr );

#define MAP_ENTRY(_v)  { typeid( user_reg_protocol::_v ),      & Type::is_allowed_##_v }


    static const std::unordered_map<std::type_index, PPMF> funcs =
    {
        MAP_ENTRY( RegisterUserRequest ),
        MAP_ENTRY( ConfirmRegistrationRequest ),
    };

    auto it = funcs.find( typeid( * req ) );

    if( it == funcs.end() )
    {
        dummy_log_fatal( MODULENAME, "is_allowed: cannot cast request to known type - %s", typeid( *req ).name() );

        ASSERT( 0 );

        return false;
    }

    return (this->*it->second)( session_user_id, req );
}

bool PermCheckerThunk::is_allowed_RegisterUserRequest( session_manager::user_id_t session_user_id, const basic_parser::Object * rr )
{
    return handler_->is_allowed( session_user_id, dynamic_cast< const user_reg_protocol::RegisterUserRequest &>( * rr ) );
}

bool PermCheckerThunk::is_allowed_ConfirmRegistrationRequest( session_manager::user_id_t session_user_id, const basic_parser::Object * rr )
{
    return handler_->is_allowed( session_user_id, dynamic_cast< const user_reg_protocol::RegisterUserRequest &>( * rr ) );
}

} // namespace user_reg_handler
