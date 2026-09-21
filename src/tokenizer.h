#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string_view>
#include <vector>

std::vector<std::string_view> Tokenize(std::string_view input);

#endif  // TOKENIZER_H_