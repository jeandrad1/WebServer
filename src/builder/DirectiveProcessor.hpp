#ifndef DIRECTIVEPROCESSOR_HPP
# define DIRECTIVEPROCESSOR_HPP

# include <string>
# include <map>

#include <iostream>

template <typename Derived>
class DirectiveProcessor {
    
    public:
        typedef void (Derived::*HandlerFunc)(const std::string &);

    protected:
        std::map<std::string, HandlerFunc> directiveHandlers;

    public:
        virtual  ~DirectiveProcessor() {}

        void    registerHandler(const std::string &name, HandlerFunc handler)
        {
            directiveHandlers[name] = handler;
        }

		void    dispatchDirective(const std::string &name, const std::string &value)
		{
			typename std::map<std::string, HandlerFunc>::iterator it = directiveHandlers.find(name);
			if (it != directiveHandlers.end())
			{
				HandlerFunc handler = it->second;
				Derived *self = static_cast<Derived *>(this);
				(self->*handler)(value);
			}
		}
};

#endif