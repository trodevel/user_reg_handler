#include <iostream>
#include <fstream>

#include "handler.h"        // user_reg_handler::Handler
#include "user_reg_email/user_reg_email.h"
#include "user_reg_protocol/object_initializer.h"
#include "basic_objects/object_initializer.h"

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
        std::cerr << "cannot read config file '" << filename << "' : " << e.what() << "\n";

        exit( EXIT_FAILURE );
    }
}

void init(
        user_manager::UserManager       * um,
        user_reg::UserReg               * ur,
        user_reg_email::UserRegEmail    * ure,
        user_reg_handler::Handler       * h,
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
    config2.body_template_filename   = body_template;

    ure->init( config2, ur );

    h->init( ure );
}

bool register_user_1(
        user_reg_handler::Handler       * h,
        const std::string               & email,
        user_reg::user_id_t             * user_id,
        std::string                     * error_msg )
{
    std::unique_ptr<user_reg_protocol::RegisterUserRequest> req( new user_reg_protocol::RegisterUserRequest );

    user_reg_protocol::User u;

    basic_objects::Email e;
    basic_objects::Date d;

    basic_objects::initialize( & e, email );
    basic_objects::initialize( & d, 2020, 9, 16 );

    user_reg_protocol::initialize( & u,
            basic_objects::gender_e::UNDEF,
            "johndoe",
            "Doe",
            "John",
            e,
            "+123",
            d );

    user_reg_protocol::initialize( req.get(), u, "secret" );

    auto resp = h->handle( 0, * req.get() );

    if( typeid( * resp ) == typeid( user_reg_protocol::RegisterUserResponse ) )
    {
        return true;
    }

    if( typeid( * resp ) == typeid( generic_protocol::ErrorResponse ) )
    {
        * error_msg = "error occurred: " + ( dynamic_cast<generic_protocol::ErrorResponse*>( resp ) )->descr;
        return false;
    }

    * error_msg = "unknown response: " + std::string( typeid( * resp ).name() );

    return false;
}

void test_kernel( const std::string & name, const std::string & config_file, const std::string & subject, const std::string & body_template )
{
    user_manager::UserManager   um;
    user_reg::UserReg ur;
    user_reg_email::UserRegEmail ure;
    user_reg_handler::Handler h;

    std::string to_email;

    init( & um, & ur, & ure, & h , & to_email, 1, config_file, subject, body_template );

    user_reg::user_id_t user_id;
    std::string         error_msg;

    auto b = register_user_1( & h, to_email, & user_id, & error_msg );

    log_test( name, b, true, "user was added", "cannot add user", error_msg );
}

void test_01_reg_ok_1()
{
    test_kernel( "test_01_reg_ok_1", "config_1.cfg", "Confirm your registration", "templ_01.txt" );
}

int main()
{
    test_01_reg_ok_1();

    return 0;
}
