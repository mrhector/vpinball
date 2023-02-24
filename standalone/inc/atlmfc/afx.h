class CException : public std::exception {
public:
   LPCTSTR GetText() const { return ""; };
};