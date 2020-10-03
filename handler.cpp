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

// $Revision: 13924 $ $Date:: 2020-10-03 #$ $Author: serge $

#include "handler.h"                // self

#include "utils/mutex_helper.h"      // MUTEX_SCOPE_LOCK
#include "utils/dummy_logger.h"      // dummy_log
#include "utils/utils_assert.h"      // ASSERT

#include "generic_protocol/object_initializer.h"        // generic_protocol::create_ErrorResponse
#include "user_reg_protocol/object_initializer.h"       // user_reg_protocol::create_RegisterUserResponse

#include "user_reg_protocol/protocol.h"    // user_reg_protocol::


#define MODULENAME      "user_reg_handler::Handler"

namespace user_reg_handler
{

Handler::Handler():
        user_reg_email_( nullptr )
{
}

bool Handler::init(
        user_reg_email::UserRegEmail    * user_reg_email )
{
    ASSERT( user_reg_email );

    MUTEX_SCOPE_LOCK( mutex_ );

    user_reg_email_   = user_reg_email;

    return true;
}

generic_protocol::BackwardMessage* Handler::handle( session_manager::user_id_t /*session_user_id*/, const user_reg_protocol::RegisterUserRequest & r )
{
    user_reg::user_id_t user_id;

    std::string error_msg;

    if( user_reg_email_->register_new_user(
            0,
            r.lead.email.email,
            r.password,
            & user_id,
            & error_msg
            ) )
    {
        return user_reg_protocol::create_RegisterUserResponse();
    }

    return generic_protocol::create_ErrorResponse( generic_protocol::ErrorResponse_type_e::RUNTIME_ERROR, error_msg );
}

generic_protocol::BackwardMessage* Handler::handle( session_manager::user_id_t /*session_user_id*/, const user_reg_protocol::ConfirmRegistrationRequest & r )
{
    std::string error_msg;

    if( user_reg_email_->confirm_registration( r.registration_key, & error_msg ) )
    {
        return user_reg_protocol::create_ConfirmRegistrationResponse();
    }

    return generic_protocol::create_ErrorResponse( generic_protocol::ErrorResponse_type_e::RUNTIME_ERROR, error_msg );
}

} // namespace user_reg_handler
