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

#ifndef USER_REG_HANDLER__PERM_CHECKER_H
#define USER_REG_HANDLER__PERM_CHECKER_H

#include "session_manager/types.h"      // session_manager::user_id_t
#include "user_reg_protocol/protocol.h"     // user_reg_protocol::

namespace user_reg_handler
{

class PermChecker
{
public:

    PermChecker();

    bool init();

    bool is_allowed( session_manager::user_id_t session_user_id, const user_reg_protocol::RegisterUserRequest & r );
    bool is_allowed( session_manager::user_id_t session_user_id, const user_reg_protocol::ConfirmRegistrationRequest & r );

private:
};

} // namespace user_reg_handler

#endif // USER_REG_HANDLER__PERM_CHECKER_H
