#pragma once

#include "Enum.hpp"

#include <iostream>
#include <memory>
#include <utility>

namespace web
{
    struct Request;
    using Request_uPtr = std::unique_ptr<Request>;

    class RequestParser
    {
    public:
        RequestParser();

        template<typename Iterator>
        std::pair<Request_uPtr, ResultType> parse(Iterator begin, Iterator end);
        void reset();

    private:
        std::unique_ptr<Request> request;
        ParserState state;

        ResultType consume(char input);
        static bool is_special(int c);
    };


    template<typename Iterator>
    inline std::pair<Request_uPtr, ResultType> RequestParser::parse(Iterator begin, Iterator end)
    {
        while (begin != end)
        {
            ResultType result = consume(*begin++);

            if (result == ResultType::good || result == ResultType::bad)
            {
                return {std::move(request), std::move(result)};
            }
        }

        return {std::move(request), ResultType::indeterminate};
    }
}