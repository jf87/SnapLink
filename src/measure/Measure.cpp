#include "measure/Measure.h"
#include "measure/MeasureWidget.h"
#include <QApplication>
#include <QResource>
#include <iostream>

int Measure::run(int argc, char *argv[]) {
  // Parse arguments
  std::string dbFile;

  po::options_description visible("command options");
  visible.add_options() // use comment to force new line using formater
      ("help,h", "print help message");

  po::options_description hidden;
  hidden.add_options() // use comment to force new line using formater
      ("dbfile", po::value<std::string>(&dbFile)->required(), "database file");

  po::options_description all;
  all.add(visible).add(hidden);

  po::positional_options_description pos;
  pos.add("dbfile", 1);

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
                                  .options(all)
                                  .positional(pos)
                                  .allow_unregistered()
                                  .run();
  po::store(parsed, vm);

  // print invalid options
  std::vector<std::string> unrecog =
      collect_unrecognized(parsed.options, po::exclude_positional);
  if (unrecog.size() > 0) {
    printInvalid(unrecog);
    printUsage(visible);
    return 1;
  }

  if (vm.count("help")) {
    printUsage(visible);
    return 0;
  }

  // check whether required options exist after handling help
  po::notify(vm);

  // Run the program

  QApplication a(argc, argv);
  MeasureWidget w;

  // open database
  if (!w.init(dbFile)) {
    return 1;
  }

  w.show();

  return a.exec();
}

void Measure::printInvalid(const std::vector<std::string> &opts) {
  std::cerr << "invalid options: ";
  for (const auto &opt : opts) {
    std::cerr << opt << " ";
  }
  std::cerr << std::endl;
}

void Measure::printUsage(const po::options_description &desc) {
  std::cout << "snaplink measure [command options] db_file" << std::endl
            << std::endl
            << desc << std::endl;
}
