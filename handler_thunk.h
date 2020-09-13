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

#ifndef USER_REG_HANDLER__HANDLER_THUNK_H
#define USER_REG_HANDLER__HANDLER_THUNK_H

#include "generic_protocol/protocol.h"  // generic_protocol::BackwardMessage
#include "session_manager/types.h"          // session_manager::user_id_t

namespace generic_handler
{
class Handler;
}

namespace user_reg_handler
{

class Handler;

class HandlerThunk
{
public:

    HandlerThunk();

    bool init(
            unsigned int                        log_id,
            generic_handler::Handler            * generic_handler,
            Handler                             * handler );

    generic_protocol::BackwardMessage* handle( session_manager::user_id_t session_user_id, const basic_parser::Object * r );

private:

    generic_protocol::BackwardMessage* handle_RegisterUserRequest( session_manager::user_id_t session_user_id, const basic_parser::Object * r );
    generic_protocol::BackwardMessage* handle_ConfirmRegistrationRequest( session_manager::user_id_t session_user_id, const basic_parser::Object * r );

private:
    unsigned int                        log_id_;

    generic_handler::Handler            * generic_handler_;
    Handler                             * handler_;
};

} // namespace user_reg_handler

#endif // USER_REG_HANDLER__HANDLER_THUNK_H
