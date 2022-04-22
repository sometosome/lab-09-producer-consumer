// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_PARSER_HPP_
#define INCLUDE_PARSER_HPP_

#include <gumbo.h>
#include <sertificate.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/bind/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

struct Item {
  std::string host;
  std::string target;
};

struct Arguments {
  std::string url;
  size_t depth;
  size_t network_threads;
  size_t parser_threads;
  std::string output;
};

std::string get_page(std::string url, std::string target);
void find_links(std::string& sBody, std::vector<std::string>& vLinks);
void item_filling(std::vector<Item>& itemList,
                  std::vector<std::string>& vLinks);
std::string get_host(std::string& url);
std::string get_target(std::string& url);
void fulling_vector_html(std::vector<std::string>& vLinks, size_t depth);
void fulling_vector_img(std::vector<std::string>& vLinks,
                        std::vector<std::string>& vLinksImg, size_t depth);
void thread_start_parser(std::vector<std::vector<std::string>>& vector,
                         std::vector<std::string>& links, size_t depth,
                         size_t threadNum);
void thread_start_img(std::vector<std::string>& vector,
                      std::vector<std::string>& links, size_t depth,
                      size_t threadNum);

auto example() -> void;

#endif  // INCLUDE_PARSER_HPP_
