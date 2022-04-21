// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_PARSER_HPP_
#define INCLUDE_PARSER_HPP_

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <utility>
#include <gumbo.h>
#include <queue>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <string>
#include <fstream>
#include <regex>
#include <thread>

#include <sertificate.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

struct Item {
  std::string host;
  std::string target;
};

struct Arguments{
  std::string url;
  size_t depth;
  size_t network_threads;
  size_t parser_threads;
  std::string output;
};

std::string get_page(std::string url, std::string target);
void find_links(std::string& sBody, std::vector<std::string>& vLinks);
void item_filling(std::vector<Item>& itemList, std::vector<std::string>& vLinks);
std::string get_host(std::string &url);
std::string get_target(std::string &url);
void fulling_vector(std::vector<std::string>& vLinks, size_t depth);
void thread_start(std::vector<std::vector<std::string>>& vector, std::vector<std::string>& links, size_t depth);

#endif // INCLUDE_PARSER_HPP_
