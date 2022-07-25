#include <iostream>
#include <fstream>
#include <filesystem>
// #include <map>
// #include <boost/spirit/home/x3.hpp>
// #include <boost/spirit/include/support_istream_iterator.hpp>
// #include <boost/fusion/adapted/std_pair.hpp>
#include <ciere/json/io.hpp>
#include <ciere/json/value.hpp>
#include <ciere/json/parser/grammar.hpp>
#include <ciere/json/parser/grammar_def.hpp>
#include <cpr/cpr.h>

// namespace {
//     using Element = std::map<std::string, int>;
//     struct Array : std::vector<Element> { };
//
//     namespace parser {
//         using namespace boost::spirit::x3;
//         rule<struct rule_key_t, std::string> s;
//         rule<struct rule_element_t, Element> r;
//         rule<struct rule_braced_t, Element>  e;
//
//         auto s_def = '"' >> ~char_('"') >> '"';
//         auto r_def = (s >> ':' >> int_) % ',';
//         auto e_def = '{' >> r >> '}';
//
//         BOOST_SPIRIT_DEFINE(s,r,e)
//     }
//
//     std::istream& operator>>(std::istream& is, Array& into) {
//         using namespace parser;
//
//         boost::spirit::istream_iterator f(is), l;
//
//         if (!phrase_parse(f, l, '[' >> '{'
//                     >> (e % ',') >> '}' >> ']', space, into))
//         {
//             is.setstate(is.rdstate() | std::ios::failbit);
//         }
//
//         return is;
//     }
// }

int main(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }
    const std::string sourceDoi = argv[1];

    const auto res = cpr::Get(
        cpr::Url{"https://opencitations.net/index/coci/api/v1/references/" + sourceDoi}
    );
    const std::string& json = res.text;

    // std::cout << json << std::endl;

    // Array array;
    // if (json >> array) {
    //     std::cout << "Parsed " << array.size() << " elements:\n";
    //
    //     for (auto& e : array) {
    //         std::cout << "--------------------\n{ ";
    //         for (auto& kv : e)
    //             std::cout << "\"" << kv.first << "\": " << kv.second << ", ";
    //         std::cout << " }\n";
    //     }
    // } else {
    //     std::cout << "Parsing failed\n";
    // }

    std::vector<std::string> cites;
    ciere::json::value j = ciere::json::construct(json);
    for (auto i = 0u; i < j.length(); i++) {
        // std::cout << j[i]["citing"] << std::endl;
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
            // bib.replace(pos, key.size(), "");
            // std::cout << "compressing..." << std::endl;
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
