#include <parser.hpp>

int main(int argc, char** argv) {
  Arguments arguments = {"www.ozon.ru", 1, 1, 1, "./output.txt"};
  std::vector<std::string> links;
  std::vector<Item> itemList;
  std::vector<std::vector<std::string>> vectorHTML;
  std::vector<std::string> vectorIMG;

  boost::program_options::options_description desc("Parcer");
  desc.add_options()("help", "produce help message")(
      "url", boost::program_options::value<std::string>(),
      "Enter url of page without http/ or https/")(
      "depth", boost::program_options::value<size_t>(),
      "Enter depth of parsing")("network_threads",
                                boost::program_options::value<size_t>(),
                                "Enter number of threads for getting page")(
      "parser_threads", boost::program_options::value<size_t>(),
      "Enter threads for parsing of page")(
      "output", boost::program_options::value<std::string>(),
      "Enter name of output file");
  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
  if (vm.count("url")) {
    arguments.url = vm["url"].as<std::string>();
  }
  if (vm.count("depth")) {
    arguments.depth = vm["depth"].as<size_t>();
  }
  if (vm.count("network_threads")) {
    arguments.network_threads = vm["url"].as<size_t>();
  }
  if (vm.count("parser_threads")) {
    arguments.parser_threads = vm["parser_threads"].as<size_t>();
  }
  if (vm.count("output")) {
    arguments.output = vm["output"].as<std::string>();
  }
  std::string first_page = get_page(arguments.url, "/");
  find_links(first_page, links);

  thread_start_parser(vectorHTML, links, arguments.depth, arguments.network_threads);
  links.clear();
  for (size_t i = 0; i < vectorHTML.size(); i++) {
    for (size_t j = 0; j < vectorHTML[i].size(); j++)
      links.push_back(vectorHTML[i][j]);
  }
  thread_start_img(vectorIMG, links, arguments.depth, arguments.parser_threads);
//  item_filling(itemList, links);

  for (size_t i = 0; i < vectorIMG.size(); i++) {
    //std::cout << itemList[i].host << " " << itemList[i].target << std::endl;
//    for (size_t j = 0; j < vectorIMG[i].size(); j++)
      std::cout << vectorIMG[i] << std::endl;
  }
}
