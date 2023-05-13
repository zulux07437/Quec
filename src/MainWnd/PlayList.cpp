#include <filesystem>

#include "PlayList.h"
#include "../Common/Logger.h"

namespace MainWnd {
    PlayList::PlayList(Gtk::TreeView* treePlayList) {
        add(id);
        add(filePath);
        add(title);

        m_store = Gtk::ListStore::create(*this);
        m_treePlayList = treePlayList;
        m_treePlayList->append_column("ID", this->id);
        m_treePlayList->append_column("Title", this->title);
        m_treePlayList->set_model(m_store);
        m_treePlayList->get_column(1)->set_resizable(true);
        m_treePlayList->get_column(1)->set_min_width(20);
        m_treePlayList->signal_row_activated().connect(
            [&](const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
                auto row = *(m_treePlayList->get_model()->get_iter(path));
                std::string get = row.get_value(this->filePath);
                m_selectionChanged(get);
                // Logger::Log(std::to_string(get), Logger::MESSAGE);
            }
        );
    }

    void PlayList::setSelectionChanged(SelectionChanged func) {
        if (func) m_selectionChanged = func;
    }

    void PlayList::addFromFolder(std::string folderPath) {
        std::filesystem::path folder(folderPath);
        std::vector<std::string> paths;
        for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            if (entry.is_regular_file()) paths.push_back(entry.path().string());
        }
        int i = 0;
        for (const std::string& path : paths) {
            i++;
            std::filesystem::path filePath(path);
            std::string ex = filePath.extension().string();
            // Logger::Log(ex + ": " + path, Logger::MESSAGE);
        
            auto row = *(m_store->append());
            row[this->id] = i;
            row[this->filePath] = path;
            row[this->title] = path;
        }
    }
}