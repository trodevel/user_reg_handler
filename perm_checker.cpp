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

#include "perm_checker.h"               // self

#include "utils/dummy_logger.h"      // dummy_log
#include "utils/utils_assert.h"      // ASSERT

#define MODULENAME      "user_reg_handler::PermChecker"

namespace user_reg_handler
{

PermChecker::PermChecker()
{
}

bool PermChecker::init()
{
    return true;
}

bool PermChecker::is_allowed( session_manager::user_id_t /*session_user_id*/, const user_reg_protocol::RegisterUserRequest & /*r*/ )
{
    return true;
}

bool PermChecker::is_allowed( session_manager::user_id_t /*session_user_id*/, const user_reg_protocol::ConfirmRegistrationRequest & /*r*/ )
{
    return true;
}

} // namespace user_reg_handler
