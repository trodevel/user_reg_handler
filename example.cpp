#include <iostream>
#include <fstream>

#include "handler.h"        // user_reg_handler::Handler
#include "user_reg_email/user_reg_email.h"

#include "utils/dummy_logger.h" // dummy_logger::set_log_level
#include "utils/log_test.h"     // log_test

bool read_config(
        std::string         * from,
        std::string         * from_name,
        std::string         * to,
        std::string         * host_name,
        uint32_t            * port,
        std::string         * user_name,
        std::string         * password,
        const std::string   & filename )
{
    try
    {
        std::ifstream file( filename );
        std::string l;

        std::getline( file, * host_name );
        std::getline( file, l );
        * port    = stoi( l );
        std::getline( file, * user_name );
        std::getline( file, * password );

        std::getline( file, * from );
        std::getline( file, * from_name );
        std::getline( file, * to );

        std::cerr << "DEBUG:" << "\n"
                << "filename  = " << filename << "\n"
                << "from      = " << * from << "\n"
                << "from name = " << * from_name << "\n"
                << "to        = " << * to << "\n"
                << "host_name = " << * host_name << "\n"
                << "port      = " << * port << "\n"
                << "user_name = " << * user_name << "\n"
                << "password  = " << "..." << "\n"
                << std::endl;

        return true;
    }
    catch( std::exception & e )
    {
        return false;
    }
}

void init(
        user_manager::UserManager       * um,
        user_reg::UserReg               * ur,
        user_reg_email::UserRegEmail    * ure,
        std::string                     * to_email,
        uint32_t expiration,
        const std::string               & config_file,
        const std::string               & subject,
        const std::string               & body_template )
{
    user_reg::Config config = { expiration };

    um->init();

    ur->init( config, um );

    user_reg_email::Config config2;

    read_config(
            & config2.sender_email,
            & config2.sender_name,
            to_email,
            & config2.host_name,
            & config2.port,
            & config2.username,
            & config2.password,
            config_file );

    config2.subject         = subject;
    config2.body_template   = body_template;

    ure->init( config2, ur );
}

bool register_user_1(
        user_reg_email::UserRegEmail    * ure,
        const std::string               & email,
        user_reg::user_id_t             * user_id,
        std::string                     * error_msg )
{
    return ure->register_new_user( 1, email, "\xff\xff\xff", user_id, error_msg );
}

void test_kernel( const std::string & name, const std::string & config_file, const std::string & subject, const std::string & body_template )
{
    user_manager::UserManager   um;
    user_reg::UserReg ur;
    user_reg_email::UserRegEmail ure;

    std::string to_email;

    init( & um, & ur, & ure, & to_email, 1, config_file, subject, body_template );

    user_reg::user_id_t user_id;
    std::string         error_msg;

    auto b = register_user_1( & ure, to_email, & user_id, & error_msg );

    log_test( name, b, true, "user was added", "cannot add user", error_msg );
}

void test_01_reg_ok_1()
{
    test_kernel( "test_01_reg_ok_1", "config_1.cfg", "Confirm your registration", "You registration key - $REGISTRATION_KEY" );
}

int main()
{
    user_reg_handler::Handler h;

    return 0;
}
