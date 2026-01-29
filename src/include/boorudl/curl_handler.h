#ifndef LIBBOORUDL_CURL_HANDLER_H
#define LIBBOORUDL_CURL_HANDLER_H

#include <curl/curl.h>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace boorudl::curl {
    class easy {
    private:
        CURL* const m_handle{ nullptr };
        curl_slist* m_slist{ nullptr };

    public:
        easy();

        easy(const easy&) = delete;

        easy(easy&& other) noexcept;

        virtual ~easy();

        easy& set_to_stream(const std::ostream& stream, const std::string& url);

        easy& mimic_user_agent();

        CURLcode perform();

    private:
        static std::size_t write_to_stream(char* contents, std::size_t size, std::size_t nmemb, void* buffer);

    public:
        friend class multi;
    };

    class multi {
    private:
        CURLM* const m_handle{ nullptr };

    public:
        multi();

        multi(const multi&) = delete;

        multi(multi&&) = delete;

        virtual ~multi();

        void add_handles(const std::vector<easy>& handles);

        void remove_handles(const std::vector<easy>& handles);

        void perform();

        void resolve(const std::vector<easy>& handles);
    };
} // boorudl::curl

#endif //LIBBOORUDL_CURL_HANDLER_H
