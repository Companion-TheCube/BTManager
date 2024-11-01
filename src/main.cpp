#include "main.h"

int main(int argc, char** argv) {
    // check command line arguments for the address that the server will listen on
    std::string versionInfo = "Bluetooth http manager version ";
#ifdef SW_VERSION
    versionInfo += SW_VERSION;
#else
    versionInfo += PROJECT_VERSION_STRING;
#endif
    versionInfo += "\n";
    versionInfo += "Built on: " + std::string(__DATE__) + " " + std::string(__TIME__) + "\n";
    versionInfo += "Built on: ";
#ifdef __linux__
    versionInfo += "Linux";
#endif
#ifdef _WIN32
    versionInfo += "Windows";
#endif
    versionInfo += "\nCompiler: ";
#ifdef __clang__
    versionInfo += "Clang";
#endif
#ifdef __GNUC__
    versionInfo += "GCC";
#endif
#ifdef _MSC_VER
    versionInfo += "MSVC";
#endif
    versionInfo += "\nC++ Standard: ";
#ifdef __cplusplus
#ifdef _MSVC_LANG
    if (_MSVC_LANG == 202300L)
        versionInfo += "C++23";
    else if (_MSVC_LANG == 202100L)
        versionInfo += "C++21";
    else if (_MSVC_LANG == 202002L)
        versionInfo += "C++20";
    else if (_MSVC_LANG == 201703L)
        versionInfo += "C++17";
    else if (_MSVC_LANG == 201402L)
        versionInfo += "C++14";
    else if (_MSVC_LANG == 201103L)
        versionInfo += "C++11";
    else
        versionInfo += std::to_string(_MSVC_LANG);
#else
    if (__cplusplus == 202300L)
        versionInfo += "C++23";
    else if (__cplusplus == 202100L)
        versionInfo += "C++21";
    else if (__cplusplus == 202002L)
        versionInfo += "C++20";
    else if (__cplusplus == 201703L)
        versionInfo += "C++17";
    else if (__cplusplus == 201402L)
        versionInfo += "C++14";
    else if (__cplusplus == 201103L)
        versionInfo += "C++11";
    else
        versionInfo += std::to_string(__cplusplus);
#endif
#endif
    versionInfo += "\nAuthor: Andrew McDaniel\nCopyright: 2024\n";
    argparse::ArgumentParser argumentParser(PROJECT_NAME, versionInfo);

    argumentParser.add_argument("-a", "--address")
            .help("The address that the server will listen on")
#ifdef _WIN32
            .default_value("localhost");
#else
            .default_value("/tmp/bt_http_manager.sock");
#endif
    argumentParser.add_argument("-p", "--port")
            .help("The port that the server will listen on")
            .default_value(55222)
            .action([](const std::string& value) { return std::stoi(value); });
    
    argumentParser.add_argument("-v", "--version")
            .help("Print the version information")
            .default_value(false)
            .implicit_value(true);
    
    argumentParser.add_argument("-d", "--debug")
            .help("Print debug information")
            .default_value(false)
            .implicit_value(true);
    argumentParser.add_argument("-u", "--auth_token")
            .help("Use the provided authentication token")
            .default_value("");
    argumentParser.add_argument("-t", "--auth_file")
            .help("Use the authentication token from the provided file")
            .default_value("");
    argumentParser.add_argument("-b", "--auth_cb")
            .help("Use the provided http callback to authenticate")
            .default_value("");
    argumentParser.add_argument("-s", "--single_instance")
            .help("Only allow a single instance of the server to run")
            .default_value(false)
            .implicit_value(true);

    try {
        argumentParser.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << argumentParser;
        return 0;
    }

    if (argumentParser.get<bool>("version")) {
        std::cout << versionInfo << std::endl;
        return 0;
    }

    bool debugEnabled = false;
    if (argumentParser.get<bool>("debug")) {
        std::cout << "Debugging enabled" << std::endl;
        debugEnabled = true;
    }

    // parse authentication arguments
    std::string authToken;
    if (argumentParser.get("auth_token") != "") {
        authToken = argumentParser.get<std::string>("auth_token");
    } else if (argumentParser.get("auth_file") != "") {
        std::filesystem::path authFilePath(argumentParser.get<std::string>("auth_file"));
        if(!std::filesystem::exists(authFilePath)) {
            std::cout << "Authentication file does not exist: " << argumentParser.get<std::string>("auth_file") << std::endl;
            return 0;
        }
        std::ifstream authFile(authFilePath);
        if (authFile.is_open()) {
            std::getline(authFile, authToken);
            authFile.close();
        } else {
            std::cout << "Unable to open authentication file: " << argumentParser.get<std::string>("auth_file") << std::endl;
            return 0;
        }
    } else if (argumentParser.get("auth_cb") != "") {
        // the argument is an http callback that will provide the authentication token
        std::cout << "Using http callback for authentication" << std::endl;
        httplib::Client cli(argumentParser.get<std::string>("auth_cb"));
        auto res = cli.Get("/auth");
        if (res && res->status == 200) {
            authToken = res->body;
        } else {
            std::cout << "Unable to get authentication token from callback" << std::endl;
            return 0;
        }
    } else {
        std::cout << "No authentication token provided" << std::endl;
    }


    std::string address = argumentParser.get<std::string>("address");
    int port = argumentParser.get<int>("port");

    // create a server
    httplib::Server svr;

    // create a route
    svr.Get("/", [authToken](const httplib::Request& req, httplib::Response& res) {
        if(!authToken.empty()) {
            // get Authorization header
            auto authHeader = req.get_header_value("Authorization");
            if (authHeader != "Bearer " + authToken) {
                res.status = 401;
                res.set_content("Unauthorized", "text/plain");
                return;
            }
        }
        res.set_content("Hello World!", "text/plain");
    });

    auto serverThread = std::jthread([&svr, address, port]() {
        svr.listen(address, port);
    });

    std::cout << "Server started on " << address << ":" << port << std::endl;

    std::string input;
    while (input != "exit" && input != "quit" && input != "q" && input != "x") {
        std::cin >> input;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // stop the server
    svr.stop();
    serverThread.join();

    return 0;
}