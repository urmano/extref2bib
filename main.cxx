#include <iostream>
#include <fstream>
#include <filesystem>
#include <ciere/json/io.hpp>
#include <ciere/json/value.hpp>
#include <ciere/json/parser/grammar.hpp>
#include <ciere/json/parser/grammar_def.hpp>
#include <cpr/cpr.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }
    const std::string sourceDoi = argv[1];

    const auto res = cpr::Get(
        cpr::Url{"https://opencitations.net/index/coci/api/v1/references/" + sourceDoi}
    );
    const std::string& json = res.text;

    std::vector<std::string> cites;
    ciere::json::value j = ciere::json::construct(json);
    for (auto i = 0u; i < j.length(); i++) {
        cites.emplace_back(j[i]["cited"]);
    }

    std::cout << "extracted " << cites.size() << " cites" << std::endl;

    std::vector<std::string> bibs;
    for (const auto& cite : cites) {
        std::cout << cite << std::endl;
        const std::string& file = "./extref2bib.tmp";
        std::string exec("betterbib doi-to-bibtex '" + cite + "' >> " + file);
        std::system(exec.data());
        std::ifstream input(file);
        std::stringstream ss;
        ss << input.rdbuf();
        std::string bib = ss.str();
        const std::string key = "\n";
        const std::size_t pos = bib.find(key);
        if (pos != std::string::npos) {
            bib = bib.substr(pos);
        }
        bibs.push_back(bib);
        std::filesystem::remove(file);
    }

    std::ofstream out("main.bib");
    for (const auto& bib : bibs) {
        out << bib;
        out.flush();
    }

}
