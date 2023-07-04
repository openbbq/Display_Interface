#pragma once

#include "../Window.h"

namespace display::ui
{
    class Layout : public ParentWindow
    {
    public:
        using ParentWindow::ParentWindow; // inherits constructors

        struct Entry
        {
            Entry(int k, int16_t s, WindowPtr p) : kind(k), size(s), window(p) {}

            int kind;
            int16_t size;
            WindowPtr window;
        };

        void addEntry(const Entry &entry)
        {
            entry.window->parent(shared_from_this());
            _entries.push_back(entry);
        }

        void addEntries(std::initializer_list<Entry> children)
        {
            for (const Entry &entry : children)
            {
                addEntry(entry);
            }
        }

        void addLeft(WindowPtr child, int16_t width = -1) { addEntry(Entry(1, width, child)); }
        void addTop(WindowPtr child, int16_t height = -1) { addEntry(Entry(2, height, child)); }
        void addRight(WindowPtr child, int16_t width = -1) { addEntry(Entry(3, width, child)); }
        void addBottom(WindowPtr child, uint16_t height = -1) { addEntry(Entry(4, height, child)); }
        void addFill(WindowPtr child) { addEntry(Entry(5, 0, child)); }

        template <typename TWindow, typename... _Args>
        std::shared_ptr<TWindow> addLeft(_Args &&...__args)
        {
            auto ptr = std::make_shared<TWindow>(std::forward<_Args>(__args)...);
            addLeft(ptr);
            return ptr;
        }

        template <typename TWindow, typename... _Args>
        std::shared_ptr<TWindow> addTop(_Args &&...__args)
        {
            auto ptr = std::make_shared<TWindow>(std::forward<_Args>(__args)...);
            addTop(ptr);
            return ptr;
        }
        
        template <typename TWindow, typename... _Args>
        std::shared_ptr<TWindow> addRight(_Args &&...__args)
        {
            auto ptr = std::make_shared<TWindow>(std::forward<_Args>(__args)...);
            addRight(ptr);
            return ptr;
        }

        template <typename TWindow, typename... _Args>
        std::shared_ptr<TWindow> addBottom(_Args &&...__args)
        {
            auto ptr = std::make_shared<TWindow>(std::forward<_Args>(__args)...);
            addBottom(ptr);
            return ptr;
        }

        template <typename TWindow, typename... _Args>
        std::shared_ptr<TWindow> addFill(_Args &&...__args)
        {
            auto ptr = std::make_shared<TWindow>(std::forward<_Args>(__args)...);
            addFill(ptr);
            return ptr;
        }

        void resizeHandler() override;

    private:
        std::vector<Entry> _entries;
    };

} // namespace display::widget
