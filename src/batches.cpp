#include "boorudl/downloadable.h"
#include "boorudl/batches.h"
#include "boorudl/common.h"
#include "boorudl/page.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace boorudl {
    batches::batches(const page& page, int batch_size, common::filter_type filter)
        : m_batch_size{ batch_size }
        , m_filter{ std::move(filter) } {
        if (batch_size <= 0)
            throw std::invalid_argument{ "batch_size must be greater than 0" };

        create_page_batches(page);
        optimize_batches();
    }

    batches::batches(const downloadable_type& to_download, int batch_size, common::filter_type filter)
        : m_batch_size{ batch_size }
        , m_filter{ std::move(filter) } {
        if (batch_size <= 0)
            throw std::invalid_argument{ "batch_size must be greater than 0" };

        for (const auto& page : to_download) {
            create_page_batches(page);
        }
        optimize_batches();
    }

    void batches::create_page_batches(const page& page) {
        return m_filter
            ? create_filtered_batches(page)
            : create_unfiltered_batches(page);
    }

    void batches::create_filtered_batches(const page& page) {
        for (const post& post : page) {
            if (!m_filter(post))
                continue;

            /* create an empty batch at the end */
            if (empty())
                emplace_back();

            /* if the last batch of the instance has fewer posts than wanted, insert the current @post at the end */
            batch_type& last{ back() };
            if (last.size() < static_cast<batch_type::size_type>(m_batch_size)) {
                last.push_back(post);
                continue;
            }

            /* create new batch with current @post and push it at the end */
            batch_type new_batch{ post };
            push_back(std::move(new_batch));
        }
    }

    void batches::create_unfiltered_batches(const page& page) {
        int total_batches{ static_cast<int>(page.size()) / m_batch_size };
        if (page.size() % m_batch_size > 0) ++total_batches;
        auto page_posts{ static_cast<int>(page.size()) };

        /* reserve space for the upcoming batches */
        reserve(size() + total_batches);
        for (int batch_index{ 0 }; batch_index < total_batches; ++batch_index) {
            const int starting_index{ batch_index * m_batch_size };
            int final_index{ (batch_index + 1) * m_batch_size };
            if (final_index > page_posts) final_index = page_posts;

            batch_type current_batch;
            current_batch.reserve(m_batch_size);
            for (int index{ starting_index }; index < final_index; ++index) {
                current_batch.push_back(page[index]);
            }
            push_back(std::move(current_batch));
        }
    }

    void batches::optimize_batches() {
        const auto comparison{ [=](const batch_type& value) -> bool {
            return (static_cast<int>(value.size()) < m_batch_size);
        } };

        while (true) {
            auto first{ std::find_if(begin(), end(), comparison) };
            if (first == end()) return; /* all batches are full */

            auto second{ std::find_if(std::next(first), end(), comparison) };
            if (second == end()) return; /* only one batch is not full */

            if (first->size() > second->size())
                move_batch(first, second);
            else
                move_batch(second, first);
        }
    }

    void batches::move_batch(iterator to, iterator from) {
        while (static_cast<int>(to->size()) < m_batch_size
            && !from->empty()) {
            to->push_back(from->back());
            from->pop_back();
        }
        if (from->empty())
            erase(from);
    }
} // boorudl