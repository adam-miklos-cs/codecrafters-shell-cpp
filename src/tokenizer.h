#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <string_view>
#include <vector>

std::vector<std::string_view> Split(std::string_view input,
                                    const char *delimiters = " \t\n");

std::vector<std::string> SplitWithShellBehaviour(std::string_view input);

#endif // TOKENIZER_H_
