#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <set>

int
main()
{
  const std::string alphabet("abcdefghijklmnopqrstuvwxyz");
  std::set<std::string> dict;

  {
    std::ifstream dict_stream("words_alpha.txt");
    std::string word;
    while (dict_stream >> word) {
      dict.insert(word);
    }
  }

  std::ifstream text_stream("text");
  std::string text((std::istreambuf_iterator<char>(text_stream)),
                   std::istreambuf_iterator<char>());
  std::regex reg("[a-zA-Z]+");

  std::sregex_token_iterator iter(text.begin(), text.end(), reg);
  std::sregex_token_iterator end;

  for (auto a = iter; a != end; ++a) {
    std::string word(a->first, a->second);

    std::vector<std::string> choices{ "A", "S", "F", "a", "s", "f" };
    if (dict.find(word) == dict.end()) {
      while (true) {
        // adding word to dict
        std::cout << "Unknown word \"" << word
                  << "\" in text. (A)dd to dictionary, (S)kip it, "
                     "(F)ind replacement: ";
        std::string choice;
        std::cin >> choice;
        if (std::find(choices.begin(), choices.end(), choice) !=
            choices.end()) {
          if (choice == "A" || choice == "a") {
            dict.insert(word);
          } else if (choice == "S" || choice == "s") {
          } else if (choice == "F" || choice == "f") {
            std::vector<std::string> possible_replacements;
            //
            // mutate word
            //

            // insertion
            for (size_t i = 0; i <= word.length(); i++) {
              std::string word_mutated(word);
              word_mutated.insert(i, "_");
              for (auto letter : alphabet) {
                word_mutated[i] = letter;
                auto search_result = dict.find(word_mutated);
                if (search_result != dict.end()) {
                  possible_replacements.push_back(*search_result);
                }
              }
            }

            // deletion
            for (size_t i = 0; i < word.length(); i++) {
              std::string word_mutated(word);
              word_mutated.erase(i, 1);
              auto search_result = dict.find(word_mutated);
              if (search_result != dict.end()) {
                possible_replacements.push_back(*search_result);
              }
            }

            // substitution
            for (size_t i = 0; i < word.length(); i++) {
              std::string word_mutated(word);
              for (auto letter : alphabet) {
                word_mutated[i] = letter;
                auto search_result = dict.find(word_mutated);
                if (search_result != dict.end()) {
                  possible_replacements.push_back(*search_result);
                }
              }
            }
            if (possible_replacements.size() == 0) {
              std::cout << "No replacements found." << std::endl;
              continue;
            }
            size_t i = 1;
            std::cout << "Possible replacements: " << std::endl;
            for (auto r : possible_replacements) {
              std::cout << "(" << i << ") " << r << std::endl;
              ++i;
            }

            while (true) {
              std::cout << "Choose replacement: ";
              std::string replacement_choice_str;
              std::cin >> replacement_choice_str;
              int replacement_choice;
              try {
                replacement_choice = std::stoi(replacement_choice_str);
                text.replace(std::distance(text.cbegin(), a->first),
                             std::distance(a->first, a->second),
                             possible_replacements.at(replacement_choice - 1));
              } catch (const std::invalid_argument& e) {
                continue;
              } catch (const std::out_of_range& e) {
                continue;
              }
              break;
            }
          }
          break;
        } else {
          continue;
        }
      }
    }
  }

  std::cout << text << std::endl;
  {
    std::ofstream outtext("text.out");
    outtext << text;
  }
}
