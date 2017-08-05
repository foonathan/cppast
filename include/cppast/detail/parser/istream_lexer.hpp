// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_DETAIL_PARSER_ISTREAM_LEXER_HPP_INCLUDED
#define CPPAST_DETAIL_PARSER_ISTREAM_LEXER_HPP_INCLUDED

#include <istream>
#include <sstream>
#include <cppast/detail/parser/lexer.hpp>

namespace cppast
{

namespace detail
{

namespace parser
{

class istream_lexer : public lexer
{
public:
    istream_lexer(std::istream& input, const diagnostic_logger& logger, const std::string& filename = "");

    bool read_next_token() override;
    const token& current_token() const override;

    bool good() const override;
    bool eof() const override;
    source_location location() const override;

private:
    std::istream& _input;
    std::string _filename;
    std::ostringstream _token_buffer;
    token _current_token;
    std::int64_t _column;
    std::size_t _line, _previous_line_length;
    std::size_t _token_line, _token_column;
    char _last_char;

    void save_token(const token::token_kind kind);
    bool advance();
    bool put_back();
    void save_char(char c);
    void discard_char();

    enum class scan_result
    {
        successful,
        unsuccessful,
        error
    };

    scan_result scan_string_literal();
    scan_result scan_identifier();
    scan_result scan_number();
    scan_result scan_samechars_symbol();
    scan_result scan_char();

    using scan_function = scan_result(istream_lexer::*)();

    static constexpr scan_function scanners[] = {
        &istream_lexer::scan_string_literal,
        &istream_lexer::scan_identifier,
        &istream_lexer::scan_number,
        &istream_lexer::scan_samechars_symbol,
        &istream_lexer::scan_char
    };
};

} // namespace cppast::detail::parser

} // namespace cppast::detail

} // namespace cppast

#endif // CPPAST_DETAIL_PARSER_ISTREAM_LEXER_HPP_INCLUDED
