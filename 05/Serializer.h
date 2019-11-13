#pragma once
#include <iostream>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
    std::ostream &out_;
public:
    explicit Serializer(std::ostream& out): out_(out) {}
    
    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsType>
    Error operator()(ArgsType... args)
    {
        return process(args...);
    }
private:
    template <class T, class... ArgsType>
    Error process(T x, ArgsType... args) {
        realize(x);
        return process(args...);
    }
    template <class T>
    Error process(T x) {
        return realize(x);;
    }
    Error realize(bool x) const {
        if (x) {
            out_ << "true" << Separator;
        } else {
            out_ << "false" << Separator;
        }
        return Error::NoError;
    }
    Error realize(uint64_t x) const {
        out_ << x << Separator;
        return Error::NoError;
    }
};

class Deserializer {
    std::istream &in_;
public:
    explicit Deserializer(std::istream &in): in_(in) {}
    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }
    template <class... ArgsType>
    Error operator()(ArgsType&... args)
    {
        return process(args...);
    }
private:
    template <class T, class... ArgsType>
    Error process(T &x, ArgsType&... args) {
        if (realize(x) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(args...);
    }
    template <class T>
    Error process(T &x) {
        return realize(x);
    }
    template<class T>
    Error realize(T &x) {
        return Error::CorruptedArchive;
    }
    Error realize(bool& value) {
        std::string text;
        in_ >> text;

        if (text == "true") {
            value = true;
            return Error::NoError;
        }
        else if (text == "false") {
            value = false;
            return Error::NoError;
        }
        return Error::CorruptedArchive;
    }
    Error realize(uint64_t& value) {
        std::string text;
        in_ >> text;
        if (!isdigit(text[0])) {
             return Error::CorruptedArchive;
        }
        try {
            value = std::stoull(text);
            return Error::NoError;
        } catch (...) {
            return Error::CorruptedArchive;
        }
    }
};
