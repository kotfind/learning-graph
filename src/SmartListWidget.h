#pragma once

#include <QListWidget>
#include <QAction>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QList>

class SmartListWidget : public QListWidget {
    Q_OBJECT

    public:
        SmartListWidget(QWidget* parent = nullptr);

        // Adds item with name name and id id
        void addItem(const QString& name, int id);

        // Returns current id
        int currentId() const;

        // Turns selection mode on if v == true
        // turns it off otherwise
        void setSelectionMode(bool v);

        // Returns ids of selected items
        // NOTE: works only when selection mode is on
        QList<int> getSelectedIds() const;

    private:
        QPoint dragStartPoint;

        bool selectionMode;

    protected:
        // Emits itemDoubleClicked
        void mouseDoubleClickEvent(QMouseEvent*) override;

        // Emits itemMenuRequested
        void contextMenuEvent(QContextMenuEvent*) override;

        // Sets dragStartPoint
        // NOTE: works only when selection mode is on
        void mousePressEvent(QMouseEvent*) override;

        // Emits drag request
        // NOTE: works only when selection mode is on
        void mouseMoveEvent(QMouseEvent*) override;

    signals:
        void itemDoubleClicked(int id);
        void itemMenuRequested(int id, const QPoint& globalPos);
        void itemDragRequested(int id);
};
