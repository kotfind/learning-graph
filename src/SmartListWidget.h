#pragma once

#include <QListWidget>
#include <QAction>
#include <QContextMenuEvent>
#include <QMouseEvent>

class SmartListWidget : public QListWidget {
    Q_OBJECT

    public:
        SmartListWidget(QWidget* parent = nullptr);

        void addItem(const QString& name, int id);

        int currentId();

    protected:
        void mouseDoubleClickEvent(QMouseEvent*) override;
        void contextMenuEvent(QContextMenuEvent*) override;

    signals:
        void doubleClicked(int id);
        void menuRequested(int id, const QPoint& globalPos);
};