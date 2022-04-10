// Copyright 2021 Your Name <your_email>

#include <stdexcept>
#include <parser.hpp>

std::string getpage(int argc, char** argv) {
  auto const host = argv[1];
  auto const port = argv[2];
  auto const target = argv[3];
  int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;

  boost::asio::io_context ioc;
  ssl::context ctx{ ssl::context::sslv23_client };
  load_root_certificates(ctx);
  tcp::resolver resolver{ioc};
  ssl::stream<tcp::socket> stream{ ioc, ctx };
  if (!SSL_set_tlsext_host_name(stream.native_handle(), host))
  {
    boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
    throw boost::system::system_error{ ec };
  }
  auto const results = resolver.resolve(host, port);
  boost::asio::connect(stream.next_layer(), results.begin(), results.end());
  stream.handshake(ssl::stream_base::client);
  http::request<http::string_body> req{http::verb::get, target, version};
  req.set(http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  http::write(stream, req);
  boost::beast::flat_buffer buffer;
  http::response<http::dynamic_body> res;
  http::read(stream, buffer, res);

  boost::system::error_code ec;
  if(ec && ec != boost::system::errc::not_connected)
    throw boost::system::system_error{ec};
  return boost::beast::buffers_to_string(res.body().data());
}

std::string getHost(std::string &url) {
  std::string host;
  int64_t skipHTTP = 0;
  int64_t skipHTTPS = 0;
  int64_t pos = 0;
  skipHTTPS = url.find("https");
  skipHTTP = url.find("http");
  if (skipHTTPS != -1)pos += skipHTTPS + 3 + 5;
  else if (skipHTTP != -1)pos += skipHTTP + 3 + 4;
  int64_t endOfHost = url.find('/', pos);
  if (endOfHost == -1)endOfHost = url.size();
  for (int64_t i = pos; i < endOfHost; ++i)
    host.push_back(url[i]);
  return host;
}

std::string getTarget(std::string &url) {
  std::string target;
  int64_t www = url.find("www");
  int64_t skipWWW = 0;
  if (www != -1) skipWWW = www + 2;
  int64_t endOfHost = url.find('.', skipWWW);
  int64_t targetStartPos = url.find('/', endOfHost);
  for (uint64_t i = targetStartPos; i < url.size(); ++i) {
    target.push_back(url[i]);//по буквам заносим в target
  }
  if (target[target.size() - 1] != '/') target.push_back('/');
  return target;
}

void item_filling(std::vector<Item>& itemList, std::vector<std::string>& vLinks)
{
  Item temp = { "", "" };
  for(size_t i = 0; i < vLinks.size(); i++)
  {
    temp.host = getHost(vLinks[i]);
    temp.target = getTarget(vLinks[i]);
    itemList.push_back(temp);
  }
}

void findLinks(std::string const& sBody, std::vector<std::string>& vLinks)
{
  std::regex rUri{ "^(?:(https?)://)?([^/]+)(/.*)?" };
  GumboOutput* output = gumbo_parse(sBody.c_str());

  std::queue<GumboNode*> qn;
  qn.push(output->root);

  while (!qn.empty())
  {
    GumboNode* node = qn.front();
    qn.pop();

    if (GUMBO_NODE_ELEMENT == node->type)
    {
      GumboAttribute* href = nullptr;
      if ((node->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&node->v.element.attributes, "href")))
          && (std::regex_match(href->value, rUri)))
      {
        vLinks.push_back(href->value);
      }

      GumboVector* children = &node->v.element.children;
      for (unsigned int i = 0; i < children->length; ++i)
      {
        qn.push(static_cast<GumboNode*>(children->data[i]));
      }
    }
  }

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

//void fulling_vector(std::string& res) {
//
//}