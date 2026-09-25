#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string_view>
#include <vector>

std::vector<std::string_view> Split(std::string_view input,
                                    const char *delimiters = " \t\n");

#endif // TOKENIZER_H_
