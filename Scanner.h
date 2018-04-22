#ifndef SCANNER_H
#define SCANNER_H
enum Token
{
    if_token,
    for_token,
    in,
    to,
    bracket_open,
    bracket_close,
    brace_open,
    brace_close,
    square_bracket_open,
    square_bracket_close,
    semicolon,
    comma,
    int_token,
    float_token,
    string_token,
    timer,
    image,
    equal_token,
    assignment,
    less_token,
    more_token,
    plus_token,
    minus_token,
    or_token,
    and_token,
    multiply,
    divide,
    compressf,
    startf,
    endf,
    loadf,
    savef,
    showTimef,
    showf,
    setParamsf,
    int_value,
    float_value,
    string_value,
    ident
};

class Scanner
{
    public:
        Scanner() { value = nullptr;}
        ~Scanner() { if(value != nullptr) delete value; }
        void setToken(Token t) { token = t; }
        Token getToken() { return token; }
        void setValue(void* v)
        {
            if(value != nullptr)
            {
                delete value;
            }
            value = nullptr;
            value = v;
        }
        void* getValue() { return value; }
        void setSize(int s) { size = s; }
        int getSize() { return size; }
    private:
        Token token;
        void* value;
        int size;
};

#endif //SCANNER_H
