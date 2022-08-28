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

    private:
        QPoint dragStartPoint;

    protected:
        void mouseDoubleClickEvent(QMouseEvent*) override;
        void contextMenuEvent(QContextMenuEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;

    signals:
        void doubleClicked(int id);
        void menuRequested(int id, const QPoint& globalPos);
        void dragRequested(int id);
};
