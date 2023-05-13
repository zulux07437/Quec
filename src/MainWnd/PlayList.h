#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <gtkmm.h>

namespace MainWnd {
    class PlayList : public Gtk::TreeModel::ColumnRecord {
    public:
        typedef std::function<void(const std::string)> SelectionChanged; // Arg: filePath
        PlayList(Gtk::TreeView* treePlayList);
        void setSelectionChanged(SelectionChanged func);
        void addFromFolder(std::string folderPath);

        Gtk::TreeModelColumn<int> id;
        Gtk::TreeModelColumn<std::string> filePath;
        Gtk::TreeModelColumn<std::string> title;

    private:
        Gtk::TreeView* m_treePlayList;
        SelectionChanged m_selectionChanged;
        Glib::RefPtr<Gtk::ListStore> m_store;
    };
}

#endif // PLAYLIST_H