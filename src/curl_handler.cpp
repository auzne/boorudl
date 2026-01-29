#include "boorudl/curl_handler.h"

#include <curl/curl.h>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace boorudl::curl {
    easy::easy()
        : m_handle{ curl_easy_init() } { }

    easy::easy(boorudl::curl::easy&& other) noexcept
        : m_handle{ other.m_handle }
        , m_slist{ other.m_slist } { }

    easy::~easy() {
        if (m_slist)
            curl_slist_free_all(m_slist);

        if (m_handle)
            curl_easy_cleanup(m_handle);
    }

    easy& easy::set_to_stream(const std::ostream& stream, const std::string& url) {
        curl_easy_setopt(m_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, write_to_stream);
        curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, &stream);
        return *this;
    }

    easy& easy::mimic_user_agent() {
        m_slist = curl_slist_append(m_slist,
            "User-Agent:"
            " Mozilla/5.0 (Windows NT 10.0; Win64; x64)"
            " AppleWebKit/537.36 (KHTML, like Gecko)"
            " Chrome/118.0.0.0 Safari/537.36");

        curl_easy_setopt(m_handle, CURLOPT_HTTPHEADER, m_slist);
        return *this;
    }

    CURLcode easy::perform() {
        return curl_easy_perform(m_handle);
    }

    std::size_t easy::write_to_stream(char* contents, std::size_t size, std::size_t nmemb, void* buffer) {
        (*static_cast<std::ostream*>(buffer)).write(contents, nmemb);
        return size * nmemb;
    }

    multi::multi()
        : m_handle{ curl_multi_init() } { }

    multi::~multi() {
        if (m_handle)
            curl_multi_cleanup(m_handle);
    }

    void multi::add_handles(const std::vector<easy>& handles) {
        for (const auto& handle : handles) {
            curl_multi_add_handle(m_handle, handle.m_handle);
        }
    }

    void multi::remove_handles(const std::vector<easy>& handles) {
        for (const auto& easy : handles)
            curl_multi_remove_handle(m_handle, easy.m_handle);
    }

    void multi::perform() {
        constexpr int timeout_ms{ 10000 };
        int running{};
        do {
            CURLMcode mc{ curl_multi_perform(m_handle, &running) };

            if (running)
                mc = curl_multi_poll(m_handle, nullptr, 0, timeout_ms, nullptr);

            if (mc != CURLM_OK)
                break;
        } while (running);
    }

    void multi::resolve(const std::vector<easy>& handles) {
        add_handles(handles);
        perform();
        remove_handles(handles);
    }
} // boorudl::curl