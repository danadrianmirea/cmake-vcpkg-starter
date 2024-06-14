#pragma once

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_tls_client> websocket_client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

class Client {

    private:
    websocket_client _client;
    std::string _uri;

    public:
    Client(std::string uri): 
        _uri{uri}, _client() {}

    void on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
        _client.get_alog().write(websocketpp::log::alevel::app, "Received message: " + msg->get_payload());
    }

    void on_open(websocketpp::connection_hdl hdl) {
        _client.get_alog().write(websocketpp::log::alevel::app, "Opened connection");
    }

    void on_close(websocketpp::connection_hdl hdl) {
        _client.get_alog().write(websocketpp::log::alevel::app, "Closed connection");
    }

    void on_fail(websocketpp::connection_hdl hdl) {
        _client.get_alog().write(websocketpp::log::alevel::app, "Failed to open connection");
    }

    std::shared_ptr<boost::asio::ssl::context> on_tls_init(websocketpp::connection_hdl hdl) {
	auto ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tls);
	// ctx->set_default_verify_paths();
	// ctx->set_verify_mode(boost::asio::ssl::verify_peer);
	return ctx;
    }

    void run() {
        try {
            _client.clear_access_channels(websocketpp::log::alevel::frame_header);
            _client.clear_access_channels(websocketpp::log::alevel::frame_payload);
            _client.init_asio();
            _client.set_open_handler([this](websocketpp::connection_hdl hdl) { this->on_open(hdl); });
            _client.set_fail_handler([this](websocketpp::connection_hdl hdl) { this->on_fail(hdl); });
            _client.set_message_handler([this](websocketpp::connection_hdl hdl, message_ptr msg) { this->on_message(hdl, msg); });
            _client.set_close_handler([this](websocketpp::connection_hdl hdl) { this->on_close(hdl); });
            _client.set_tls_init_handler([this](websocketpp::connection_hdl hdl) { return this->on_tls_init(hdl); });

            websocketpp::lib::error_code ec;
            websocket_client::connection_ptr con = _client.get_connection(_uri, ec);
            _client.connect(con);
            _client.run();
        } catch (const std::exception & e) {
            std::cout << e.what() << std::endl;
        } catch (websocketpp::lib::error_code e) {
            std::cout << e.message() << std::endl;
        } catch (...) {
            std::cout << "other exception" << std::endl;
        }
    }

    void shutdown() {
        _client.stop();
    }
};
