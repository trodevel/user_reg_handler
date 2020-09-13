/*

User Reg request handler.

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

// $Revision: 13805 $ $Date:: 2020-09-13 #$ $Author: serge $

#include "handler_thunk.h"          // self

#include <typeindex>                // std::type_index
#include <unordered_map>

#include "utils/dummy_logger.h"      // dummy_log
#include "utils/utils_assert.h"      // ASSERT

#include "user_reg_protocol/protocol.h"    // user_reg_protocol::

#include "generic_handler/handler.h"                // generic_handler::Handler

#include "handler.h"                                // Handler

#define MODULENAME      "user_reg_handler::HandlerThunk"

namespace user_reg_handler
{

HandlerThunk::HandlerThunk():
        log_id_( 0 ),
        generic_handler_( nullptr ),
        handler_( nullptr )
{
}

bool HandlerThunk::init(
        unsigned int                        log_id,
        generic_handler::Handler            * generic_handler,
        Handler                             * handler )
{
    ASSERT( generic_handler );
    ASSERT( handler );

    log_id_             = log_id;
    generic_handler_    = generic_handler;
    handler_            = handler;

    return true;
}

generic_protocol::BackwardMessage* HandlerThunk::handle( session_manager::user_id_t session_user_id, const basic_parser::Object * req )
{
    typedef HandlerThunk Type;

    typedef generic_protocol::BackwardMessage* (Type::*PPMF)( session_manager::user_id_t session_user_id, const basic_parser::Object * r );

#define HANDLER_MAP_ENTRY(_v)       { typeid( user_reg_protocol::_v ),        & Type::handle_##_v }

    static const std::unordered_map<std::type_index, PPMF> funcs =
    {
        HANDLER_MAP_ENTRY( RegisterUserRequest ),
        HANDLER_MAP_ENTRY( ConfirmRegistrationRequest ),
    };

#undef HANDLER_MAP_ENTRY

    auto it = funcs.find( typeid( * req ) );

    if( it == funcs.end() )
    {
        return generic_handler_->handle( session_user_id, req );
    }

    return (this->*it->second)( session_user_id, req );
}

generic_protocol::BackwardMessage* HandlerThunk::handle_RegisterUserRequest( session_manager::user_id_t session_user_id, const basic_parser::Object * rr )
{
    return handler_->handle( session_user_id, dynamic_cast< const user_reg_protocol::RegisterUserRequest &>( * rr ) );
}

generic_protocol::BackwardMessage* HandlerThunk::handle_ConfirmRegistrationRequest( session_manager::user_id_t session_user_id, const basic_parser::Object * rr )
{
    return handler_->handle( session_user_id, dynamic_cast< const user_reg_protocol::ConfirmRegistrationRequest &>( * rr ) );
}

} // namespace user_reg_handler
